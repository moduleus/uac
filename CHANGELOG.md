# Changelog

## 1.2.0

### C++

#### What's new (improvement)

  * Major refactor of urx reader / writer to merge it in a class.

  * Speedup recursion detection between DestinationLink and IGroup.

#### Bug fixes

  * Don't lose empty vector in HwConfig.

### MATLAB

#### Bug fixes

  * Every bug fixes in urx v1.2.0

  * Fix fields of inherited classes (IGroup) when they are initialized by `loadFromFile` function.

  * Hide private field in `TransmitSetup` and `Wave` classes.

#### What's new (improvement)

  * Merge Windows and Linux toolbox.

## 1.1.0

### C++

#### Bug fixes

Fix default version number in Version class.

### MATLAB

#### What's new

Add support for MATLAB 2018.

Implement `eq` / `==` feature. These fonction compare C pointer and not content of fields.

Implement `isequaln` feature. This fonction compares content of fields. `isequal` do the same than `isequaln` function.

Handle exception for `uac.loadFromFile` / `uac.saveToFile`.

Add a constructor with 3 parameters to `Vector3D`.

#### API break

`uac.Dataset.saveToFile` / `uac.Dataset.loadFromFile` / `uac.Dataset.toUrx` is deprecated. Use `uac.saveToFile` / `uac.loadFromFile` and `uac.toUrx` instead.

#### Bug fixes

Improve stability of MATLAB implementation.

### Python

#### What's new

Implement of `__float__`, `__array__` and `__repr__` for `DoubleNan` to ease usage.

Implement of `__array__` for `Vector3D` to ease usage and add a constructor that allow `np.array` of `list` with a size of 3.

Handle exception for `uac.loadFromFile` / `uac.saveToFile` with `ReadFileException` and `WriteFileException`.

Fix support for Python 3.8 to 3.13 and for pypy 3.8 to 3.10.

## 1.0.2

Fix bad cast from signed to unsigned number.

## 1.0.1

Fix reader. Groups may have destination link to a group that has not yet been read.

## 1.0.0

Initial release.
