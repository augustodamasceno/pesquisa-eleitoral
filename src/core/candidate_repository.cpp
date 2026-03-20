/*
 * Pesquisa Eleitoral - Candidate Repository
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include "candidate_repository.h"

#include <stdexcept>

namespace pesquisae::core::database {

void CandidateRepository::insert(const Candidate& c) {
    SQLite::Statement q(_db,
        "INSERT INTO candidate (id, name, party) VALUES (?, ?, ?)");
    q.bind(1, c.id);
    q.bind(2, c.name);
    q.bind(3, c.party);
    q.exec();
}

void CandidateRepository::update(const Candidate& c) {
    SQLite::Statement q(_db,
        "UPDATE candidate SET name=?, party=? WHERE id=?");
    q.bind(1, c.name);
    q.bind(2, c.party);
    q.bind(3, c.id);
    q.exec();
}

Candidate CandidateRepository::find_by_id(int id) {
    SQLite::Statement q(_db,
        "SELECT id, name, party FROM candidate WHERE id = ?");
    q.bind(1, id);
    if (q.executeStep())
        return { q.getColumn(0), q.getColumn(1), q.getColumn(2) };
    throw std::runtime_error("Candidate not found: " + std::to_string(id));
}

std::vector<Candidate> CandidateRepository::find_by_party(const std::string& party) {
    SQLite::Statement q(_db,
        "SELECT id, name, party FROM candidate WHERE party = ?");
    q.bind(1, party);
    std::vector<Candidate> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1), q.getColumn(2) });
    return result;
}

std::vector<Candidate> CandidateRepository::find_all() {
    SQLite::Statement q(_db,
        "SELECT id, name, party FROM candidate");
    std::vector<Candidate> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1), q.getColumn(2) });
    return result;
}

void CandidateRepository::remove(int id) {
    SQLite::Statement q(_db, "DELETE FROM candidate WHERE id = ?");
    q.bind(1, id);
    q.exec();
}

} // namespace pesquisae::core::database
