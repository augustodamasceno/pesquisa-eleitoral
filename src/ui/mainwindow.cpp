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
    setWindowIcon(QIcon(":/icon.png"));

    QString appDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                     + "/pesquisa-eleitoral";
    QDir().mkpath(appDir);
    std::string dbPath = (appDir + "/database.db").toStdString();

    SQLite::Database db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    _db         = std::make_unique<SQLite::Database>(std::move(db));
    _db_context = std::make_unique<pesquisae::core::database::DatabaseContext>(*_db);

    this->demographic_update();
}

DemoUpdateResult MainWindow::demographic_update(bool force){
    if (!force) {
        auto last_update = _db_context->get_logs().last_demo_update();
        if (last_update.has_value()) {
            std::tm tm{};
            std::istringstream ss(last_update->timestamp);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            std::time_t update_time = std::mktime(&tm);
            std::time_t now         = std::time(nullptr);
            if (std::difftime(now, update_time) <= 30.0 * 24.0 * 3600.0)
                return DemoUpdateResult::AlreadyUpdated;
        }
    }

    auto fetch_result = pesquisae::core::database::ibge_fetch_all_cities();
    auto city_list    = pesquisae::core::database::ibge_parse_cities(fetch_result);
    if (!city_list.empty()) {
        _db_context->get_cities().upsert_all(city_list);
        _db_context->get_logs().insert(
            {0, "", "INFO", "DEMOGRAPH", "UPDATE"});
        return DemoUpdateResult::Success;
    }
    return DemoUpdateResult::Failure;
}

void MainWindow::on_menuUpdateDatabase_triggered()
{
    switch (this->demographic_update(true)) {
    case DemoUpdateResult::Success:
        QMessageBox::information(this, tr("Base Demográfica"),
            tr("Base demográfica atualizada com sucesso."));
        break;
    case DemoUpdateResult::AlreadyUpdated:
        QMessageBox::information(this, tr("Base Demográfica"),
            tr("Base demográfica já está atualizada."));
        break;
    case DemoUpdateResult::Failure:
        QMessageBox::critical(this, tr("Base Demográfica"),
            tr("Falha ao atualizar a base demográfica."));
        break;
    }
}

void MainWindow::on_menuAbout_triggered()
{
    QMessageBox::about(this, tr("Pesquisa Eleitoral"),
        tr("Pesquisa Eleitoral\n"
           "Copyright (c) 2026, Augusto Damasceno.\n"
           "All rights reserved.\n\n"
           "SPDX-License-Identifier: BSD-2-Clause\n\n"
           "See https://github.com/augustodamasceno/pesquisa-eleitoral"));
}

void MainWindow::draw_bar_chart(
    const std::vector<pesquisae::core::database::Result>& results,
    const std::string& date)
{
    auto* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    if (results.empty())
        return;

    const int    viewW      = ui->graphicsView->width()  - 20;
    const int    viewH      = ui->graphicsView->height() - 20;
    const int    n          = static_cast<int>(results.size());
    const int    barH       = std::max(20, (viewH - 60) / n - 10);
    const int    labelColW  = 80;
    const int    pctColW    = 60;
    const int    chartW     = viewW - labelColW - pctColW - 10;
    const int    marginTop  = 30;

    // Title
    auto* title = scene->addText(QString("Resultado - ") + QString::fromStdString(date));
    title->setDefaultTextColor(Qt::white);
    title->setPos(labelColW, 4);

    // Palette — one colour per bar, cycling if > colours defined
    static const QColor kPalette[] = {
        QColor(0x42, 0x85, 0xF4),
        QColor(0xEA, 0x43, 0x35),
        QColor(0x34, 0xA8, 0x53),
        QColor(0xFB, 0xBC, 0x05),
        QColor(0xAB, 0x47, 0xBC),
        QColor(0x00, 0xAC, 0xC1),
        QColor(0xFF, 0x70, 0x43),
    };
    constexpr int kPaletteSize = static_cast<int>(sizeof(kPalette)/sizeof(kPalette[0]));

    for (int i = 0; i < n; ++i) {
        const auto& r   = results[i];
        const int   y   = marginTop + i * (barH + 10);
        const int   barW = static_cast<int>(r.proportion * chartW);
        const QColor colour = kPalette[i % kPaletteSize];

        // Candidate ID label
        auto* lbl = scene->addText(QString("Cand %1").arg(r.candidate_id));
        lbl->setDefaultTextColor(Qt::white);
        lbl->setPos(0, y + (barH - 16) / 2);

        // Bar
        auto* bar = scene->addRect(labelColW, y, std::max(barW, 2), barH,
                                   QPen(Qt::NoPen), QBrush(colour));
        (void)bar;

        // Percentage label
        auto* pct = scene->addText(
            QString("%1%").arg(r.proportion * 100.0, 0, 'f', 1));
        pct->setDefaultTextColor(Qt::white);
        pct->setPos(labelColW + chartW + 6, y + (barH - 16) / 2);
    }

    scene->setBackgroundBrush(QColor(0x30, 0x30, 0x30));
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menuLoadPoll_triggered()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        tr("Carregue o Arquivo da Pesquisa Eleitoral"),
        QString(),
        tr("Arquivos de Pesquisa Eleitoral (*.txt);;Todos os Tipos (*)")
    );
    if (path.isEmpty())
        return;

    try {
        pesquisae::core::poll::PollParser parser(path.toStdString());
        pesquisae::core::poll::ResultProcessor proc(
            parser.get_vote_intentions(), *_db_context);
        auto results = _db_context->get_results().find_by_date(proc.get_date());
        draw_bar_chart(results, proc.get_date());
        QMessageBox::information(this, tr("Pesquisa Eleitoral Carregada"),
            tr("Pesquisa Eleitoral Carregada e resultados processados."));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Erro"),
            tr("Falha ao carregar a pesquisa eleitoral: ") + QString::fromStdString(e.what()));
    }
}
