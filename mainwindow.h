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
    // React to when the color has been changed
    void updateColor(QColor a_newColor);

    //void changeCurrentTool(BaseTool* a_newTool);

    // React to when the user chooses to clear the canvas
    void clearCanvas();

private:
    PaintModel *m_model;        // The information about the user and available brushes
    ColorWidget *m_colorPicker; // GUI component that lets the user change the brush color
    ToolSettingWidget *m_toolSettings;

    QMenu *fileMenu;    // The File section of the file menu bar
    QMenu *editMenu;    // The Edit section of the file menu bar

    void setupColorPicker();    // Prepare the color picker and its interactions
    void setupToolSettings();   // Prepare the tool settings and its interactions
    void setupToolBar();        // Prepare the tool bar and its interactions
    void setupMenu();           // Prepare the file menu bar and its interactions

};
#endif // MAINWINDOW_H
