Open questions for Duncan:

What does `extern "C"` do?

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

Mon Jul 24 18:01:10 PDT 2017

Moved development onto my Mac laptop using the brew version of llvm 4.0.
Things seem to be working.

## Local issues

Tue Jul 25 14:27:41 PDT 2017

Now I'm trying to make an R package that basically does nothing but load one header file from Arrow, ie. just

```
#include <arrow/array.h>
```

Here's the command and the first error:

```

clang++ -H -std=c++11 -I/Library/Frameworks/R.framework/Resources/include -DNDEBUG -I/Users/clark/dev/arrow/cpp/src -I/usr/local/opt/gettext/include -I/usr/local/opt/llvm/include    -fPIC  -Wall -mtune=core2 -g -O2 -c Rarrow.cpp -o Rarrow.o

In file included from Rarrow.cpp:3:
In file included from /Users/clark/dev/arrow/cpp/src/arrow/array.h:28:
In file included from /Users/clark/dev/arrow/cpp/src/arrow/type.h:24:
In file included from /usr/local/Cellar/llvm/4.0.0_1/bin/../include/c++/v1/ostream:138:
In file included from /usr/local/Cellar/llvm/4.0.0_1/bin/../include/c++/v1/ios:216:
/usr/local/Cellar/llvm/4.0.0_1/bin/../include/c++/v1/__locale:870:34: error: too many arguments provided to function-like macro invocation
    int length(state_type& __st, const extern_type* __frm, const extern_type* __end, size_t __mx) const
                                 ^
```

This means that `Rarrow.cpp` included `arrow/array.h` on line 3. Then array.h
included type.h, and type.h included ostream, and so on. The error therefore
happens internally in `/usr/local/Cellar/llvm`. This means that it's probably some
configuration error on my part.

In particular the problem is in:

```
/usr/local/Cellar/llvm/4.0.0_1/bin/../include/c++/v1/__locale:870:34
```

Looking again at the error messages, many of the problems come back to this
particular file.

This is going into `/usr/local/Cellar`, which is where Homebrew installs
things. Did my homebrew install llvm correctly?

I can isolate this issue further:

```
echo "#include <ostream>" > test.cc

# Do I need to do this?
echo "std::ostream & Print (std::ostream & stream);" >> test.cc

clang++ -H -std=c++11 -DNDEBUG -I/usr/local/opt/llvm/include -fPIC  -Wall -mtune=core2 -g -O2 -c test.cc -o test.o

clang++ -H -std=c++11 -DNDEBUG -fPIC  -Wall -mtune=core2 -g -O2 -c test.cc -o test.o

```

Both of the above run with no issue. Is this because nothing actually happens
since there's essentially no code, or for some other reason?

__Question__
Why does this get included from /usr/local/Cellar? How do I find out where and
how files are included?

__Answer__
Use the `-H` flag to view which files get included. [Stack Overflow](https://stackoverflow.com/questions/5834778/how-to-tell-where-a-header-file-is-included-from)


Wed Jul 26 09:40:02 PDT 2017

Went through and reinstalled Apple's Xcode. 

```
clang++ -std=c++11 -I/Library/Frameworks/R.framework/Resources/include -DNDEBUG -I/Users/clark/dev/arrow/cpp/src -I/usr/local/opt/gettext/include -I/usr/local/opt/llvm/include    -fPIC  -Wall -mtune=core2 -g -O2 -c Rarrow.cpp -o Rarrow.o
In file included from Rarrow.cpp:3:
In file included from /Users/clark/dev/arrow/cpp/src/arrow/array.h:28:
In file included from /Users/clark/dev/arrow/cpp/src/arrow/type.h:24:
In file included from /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../include/c++/v1/ostream:138:
In file included from /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../include/c++/v1/ios:216:
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../include/c++/v1/__locale:871:34: error: too many arguments provided to function-like
      macro invocation
    int length(state_type& __st, const extern_type* __frm, const extern_type* __end, size_t __mx) const
```

This is exactly the same issue, just from a different underlying library. Then is the issue with my `clang++`? 

```

which clang++ | xargs ls -l

```

```

clang++ -H -std=c++11 -I/Library/Frameworks/R.framework/Resources/include -DNDEBUG -I/Users/clark/dev/arrow/cpp/src -I/usr/local/opt/gettext/include -I/usr/local/opt/llvm/include    -fPIC  -Wall -mtune=core2 -g -O2 -c test.cc -o test.o

clang++ -H -std=c++11 -I/Library/Frameworks/R.framework/Resources/include -DNDEBUG -I/Users/clark/dev/arrow/cpp/src -I/usr/local/opt/gettext/include -I/usr/local/opt/llvm/include    -fPIC  -Wall -mtune=core2 -g -O2 -c test2.cc -o test2.o

```

Both of these also work fine.


All of the issues disappeared when I removed the line `#include <Rdefines.h>`.
So there was some bad interaction with the preprocessing that happens in 
`<Rdefines.h>` before arrow includes the standard libraries. TODO: see if
Duncan has any insight on this.


