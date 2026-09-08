cmake_minimum_required(VERSION 3.6)

project(
  PeeloNumber
  VERSION 5.0.0
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
  ./src/storage.cpp
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
  PUBLIC
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

if(EMSCRIPTEN)
  target_compile_options(
    PeeloNumber
    PRIVATE
      -fwasm-exceptions
  )
  target_link_options(
    PeeloNumber
    PUBLIC
      -fwasm-exceptions
  )
endif()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  install(
    TARGETS
      PeeloNumber
    EXPORT
      PeeloNumberTargets
    ARCHIVE DESTINATION
      ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION
      ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION
      ${CMAKE_INSTALL_BINDIR}
  )
  install(
    DIRECTORY
      ./include/peelo
    DESTINATION
      ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN
      "*.hpp"
  )
  install(
    EXPORT
      PeeloNumberTargets
    FILE
      PeeloNumberTargets.cmake
    NAMESPACE
      PeeloNumber::
    DESTINATION
      ${CMAKE_INSTALL_LIBDIR}/cmake/PeeloNumber
  )
  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PeeloNumberConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PeeloNumberConfig.cmake
    INSTALL_DESTINATION
      ${CMAKE_INSTALL_LIBDIR}/cmake/PeeloNumber
  )
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PeeloNumberConfigVersion.cmake
    VERSION
      ${PROJECT_VERSION}
    COMPATIBILITY
      SameMajorVersion
  )
  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/PeeloNumberConfig.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/PeeloNumberConfigVersion.cmake
    DESTINATION
      ${CMAKE_INSTALL_LIBDIR}/cmake/PeeloNumber
  )
  enable_testing()
  add_subdirectory(test)
endif()
