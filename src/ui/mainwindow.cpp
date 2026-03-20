/*
 * Pesquisa Eleitoral - Ui Main Window
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#include <ctime>
#include <iomanip>
#include <sstream>

#include <QString>
#include <QDir>
#include <QStandardPaths>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database_context.h"
#include "ibge_demographic.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString appDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                     + "/pesquisa-eleitoral";
    QDir().mkpath(appDir);
    std::string dbPath = (appDir + "/database.db").toStdString();

    SQLite::Database db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    _db         = std::make_unique<SQLite::Database>(std::move(db));
    _db_context = std::make_unique<pesquisae::core::database::DatabaseContext>(*_db);

    auto last_update = _db_context->get_logs().last_demo_update();

    bool outdated = true;
    if (last_update.has_value()) {
        std::tm tm{};
        std::istringstream ss(last_update->timestamp);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        std::time_t update_time = std::mktime(&tm);
        std::time_t now         = std::time(nullptr);
        outdated = std::difftime(now, update_time) > 30.0 * 24.0 * 3600.0;
    }

    if (outdated) {
        auto fetch_result = pesquisae::core::database::ibge_fetch_all_cities();
        auto city_list    = pesquisae::core::database::ibge_parse_cities(fetch_result);
        if (!city_list.empty()) {
            _db_context->get_cities().upsert_all(city_list);
            _db_context->get_logs().insert(
                {0, "", "INFO", "DEMOGRAPH", "UPDATE"});
            last_update = _db_context->get_logs().last_demo_update();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
