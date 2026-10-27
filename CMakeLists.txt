cmake_minimum_required(VERSION 3.21)
project(vipermuffin_codes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)            # bump to 23 if your compilers are new enough
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # for clangd / VS Code

# clangd looks for compile_commands.json in the source tree, so link the most
# recently configured build's copy there (Xcode doesn't generate one).
if(PROJECT_IS_TOP_LEVEL AND CMAKE_GENERATOR MATCHES "Makefiles|Ninja")
  file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json
       ${CMAKE_SOURCE_DIR}/compile_commands.json SYMBOLIC)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)  # part 3 inputs can be big
endif()

option(VMC_SANITIZE "Build with address + undefined-behaviour sanitizers" OFF)

add_library(vmc_options INTERFACE)
if(MSVC)
  target_compile_options(vmc_options INTERFACE /W4 /permissive-)
else()
  target_compile_options(vmc_options INTERFACE -Wall -Wextra -Wpedantic -Wshadow)
  if(VMC_SANITIZE)
    target_compile_options(vmc_options INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(vmc_options INTERFACE -fsanitize=address,undefined)
  endif()
endif()

# --- googletest -------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# Keep gtest out of compile_commands.json, or clangd may borrow its flags for lib/ headers.
set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES EXPORT_COMPILE_COMMANDS OFF)

# --- shared puzzle library (header-only) ------------------------------------
add_library(vmclib INTERFACE)
target_include_directories(vmclib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(vmclib INTERFACE vmc_options)

# --- every quest under events/<event>/qNN/ -----------------------------------
# CONFIGURE_DEPENDS: new quest folders are picked up on the next build.
file(GLOB_RECURSE VMC_ALL_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/events/*.cpp)
set(VMC_QUEST_SOURCES ${VMC_ALL_SOURCES})
set(VMC_TEST_SOURCES ${VMC_ALL_SOURCES})
list(FILTER VMC_QUEST_SOURCES EXCLUDE REGEX "_test\\.cpp$")
list(FILTER VMC_TEST_SOURCES INCLUDE REGEX "_test\\.cpp$")

# Not compiled; listed so IDEs (Xcode especially) show them next to the code.
file(GLOB_RECURSE VMC_QUEST_EXTRAS CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/events/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/*/NOTES.md
  ${CMAKE_CURRENT_SOURCE_DIR}/events/*/notes_p*.txt)
set_source_files_properties(${VMC_QUEST_EXTRAS} PROPERTIES HEADER_FILE_ONLY ON)
# Mirror events/<event>/qNN/ as groups in the IDE instead of one flat list.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/events PREFIX events
  FILES ${VMC_ALL_SOURCES} ${VMC_QUEST_EXTRAS})

# Object library so each quest's self-registration is linked into both
# the runner and the tests (a static lib would drop it).
add_library(quests OBJECT ${VMC_QUEST_SOURCES} ${VMC_QUEST_EXTRAS})
target_link_libraries(quests PUBLIC vmclib)

add_executable(vmc runner/main.cpp)
target_link_libraries(vmc PRIVATE quests)

enable_testing()
add_executable(vmc_tests tests/lib_test.cpp ${VMC_TEST_SOURCES})
target_link_libraries(vmc_tests PRIVATE quests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(vmc_tests)
