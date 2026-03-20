/*
 * Pesquisa Eleitoral - Log Repository
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include "log_repository.h"

#include <stdexcept>

namespace pesquisae::core::database {

void LogRepository::insert(const Log& l) {
    SQLite::Statement q(_db,
        "INSERT INTO log (timestamp, level, source, message) VALUES (?, ?, ?, ?)");
    q.bind(1, l.timestamp);
    q.bind(2, l.level);
    q.bind(3, l.source);
    q.bind(4, l.message);
    q.exec();
}

void LogRepository::update(const Log& l) {
    SQLite::Statement q(_db,
        "UPDATE log SET timestamp=?, level=?, source=?, message=? WHERE id=?");
    q.bind(1, l.timestamp);
    q.bind(2, l.level);
    q.bind(3, l.source);
    q.bind(4, l.message);
    q.bind(5, l.id);
    q.exec();
}

Log LogRepository::find_by_id(int id) {
    SQLite::Statement q(_db,
        "SELECT id, timestamp, level, source, message FROM log WHERE id = ?");
    q.bind(1, id);
    if (q.executeStep())
        return { q.getColumn(0), q.getColumn(1), q.getColumn(2),
                 q.getColumn(3), q.getColumn(4) };
    throw std::runtime_error("Log entry not found: " + std::to_string(id));
}

std::vector<Log> LogRepository::find_by_level(const std::string& level) {
    SQLite::Statement q(_db,
        "SELECT id, timestamp, level, source, message FROM log WHERE level = ?");
    q.bind(1, level);
    std::vector<Log> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1), q.getColumn(2),
                           q.getColumn(3), q.getColumn(4) });
    return result;
}

std::vector<Log> LogRepository::find_all() {
    SQLite::Statement q(_db,
        "SELECT id, timestamp, level, source, message FROM log ORDER BY id");
    std::vector<Log> result;
    while (q.executeStep())
        result.push_back({ q.getColumn(0), q.getColumn(1), q.getColumn(2),
                           q.getColumn(3), q.getColumn(4) });
    return result;
}

void LogRepository::remove(int id) {
    SQLite::Statement q(_db, "DELETE FROM log WHERE id = ?");
    q.bind(1, id);
    q.exec();
}

std::optional<Log> LogRepository::last_demo_update() {
    SQLite::Statement q(_db,
        "SELECT id, timestamp, level, source, message FROM log"
        " WHERE level = 'INFO' AND source = 'DEMOGRAPH' AND message = 'UPDATE'"
        " ORDER BY id DESC LIMIT 1");
    if (q.executeStep())
        return Log{ q.getColumn(0), q.getColumn(1), q.getColumn(2),
                    q.getColumn(3), q.getColumn(4) };
    return std::nullopt;
}

} // namespace pesquisae::core::database
