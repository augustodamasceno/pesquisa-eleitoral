/*
 * Pesquisa Eleitoral - Vote Repository  
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once

#include <string>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>

#include "irepository.h"
#include "vote.h"

namespace pesquisae::core::database {

class VoteRepository : public IRepository<Vote> {
public:
    explicit VoteRepository(SQLite::Database& db) : _db(db) {}

    void insert(const Vote& v) override;
    void update(const Vote& v) override;
    Vote find_by_id(int id) override;
    std::vector<Vote> find_all() override;
    void remove(int id) override;

    std::vector<Vote> find_by_candidate(int candidate_id);
    std::vector<Vote> find_by_city(int city_id);
    std::vector<Vote> find_by_date(const std::string& date);

private:
    SQLite::Database& _db;
};

} // namespace pesquisae::core::database
