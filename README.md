
# A benchmark project for RocksDB/flatbuffers

## Content
This repository derived from [CppCMakeVcpkgTemplate](https://github.com/lukka/CppCMakeVcpkgTemplate). 

Supports `Linux`/`macOS`/`Windows` on `x64` and `arm64`.

## Build

```shell
cmake --preset ninja-multiconfiguration-vcpkg
cmake --build ./builds/ninja-multiconfiguration-vcpkg/ --config Release
```
## Prepare

Generate data for Get/Scan benchmarks:

```text
$ ./builds/ninja-multiconfiguration-vcpkg/Release/prepare_rocksdb
```

## Run

Run all benchmarks:

```text
$ ./builds/ninja-multiconfiguration-vcpkg/Release/benchmark_rocksdb
```

Run Specific benchmark:

```text
$ ./builds/ninja-multiconfiguration-vcpkg/Release/benchmark_rocksdb --benchmark_filter=BM_rocksdb_get 
```

## Reference:

- https://github.com/facebook/rocksdb
- https://github.com/google/benchmark

