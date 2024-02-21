#include "PaintModel.h"

#include "DrawTool.h"
#include "User.h"

PaintModel::PaintModel(QWidget *parent) :
    m_canvas(CanvasWidget(&m_user, parent))
{
    m_tools.append(new DrawTool());
    m_user.setCurrentTool(m_tools[0]);
}


CanvasWidget* PaintModel::getCanvas()
{
    return &m_canvas;
}

void PaintModel::setColor(QColor a_color, int a_which) {
    m_user.setColor(a_color, a_which);
}


void PaintModel::fillCanvas() {
    m_canvas.fillCanvas(m_user.getColor(0));
}

void PaintModel::clearCanvas() {
    m_canvas.fillCanvas(m_user.getColor(1));
}
