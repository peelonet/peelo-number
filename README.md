# peelo-number

![build](https://github.com/peelonet/peelo-number/workflows/build/badge.svg)

[C++17] library that implements multiple-precision floating-point decimals with
measurement unit support using [GNU MPFR] library.

[Doxygen generated API documentation](https://peelonet.github.io/peelo-number/)

## WebAssembly

This library can be compiled to WebAssembly with [Emscripten] and used as a
static dependency in other Emscripten projects.

### Build

Install the [Emscripten SDK], then run:

```bash
./scripts/build-wasm.sh
```

This builds GMP and MPFR for WebAssembly (into `wasm-deps/`), compiles
peelo-number, runs the test suite, and installs everything under
`wasm-deps/install/`.

To build only the dependencies:

```bash
./scripts/build-wasm-deps.sh
```

### Use as a dependency

After building, point CMake at the install prefix:

```bash
source /path/to/emsdk/emsdk_env.sh
emcmake cmake -B build -S your-project \
  -DCMAKE_PREFIX_PATH=/path/to/peelo-number/wasm-deps/install \
  -DPeeloNumber_DIR=/path/to/peelo-number/wasm-deps/install/lib/cmake/PeeloNumber
```

When cross-compiling with Emscripten, also set
`CMAKE_FIND_ROOT_PATH_MODE_PACKAGE` to `BOTH` in your `CMakeLists.txt` so
`find_package` can locate libraries outside the sysroot.

Then link against the imported target:

```cmake
if(EMSCRIPTEN)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
endif()

find_package(PeeloNumber REQUIRED)
target_link_libraries(your_target PRIVATE PeeloNumber::PeeloNumber)
```

The install prefix also contains `libgmp.a` and `libmpfr.a`. When linking
manually, link in this order: `PeeloNumber`, `mpfr`, `gmp`.

Because peelo-number throws C++ exceptions (for example `peelo::number::unit_error`),
compile and link your project with `-fwasm-exceptions`.

### Example consumer

See [`examples/wasm-consumer/`](examples/wasm-consumer/) for a minimal project that
links against the WebAssembly build. Build and run it with:

```bash
./scripts/build-wasm.sh
./examples/wasm-consumer/build.sh
```

Expected output:

```
1.25 + 2.75 = 4
1km + 500m = 1500m
```

[C++17]: https://en.cppreference.com/w/cpp/17
[Emscripten]: https://emscripten.org/
[Emscripten SDK]: https://emscripten.org/docs/getting-started/downloads.html
[GNU MPFR]: https://www.mpfr.org/
