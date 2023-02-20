# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Find dependencies provided by vcpkg (via vcpkg.cmake)
# find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(flatbuffers CONFIG REQUIRED)
find_package(RocksDB CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)

# main target
add_executable(main)
target_sources(main PRIVATE src/main.cpp)
# target_link_libraries(main PRIVATE fmt::fmt)
target_link_libraries(main PRIVATE fmt::fmt fmt::fmt-header-only)
set_property(TARGET main PROPERTY CXX_STANDARD 20)

add_executable(sample_binary)
target_sources(sample_binary PRIVATE src/sample_binary.cpp)
target_link_libraries(sample_binary PRIVATE fmt::fmt flatbuffers::flatbuffers)
set_property(TARGET sample_binary PROPERTY CXX_STANDARD 20)

add_executable(sample_bfbs)
target_sources(sample_bfbs PRIVATE src/sample_bfbs.cpp)
target_link_libraries(sample_bfbs PRIVATE fmt::fmt flatbuffers::flatbuffers)
set_property(TARGET sample_bfbs PROPERTY CXX_STANDARD 20)

add_executable(sample_text)
target_sources(sample_text PRIVATE src/sample_text.cpp)
target_link_libraries(sample_text PRIVATE fmt::fmt flatbuffers::flatbuffers)
set_property(TARGET sample_text PROPERTY CXX_STANDARD 20)

add_executable(main_txn_rocksdb)
target_sources(main_txn_rocksdb PRIVATE src/main_txn_rocksdb.cpp src/utils.cpp)
target_link_libraries(main_txn_rocksdb PRIVATE fmt::fmt RocksDB::rocksdb)
set_property(TARGET main_txn_rocksdb PROPERTY CXX_STANDARD 20)

add_executable(prepare_rocksdb)
target_sources(prepare_rocksdb PRIVATE src/prepare_rocksdb.cpp src/utils.cpp)
target_link_libraries(prepare_rocksdb PRIVATE fmt::fmt RocksDB::rocksdb)
set_property(TARGET prepare_rocksdb PROPERTY CXX_STANDARD 20)

add_executable(benchmark_rocksdb)
target_sources(benchmark_rocksdb PRIVATE src/benchmark_rocksdb.cpp src/utils.cpp)
target_link_libraries(benchmark_rocksdb PRIVATE fmt::fmt benchmark::benchmark benchmark::benchmark_main RocksDB::rocksdb)
set_property(TARGET benchmark_rocksdb PROPERTY CXX_STANDARD 20)

add_executable(benchmark_txn_rocksdb)
target_sources(benchmark_txn_rocksdb PRIVATE src/benchmark_txn_rocksdb.cpp)
target_link_libraries(benchmark_txn_rocksdb PRIVATE fmt::fmt benchmark::benchmark benchmark::benchmark_main RocksDB::rocksdb)
set_property(TARGET benchmark_txn_rocksdb PROPERTY CXX_STANDARD 20)

add_executable(benchmark)
target_sources(benchmark PRIVATE src/benchmark.cpp)
target_link_libraries(benchmark PRIVATE benchmark::benchmark benchmark::benchmark_main)
set_property(TARGET benchmark PROPERTY CXX_STANDARD 20)

# tests target
add_executable(tests)
target_sources(tests PRIVATE src/main.cpp)
target_link_libraries(tests PRIVATE fmt::fmt)
set_property(TARGET tests PROPERTY CXX_STANDARD 20)
enable_testing()
add_test(tests tests)
