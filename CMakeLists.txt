# CMake version
cmake_minimum_required (VERSION 3.9)
MESSAGE(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT})

# Project name
project(WorkStealingQueue VERSION 1.0.0 LANGUAGES CXX)

# Turn on the verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

# Compiler vendors
## g++
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.3")
    message(FATAL_ERROR "\nCpp-Taskflow requires g++ at least v7.3")
  endif()
## clang++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6.0")
    message(FATAL_ERROR "\nCpp-Taskflow requires clang++ at least v6.0")
  endif() 
## AppleClang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "11.0")
    message(FATAL_ERROR "\nCpp-Taskflow requires AppleClang at least v11.0")
  endif()
## microsoft visual c++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  if(NOT MSVC_VERSION GREATER_EQUAL 1914)
    message(FATAL_ERROR "\nCpp-Taskflow requires MSVC++ at least v14.14") 
  endif()
else()
  message(FATAL_ERROR "\n\
Cpp-Taskflow currently supports the following compilers:\n\
- g++ v7.3 or above\n\
- clang++ v6.0 or above\n\
- MSVC++ v19.14 or above\n\
")
endif()

# defult release build
set(WSQ_DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${WSQ_DEFAULT_BUILD_TYPE}'")
  set(
    CMAKE_BUILD_TYPE "${WSQ_DEFAULT_BUILD_TYPE}" 
    CACHE
    STRING "Choose the type of build." 
    FORCE
  )
  # Set the possible values of build type for cmake-gui
  set_property(
    CACHE 
    CMAKE_BUILD_TYPE 
    PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
  )
endif()


# error setting
add_library(error_settings INTERFACE)
add_library(wsq::error_settings ALIAS error_settings)

target_compile_options(
  error_settings
  INTERFACE
  $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wfatal-errors>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wfatal-errors>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wfatal-errors>
  $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive->
)

# additional features
add_library(features_settings INTERFACE)
add_library(wsq::features_settings ALIAS features_settings)
target_compile_definitions(
  features_settings 
  INTERFACE 
  $<$<CXX_COMPILER_ID:MSVC>:_ENABLE_EXTENDED_ALIGNED_STORAGE>
)

# optimization
#
##! Msvc flags info
# /Zi - Produces a program database (PDB) that contains type information 
#       and symbolic debugging information for use with the debugger.
# /FS - Allows multiple cl.exe processes to write to the same .pdb file
# /DEBUG - Enable debug during linking
# /Od - Disables optimization
# /Ox - Full optimization
# /Oy- do not suppress frame pointers (recommended for debugging)
#
add_library(optimize_settings INTERFACE)
add_library(wsq::optimize_settings ALIAS optimize_settings)

target_compile_options(
  optimize_settings INTERFACE
  $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-O2 -march=native>
  $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:AppleClang>>:-O2 -march=native>
  $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-O2 -march=native>
  $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2 -DNDEBUG /MP>
  $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:-O0 -g>
  $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-O0 -g>
  $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:AppleClang>>:-O0 -g>
  $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Zi /FS /DEBUG /Od /MP /MDd /Oy->
)

add_library(default_settings INTERFACE)
add_library(wsq::default_settings ALIAS default_settings)
target_link_libraries(
  default_settings 
  INTERFACE 
  wsq::error_settings 
  wsq::optimize_settings 
  wsq::features_settings
)

# CXX target properties
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# build options
option(WSQ_BUILD_EXAMPLES "Enables build of examples" ON)
option(WSQ_BUILD_TESTS "Enables build of tests" ON)
option(WSQ_BUILD_BENCHMARKS "Enables build of benchmarks" OFF)

# installation path
set(WSQ_INC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(WSQ_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(WSQ_UTEST_DIR ${PROJECT_SOURCE_DIR}/unittests)
set(WSQ_EXAMPLE_DIR ${PROJECT_SOURCE_DIR}/examples)
set(WSQ_BENCHMARK_DIR ${PROJECT_SOURCE_DIR}/benchmarks)
set(WSQ_3RD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rd-party)

message(STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message(STATUS "CMAKE_CXX_COMPILER_VERSION: " ${CMAKE_CXX_COMPILER_VERSION})
message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "CMAKE_EXE_LINKER_FLAGS: " ${CMAKE_EXE_LINKER_FLAGS})
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
message(STATUS "PROJECT_NAME: " ${PROJECT_NAME})
message(STATUS "WSQ_BUILD_EXAMPLES: " ${WSQ_BUILD_EXAMPLES})
message(STATUS "WSQ_BUILD_TESTS: " ${WSQ_BUILD_TESTS})
message(STATUS "WSQ_BUILD_BENCHMARKS: " ${WSQ_BUILD_BENCHMARKS})
message(STATUS "WSQ_INC_INSTALL_DIR: " ${WSQ_INC_INSTALL_DIR})
message(STATUS "WSQ_LIB_INSTALL_DIR: " ${WSQ_LIB_INSTALL_DIR})
message(STATUS "WSQ_UTEST_DIR: " ${WSQ_UTEST_DIR})
message(STATUS "WSQ_EXAMPLE_DIR: " ${WSQ_EXAMPLE_DIR})
message(STATUS "WSQ_BENCHMARK_DIR: " ${WSQ_BENCHMARK_DIR})
message(STATUS "WSQ_3RD_PARTY_DIR: " ${WSQ_3RD_PARTY_DIR})

# add the binary tree to the search path for include files
include_directories(${PROJECT_SOURCE_DIR})

# -----------------------------------------------------------------------------
# must-have package include
# -----------------------------------------------------------------------------

# Enable test
include(CTest)

# Find pthread package
find_package(Threads REQUIRED)

# -----------------------------------------------------------------------------
# Cpp-Taskflow library interface
# -----------------------------------------------------------------------------

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/> 
)

# -----------------------------------------------------------------------------
# Example program 
# -----------------------------------------------------------------------------

if(${WSQ_BUILD_EXAMPLES})

message(STATUS "Building examples ...")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WSQ_EXAMPLE_DIR})

add_executable(simple ${WSQ_EXAMPLE_DIR}/simple.cpp)
target_link_libraries(
  simple ${PROJECT_NAME} Threads::Threads wsq::default_settings
)

endif()

# -----------------------------------------------------------------------------
# Unittest
# -----------------------------------------------------------------------------

if(${WSQ_BUILD_TESTS})
 
enable_testing()
message(STATUS "Building unit tests ...")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WSQ_UTEST_DIR})

# unittest for WorkStealingQueue 
add_executable(wsq ${WSQ_UTEST_DIR}/wsq.cpp)
target_link_libraries(wsq ${PROJECT_NAME} Threads::Threads)
target_include_directories(wsq PRIVATE ${WSQ_3RD_PARTY_DIR}/doctest wsq::default_settings)
add_test(work_stealing_queue.Owner  ${WSQ_UTEST_DIR}/wsq -tc=WSQ.Owner)
add_test(work_stealing_queue.1Thief ${WSQ_UTEST_DIR}/wsq -tc=WSQ.1Thief)
add_test(work_stealing_queue.2Thieves ${WSQ_UTEST_DIR}/wsq -tc=WSQ.2Thieves)
add_test(work_stealing_queue.3Thieves ${WSQ_UTEST_DIR}/wsq -tc=WSQ.3Thieves)
add_test(work_stealing_queue.4Thieves ${WSQ_UTEST_DIR}/wsq -tc=WSQ.4Thieves)
add_test(work_stealing_queue.5Thieves ${WSQ_UTEST_DIR}/wsq -tc=WSQ.5Thieves)
add_test(work_stealing_queue.6Thieves ${WSQ_UTEST_DIR}/wsq -tc=WSQ.6Thieves)
add_test(work_stealing_queue.7Thieves ${WSQ_UTEST_DIR}/wsq -tc=WSQ.7Thieves)
add_test(work_stealing_queue.8Thieves ${WSQ_UTEST_DIR}/wsq -tc=WSQ.8Thieves)

endif()


