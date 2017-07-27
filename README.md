## Development install

First [install Arrow](https://github.com/apache/arrow/tree/master/cpp) and
the dependencies.

Navigate to `arrow/cpp` in the Arrow source, and build the latest release.

```
mkdir release
cd release
cmake .. -DCMAKE_BUILD_TYPE=Release
make unittest
make install
```

To install Rarrow from within the `Rarrow` directory run:

```
./configure
R CMD INSTALL .
```
