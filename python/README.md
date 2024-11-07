# README

## Build

First, read README.md from urx/python/README.md.

### Build for dev

With this configuration, urx will be get from vcpkg registry (CMake set `-DWITH_VCPKG_URX:BOOL=ON`).

You may add `--config-settings="--global-option=urx_src=D:\\repos\\urx"` if you want to use local urx repository. This will set `-DWITH_VCPKG_URX:BOOL=OFF` and `-DURX_SRC:STRING=${urx_src}`.

## Technical choice

### Conflict between Python Urx and Uac

There was a symbol duplication problem because all the symbols (Dataset and all the others) existed in urx and uac.

Problem: pybind11 exposes all symbols globally and not in module (or local) level, which causes duplicate symbols. However, if it's set to local, the Python module (B) cannot convert a C++ class registered in module A to Python (https://pybind11.readthedocs.io/en/stable/advanced/classes.html#module-local-class-bindings). Specifically, the uacToUrx method (which is in the python module uac) can't work because uacToUrx must return a Dataset declared in another python module urx.

Solution 1 (not adopted): Set all symbols locally to avoid naming conflicts. Problem: all urx project will have to be declared/compiled in the uac module. It also won't be possible to create a third Python module via pybind11 using urx / uac since the symbols are local.

Solution 2 (not adopted): Set all symbols to global and rename symbols in Uac to avoid conflicts. For example, prefix all symbols with Uac. Problem: naming will be urx.Dataset and uac.UacDataset.

Solution 3 (adopted): Set all symbols to global and prefix all symbols in Urx and Uac to avoid conflicts, then manually add an non-prefixed local alias in modules. For example, in urx, create a local alias of the symbol Dataset to UrxDataset. In Python, it will get urx.Dataset and uac.Dataset that do not point to the same symbol since they are aliases to urx.UrxDataset and uac.UacDataset. This is clearly not the perfect solution but the only one that works.

There is also a second issue: Python also refuses to register two identical C++ symbols (such as DoubleNan) globally under two different names. So once again, it's required to create aliases in uac that point to the symbols in urx.
