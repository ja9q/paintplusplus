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
#include "ToolSetting.h"

// default constructor
DrawTool::DrawTool() : BaseTool(), m_size(10), m_opacity(100), m_color(0)
    {}

// parametric constructor
DrawTool::DrawTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
    BaseTool(a_name, {ToolSetting::SIZE}),
    m_size(10), m_color(a_color), m_opacity(100) {
    if (!a_moreProperties.empty()) {
        addProperties(a_moreProperties);
    }
}

void DrawTool::setProperty(const int a_propId, const int a_newValue) {
    switch(a_propId) {
    case ToolSetting::SIZE:
        m_size = a_newValue;
        break;
    case ToolSetting::OPACITY:
        m_opacity = a_newValue;
        break;
    default:
        qDebug() << "ERROR: tried to set incompatible property";
        break;
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
    drawColor.setAlpha(m_opacity * 2.55);

    // fill brush stencil with desired color
    QImage rawBrush(m_size, m_size, QImage::Format_ARGB32);
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            rawBrush.setPixelColor(i, j, drawColor);
        }
    }

    // convert the QImage to a pixmap that the painter can use
    QPixmap brush = QPixmap::fromImage(rawBrush);

    QPointF drawPoint(a_point.x()-(m_size/2),a_point.y()-(m_size/2));

    // have a painter draw out this line
    QPainter painter(a_canvas);


    painter.drawPixmap(drawPoint, brush);


    painter.end();

    m_lastPoint = drawPoint;


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
    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    drawColor.setAlpha((m_opacity*.255));

    // fill brush stencil with desired color
    QImage rawBrush(m_size, m_size, QImage::Format_ARGB32);

    rawBrush.fill(drawColor);

    // convert the QImage to a pixmap that the painter can use
    QPixmap brush = QPixmap::fromImage(rawBrush);

    QPointF drawPoint(a_point.x()-(m_size/2),a_point.y()-(m_size/2));

    // define a line between the last point and the current point
    QPainterPath line;
    line.moveTo(m_lastPoint);
    line.lineTo(drawPoint);

    qreal length = line.length();
    qreal pos = 0;
    qreal percent = 0;

    // have a painter draw out this line
    QPainter painter(a_canvas);
    while (pos < length){
        percent = line.percentAtLength(pos);

        painter.drawPixmap(line.pointAtPercent(percent), brush);

        pos += .1;
    }
    painter.drawPixmap(drawPoint, brush);

    painter.end();

    m_lastPoint = drawPoint;

    return 0;
}


