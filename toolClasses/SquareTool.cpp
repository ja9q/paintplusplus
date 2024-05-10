//
// Implementation for the SquareTool class
//

#include "SquareTool.h"

#include <QPainter>
#include <QRectF>
#include <QMouseEvent>


/**/
/*
SquareTool::SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties);

NAME

    SquareTool::SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties) - parameter constructor

SYNOPSIS

    SquareTool::SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties);
        a_name - the name of the tool
        a_color - the color that is used to draw the outline
        a_moreProperties - additional properties the tool can use

DESCRIPTION

    Constructor for the SquareTool

RETURNS

    The constructed SquareTool

*/
/**/
SquareTool::SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    ShapeTool(a_name, a_color, a_moreProperties)
{}


/**/
/*
void SquareTool::drawShape(QImage* a_canvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    SquareTool::drawShape(QImage* a_canvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - draws the shape

SYNOPSIS

    void SquareTool::drawShape(QImage* a_canvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas where the shape is drawn; this is always the temporary canvas
        a_event  --> the related mouse event; contains the position of the other corner and modifier keys
        a_color1 --> the user's color 1
        a_color2 --> the user's color 2

DESCRIPTION

    Draws the square onto the temporary canvas. The corners of the rectangle is determined by the position of the initial click
    and the position of the current drag. If the drag happened while the shift key is held, then a perfect square is drawn

RETURNS

    None

*/
/**/
void SquareTool::calcShape(const QMouseEvent* a_event){
    // if the shift button is held down, then make this a square; otherwise, draw the rectangle as is
    if (a_event->modifiers() & Qt::ShiftModifier) {
        QPoint fixedPoint = a_event->position().toPoint();
        int x_distance = qFabs(fixedPoint.x()-m_lastPoint.x());
        int y_distance = qFabs(fixedPoint.y()-m_lastPoint.y());
        // set the square's length to be whatever is shorter between the height and width
        if (x_distance > y_distance) {
            fixedPoint.rx() = (fixedPoint.x() > m_lastPoint.x()) ? m_lastPoint.x() + y_distance : m_lastPoint.x() - y_distance;
        } else {
            fixedPoint.ry() = (fixedPoint.y() > m_lastPoint.y()) ? m_lastPoint.y() + x_distance : m_lastPoint.y() - x_distance;
        }
        m_shape.setShape( QPolygon(QRect(m_lastPoint.toPoint(), fixedPoint)));
    } else {
        m_shape.setShape(QPolygon(QRect(m_lastPoint.toPoint(), a_event->pos())));
    }

}
