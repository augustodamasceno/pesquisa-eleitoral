# Pesquisa Eleitoral - Tests
# Copyright (c) 2026, Augusto Damasceno.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#
# See https://github.com/augustodamasceno/pesquisa-eleitoral

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

function(add_repo_test TARGET SRC)
    add_executable(${TARGET} ${SRC})
    target_link_libraries(${TARGET} PRIVATE core GTest::gtest_main)
    add_test(NAME ${TARGET} COMMAND ${TARGET})
endfunction()

add_repo_test(test_candidate    test_candidate_repository.cpp)
add_repo_test(test_city         test_city_repository.cpp)
add_repo_test(test_result       test_result_repository.cpp)
add_repo_test(test_vote         test_vote_repository.cpp)
add_repo_test(test_ibge         test_ibge_demographic.cpp)
add_repo_test(test_integration_ibge_database_update  test_integration_ibge_database_update.cpp)

add_executable(test_poll_parser test_poll_parser.cpp)
target_link_libraries(test_poll_parser PRIVATE core GTest::gtest_main)
target_compile_definitions(test_poll_parser PRIVATE
    TEST_RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources"
)
add_test(NAME test_poll_parser COMMAND test_poll_parser)
