/*
 * Pesquisa Eleitoral - PollParser Tests
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <gtest/gtest.h>
#include "poll_parser.h"


// Well-formed file
TEST(PollParserTest, GoodFile_RecordCount) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/poll_good.txt");
    EXPECT_EQ(parser.get_vote_intentions().size(), 3u);
}

TEST(PollParserTest, GoodFile_FirstRecord) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/poll_good.txt");
    const auto& vi = parser.get_vote_intentions()[0];
    EXPECT_EQ(vi.getId(),          1);
    EXPECT_EQ(vi.getDate(),        "2026-03-01");
    EXPECT_EQ(vi.getCity(),        "Sao José dos Campos");
    EXPECT_EQ(vi.getState(),       "SP");
    EXPECT_EQ(vi.getCandidateId(), 10);
}

TEST(PollParserTest, GoodFile_SecondRecord) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/poll_good.txt");
    const auto& vi = parser.get_vote_intentions()[1];
    EXPECT_EQ(vi.getId(),          2);
    EXPECT_EQ(vi.getDate(),        "2026-03-02");
    EXPECT_EQ(vi.getCity(),        "Natal");
    EXPECT_EQ(vi.getState(),       "RN");
    EXPECT_EQ(vi.getCandidateId(), 11);
}

TEST(PollParserTest, GoodFile_ThirdRecord) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/poll_good.txt");
    const auto& vi = parser.get_vote_intentions()[2];
    EXPECT_EQ(vi.getId(),          3);
    EXPECT_EQ(vi.getDate(),        "2026-03-15");
    EXPECT_EQ(vi.getCity(),        "Recife");
    EXPECT_EQ(vi.getState(),       "PE");
    EXPECT_EQ(vi.getCandidateId(), 12);
}

// Malformed file: record 2 has a non-integer
TEST(PollParserTest, MalformedFile_OnlyValidRecordsStored) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/poll_malformed.txt");
    EXPECT_EQ(parser.get_vote_intentions().size(), 2u);
}

TEST(PollParserTest, MalformedFile_FirstValidRecord) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/poll_malformed.txt");
    const auto& vi = parser.get_vote_intentions()[0];
    EXPECT_EQ(vi.getId(),          1);
    EXPECT_EQ(vi.getDate(),        "2026-03-01");
    EXPECT_EQ(vi.getCity(),        "Sao José dos Campos");
    EXPECT_EQ(vi.getState(),       "SP");
    EXPECT_EQ(vi.getCandidateId(), 10);
}

TEST(PollParserTest, MalformedFile_SecondValidRecord) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/poll_malformed.txt");
    const auto& vi = parser.get_vote_intentions()[1];
    EXPECT_EQ(vi.getId(),          3);
    EXPECT_EQ(vi.getDate(),        "2026-03-15");
    EXPECT_EQ(vi.getCity(),        "Recife");
    EXPECT_EQ(vi.getState(),       "PE");
    EXPECT_EQ(vi.getCandidateId(), 12);
}


// Non-existent file
TEST(PollParserTest, NonExistentFile_EmptyResult) {
    pesquisae::core::poll::PollParser parser(TEST_RESOURCES_DIR "/invalid.txt");
    EXPECT_TRUE(parser.get_vote_intentions().empty());
}
