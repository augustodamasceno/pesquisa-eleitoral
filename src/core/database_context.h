/*
 * Pesquisa Eleitoral - Database Context   
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "candidate_repository.h"
#include "city_repository.h"
#include "result_repository.h"
#include "vote_repository.h"

namespace pesquisae::core::database {

class DatabaseContext {
public:
    DatabaseContext(
        SQLite::Database& db,
        CandidateRepository& candidates,
        CityRepository& cities,
        ResultRepository& results,
        VoteRepository& votes
    )
        : _db(db)
        , _candidates(candidates)
        , _cities(cities)
        , _results(results)
        , _votes(votes)
    {}

    SQLite::Database& get_db() const { return _db; }
    CandidateRepository& get_candidates() const { return _candidates; }
    CityRepository& get_cities() const { return _cities; }
    ResultRepository& get_results() const { return _results; }
    VoteRepository& get_votes() const { return _votes; }

private:
    SQLite::Database& _db;
    CandidateRepository& _candidates;
    CityRepository& _cities;
    ResultRepository& _results;
    VoteRepository& _votes;
};

} // namespace pesquisae::core::database
