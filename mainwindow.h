//
// The main window that contains all components
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenu>
#include "PaintModel.h"
#include "ColorWidget.h"
#include "ToolSettingWidget.h"
#include "ToolSelector.h"
#include "BaseTool.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // constructor
    explicit MainWindow(QWidget *parent = nullptr);

    // destructor
    ~MainWindow();

private slots:

    // React to when the tool type has changed
    void changeToolType(const int a_newType);

    // React to when the tool has changed;
    void changeTool(const int a_newTool);

signals:
    // Alert other parts of the GUI that there is a new tool
    void changedCurrentTool(BaseTool* a_newTool);

    void changedToolType(QList<BaseTool*> a_tools, int a_currentTool);

private:
    PaintModel *m_model;        // The information about the user and available brushes
    ColorWidget *m_colorPicker; // GUI component that lets the user change the brush color
    ToolSelector *m_toolSelector;
    ToolSettingWidget *m_toolSettings;

    QAction *m_toolButtons[PaintModel::TOOLCOUNT];    // The buttons to be added to the toolbar

    QMenu *m_fileMenu;    // The File section of the file menu bar
    QMenu *m_editMenu;    // The Edit section of the file menu bar

    void setupColorPicker();    // Prepare the color picker and its interactions
    void setupToolSelector();   // Prepare the tool selector and its interactions
    void setupToolSettings();   // Prepare the tool settings and its interactions
    void setupToolBar();        // Prepare the tool bar and its interactions
    void setupMenu();           // Prepare the file menu bar and its interactions

};
#endif // MAINWINDOW_H
