/*
 * Pesquisa Eleitoral - Candidate Repository Tests
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <cstdint>
#include <memory>
#include <stdexcept>

#include <gtest/gtest.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "candidate_repository.h"

class CandidateRepositoryTest : public ::testing::Test {
protected:
    std::unique_ptr<SQLite::Database> db;

    void SetUp() override {
        db = std::make_unique<SQLite::Database>(
            ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db->exec(
            "CREATE TABLE candidate ("
            "    id    INTEGER PRIMARY KEY,"
            "    name  VARCHAR(128),"
            "    party VARCHAR(32)"
            ")"
        );
    }
};

TEST_F(CandidateRepositoryTest, InsertAndFindById) {
    CandidateRepository repo(*db);
    repo.insert({1, "Candidate1", "P1"});

    Candidate found = repo.find_by_id(1);
    EXPECT_EQ(found.id,    1);
    EXPECT_EQ(found.name,  "Candidate1");
    EXPECT_EQ(found.party, "P1");
}

TEST_F(CandidateRepositoryTest, Update) {
    CandidateRepository repo(*db);
    repo.insert({1, "Candidate1", "P1"});
    repo.update({1, "Candidate1 Updated", "P2"});

    Candidate found = repo.find_by_id(1);
    EXPECT_EQ(found.name,  "Candidate1 Updated");
    EXPECT_EQ(found.party, "P2");
}

TEST_F(CandidateRepositoryTest, FindAll) {
    CandidateRepository repo(*db);
    repo.insert({1, "Candidate1", "P1"});
    repo.insert({2, "Candidate2", "P2"});
    repo.insert({3, "Candidate3", "P1"});

    auto all = repo.find_all();
    EXPECT_EQ(all.size(), 3u);
}

TEST_F(CandidateRepositoryTest, Remove) {
    CandidateRepository repo(*db);
    repo.insert({1, "Candidate1", "P1"});
    repo.remove(1);

    EXPECT_THROW(repo.find_by_id(1), std::runtime_error);
}

TEST_F(CandidateRepositoryTest, FindByIdNotFound) {
    CandidateRepository repo(*db);
    EXPECT_THROW(repo.find_by_id(999), std::runtime_error);
}

TEST_F(CandidateRepositoryTest, FindAllEmpty) {
    CandidateRepository repo(*db);
    auto all = repo.find_all();
    EXPECT_TRUE(all.empty());
}

TEST_F(CandidateRepositoryTest, FindByParty) {
    CandidateRepository repo(*db);
    repo.insert({1, "Candidate 1", "P1"});
    repo.insert({2, "Candidate 2", "P2"});
    repo.insert({3, "Candidate 3", "P1"});

    auto partyA = repo.find_by_party("P1");
    EXPECT_EQ(partyA.size(), 2u);

    auto partyB = repo.find_by_party("P2");
    ASSERT_EQ(partyB.size(), 1u);
    EXPECT_EQ(partyB[0].name, "Candidate 2");
}

TEST_F(CandidateRepositoryTest, FindByPartyEmpty) {
    CandidateRepository repo(*db);
    auto result = repo.find_by_party("Nonexistent");
    EXPECT_TRUE(result.empty());
}
