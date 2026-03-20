/*
 * Pesquisa Eleitoral - Log Repository Tests
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <memory>
#include <stdexcept>

#include <gtest/gtest.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "log_repository.h"

class LogRepositoryTest : public ::testing::Test {
protected:
    std::unique_ptr<SQLite::Database> db;

    void SetUp() override {
        db = std::make_unique<SQLite::Database>(
            ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db->exec(
            "CREATE TABLE log ("
            "    id        INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    timestamp TEXT    NOT NULL DEFAULT (datetime('now')),"
            "    level     VARCHAR(16) NOT NULL,"
            "    source    VARCHAR(128),"
            "    message   TEXT    NOT NULL"
            ")"
        );
    }
};

TEST_F(LogRepositoryTest, InsertAndFindById) {
    pesquisae::core::database::LogRepository repo(*db);
    repo.insert({0, "2026-03-20 10:00:00", "INFO", "poll_parser", "Parsed successfully"});

    auto all = repo.find_all();
    ASSERT_EQ(all.size(), 1u);

    auto found = repo.find_by_id(all[0].id);
    EXPECT_EQ(found.level,   "INFO");
    EXPECT_EQ(found.source,  "poll_parser");
    EXPECT_EQ(found.message, "Parsed successfully");
}

TEST_F(LogRepositoryTest, Update) {
    pesquisae::core::database::LogRepository repo(*db);
    repo.insert({0, "2026-03-20 10:00:00", "INFO", "source_a", "original"});

    auto all = repo.find_all();
    ASSERT_EQ(all.size(), 1u);
    int id = all[0].id;

    repo.update({id, "2026-03-20 10:01:00", "WARNING", "source_a", "updated"});

    auto found = repo.find_by_id(id);
    EXPECT_EQ(found.level,   "WARNING");
    EXPECT_EQ(found.message, "updated");
}

TEST_F(LogRepositoryTest, FindAll) {
    pesquisae::core::database::LogRepository repo(*db);
    repo.insert({0, "2026-03-20 10:00:00", "INFO",    "src1", "msg1"});
    repo.insert({0, "2026-03-20 10:01:00", "WARNING", "src2", "msg2"});
    repo.insert({0, "2026-03-20 10:02:00", "ERROR",   "src3", "msg3"});

    auto all = repo.find_all();
    EXPECT_EQ(all.size(), 3u);
}

TEST_F(LogRepositoryTest, FindByLevel) {
    pesquisae::core::database::LogRepository repo(*db);
    repo.insert({0, "2026-03-20 10:00:00", "INFO",    "src1", "msg1"});
    repo.insert({0, "2026-03-20 10:01:00", "ERROR",   "src2", "msg2"});
    repo.insert({0, "2026-03-20 10:02:00", "INFO",    "src3", "msg3"});

    auto errors = repo.find_by_level("ERROR");
    ASSERT_EQ(errors.size(), 1u);
    EXPECT_EQ(errors[0].source, "src2");

    auto infos = repo.find_by_level("INFO");
    EXPECT_EQ(infos.size(), 2u);
}

TEST_F(LogRepositoryTest, Remove) {
    pesquisae::core::database::LogRepository repo(*db);
    repo.insert({0, "2026-03-20 10:00:00", "INFO", "src", "msg"});

    auto all = repo.find_all();
    ASSERT_EQ(all.size(), 1u);
    int id = all[0].id;

    repo.remove(id);
    EXPECT_THROW(repo.find_by_id(id), std::runtime_error);
}

TEST_F(LogRepositoryTest, FindByIdNotFound) {
    pesquisae::core::database::LogRepository repo(*db);
    EXPECT_THROW(repo.find_by_id(999), std::runtime_error);
}

TEST_F(LogRepositoryTest, FindAllEmpty) {
    pesquisae::core::database::LogRepository repo(*db);
    auto all = repo.find_all();
    EXPECT_TRUE(all.empty());
}
