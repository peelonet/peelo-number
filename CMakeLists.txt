cmake_minimum_required(VERSION 3.6)

project(
  PeeloNumber
  VERSION 2.1.0
  DESCRIPTION "C++17 wrapper for GNU MPFR with measurement units"
  HOMEPAGE_URL "https://github.com/peelonet/peelo-number"
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
cmake_policy(SET CMP0074 NEW)
find_package(MPFR REQUIRED)

add_library(
  PeeloNumber
  ./src/arithmetic.cpp
  ./src/assign.cpp
  ./src/comparison.cpp
  ./src/conversion.cpp
  ./src/format.cpp
  ./src/math.cpp
  ./src/number.cpp
  ./src/parser.cpp
  ./src/rounding.cpp
  ./src/unit.cpp
  ./src/utils.cpp
)

target_include_directories(
  PeeloNumber
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${MPFR_INCLUDE_DIR}
)

target_link_libraries(
  PeeloNumber
  ${MPFR_LIBRARY}
)

if(MSVC)
  target_compile_options(
    PeeloNumber
    PRIVATE
      /W4 /WX
  )
else()
  target_compile_options(
    PeeloNumber
    PRIVATE
      -Wall -Werror
  )
endif()

target_compile_features(
  PeeloNumber
  PRIVATE
    cxx_std_17
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  install(
    TARGETS
      PeeloNumber
    RUNTIME DESTINATION
      bin
  )
  install(
    DIRECTORY
      ./include/peelo
    DESTINATION
      include
    FILES_MATCHING PATTERN
      "*.hpp"
  )
  enable_testing()
  add_subdirectory(test)
endif()
