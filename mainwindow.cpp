//
// Implementation for the MainWindow class
//

#include "mainwindow.h"
#include "PaintModel.h"
#include "CanvasWidget.h"
#include "ColorWidget.h"
#include "ToolSettingWidget.h"
#include "ResizeDialog.h"
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
    setWindowTitle(tr("Paint++"));

    // initialize the paint model and display its canvas
    m_model = new PaintModel();

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(false);
    scrollArea->setWidget(m_model->getCanvas());
    scrollArea->setAlignment((Qt::AlignHCenter | Qt::AlignVCenter));
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setCentralWidget(scrollArea);

    // set up the other GUI components
    setupToolBar();
    setupMenu();
    setupColorPicker();
    setupToolSelector();
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

    emit changedToolType(m_model->getToolSet(), m_model->getCurrentToolInd());

    emit changedCurrentTool(m_model->getCurrentTool());
}

void MainWindow::changeTool(const int a_newTool) {
    m_model->setTool(a_newTool);
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

void MainWindow::setupToolSelector() {
    // Make the dock for the settings dock
    QDockWidget* selectorDock = new QDockWidget(tr("Tools"), this);
    selectorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    selectorDock->setFeatures((QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable));
    addDockWidget(Qt::LeftDockWidgetArea, selectorDock);

    // create and add the tool settings
    m_toolSelector = new ToolSelector(m_model->getToolSet(), m_model->getCurrentToolInd());
    selectorDock->setWidget(m_toolSelector);

    connect(this, &MainWindow::changedToolType, m_toolSelector, &ToolSelector::generateTools);
    connect(m_toolSelector, &ToolSelector::updateTool, this, &MainWindow::changeTool);
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
    settingsDock->setFeatures((QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable));
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

    QString buttonNames[] = {tr("Pencil"), tr("Eraser"), tr("Select"), tr("Shapes"), tr("Fill"), tr("Text")};

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

    QAction* openAction = new QAction(tr("&Open"));
    openAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(openAction, &QAction::triggered, m_model, &PaintModel::openFile);
    m_fileMenu->addAction(openAction);

    QAction* saveAction = new QAction(tr("&Save"));
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    connect(saveAction, &QAction::triggered, m_model, &PaintModel::saveFile);
    m_fileMenu->addAction(saveAction);

    QAction* saveAsAction = new QAction(tr("&Save as"));
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction, &QAction::triggered, m_model, &PaintModel::saveNewFile);
    m_fileMenu->addAction(saveAsAction);


    QAction* clearAction = new QAction(tr("&Clear Canvas"));
    clearAction->setShortcut(QKeySequence::Delete);
    connect(clearAction, &QAction::triggered, this, [=](){m_model->clearCanvas();});
    m_editMenu->addAction(clearAction);

    m_editMenu->addSeparator();

    QAction* undoAction = new QAction(tr("&Undo"));
    undoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
    connect(undoAction, &QAction::triggered, m_model, &PaintModel::undo);
    m_editMenu->addAction(undoAction);

    QAction* redoAction = new QAction(tr("&Redo"));
    redoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));
    connect(redoAction, &QAction::triggered, m_model, &PaintModel::redo);
    m_editMenu->addAction(redoAction);

    m_editMenu->addSeparator();

    QAction* copyAction = new QAction(tr("&Copy"));
    copyAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
    connect(copyAction, &QAction::triggered, m_model, &PaintModel::copy);
    m_editMenu->addAction(copyAction);

    QAction* cutAction = new QAction(tr("&Cut"));
    cutAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
    connect(cutAction, &QAction::triggered, m_model, &PaintModel::cut);
    m_editMenu->addAction(cutAction);

    QAction* pasteAction = new QAction(tr("&Paste"));
    pasteAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_V));
    connect(pasteAction, &QAction::triggered, this, [=](){changeToolType((int)PaintModel::ToolType::SELECTTOOL); m_model->paste();});
    m_editMenu->addAction(pasteAction);

    m_editMenu->addSeparator();

    QAction* resizeAction = new QAction(tr("&Resize Canvas"));
    resizeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C));
    connect(resizeAction, &QAction::triggered, this, [=](){m_model->setCanvasSize(ResizeDialog::promptUser(m_model->getCanvas()->size(), this));});
    m_editMenu->addAction(resizeAction);

}
