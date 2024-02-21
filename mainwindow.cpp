#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PaintModel.h"
#include "CanvasWidget.h"
#include "ColorWidget.h"
#include <QVBoxLayout>
#include <QDockWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolBar>
#include <QGroupBox>
#include <QDebug>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1000,500);

    m_model = new PaintModel(parent);




    setCentralWidget(m_model->getCanvas());

    setupToolBar();
    setupMenu();

    // Make the dock for the color widget
    QDockWidget* colorDock = new QDockWidget(tr("Color"), this);
    colorDock->setFeatures((QDockWidget::DockWidgetFloatable |
                            QDockWidget::DockWidgetMovable));
    colorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, colorDock);

    m_colorPicker = new ColorWidget();
    colorDock->setWidget(m_colorPicker);


    connect(m_colorPicker, &ColorWidget::valueChanged, this, &updateColor);

    // Make the dock for the settings dock
    QDockWidget* settingsDock = new QDockWidget(tr("Tool Settings"), this);
    settingsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, settingsDock);

    setWindowTitle(tr("Paint++"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateColor(QColor a_newColor) {
    m_model->setColor(a_newColor, 0);
}

void MainWindow::clearCanvas() {
    m_model->clearCanvas();
}

void MainWindow::setupColorPicker() {

}

void MainWindow::setupToolSettings() {

}

void MainWindow::setupToolBar() {
    QToolBar* toolbar = new QToolBar();
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
}


void MainWindow::setupMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction* clearAction = new QAction(tr("&Clear Canvas"));
    clearAction->setShortcut(QKeySequence::Delete);
    connect(clearAction, &QAction::triggered, this, &MainWindow::clearCanvas);
    editMenu->addAction(clearAction);

}
