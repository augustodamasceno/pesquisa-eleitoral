/*
 * Pesquisa Eleitoral - Result Repository Tests
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
#include "result_repository.h"

class ResultRepositoryTest : public ::testing::Test {
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
        db->exec(
            "CREATE TABLE result ("
            "    id           INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    date         DATE    NOT NULL,"
            "    position     INTEGER NOT NULL,"
            "    votes        INTEGER NOT NULL,"
            "    proportion   REAL    NOT NULL,"
            "    candidate_id INTEGER NOT NULL REFERENCES candidate(id)"
            ")"
        );
        db->exec("INSERT INTO candidate (id, name, party) VALUES (1, 'Candidate1', 'P1')");
        db->exec("INSERT INTO candidate (id, name, party) VALUES (2, 'Candidate2', 'P2')");
    }
};

TEST_F(ResultRepositoryTest, InsertAndFindById) {
    pesquisae::core::database::ResultRepository repo(*db);
    repo.insert({0, "2026-02-01", 1, 500000, 0.5, 1});
    int id = static_cast<int>(db->getLastInsertRowid());

    pesquisae::core::database::Result found = repo.find_by_id(id);
    EXPECT_EQ(found.id,           id);
    EXPECT_EQ(found.date,         "2026-02-01");
    EXPECT_EQ(found.position,     1);
    EXPECT_EQ(found.votes,        500000);
    EXPECT_DOUBLE_EQ(found.proportion, 0.5);
    EXPECT_EQ(found.candidate_id, 1);
}

TEST_F(ResultRepositoryTest, Update) {
    pesquisae::core::database::ResultRepository repo(*db);
    repo.insert({0, "2026-02-01", 1, 500000, 0.5, 1});
    int id = static_cast<int>(db->getLastInsertRowid());

    repo.update({id, "2026-03-02", 2, 600000, 0.75, 2});

    pesquisae::core::database::Result found = repo.find_by_id(id);
    EXPECT_EQ(found.date,         "2026-03-02");
    EXPECT_EQ(found.position,     2);
    EXPECT_EQ(found.votes,        600000);
    EXPECT_DOUBLE_EQ(found.proportion, 0.75);
    EXPECT_EQ(found.candidate_id, 2);
}

TEST_F(ResultRepositoryTest, FindAll) {
    pesquisae::core::database::ResultRepository repo(*db);
    repo.insert({0, "2026-02-01", 1, 500000, 0.5,  1});
    repo.insert({0, "2026-02-01", 2, 400000, 0.25, 2});

    auto all = repo.find_all();
    EXPECT_EQ(all.size(), 2u);
}

TEST_F(ResultRepositoryTest, Remove) {
    pesquisae::core::database::ResultRepository repo(*db);
    repo.insert({0, "2026-02-01", 1, 500000, 0.5, 1});
    int id = static_cast<int>(db->getLastInsertRowid());
    repo.remove(id);

    EXPECT_THROW(repo.find_by_id(id), std::runtime_error);
}

TEST_F(ResultRepositoryTest, FindByIdNotFound) {
    pesquisae::core::database::ResultRepository repo(*db);
    EXPECT_THROW(repo.find_by_id(999), std::runtime_error);
}

TEST_F(ResultRepositoryTest, FindAllEmpty) {
    pesquisae::core::database::ResultRepository repo(*db);
    auto all = repo.find_all();
    EXPECT_TRUE(all.empty());
}

TEST_F(ResultRepositoryTest, FindByDate) {
    pesquisae::core::database::ResultRepository repo(*db);
    repo.insert({0, "2026-02-01", 1, 500000, 0.5,  1});
    repo.insert({0, "2026-02-01", 2, 400000, 0.25, 2});
    repo.insert({0, "2026-03-01", 1, 550000, 0.6,  1});

    auto feb = repo.find_by_date("2026-02-01");
    EXPECT_EQ(feb.size(), 2u);

    auto mar = repo.find_by_date("2026-03-01");
    ASSERT_EQ(mar.size(), 1u);
    EXPECT_EQ(mar[0].votes, 550000);
}

TEST_F(ResultRepositoryTest, FindByDateEmpty) {
    pesquisae::core::database::ResultRepository repo(*db);
    auto result = repo.find_by_date("2000-01-01");
    EXPECT_TRUE(result.empty());
}

TEST_F(ResultRepositoryTest, FindByCandidate) {
    pesquisae::core::database::ResultRepository repo(*db);
    repo.insert({0, "2026-10-01", 1, 500000, 0.5,  1});
    repo.insert({0, "2026-10-01", 2, 400000, 0.25, 2});
    repo.insert({0, "2026-11-01", 1, 550000, 0.6,  1});

    auto alice = repo.find_by_candidate(1);
    EXPECT_EQ(alice.size(), 2u);

    auto bob = repo.find_by_candidate(2);
    ASSERT_EQ(bob.size(), 1u);
    EXPECT_EQ(bob[0].votes, 400000);
}

TEST_F(ResultRepositoryTest, FindByCandidateEmpty) {
    pesquisae::core::database::ResultRepository repo(*db);
    auto result = repo.find_by_candidate(999);
    EXPECT_TRUE(result.empty());
}
