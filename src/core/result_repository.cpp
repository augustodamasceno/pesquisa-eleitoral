/*
 * Pesquisa Eleitoral - Result Repository
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include "result_repository.h"

#include <stdexcept>

void ResultRepository::insert(const Result& r) {
    SQLite::Statement q(_db,
        "INSERT INTO result (date, position, votes, candidate_id) VALUES (?, ?, ?, ?)");
    q.bind(1, r.date);
    q.bind(2, r.position);
    q.bind(3, r.votes);
    q.bind(4, r.candidate_id);
    q.exec();
}

void ResultRepository::update(const Result& r) {
    SQLite::Statement q(_db,
        "UPDATE result SET date=?, position=?, votes=?, candidate_id=? WHERE id=?");
    q.bind(1, r.date);
    q.bind(2, r.position);
    q.bind(3, r.votes);
    q.bind(4, r.candidate_id);
    q.bind(5, r.id);
    q.exec();
}

Result ResultRepository::find_by_id(int id) {
    SQLite::Statement q(_db,
        "SELECT id, date, position, votes, candidate_id FROM result WHERE id = ?");
    q.bind(1, id);
    if (q.executeStep())
        return { q.getColumn(0), q.getColumn(1).getString(),
                 q.getColumn(2), q.getColumn(3), q.getColumn(4) };
    throw std::runtime_error("Result not found: " + std::to_string(id));
}

std::vector<Result> ResultRepository::find_by_date(const std::string& date) {
    SQLite::Statement q(_db,
        "SELECT id, date, position, votes, candidate_id "
        "FROM result WHERE date = ? ORDER BY position ASC");
    q.bind(1, date);
    std::vector<Result> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1).getString(),
                           q.getColumn(2), q.getColumn(3), q.getColumn(4) });
    return result;
}

std::vector<Result> ResultRepository::find_by_candidate(int candidate_id) {
    SQLite::Statement q(_db,
        "SELECT id, date, position, votes, candidate_id "
        "FROM result WHERE candidate_id = ? ORDER BY date DESC");
    q.bind(1, candidate_id);
    std::vector<Result> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1).getString(),
                           q.getColumn(2), q.getColumn(3), q.getColumn(4) });
    return result;
}

std::vector<Result> ResultRepository::find_all() {
    SQLite::Statement q(_db,
        "SELECT id, date, position, votes, candidate_id FROM result");
    std::vector<Result> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1).getString(),
                           q.getColumn(2), q.getColumn(3), q.getColumn(4) });
    return result;
}

void ResultRepository::remove(int id) {
    SQLite::Statement q(_db, "DELETE FROM result WHERE id = ?");
    q.bind(1, id);
    q.exec();
}
