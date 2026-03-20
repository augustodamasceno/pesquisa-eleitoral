# Pesquisa Eleitoral
# Copyright (c) 2026, Augusto Damasceno.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#
# See https://github.com/augustodamasceno/pesquisa-eleitoral
#
# Build:
#   cmake -B build && cmake --build build
#   ctest --test-dir build

cmake_minimum_required(VERSION 3.14)
project(pesquisa-eleitoral CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG        3.3.3
)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SQLiteCpp)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.14.2
)
set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(core STATIC
    src/core/candidate_repository.cpp
    src/core/city_repository.cpp
    src/core/result_repository.cpp
    src/core/vote_repository.cpp
    src/core/ibge_demographic.cpp
    src/core/vote_intention.cpp
    src/core/poll_parser.cpp
)
target_include_directories(core PUBLIC src/core)
target_link_libraries(core PUBLIC SQLiteCpp cpr::cpr nlohmann_json::nlohmann_json)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
