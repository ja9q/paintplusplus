#ifndef PAINTMODEL_H
#define PAINTMODEL_H

#include "CanvasWidget.h"
#include "User.h"
#include "BaseTool.h"
#include <QVector>

class PaintModel
{
public:
    explicit PaintModel(QWidget *parent = nullptr);
    ~PaintModel();

    CanvasWidget* getCanvas();

    void setColor(QColor a_color, int a_which);
    void fillCanvas();
    void clearCanvas();

private:
    QVector<BaseTool*> m_tools;
    User m_user;
    CanvasWidget m_canvas;
};

#endif // PAINTMODEL_H
