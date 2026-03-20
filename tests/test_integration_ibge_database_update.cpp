/*
 * Pesquisa Eleitoral - Integration Test: IBGE fetch and City upsert
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "ibge_demographic.h"
#include "city_repository.h"

// 8-city URL covering 2 states and all 4 tiers
static constexpr std::string_view EIGHT_CITIES_URL =
    "https://servicodados.ibge.gov.br/api/v3/agregados/4714/periodos/2022"
    "/variaveis/93?localidades=N6[2404002,2400109,2402006,2409407,2403251,3549904,2408102,3550308]";

namespace pcd = pesquisae::core::database;

class IBGEDatabaseIntegrationTest : public ::testing::Test {
protected:
    std::unique_ptr<SQLite::Database> db;
    std::vector<pcd::City> fetched_cities;

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

        pcd::IBGEFetchResult result =
            pcd::ibge_fetch_cities(std::string(EIGHT_CITIES_URL));
        ASSERT_TRUE(result.ok) << "IBGE fetch failed: " << result.data;
        fetched_cities = pcd::ibge_parse_cities(result);
        ASSERT_EQ(fetched_cities.size(), 8u) << "Expected 8 cities from IBGE";
    }

    const pcd::City* find(const std::vector<pcd::City>& v,
                          const std::string& name, const std::string& state) const {
        auto it = std::find_if(v.begin(), v.end(),
            [&](const pcd::City& c) { return c.name == name && c.state == state; });
        return it != v.end() ? &(*it) : nullptr;
    }
};

// After upsert_all the database must contain exactly 8 rows
TEST_F(IBGEDatabaseIntegrationTest, UpsertInsertsAllFetchedCities) {
    pcd::CityRepository repo(*db);
    repo.upsert_all(fetched_cities);

    EXPECT_EQ(repo.find_all().size(), 8u);
}

// Verify name, state, and tier for each of the 8 expected cities
TEST_F(IBGEDatabaseIntegrationTest, UpsertedCitiesHaveCorrectTiers) {
    pcd::CityRepository repo(*db);
    repo.upsert_all(fetched_cities);

    const auto all = repo.find_all();

    struct Expected { const char* name; const char* state; int tier; };
    const Expected expected[] = {
        {"Frutuoso Gomes",      "RN", 1},   // pop     4,122
        {"Acari",               "RN", 1},   // pop    10,597
        {"Pau dos Ferros",      "RN", 2},   // pop    30,479
        {"Caicó",               "RN", 2},   // pop    61,146
        {"Parnamirim",          "RN", 3},   // pop   252,716
        {"São José dos Campos", "SP", 3},   // pop   697,054
        {"Natal",               "RN", 3},   // pop   751,300
        {"São Paulo",           "SP", 4},   // pop 11,451,999
    };

    for (const auto& e : expected) {
        const pcd::City* c = find(all, e.name, e.state);
        ASSERT_NE(c, nullptr) << e.name << " (" << e.state << ") not found";
        EXPECT_EQ(c->tier, e.tier) << e.name << " (" << e.state << ")";
    }
}

// A second upsert_all must not create duplicates
TEST_F(IBGEDatabaseIntegrationTest, UpsertIsIdempotent) {
    pcd::CityRepository repo(*db);
    repo.upsert_all(fetched_cities);
    repo.upsert_all(fetched_cities);  // second call with same data

    EXPECT_EQ(repo.find_all().size(), 8u);
}

// Pre-insert a city with the wrong tier; upsert must correct it
TEST_F(IBGEDatabaseIntegrationTest, UpsertUpdatesTierForExistingCity) {
    pcd::CityRepository repo(*db);
    repo.insert({0, "RN", "Natal", 1});  // wrong tier — should be 3

    repo.upsert_all(fetched_cities);

    const auto rn = repo.find_by_state("RN");
    const pcd::City* natal = find(rn, "Natal", "RN");
    ASSERT_NE(natal, nullptr);
    EXPECT_EQ(natal->tier, 3);  // corrected by upsert
}
