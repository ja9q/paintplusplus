//
// Implementation for the PaintModel class
//

#include "PaintModel.h"

#include "DrawTool.h"
#include "User.h"

/**/
/*
PaintModel::PaintModel(QWidget *parent)

NAME

    PaintModel::PaintModel(QWidget *parent) - Construct the paint model

SYNOPSIS

    PaintModel::PaintModel(QWidget *parent)
        parent - the parent for the canvas widget

DESCRIPTION

    Construct the paint model

RETURNS

    The constructed paint model

*/
/**/
PaintModel::PaintModel(QWidget *parent) :
    QObject{parent},
    m_canvas(CanvasWidget(&m_user, parent))
{
    m_tools.append(new DrawTool());
    m_user.setCurrentTool(m_tools[0]);
}

// Destructor
PaintModel::~PaintModel()
{
}

/**/
/*
CanvasWidget* PaintModel::getCanvas()

NAME

    PaintModel::getCanvas() - getter for the canvas

SYNOPSIS

    PaintModel::getCanvas()

DESCRIPTION

    Getter for the canvas

RETURNS

    The internal canvas

*/
/**/
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
