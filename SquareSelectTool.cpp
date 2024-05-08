//
// Implementation for the SquareSelectTool class
//

#include "SquareSelectTool.h"

#include <QPainter>

/**/
/*
SquareSelectTool::SquareSelectTool(QString a_name, QVector<int> a_moreProperties)

NAME

    SquareSelectTool::SquareSelectTool(a_name, QVector<int> a_moreProperties) - parametric constructor

SYNOPSIS

    SquareSelectTool::SquareSelectTool(QString a_name, QVector<int> a_moreProperties);
        a_name --> the name of the tool
        a_moreProperties --> additional properties

DESCRIPTION

    parametric constructor

RETURNS

    The constructed SquareSelectTool

*/
/**/
SquareSelectTool::SquareSelectTool(QString a_name, QVector<int> a_moreProperties):
    SelectTool(a_name)
{
    addProperties(a_moreProperties);
}

/**/
/*
void SquareSelectTool::drawSelection(QImage* a_canvas)

NAME

    SquareSelectTool::drawSelection(QImage* a_canvas) - draw the selection on the canvas

SYNOPSIS

    void SquareSelectTool::drawSelection(QImage* a_canvas);
        a_canvas --> the canvas to draw it on

DESCRIPTION

    draw the selection on the canvas;

RETURNS

    None

*/
/**/
void SquareSelectTool::drawSelection(QImage* a_canvas) {
    QImage temp = m_selection;

    QPainter painter(a_canvas);

    // scale the shape first because it displaces the center + has to be before rotation
    temp = temp.transformed(QTransform().scale(m_selectArea.getScale().x(), m_selectArea.getScale().y()));

    // rotate the shape with the new center
    temp = temp.transformed(QTransform().rotate(m_selectArea.getRotation()));

    QRect extraOffset = m_selectArea.getTransformedBoundRect().boundingRect();

    painter.drawPixmap(extraOffset.left(), extraOffset.top(), QPixmap::fromImage(temp));
}

/**/
/*
void SquareSelectTool::drawBounds(QImage* a_canvas, const QColor a_color)

NAME

    SquareSelectTool::drawBounds(QImage* a_canvas, const QColor a_color) - draw the bounds or selection area

SYNOPSIS

    void SquareSelectTool::drawBounds(QImage* a_canvas, const QColor a_color);
        a_canvas --> the canvas to draw the bounds on
        a_color --> the color to cover the selection area with

DESCRIPTION

    if the bounds are still being defined, the draw them, otherwise, cover the initial selection area

RETURNS

    None

*/
/**/
void SquareSelectTool::drawBounds(QImage* a_canvas, const QColor a_color) {
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    if (!m_selectArea.isEditing()) {
        painter.setPen(Qt::white);
        painter.drawPolygon(m_selectArea.getShape().translated(-1,-1));

        painter.setPen(Qt::black);
        painter.drawPolygon(m_selectArea.getShape());
    } else {
        painter.setPen(Qt::transparent);
        painter.setBrush(a_color);
        painter.drawPolygon(m_selectArea.getShape());
    }

}

/**/
/*
void SquareSelectTool::calcBounds(const QMouseEvent* a_event)

NAME

    SquareSelectTool::calcBounds(const QMouseEvent* a_event) - calculate the bounds of the selection area

SYNOPSIS

    void SquareSelectTool::calcBounds(const QMouseEvent* a_event);
        a_event --> The mouse event (drag) that triggered this

DESCRIPTION

    calculate the bounds of the selection area, which is always a rectangle

RETURNS

    None

*/
/**/

void SquareSelectTool::calcBounds(const QMouseEvent* a_event) {
    QPolygon shape = m_selectArea.getShape();

    if (shape.isEmpty() || shape.back() != a_event->position().toPoint()) {
        shape.append(a_event->position().toPoint());
    }

    QRect newShape(shape.at(0), a_event->position().toPoint());


    m_selectArea.setShape(QPolygon(newShape));
}
