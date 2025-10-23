cmake_policy(SET CMP0079 NEW)

set(PYPI_UAC_PACKAGE "ultrasound_acquisition_configuration")

pybind11_add_module(UacBindingsPython THIN_LTO uac/python/bindings.h
                    bindings.cpp CMakeLists.txt)
target_link_libraries(
  UacBindingsPython PRIVATE ${PROJECT_NAME} Uac::UacUtils Urx::UrxPython
                            Urx::UrxCompilerFlags)
target_include_directories(UacBindingsPython PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libstdc++.so.6
    OUTPUT_VARIABLE STDCPP_SO_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "Found libstdc++.so.6 in ${STDCPP_SO_PATH}")
  target_link_options(UacBindingsPython PRIVATE "-Wl,-rpath,$ORIGIN")
  add_custom_command(
    TARGET UacBindingsPython
    POST_BUILD
    COMMAND cp -L ${STDCPP_SO_PATH} $<TARGET_FILE_DIR:UacBindingsPython>
    COMMAND_EXPAND_LISTS)
endif(UNIX)

set_target_properties(UacBindingsPython PROPERTIES OUTPUT_NAME "bindings")

# Build python binary distribuable.
if(NOT CALL_FROM_SETUP_PY AND WITH_PYTHON_WHL)
  set(ENV_COMMAND ${CMAKE_COMMAND} -E env CFLAGS=$ENV{CFLAGS}
                  CXXFLAGS=$ENV{CXXFLAGS})

  if(DEFINED ENV{CMAKE_BUILD_PARALLEL_LEVEL})
    set(ENV_COMMAND ${ENV_COMMAND}
                    CMAKE_BUILD_PARALLEL_LEVEL=$ENV{CMAKE_BUILD_PARALLEL_LEVEL})
  endif()

  set(PIP_WHEEL_OPTIONS
      -m
      pip
      wheel
      .
      --config-settings=--global-option=DISABLE_VCPKG=ON
      --config-settings=--global-option=cmake_build_type=$<CONFIG>
      --config-settings=--global-option=-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
      --config-settings=--global-option=-DCRT_SHARED_LIBS=${CRT_SHARED_LIBS}
      --config-settings=--global-option=-DWITH_HDF5=${WITH_HDF5}
      --config-settings=--global-option=-DCatch2_DIR=${Catch2_DIR}
      --config-settings=--global-option=-DHDF5_DIR=${HDF5_DIR}
      --config-settings=--global-option=-DBUILD_TESTING=${BUILD_TESTING}
      --config-settings=--global-option=-Dpybind11_DIR=${pybind11_DIR}
      --config-settings=--global-option=-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
      --config-settings=--global-option=-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      --config-settings=--global-option=-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
      --config-settings=--global-option=-DCMAKE_AR=${CMAKE_AR}
      --config-settings=--global-option=-DCMAKE_AS=${CMAKE_AS}
      --config-settings=--global-option=-DCMAKE_RANLIB=${CMAKE_RANLIB}
      --config-settings=--global-option=-DCMAKE_LINKER_TYPE=${CMAKE_LINKER_TYPE}
      --config-settings=--global-option=-DSCCACHE=${SCCACHE}
      --config-settings=--global-option=-DCCACHE=${CCACHE}
      --config-settings=--global-option=-DSCCACHE_PROGRAM="${SCCACHE_PROGRAM}"
      --wheel-dir
      ${CMAKE_CURRENT_BINARY_DIR}
      --no-clean
      -v)
  if(NOT URX_SRC STREQUAL "")
    set(PIP_WHEEL_OPTIONS
        ${PIP_WHEEL_OPTIONS}
        --config-settings=--global-option=-DURX_SRC=${URX_SRC})
  elseif(Urx_DIR)
    set(PIP_WHEEL_OPTIONS
        ${PIP_WHEEL_OPTIONS}
        --config-settings=--global-option=-DUrx_DIR=${Urx_DIR})
  endif()
  if(DEFINED PYTHON_IS_DEBUG)
    set(PIP_WHEEL_OPTIONS
        ${PIP_WHEEL_OPTIONS}
        --config-settings="--global-option=-DPYTHON_IS_DEBUG=${PYTHON_IS_DEBUG}"
    )
  endif()
  if(VCPKG_INSTALL_OPTIONS)
    set(PIP_WHEEL_OPTIONS
        ${PIP_WHEEL_OPTIONS}
        --config-settings=--global-option=-DVCPKG_INSTALL_OPTIONS=${VCPKG_INSTALL_OPTIONS}
    )
  endif()
  string(REPLACE ";" " " PIP_WHEEL_OPTIONS_ECHO "${PIP_WHEEL_OPTIONS}")

  if(Python3_EXECUTABLE_IS_DEBUG)
    set(Python3_EXECUTABLE_EXEC ${CMAKE_COMMAND} -E true)
  else()
    if(Python3_EXECUTABLE_DEBUG_IS_DEBUG)
      set(Python3_EXECUTABLE_EXEC
          ${ENV_COMMAND}
          "$<IF:$<CONFIG:Debug>,${Python3_EXECUTABLE_DEBUG},${Python3_EXECUTABLE}>"
      )
    else()
      set(Python3_EXECUTABLE_EXEC ${ENV_COMMAND} "${Python3_EXECUTABLE}")
    endif()
  endif()
  if(NOT Python3_EXECUTABLE_DEBUG_IS_DEBUG)
    set(Python3_EXECUTABLE_DEBUG_EXEC ${CMAKE_COMMAND} -E true)
  else()
    set(Python3_EXECUTABLE_DEBUG_EXEC ${ENV_COMMAND}
                                      "${Python3_EXECUTABLE_DEBUG}")
  endif()

  string(REPLACE ";" " " Python3_EXECUTABLE_EXEC_ECHO
                 "${Python3_EXECUTABLE_EXEC}")
  string(REPLACE ";" " " Python3_EXECUTABLE_DEBUG_EXEC_ECHO
                 "${Python3_EXECUTABLE_DEBUG_EXEC}")

  add_custom_target(
    UacPythonWheel ALL
    COMMAND
      ${CMAKE_COMMAND} -E echo
      "$<IF:$<CONFIG:Debug>,${Python3_EXECUTABLE_DEBUG_EXEC_ECHO},${Python3_EXECUTABLE_EXEC_ECHO}> ${PIP_WHEEL_OPTIONS_ECHO}"
    COMMAND
      "$<IF:$<CONFIG:Debug>,${Python3_EXECUTABLE_DEBUG_EXEC},${Python3_EXECUTABLE_EXEC}>"
      ${PIP_WHEEL_OPTIONS}
    COMMAND_EXPAND_LISTS VERBATIM
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)

  # May generate uac-0.6.26-cp311-cp311d-win_amd64.whl
  # ultrasound_acquisition_configuration-1.2.0-pp310-pypy310_pp73-linux_x86_64.whl
  execute_process(
    COMMAND
      ${ENV_COMMAND} ${Python3_EXECUTABLE} -c
      "from packaging import tags; tag = next(tags.sys_tags()); print(str(tag))"
    OUTPUT_VARIABLE PYTHON_TAG_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE PYTHON_TAG_RESULT)
  if(PYTHON_TAG_RESULT OR PYTHON_TAG_RELEASE STREQUAL "")
    message(FATAL_ERROR "Failed to get release ABI tags")
  endif()

  if(NOT ${Python3_EXECUTABLE_DEBUG} STREQUAL "")
    execute_process(
      COMMAND
        ${Python3_EXECUTABLE_DEBUG_EXEC} -c
        "from packaging import tags; tag = next(tags.sys_tags()); print(str(tag))"
      OUTPUT_VARIABLE PYTHON_TAG_DEBUG
      OUTPUT_STRIP_TRAILING_WHITESPACE
      RESULT_VARIABLE PYTHON_TAG_RESULT)
    if(PYTHON_TAG_RESULT OR PYTHON_TAG_DEBUG STREQUAL "")
      message(FATAL_ERROR "Failed to get debug ABI tags")
    endif()
  endif()

  if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(PYTHON_TAG_POSTFIX "win_amd64.whl")
    else()
      set(PYTHON_TAG_POSTFIX "win32.whl")
    endif()
  elseif(UNIX AND NOT APPLE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(PYTHON_TAG_POSTFIX "linux_x86_64.whl")
    else()
      message(FATAL_ERROR "Unsupported architecture for python output.")
    endif()
  else()
    message(FATAL_ERROR "Unsupported architecture for python output.")
  endif()

  if(NOT ${Python3_EXECUTABLE_DEBUG} STREQUAL "")
    set(PYTHON_MODULE_UAC_OUTPUT_DEBUG
        "${PYPI_UAC_PACKAGE}-${CMAKE_PROJECT_VERSION}-${PYTHON_TAG_DEBUG}-${PYTHON_TAG_POSTFIX}"
    )
    install(
      FILES "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_UAC_OUTPUT_DEBUG}"
      DESTINATION "share/${INSTALL_NAME}"
      OPTIONAL)
  endif()

  set(PYTHON_MODULE_UAC_OUTPUT_RELEASE
      "${PYPI_UAC_PACKAGE}-${CMAKE_PROJECT_VERSION}-${PYTHON_TAG_RELEASE}-${PYTHON_TAG_POSTFIX}"
  )
  install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_MODULE_UAC_OUTPUT_RELEASE}"
    DESTINATION "share/${INSTALL_NAME}"
    OPTIONAL)
endif()

install(
  TARGETS UacBindingsPython
  COMPONENT bindings
  LIBRARY DESTINATION ${PYTHON_INSTALL_LIBDIR})
if(UNIX)
  install(
    FILES $<TARGET_FILE_DIR:UacBindingsPython>/libstdc++.so.6
    COMPONENT bindings
    DESTINATION ${PYTHON_INSTALL_LIBDIR})
endif()
