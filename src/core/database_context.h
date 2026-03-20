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
#include "log_repository.h"
#include "result_repository.h"
#include "vote_repository.h"

namespace pesquisae::core::database {

class DatabaseContext {
public:
    explicit DatabaseContext(SQLite::Database& db)
        : _db(db)
        , _candidates(db)
        , _cities(db)
        , _logs(db)
        , _results(db)
        , _votes(db)
    {}

    SQLite::Database&    get_db()         const { return _db; }
    CandidateRepository& get_candidates() const { return _candidates; }
    CityRepository&      get_cities()     const { return _cities; }
    LogRepository&       get_logs()       const { return _logs; }
    ResultRepository&    get_results()    const { return _results; }
    VoteRepository&      get_votes()      const { return _votes; }

private:
    SQLite::Database&           _db;
    mutable CandidateRepository _candidates;
    mutable CityRepository      _cities;
    mutable LogRepository       _logs;
    mutable ResultRepository    _results;
    mutable VoteRepository      _votes;
};

} // namespace pesquisae::core::database
