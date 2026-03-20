/*
 * Pesquisa Eleitoral - Vote Repository Tests
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
#include "vote_repository.h"

class VoteRepositoryTest : public ::testing::Test {
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
            "CREATE TABLE city ("
            "    id    INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    state CHAR(2)     NOT NULL,"
            "    name  VARCHAR(64) NOT NULL,"
            "    tier  INTEGER     NOT NULL,"
            "    UNIQUE (state, name)"
            ")"
        );
        db->exec(
            "CREATE TABLE vote ("
            "    id           INTEGER PRIMARY KEY,"
            "    date         DATE    NOT NULL,"
            "    city_id      INTEGER NOT NULL REFERENCES city(id),"
            "    candidate_id INTEGER NOT NULL REFERENCES candidate(id)"
            ")"
        );
        db->exec("INSERT INTO candidate (id, name, party) VALUES (1, 'Candidate1', 'P1')");
        db->exec("INSERT INTO candidate (id, name, party) VALUES (2, 'Candidate2', 'P2')");
        db->exec("INSERT INTO city (state, name, tier) VALUES ('SP', 'São José dos Campos', 4)");
        db->exec("INSERT INTO city (state, name, tier) VALUES ('RN', 'Natal', 4)");
    }
};

TEST_F(VoteRepositoryTest, InsertAndFindById) {
    pesquisae::core::database::VoteRepository repo(*db);
    repo.insert({1, "2026-02-01", 1, 1});

    pesquisae::core::database::Vote found = repo.find_by_id(1);
    EXPECT_EQ(found.id,           1);
    EXPECT_EQ(found.date,         "2026-02-01");
    EXPECT_EQ(found.city_id,      1);
    EXPECT_EQ(found.candidate_id, 1);
}

TEST_F(VoteRepositoryTest, Update) {
    pesquisae::core::database::VoteRepository repo(*db);
    repo.insert({1, "2026-02-01", 1, 1});
    repo.update({1, "2026-02-02", 2, 2});

    pesquisae::core::database::Vote found = repo.find_by_id(1);
    EXPECT_EQ(found.date,         "2026-02-02");
    EXPECT_EQ(found.city_id,      2);
    EXPECT_EQ(found.candidate_id, 2);
}

TEST_F(VoteRepositoryTest, FindAll) {
    pesquisae::core::database::VoteRepository repo(*db);
    repo.insert({1, "2026-02-01", 1, 1});
    repo.insert({2, "2026-02-01", 2, 2});
    repo.insert({3, "2026-02-01", 1, 2});

    auto all = repo.find_all();
    EXPECT_EQ(all.size(), 3u);
}

TEST_F(VoteRepositoryTest, Remove) {
    pesquisae::core::database::VoteRepository repo(*db);
    repo.insert({1, "2026-02-01", 1, 1});
    repo.remove(1);

    EXPECT_THROW(repo.find_by_id(1), std::runtime_error);
}

TEST_F(VoteRepositoryTest, FindByIdNotFound) {
    pesquisae::core::database::VoteRepository repo(*db);
    EXPECT_THROW(repo.find_by_id(999), std::runtime_error);
}

TEST_F(VoteRepositoryTest, FindAllEmpty) {
    pesquisae::core::database::VoteRepository repo(*db);
    auto all = repo.find_all();
    EXPECT_TRUE(all.empty());
}

TEST_F(VoteRepositoryTest, FindByCandidate) {
    pesquisae::core::database::VoteRepository repo(*db);
    repo.insert({1, "2026-02-01", 1, 1});
    repo.insert({2, "2026-02-01", 2, 2});
    repo.insert({3, "2026-02-01", 1, 1});

    auto alice = repo.find_by_candidate(1);
    EXPECT_EQ(alice.size(), 2u);

    auto bob = repo.find_by_candidate(2);
    ASSERT_EQ(bob.size(), 1u);
    EXPECT_EQ(bob[0].id, 2);
}

TEST_F(VoteRepositoryTest, FindByCandidateEmpty) {
    pesquisae::core::database::VoteRepository repo(*db);
    auto result = repo.find_by_candidate(999);
    EXPECT_TRUE(result.empty());
}

TEST_F(VoteRepositoryTest, FindByCity) {
    pesquisae::core::database::VoteRepository repo(*db);
    repo.insert({1, "2026-02-01", 1, 1});
    repo.insert({2, "2026-02-01", 2, 2});
    repo.insert({3, "2026-02-01", 1, 2});

    auto sp = repo.find_by_city(1);
    EXPECT_EQ(sp.size(), 2u);

    auto rj = repo.find_by_city(2);
    ASSERT_EQ(rj.size(), 1u);
    EXPECT_EQ(rj[0].id, 2);
}

TEST_F(VoteRepositoryTest, FindByCityEmpty) {
    pesquisae::core::database::VoteRepository repo(*db);
    auto result = repo.find_by_city(999);
    EXPECT_TRUE(result.empty());
}

TEST_F(VoteRepositoryTest, FindByDate) {
    pesquisae::core::database::VoteRepository repo(*db);
    repo.insert({1, "2026-02-01", 1, 1});
    repo.insert({2, "2026-02-01", 2, 2});
    repo.insert({3, "2026-03-01", 1, 1});

    auto feb = repo.find_by_date("2026-02-01");
    EXPECT_EQ(feb.size(), 2u);

    auto mar = repo.find_by_date("2026-03-01");
    ASSERT_EQ(mar.size(), 1u);
    EXPECT_EQ(mar[0].id, 3);
}

TEST_F(VoteRepositoryTest, FindByDateEmpty) {
    pesquisae::core::database::VoteRepository repo(*db);
    auto result = repo.find_by_date("1991-01-01");
    EXPECT_TRUE(result.empty());
}
