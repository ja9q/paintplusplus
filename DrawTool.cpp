//
// Implementation for the DrawTool class
//

#include "DrawTool.h"

#include <QPen>
#include <QDebug>
#include <QRgb>
#include <QPainter>
#include <QPainterPath>
#include "BaseTool.h"

// default constructor
DrawTool::DrawTool() : BaseTool(), m_size(10), m_opacity(255), m_color(0)
    {}

// parametric constructor
DrawTool::DrawTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
    BaseTool(a_name, {BaseTool::MOD_SIZE}),
    m_size(10), m_color(a_color), m_opacity(255) {
    if (!a_moreProperties.empty()) {
        addProperties(a_moreProperties);
    }
}

/**/
/*
int DrawTool::processClick(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2)

NAME

    DrawTool::processClick(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) - draw a dot

SYNOPSIS

    int DrawTool::processClick(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2);
        a_canvas - the canvas where operation must happen
        a_point - where the mouse was clicked
        a_color1 - the user's first color
        a_color2 - the user's second color

DESCRIPTION

    Draw a dot on the mouse click

RETURNS

    0 when the click was successfully processed

*/
/**/
int DrawTool::processClick(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) {
    // set the user-set color to the brush color
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;

    // have a painter draw out this line
    QPainter painter(a_canvas);

    // color and brush are currently hardcoded; modify later to communicate with user more
    painter.setBrush(drawColor);
    painter.setPen(QPen(painter.brush(), m_size, Qt::SolidLine,Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(a_point);
    painter.end();
    m_lastPoint = a_point;

    return 0;
}

/**/
/*
int DrawTool::processDrag(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2)

NAME

    DrawTool::processDrag(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) - draw a line

SYNOPSIS

    int DrawTool::processDrag(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2);
        a_canvas - the canvas where operation must happen
        a_point - where the mouse was clicked
        a_color1 - the user's first color
        a_color2 - the user's second color

DESCRIPTION

    Draw a line between the last point and the new point

RETURNS

    0 when the drag was successfully processed

*/
/**/
int DrawTool::processDrag(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) {
    // set the user-set color to the brush color
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;

    // define a painter path the connects the last drawn point to the new point
    QPainterPath line;
    line.moveTo(m_lastPoint.x(), m_lastPoint.y());
    line.lineTo(a_point.x(), a_point.y());

    // have a painter draw out this line
    QPainter painter(a_canvas);

    // color and brush are currently hardcoded; modify later to communicate with user more
    painter.setBrush(drawColor);
    painter.setPen(QPen(painter.brush(), 10, Qt::SolidLine,Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(line);
    painter.end();
    m_lastPoint = a_point;

    return 0;
}
