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

add_repo_test(test_candidate test_candidate_repository.cpp)
add_repo_test(test_city      test_city_repository.cpp)
add_repo_test(test_result    test_result_repository.cpp)
add_repo_test(test_vote      test_vote_repository.cpp)
