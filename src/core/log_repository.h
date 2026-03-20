/*
 * Pesquisa Eleitoral - Log Repository
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#pragma once

#include <optional>
#include <string>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>

#include "irepository.h"
#include "log.h"

namespace pesquisae::core::database {

class LogRepository : public IRepository<Log> {
public:
    explicit LogRepository(SQLite::Database& db) : _db(db) {}

    void insert(const Log& l) override;
    void update(const Log& l) override;
    Log find_by_id(int id) override;
    std::vector<Log> find_all() override;
    void remove(int id) override;

    std::vector<Log> find_by_level(const std::string& level);
    std::optional<Log> last_demo_update();

private:
    SQLite::Database& _db;
};

} // namespace pesquisae::core::database
