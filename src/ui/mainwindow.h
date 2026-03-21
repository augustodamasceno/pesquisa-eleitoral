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
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

#include <SQLiteCpp/SQLiteCpp.h>
#include "database_context.h"
#include "poll_parser.h"
#include "result_processor.h"
#include "result.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class DemoUpdateResult { Success, Failure, AlreadyUpdated };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menuLoadPoll_triggered();
    void on_menuUpdateDatabase_triggered();
    void on_menuAbout_triggered();

private:
    DemoUpdateResult demographic_update(bool force = false);
    void draw_bar_chart(const std::vector<pesquisae::core::database::Result>& results,
                        const std::string& date);

    Ui::MainWindow *ui;

    std::unique_ptr<SQLite::Database>                           _db;
    std::unique_ptr<pesquisae::core::database::DatabaseContext> _db_context;
};
#endif // MAINWINDOW_H
