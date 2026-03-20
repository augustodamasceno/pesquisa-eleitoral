/*
 * Pesquisa Eleitoral - Ui Main Window
 * Copyright (c) 2026, Augusto Damasceno.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * See https://github.com/augustodamasceno/pesquisa-eleitoral
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

#include <SQLiteCpp/SQLiteCpp.h>
#include "database_context.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<SQLite::Database>                           _db;
    std::unique_ptr<pesquisae::core::database::DatabaseContext> _db_context;
};
#endif // MAINWINDOW_H
