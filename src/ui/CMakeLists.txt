# Pesquisa Eleitoral - Qt UI (standalone build)
# Copyright (c) 2026, Augusto Damasceno.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#
# See https://github.com/augustodamasceno/pesquisa-eleitoral
#
# Build (from this directory):
#   cmake -B build && cmake --build build

cmake_minimum_required(VERSION 3.19)
project(pesquisa-eleitoral-ui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG        3.3.3
)
FetchContent_MakeAvailable(SQLiteCpp)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.10.5
)

set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CURL_ZLIB   ON  CACHE BOOL "" FORCE)
set(CURL_BROTLI OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD   OFF CACHE BOOL "" FORCE)
set(ZLIB_INCLUDE_DIR "C:/Qt/Tools/mingw1310_64/x86_64-w64-mingw32/include"
    CACHE PATH "" FORCE)
set(ZLIB_LIBRARY "C:/Qt/Tools/mingw1310_64/x86_64-w64-mingw32/lib/libz.a"
    CACHE FILEPATH "" FORCE)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

set(CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../core")

add_library(core STATIC
    ${CORE_DIR}/candidate_repository.cpp
    ${CORE_DIR}/city_repository.cpp
    ${CORE_DIR}/log_repository.cpp
    ${CORE_DIR}/result_repository.cpp
    ${CORE_DIR}/result_processor.cpp
    ${CORE_DIR}/vote_repository.cpp
    ${CORE_DIR}/ibge_demographic.cpp
    ${CORE_DIR}/vote_intention.cpp
    ${CORE_DIR}/poll_parser.cpp
)
target_include_directories(core PUBLIC ${CORE_DIR})
target_link_libraries(core PUBLIC SQLiteCpp cpr::cpr nlohmann_json::nlohmann_json)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(ui
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
)

qt_add_translations(
    TARGETS ui
    TS_FILES ui_pt_BR.ts
)

target_link_libraries(ui
    PRIVATE
        Qt::Core
        Qt::Widgets
        core
)

include(GNUInstallDirs)

install(TARGETS ui
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ui
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
