/*
 * Pesquisa Eleitoral - Vote Repository
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include "vote_repository.h"

#include <stdexcept>

namespace pesquisae::core::database {

void VoteRepository::insert(const Vote& v) {
    SQLite::Statement q(_db,
        "INSERT INTO vote (date, city_id, candidate_id) VALUES (?, ?, ?)");
    q.bind(1, v.date);
    q.bind(2, v.city_id);
    q.bind(3, v.candidate_id);
    q.exec();
}

void VoteRepository::update(const Vote& v) {
    SQLite::Statement q(_db,
        "UPDATE vote SET date=?, city_id=?, candidate_id=? WHERE id=?");
    q.bind(1, v.date);
    q.bind(2, v.city_id);
    q.bind(3, v.candidate_id);
    q.bind(4, v.id);
    q.exec();
}

Vote VoteRepository::find_by_id(int id) {
    SQLite::Statement q(_db,
        "SELECT id, date, city_id, candidate_id FROM vote WHERE id = ?");
    q.bind(1, id);
    if (q.executeStep())
        return { q.getColumn(0), q.getColumn(1).getString(),
                 q.getColumn(2), q.getColumn(3) };
    throw std::runtime_error("Vote not found: " + std::to_string(id));
}

std::vector<Vote> VoteRepository::find_by_candidate(int candidate_id) {
    SQLite::Statement q(_db,
        "SELECT id, date, city_id, candidate_id FROM vote WHERE candidate_id = ?");
    q.bind(1, candidate_id);
    std::vector<Vote> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1).getString(),
                           q.getColumn(2), q.getColumn(3) });
    return result;
}

std::vector<Vote> VoteRepository::find_by_city(int city_id) {
    SQLite::Statement q(_db,
        "SELECT id, date, city_id, candidate_id FROM vote WHERE city_id = ?");
    q.bind(1, city_id);
    std::vector<Vote> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1).getString(),
                           q.getColumn(2), q.getColumn(3) });
    return result;
}

std::vector<Vote> VoteRepository::find_by_date(const std::string& date) {
    SQLite::Statement q(_db,
        "SELECT id, date, city_id, candidate_id FROM vote WHERE date = ?");
    q.bind(1, date);
    std::vector<Vote> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1).getString(),
                           q.getColumn(2), q.getColumn(3) });
    return result;
}

std::vector<Vote> VoteRepository::find_all() {
    SQLite::Statement q(_db,
        "SELECT id, date, city_id, candidate_id FROM vote");
    std::vector<Vote> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1).getString(),
                           q.getColumn(2), q.getColumn(3) });
    return result;
}

void VoteRepository::remove(int id) {
    SQLite::Statement q(_db, "DELETE FROM vote WHERE id = ?");
    q.bind(1, id);
    q.exec();
}

} // namespace pesquisae::core::database
