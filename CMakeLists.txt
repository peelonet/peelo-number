cmake_minimum_required(VERSION 3.6)

project(
  PeeloNumber
  VERSION 2.0.2
  DESCRIPTION "C++17 wrapper for GNU MPFR with measurement units"
  HOMEPAGE_URL "https://github.com/peelonet/peelo-number"
  LANGUAGES CXX
)

find_path(
  MPFR_INCLUDE_DIR
  NAMES mpfr.h
  DOC "The directory containing MPFR include files."
)
find_library(
  MPFR_LIBRARIES
  NAMES mpfr libmpfr
  DOC "Path to the MPFR library."
)

if(NOT MPFR_LIBRARIES)
  message(FATAL_ERROR "Could not find MPFR library.")
endif()

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
  ${MPFR_LIBRARIES}
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
      ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION
      include
    FILES_MATCHING PATTERN
      "*.hpp"
  )
  enable_testing()
  add_subdirectory(test)
endif()
