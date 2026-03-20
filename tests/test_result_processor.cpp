/*
 * Pesquisa Eleitoral - Result Processor Tests
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <memory>
#include <vector>

#include <gtest/gtest.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "result_processor.h"
#include "candidate_repository.h"
#include "city_repository.h"
#include "result_repository.h"
#include "vote_repository.h"
#include "database_context.h"
#include "vote_intention.h"


static std::unique_ptr<SQLite::Database> make_db()
{
    auto db = std::make_unique<SQLite::Database>(
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
        "    id           INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    date         DATE    NOT NULL,"
        "    city_id      INTEGER NOT NULL REFERENCES city(id),"
        "    candidate_id INTEGER NOT NULL REFERENCES candidate(id)"
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

    db->exec("INSERT INTO city (state, name, tier) VALUES ('SP', 'São José dos Campos', 3)");
    db->exec("INSERT INTO city (state, name, tier) VALUES ('SP', 'São Paulo', 4)");

    return db;
}


class ResultProcessorTest : public ::testing::Test {
protected:
    std::unique_ptr<SQLite::Database>                 db;
    std::unique_ptr<pesquisae::core::database::CandidateRepository> candidates;
    std::unique_ptr<pesquisae::core::database::CityRepository>      cities;
    std::unique_ptr<pesquisae::core::database::ResultRepository>    results;
    std::unique_ptr<pesquisae::core::database::VoteRepository>      votes;
    std::unique_ptr<pesquisae::core::database::DatabaseContext>     ctx;

    void SetUp() override {
        db         = make_db();
        candidates = std::make_unique<pesquisae::core::database::CandidateRepository>(*db);
        cities     = std::make_unique<pesquisae::core::database::CityRepository>(*db);
        results    = std::make_unique<pesquisae::core::database::ResultRepository>(*db);
        votes      = std::make_unique<pesquisae::core::database::VoteRepository>(*db);
        ctx        = std::make_unique<pesquisae::core::database::DatabaseContext>(
                         *db, *candidates, *cities, *results, *votes);
    }
};

// Two candidates, two tiers.
// Candidate 1: 2 votes in tier-3 (São José dos Campos), 1 vote in tier-4 (São Paulo)
// Candidate 2: 1 vote in tier-3 (São José dos Campos), 2 votes in tier-4 (São Paulo)
//
// votes_by_tier = [0, 0, 3, 3]
// w3=10000, w4=2850000
// score_1 = 10000*(2/3) + 2850000*(1/3) =  956666.67
// score_2 = 10000*(1/3) + 2850000*(2/3) = 1903333.33
// total   = 2860000.0
// proportion_2 = 2/3, proportion_1 = 1/3
// Candidate 2 ranks 1st.
TEST_F(ResultProcessorTest, WeightedRankingFavorsMetroCandidate) {
    using VI = pesquisae::core::poll::VoteIntention;
    std::vector<VI> vis = {
        {0, "2026-10-05", "São José dos Campos", "SP", 1},
        {0, "2026-10-05", "São José dos Campos", "SP", 1},
        {0, "2026-10-05", "São Paulo", "SP", 1},
        {0, "2026-10-05", "São José dos Campos", "SP", 2},
        {0, "2026-10-05", "São Paulo", "SP", 2},
        {0, "2026-10-05", "São Paulo", "SP", 2},
    };

    pesquisae::core::poll::ResultProcessor proc(vis, *ctx);

    auto all = results->find_by_date("2026-10-05");
    ASSERT_EQ(all.size(), 2u);

    EXPECT_EQ(all[0].position,     1);
    EXPECT_EQ(all[0].candidate_id, 2);   
    EXPECT_EQ(all[0].votes,        3);   

    EXPECT_EQ(all[1].position,     2);
    EXPECT_EQ(all[1].candidate_id, 1);
    EXPECT_EQ(all[1].votes,        3);

    EXPECT_NEAR(all[0].proportion, 2.0 / 3.0, 1e-6);
    EXPECT_NEAR(all[1].proportion, 1.0 / 3.0, 1e-6);
}

TEST_F(ResultProcessorTest, SingleCandidateSingleTier) {
    using VI = pesquisae::core::poll::VoteIntention;
    std::vector<VI> vis = {
        {0, "2026-10-05", "São José dos Campos", "SP", 7},
        {0, "2026-10-05", "São José dos Campos", "SP", 7},
    };

    pesquisae::core::poll::ResultProcessor proc(vis, *ctx);

    auto all = results->find_by_date("2026-10-05");
    ASSERT_EQ(all.size(), 1u);
    EXPECT_EQ(all[0].position,     1);
    EXPECT_EQ(all[0].candidate_id, 7);
    EXPECT_EQ(all[0].votes,        2);
    EXPECT_DOUBLE_EQ(all[0].proportion, 1.0);
}

TEST_F(ResultProcessorTest, EmptyInputProducesNoResults) {
    pesquisae::core::poll::ResultProcessor proc({}, *ctx);
    EXPECT_TRUE(results->find_all().empty());
}

TEST_F(ResultProcessorTest, VotesArePersistedToVoteTable) {
    using VI = pesquisae::core::poll::VoteIntention;
    std::vector<VI> vis = {
        {0, "2026-10-05", "São José dos Campos",  "SP", 1},
        {0, "2026-10-05", "São Paulo", "SP", 2},
    };

    pesquisae::core::poll::ResultProcessor proc(vis, *ctx);

    auto all_votes = votes->find_all();
    EXPECT_EQ(all_votes.size(), 2u);
}
