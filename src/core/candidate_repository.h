/*
 * Pesquisa Eleitoral - Candidate Repository  
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
#include "candidate.h"

class CandidateRepository : public IRepository<Candidate> {
public:
    explicit CandidateRepository(SQLite::Database& db) : _db(db) {}

    void insert(const Candidate& c) override;
    void update(const Candidate& c) override;
    Candidate find_by_id(int id) override;
    std::vector<Candidate> find_all() override;
    void remove(int id) override;

    std::vector<Candidate> find_by_party(const std::string& party);

private:
    SQLite::Database& _db;
};