#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenu>
#include "PaintModel.h"
#include "ColorWidget.h"
#include "BaseTool.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateColor(QColor a_newColor);
    //void changeCurrentTool(BaseTool* a_newTool);
    void clearCanvas();

private:
    PaintModel *m_model;
    ColorWidget *m_colorPicker;

    QMenu *fileMenu;
    QMenu *editMenu;

    void setupColorPicker();
    void setupToolSettings();
    void setupToolBar();
    void setupMenu();

};
#endif // MAINWINDOW_H
