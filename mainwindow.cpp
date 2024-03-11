//
// Implementation for the MainWindow class
//

#include "mainwindow.h"
#include "PaintModel.h"
#include "CanvasWidget.h"
#include "ColorWidget.h"
#include "ToolSettingWidget.h"
#include <QVBoxLayout>
#include <QDockWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QShortcut>
#include <QMenuBar>
#include <QToolBar>
#include <QGroupBox>
#include <QDebug>
#include <QtGui>

/**/
/*
MainWindow::MainWindow(QWidget *parent)

NAME

    MainWindow::MainWindow(QWidget *parent) - constructor

SYNOPSIS

    MainWindow::MainWindow(QWidget *parent);
        parent - the container for the widget

DESCRIPTION

    Construct the main window

RETURNS

    The constructed the main window

*/
/**/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1000,500);
    setWindowTitle(tr("Paint++"));

    // initialize the paint model and display its canvas
    m_model = new PaintModel(parent);
    setCentralWidget(m_model->getCanvas());

    // set up the other GUI components
    setupToolBar();
    setupMenu();
    setupColorPicker();
    setupToolSettings();

    m_model->getCanvas()->setFocus();

}

// Destructor
MainWindow::~MainWindow()
{
}

void MainWindow::changeToolType(const int a_newType) {
    for (int i = 0; i < PaintModel::TOOLCOUNT; i++) {
        m_toolButtons[i]->setChecked(false);
    }

    m_toolButtons[a_newType]->setChecked(true);
    m_model->setToolType(a_newType);

    emit changedCurrentTool(m_model->getCurrentTool());
}

/**/
/*
void MainWindow::setupColorPicker()

NAME

    MainWindow::setupColorPicker() - Set up the color picker.

SYNOPSIS

    void MainWindow::setupColorPicker();

DESCRIPTION

    Initialize the color widget, insert it, and connect it to other components

RETURNS

    None

*/
/**/
void MainWindow::setupColorPicker() {
    // Make the dock for the color widget
    QDockWidget* colorDock = new QDockWidget(tr("Color"), this);
    colorDock->setFeatures((QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable));
    colorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, colorDock);

    m_colorPicker = new ColorWidget(m_model);
    colorDock->setWidget(m_colorPicker);

    // connect the interactables
    connect((m_model->getCanvas()), &CanvasWidget::colorChanged, m_colorPicker, &ColorWidget::updateColor);
}

/**/
/*
void MainWindow::setupToolSettings()

NAME

    MainWindow::setupToolSettings() - Set up the tool setting.

SYNOPSIS

    void MainWindow::setupToolSettings();

DESCRIPTION

    Initialize the tool setting widget, insert it, and connect it to other components

RETURNS

    None

*/
/**/
void MainWindow::setupToolSettings() {
    // Make the dock for the settings dock
    QDockWidget* settingsDock = new QDockWidget(tr("Tool Settings"), this);
    settingsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, settingsDock);



    BaseTool* currentTool = m_model->getCurrentTool();

    // create and add the tool settings
    m_toolSettings = new ToolSettingWidget(currentTool);
    settingsDock->setWidget(m_toolSettings);

    connect(m_toolSettings, &ToolSettingWidget::updateSetting, m_model, &PaintModel::updateToolSetting);
    connect(this, &MainWindow::changedCurrentTool, m_toolSettings, &ToolSettingWidget::generateSettings);
}

/**/
/*
void MainWindow::setupToolBar()

NAME

    MainWindow::setupToolBar() - Set up the tool bar.

SYNOPSIS

    void MainWindow::setupToolBar();

DESCRIPTION

    Initialize the tool bar widget, insert it, and connect it to other components

RETURNS

    None

*/
/**/
void MainWindow::setupToolBar() {
    QToolBar* toolbar = new QToolBar();
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);

    QString buttonNames[] = {tr("Pencil"), tr("Eraser"), tr("Select"), tr("Shapes")};

    for (int i = 0; i < PaintModel::TOOLCOUNT; i++) {
        m_toolButtons[i] = new QAction(buttonNames[i]);
        m_toolButtons[i]->setCheckable(true);
        connect(m_toolButtons[i], &QAction::triggered, this, [=](){changeToolType(i);});
        toolbar->addAction(m_toolButtons[i]);
    }

    m_toolButtons[0]->setChecked(true);

}

/**/
/*
void MainWindow::setupMenu()

NAME

    MainWindow::setupMenu() - Set up the file menu bar.

SYNOPSIS

    void MainWindow::setupMenu();

DESCRIPTION

    Initialize the file menu bar widget, insert it, and connect it to other components

RETURNS

    None

*/
/**/
void MainWindow::setupMenu() {
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction* clearAction = new QAction(tr("&Clear Canvas"));
    clearAction->setShortcut(QKeySequence::Delete);
    connect(clearAction, &QAction::triggered, this, [=](){m_model->clearCanvas();});
    m_editMenu->addAction(clearAction);
    addAction(clearAction);

}
