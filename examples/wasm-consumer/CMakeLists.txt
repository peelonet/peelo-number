cmake_minimum_required(VERSION 3.14)

project(
  peelo-number-wasm-example
  LANGUAGES CXX
)

if(EMSCRIPTEN)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
endif()

find_package(PeeloNumber REQUIRED)

add_executable(example main.cpp)

target_link_libraries(
  example
  PRIVATE
    PeeloNumber::PeeloNumber
)

target_compile_features(
  example
  PRIVATE
    cxx_std_17
)

if(EMSCRIPTEN)
  target_compile_options(
    example
    PRIVATE
      -fwasm-exceptions
  )
  target_link_options(
    example
    PRIVATE
      -fwasm-exceptions
  )
endif()
