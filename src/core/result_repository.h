/*
 * Pesquisa Eleitoral - Result Repository  
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
#include "result.h"

class ResultRepository : public IRepository<Result> {
public:
    explicit ResultRepository(SQLite::Database& db) : _db(db) {}

    void insert(const Result& r) override;
    void update(const Result& r) override;
    Result find_by_id(int id) override;
    std::vector<Result> find_all() override;
    void remove(int id) override;

    std::vector<Result> find_by_date(const std::string& date);
    std::vector<Result> find_by_candidate(int candidate_id);

private:
    SQLite::Database& _db;
};