Mon Jul 24 09:30:26 PDT 2017

Notes while generating the R bindings for Arrow.

Side note: What should we target, the C++ API or the Arrow GLib C API?
Python goes directly to the C++, which seems to be more ideal since that's
where the development happens.


## Prerequisites

Starting off with getting the latest version of the library installed.

```
cd ~/dev/arrow/cpp
mkdir release
cd release

cmake .. -DCMAKE_BUILD_TYPE=Release

make unittest
```

Works fine, unit tests pass.

Next is updating to the latest versions of Duncan's code generation stuff.
Ok, running latest versions of RCIndex (aka RClangSimple) and RCodeGen from
Github.

Looking at the [Rtesseract](git@github.com:duncantl/Rtesseract.git) project
as an example because Duncan and Matt are working on this now.


## Install

The file `arrow/cpp/src/arrow/api.h` is the target. This file includes all the
other header files. Comment calls it:

> Coarse public API while the library is in development

First thing, let's see if I can instantiate and go between a vector of
doubles for Arrow and R.
