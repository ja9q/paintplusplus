//
// The main window that contains all components
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenu>
#include "./../miscClasses/PaintModel.h"
#include "./../widgetClasses/ColorWidget.h"
#include "./../widgetClasses/ToolSettingWidget.h"
#include "./../widgetClasses/ToolSelector.h"
#include "./../toolClasses/BaseTool.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // constructor
    explicit MainWindow(QWidget *parent = nullptr);

    // destructor
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

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
    ToolSelector *m_toolSelector;   // GUI component for displaying possible subtools
    ToolSettingWidget *m_toolSettings;  // GUI component for adjusting tool settings

    QAction *m_toolButtons[PaintModel::TOOLCOUNT];    // The buttons to be added to the toolbar

    QMenu *m_fileMenu;    // The File section of the file menu bar
    QMenu *m_editMenu;    // The Edit section of the file menu bar

    QDockWidget* setupColorPicker();    // Prepare the color picker and its interactions
    QDockWidget* setupToolSelector();   // Prepare the tool selector and its interactions
    QDockWidget* setupToolSettings();   // Prepare the tool settings and its interactions

    void setupToolBar();        // Prepare the tool bar and its interactions
    void setupMenu();           // Prepare the file menu bar and its interactions

    bool saveCheck();   // Check if the user wants to save unsaved changes

};
#endif // MAINWINDOW_H
