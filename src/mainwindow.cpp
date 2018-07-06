#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boolmatrix.h"
#include "serializer.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto generationLabel = new QLabel(ui->statusBar);
    auto runStateLabel = new QLabel(ui->statusBar);
    ui->statusBar->addWidget(generationLabel);
    ui->statusBar->addWidget(runStateLabel);

    ui->universe->setRenderScaleFactor(6);
    ui->universe->setCellMatrix(m_engine.cellMatrix());

    generationLabel->setText("Generation: 0");
    runStateLabel->setText("Stopped");

    connect(&m_engine, &Engine::updated, [this, generationLabel, runStateLabel] {
        ui->universe->setCellMatrix(m_engine.cellMatrix());
        generationLabel->setText(QString{ "Generation: %1" }.arg(m_engine.generation()));
        runStateLabel->setText(m_engine.isRunning() ? "Running" : "Stopped");
    });

    connect(ui->universe, &UniverseWidget::cellToggled, &m_engine, &Engine::toggleCell);

    connect(ui->startStopButton, &QPushButton::clicked, [this] {
        if (m_engine.isRunning())
            m_engine.stop();
        else
            m_engine.start();
    });

    connect(ui->clearButton, &QPushButton::clicked, &m_engine, &Engine::clear);

    connect(ui->actionSave_state, &QAction::triggered, [this] {
        m_engine.stop();

        auto&& filePath = QFileDialog::getSaveFileName(this, "Select file");

        if (filePath.isEmpty())
            return;

        QFile f{ filePath };

        if (f.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            f.write(Serializer::serialize(m_engine.cellMatrix()).toLocal8Bit().constData());
            f.close();
        }
    });

    connect(ui->actionLoad_state, &QAction::triggered, [this] {
        m_engine.stop();

        auto&& filePath = QFileDialog::getOpenFileName(this, "Select saved state file");

        if (filePath.isEmpty())
            return;

        QFile f{ filePath };

        if (!f.open(QIODevice::ReadOnly))
        {
            qWarning() << "Failed to open saved state file:" << filePath;
            return;
        }

        Engine::MatrixType matrix;

        if (!Serializer::deserialize(f.readAll(), matrix))
        {
            qWarning() << "Failed to load saved state";
            return;
        }

        m_engine.setCellMatrix(matrix);
    });

    connect(ui->randomizeButton, &QPushButton::clicked, &m_engine, &Engine::randomize);
    connect(ui->stepButton, &QPushButton::clicked, &m_engine, &Engine::step);
}

MainWindow::~MainWindow()
{
    delete ui;
}
