/*
 * Pesquisa Eleitoral - City Repository Tests
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
#include "city_repository.h"

class CityRepositoryTest : public ::testing::Test {
protected:
    std::unique_ptr<SQLite::Database> db;

    void SetUp() override {
        db = std::make_unique<SQLite::Database>(
            ":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db->exec(
            "CREATE TABLE city ("
            "    id    INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    state CHAR(2)     NOT NULL,"
            "    name  VARCHAR(64) NOT NULL,"
            "    tier  INTEGER     NOT NULL CHECK (tier BETWEEN 1 AND 4),"
            "    UNIQUE (state, name)"
            ")"
        );
    }
};

TEST_F(CityRepositoryTest, InsertAndFindById) {
    pesquisae::core::database::CityRepository repo(*db);
    repo.insert({0, "SP", "São José dos Campos", 3});
    int id = static_cast<int>(db->getLastInsertRowid());

    pesquisae::core::database::City found = repo.find_by_id(id);
    EXPECT_EQ(found.id,    id);
    EXPECT_EQ(found.state, "SP");
    EXPECT_EQ(found.name,  "São José dos Campos");
    EXPECT_EQ(found.tier,  3);
}

TEST_F(CityRepositoryTest, Update) {
    pesquisae::core::database::CityRepository repo(*db);
    repo.insert({0, "SP", "São José dos Campos", 3});
    int id = static_cast<int>(db->getLastInsertRowid());

    repo.update({id, "SP", "São José dos Campos Updated", 4});

    pesquisae::core::database::City found = repo.find_by_id(id);
    EXPECT_EQ(found.name, "São José dos Campos Updated");
    EXPECT_EQ(found.tier, 4);
}

TEST_F(CityRepositoryTest, FindAll) {
    pesquisae::core::database::CityRepository repo(*db);
    repo.insert({0, "SP", "São José dos Campos", 4});
    repo.insert({0, "RN", "Natal", 4});

    auto all = repo.find_all();
    EXPECT_EQ(all.size(), 2u);
}

TEST_F(CityRepositoryTest, Remove) {
    pesquisae::core::database::CityRepository repo(*db);
    repo.insert({0, "SP", "São José dos Campos", 3});
    int id = static_cast<int>(db->getLastInsertRowid());
    repo.remove(id);

    EXPECT_THROW(repo.find_by_id(id), std::runtime_error);
}

TEST_F(CityRepositoryTest, FindByIdNotFound) {
    pesquisae::core::database::CityRepository repo(*db);
    EXPECT_THROW(repo.find_by_id(999), std::runtime_error);
}

TEST_F(CityRepositoryTest, FindAllEmpty) {
    pesquisae::core::database::CityRepository repo(*db);
    auto all = repo.find_all();
    EXPECT_TRUE(all.empty());
}

TEST_F(CityRepositoryTest, FindByState) {
    pesquisae::core::database::CityRepository repo(*db);
    repo.insert({0, "SP", "São José dos Campos", 4});
    repo.insert({0, "RN", "Natal", 4});

    auto sp = repo.find_by_state("SP");
    EXPECT_EQ(sp.size(), 1u);

    auto rn = repo.find_by_state("RN");
    ASSERT_EQ(rn.size(), 1u);
    EXPECT_EQ(rn[0].name, "Natal");
}

TEST_F(CityRepositoryTest, FindByStateEmpty) {
    pesquisae::core::database::CityRepository repo(*db);
    auto result = repo.find_by_state("AB");
    EXPECT_TRUE(result.empty());
}

TEST_F(CityRepositoryTest, FindByStateAndName_Found) {
    pesquisae::core::database::CityRepository repo(*db);
    repo.insert({0, "SP", "São José dos Campos", 3});
    repo.insert({0, "RN", "Natal", 4});

    pesquisae::core::database::City found = repo.find_by_state_and_name("SP", "São José dos Campos");
    EXPECT_EQ(found.state, "SP");
    EXPECT_EQ(found.name,  "São José dos Campos");
    EXPECT_EQ(found.tier,  3);
}

TEST_F(CityRepositoryTest, FindByStateAndName_WrongState) {
    pesquisae::core::database::CityRepository repo(*db);
    repo.insert({0, "SP", "São José dos Campos", 3});

    // Same name, different state — must not be found.
    EXPECT_THROW(
        repo.find_by_state_and_name("RN", "São José dos Campos"),
        std::runtime_error);
}

TEST_F(CityRepositoryTest, FindByStateAndName_NotFound) {
    pesquisae::core::database::CityRepository repo(*db);
    EXPECT_THROW(
        repo.find_by_state_and_name("SP", "Nonexistent City"),
        std::runtime_error);
}
