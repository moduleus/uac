export CMAKE_BUILD_PARALLEL_LEVEL=$(cat /proc/cpuinfo | grep processor | wc -l)

echo "Generating manylinux wheel for $1"

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" --plat $PLAT -w . || exit 1
    fi
}

for f in /opt/python/*/bin/python; do
    if [[ ";$SKIP_PYTHON;" == *";$(echo $f | cut -d '/' -f 4);"* ]]; then
        echo "Skipping python $f"
        continue
    fi
    if [[ -n "$ONLY_PYTHON" && ";$ONLY_PYTHON;" != *";$(echo $f | cut -d '/' -f 4);"* ]]; then
        echo "Skipping python $f"
        continue
    fi
    echo "Building with $f..."
    $f -m ensurepip
    $f -m pip install --upgrade pip || exit 1
    $f -m pip install -r requirements-gitlab.txt || exit 1

    if [ -d urx ]; then
        cp -R vcpkg-registry urx
        cmake -S urx -B urx_$f -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$VCPKG_LOCAL_GIT/scripts/buildsystems/vcpkg.cmake" -DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets -DBUILD_SHARED_LIBS=OFF -DWITH_HDF5=ON -DBUILD_TESTING=ON -DWITH_PYTHON:BOOL=ON -DWITH_PYTHON_WHL:BOOL=ON -DWITH_MATLAB:BOOL=OFF -DENABLE_PCH:BOOL=OFF -DPython3_EXECUTABLE=$f -DCMAKE_CXX_FLAGS="-Wno-array-bounds -Wno-stringop-overread"
        cmake --build urx_$f --config Release --parallel $NUM_PARALLEL --target UrxPythonWheel
    fi
    pyt=$(basename $(dirname $(dirname $f)))
    echo $f -m pip wheel . --config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=${VCPKG_LOCAL_GIT}/scripts/buildsystems/vcpkg.cmake" --config-settings="--global-option=cmake_build_type=Release" --wheel-dir "." -v --config-settings=--global-option=-DCMAKE_CXX_FLAGS="-Wno-array-bounds -Wno-stringop-overread" $([ -d urx ] && echo --config-settings="--global-option=-DURX_SRC=urx" || echo --config-settings="--global-option=-DWITH_VCPKG_URX:BOOL=ON" --config-settings="--global-option=-DWITH_PYTHON_WHL:BOOL=ON") --config-settings=--global-option=-DBUILD_TESTING:BOOL=ON
    $f -m pip wheel . --config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=${VCPKG_LOCAL_GIT}/scripts/buildsystems/vcpkg.cmake" --config-settings="--global-option=cmake_build_type=Release" --wheel-dir "." -v --config-settings=--global-option=-DCMAKE_CXX_FLAGS="-Wno-array-bounds -Wno-stringop-overread" $([ -d urx ] && echo --config-settings="--global-option=-DURX_SRC=urx" || echo --config-settings="--global-option=-DWITH_VCPKG_URX:BOOL=ON" --config-settings="--global-option=-DWITH_PYTHON_WHL:BOOL=ON") --config-settings=--global-option=-DBUILD_TESTING:BOOL=ON || (
        cat /cache/vcpkg-git/buildtrees/urx/install-x64-lsrp-rel-out.log
        cat /cache/vcpkg-git/buildtrees/urx/install-x64-lsrp-rel-err.log
        cat /builds/common/sw/uac/build/temp.linux-x86_64-pypy310_ultrasound-acquisition-configuration/vcpkg_installed/vcpkg/issue_body.md
        exit 1
    )
    find . -name "*.whl"
    $f -m pip install $(find . -name "ultrasound_rawdata_exchange-*-$pyt-*.whl")
    $f -m pip install *-$pyt-*.whl || exit 1
    $f -m unittest discover test/python || exit 1
done

echo "Looking for $1*.whl"
ls $1*.whl

for whl in $1*.whl; do
    repair_wheel "$whl"
done
