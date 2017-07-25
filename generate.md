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

First try:

```
> tu = createTU("arrow.cc", includes = arrow_dir)
/usr/lib/gcc/x86_64-linux-gnu/5.4.0/../../../../include/c++/5.4.0/bits/c++0x_warning.h:32:2: error: This file requires compiler and library support for the ISO C++ 2011 standard. This support must be enabled with the -std=c++11 or -std=gnu++11 compiler options.
/usr/include/wchar.h:39:11: fatal error: 'stdarg.h' file not found
```

What doesn't have C++ 11 support? Just my call to `createTU()`. Need to
pass in args here.

Then the issue with `stdarg.h not found` still occurs. This shows up in the
[clang FAQ's](https://clang.llvm.org/docs/FAQ.html), so likely that's where
the issue comes up. Duncan recommended a local build of Clang, then I'll
know where everything is.
