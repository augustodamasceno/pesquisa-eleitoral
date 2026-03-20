/*
 * Pesquisa Eleitoral - City Repository
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include "city_repository.h"

#include <stdexcept>

namespace pesquisae::core::database {

void CityRepository::insert(const City& c) {
    SQLite::Statement q(_db,
        "INSERT INTO city (state, name, tier) VALUES (?, ?, ?)");
    q.bind(1, c.state);
    q.bind(2, c.name);
    q.bind(3, c.tier);
    q.exec();
}

void CityRepository::update(const City& c) {
    SQLite::Statement q(_db,
        "UPDATE city SET state=?, name=?, tier=? WHERE id=?");
    q.bind(1, c.state);
    q.bind(2, c.name);
    q.bind(3, c.tier);
    q.bind(4, c.id);
    q.exec();
}

City CityRepository::find_by_id(int id) {
    SQLite::Statement q(_db,
        "SELECT id, state, name, tier FROM city WHERE id = ?");
    q.bind(1, id);
    if (q.executeStep())
        return { q.getColumn(0), q.getColumn(1),
                 q.getColumn(2), q.getColumn(3) };
    throw std::runtime_error("City not found: " + std::to_string(id));
}

std::vector<City> CityRepository::find_by_state(const std::string& state) {
    SQLite::Statement q(_db,
        "SELECT id, state, name, tier FROM city WHERE state = ?");
    q.bind(1, state);
    std::vector<City> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1),
                           q.getColumn(2), q.getColumn(3) });
    return result;
}

City CityRepository::find_by_state_and_name(const std::string& state,
                                             const std::string& name) {
    SQLite::Statement q(_db,
        "SELECT id, state, name, tier FROM city WHERE state = ? AND name = ?");
    q.bind(1, state);
    q.bind(2, name);
    if (q.executeStep())
        return { q.getColumn(0), q.getColumn(1),
                 q.getColumn(2), q.getColumn(3) };
    throw std::runtime_error("City not found: " + state + "/" + name);
}

std::vector<City> CityRepository::find_all() {
    SQLite::Statement q(_db,
        "SELECT id, state, name, tier FROM city");
    std::vector<City> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1),
                           q.getColumn(2), q.getColumn(3) });
    return result;
}

void CityRepository::remove(int id) {
    SQLite::Statement q(_db, "DELETE FROM city WHERE id = ?");
    q.bind(1, id);
    q.exec();
}

void CityRepository::upsert_all(const std::vector<City>& cities) {
    auto population_to_tier = [](int pop) noexcept {
        return 1 + (pop > 20000) + (pop > 100000) + (pop > 1000000);
    };
    (void)population_to_tier;

    SQLite::Transaction tx(_db);
    SQLite::Statement q(_db,
        "INSERT INTO city (state, name, tier) VALUES (?, ?, ?)"
        " ON CONFLICT(state, name) DO UPDATE SET tier = excluded.tier");
    for (const auto& c : cities) {
        q.bind(1, c.state);
        q.bind(2, c.name);
        q.bind(3, c.tier);
        q.exec();
        q.reset();
    }
    tx.commit();
}

} // namespace pesquisae::core::database
