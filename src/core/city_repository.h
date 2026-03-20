/*
 * Pesquisa Eleitoral - City Repository  
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
#include "city.h"

namespace pesquisae::core::database {

class CityRepository : public IRepository<City> {
public:
    explicit CityRepository(SQLite::Database& db) : _db(db) {}

    void insert(const City& c) override;
    void update(const City& c) override;
    City find_by_id(int id) override;
    std::vector<City> find_all() override;
    void remove(int id) override;

    std::vector<City> find_by_state(const std::string& state);
    City find_by_state_and_name(const std::string& state, const std::string& name);
    void upsert_all(const std::vector<City>& cities);

private:
    SQLite::Database& _db;
};

} // namespace pesquisae::core::database