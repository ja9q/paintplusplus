//
// Implementation for the DrawTool class
//

#include "DrawTool.h"

#include <QPen>
#include <QDebug>
#include <QRgb>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include "BaseTool.h"
#include "ToolSetting.h"

// parametric constructor
DrawTool::DrawTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
        BaseTool(a_name, {ToolSetting::SIZE, ToolSetting::OPACITY}),
    m_size(10), m_color(a_color), m_opacity(100) {
    if (!a_moreProperties.empty()) {
        addProperties(a_moreProperties);
    }
}

/**/
/*
int DrawTool::getProperty(const int a_propId)

NAME

    DrawTool::getProperty(const int a_propId) - get a certain property's value

SYNOPSIS

    int DrawTool::getProperty(const int a_propId);
        a_propId --> the ID of the property the tool wants to receive

DESCRIPTION

    Get a tool's current property value

RETURNS

    The value of the requested property

*/
/**/
int DrawTool::getProperty(const int a_propId) {
    switch(a_propId) {
    case ToolSetting::SIZE:
        return m_size;
        break;
    case ToolSetting::OPACITY:
        return m_opacity;
        break;
    default:
        return -1;
        break;
    }
}

/**/
/*
void DrawTool::setProperty(const int a_propId, const int a_newValue)

NAME

    DrawTool::setProperty(const int a_propId, const int a_newValue) - set a certain property's value

SYNOPSIS

    void DrawTool::setProperty(const int a_propId, const int a_newValue);
        a_propId --> the ID of the property to change
        a_newValue --> the new value of the changed property

DESCRIPTION

    Changes a tool's property to a new value.

RETURNS

    None

*/
/**/
int DrawTool::setProperty(const int a_propId, const int a_newValue) {
    switch(a_propId) {
    case ToolSetting::SIZE:
        m_size = a_newValue;
        break;
    case ToolSetting::OPACITY:
        m_opacity = a_newValue;
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

/**/
/*
int DrawTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    DrawTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - draw a dot

SYNOPSIS

    int DrawTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QPointF a_point, const QColor a_color1, const QColor a_color2);
        a_canvas - the canvas where the drawing is eventually applied; this is used by some tools as a reference for colors, but not this one
        a_tempCanvas - the canvas where the stroke is temporarily applied; this is to ensure the opacity works well
        a_event - the related mouse event; contains the position and modifier keys
        a_color1 - the user's first color
        a_color2 - the user's second color
DESCRIPTION

    Draw a dot on the mouse click

RETURNS

    0 when the click was successfully processed

*/
/**/
int DrawTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {

    (void)a_canvas;

    QPointF point = a_event->position();

    // set the user-set color to the brush color
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    if (m_opacity != 100.0)
    {
        drawColor.setAlpha((m_opacity*0.1));
    }

    // fill brush stencil with desired color
    QImage rawBrush(m_size, m_size, QImage::Format_ARGB32_Premultiplied);
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            rawBrush.setPixelColor(i, j, drawColor);
        }
    }

    // convert the QImage to a pixmap that the painter can use
    QPixmap brush = QPixmap::fromImage(rawBrush);

    QPointF drawPoint(point.x()-(m_size/2),point.y()-(m_size/2));

    // have a painter draw out this line
    QPainter painter(a_tempCanvas);

    painter.drawPixmap(drawPoint, brush);

    painter.end();

    m_lastPoint = drawPoint;


    return 0;
}

/**/
/*
int DrawTool::processDragQImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    DrawTool::processDragQImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) -
        draw a line

SYNOPSIS

    int DrawTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas - the canvas where the drawing is eventually applied; this is used by some tools as a reference for colors, but not this one
        a_tempCanvas - the canvas where the stroke is temporarily applied; this is to ensure the opacity works well
        a_event - the related mouse event; contains the position and modifier keys
        a_color1 - the user's first color
        a_color2 - the user's second color

DESCRIPTION

    Draw a line between the last point and the new point

RETURNS

    0 when the drag was successfully processed

*/
/**/
int DrawTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void)a_canvas;

    QPointF point = a_event->pos();

    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;

    drawColor.setAlpha((m_opacity*2.55));


    // fill brush stencil with desired color
    QImage rawBrush(m_size, m_size, QImage::Format_ARGB32_Premultiplied);

    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            rawBrush.setPixelColor(i, j, drawColor);
        }
    }

    // convert the QImage to a pixmap that the painter can use
    QPixmap brush = QPixmap::fromImage(rawBrush);

    QPointF drawPoint(point.x()-(m_size/2),point.y()-(m_size/2));

    // define a line between the last point and the current point
    QPainterPath line;
    line.moveTo(m_lastPoint);
    line.lineTo(drawPoint);

    qreal length = line.length();
    qreal pos = 0;
    qreal percent = 0;

    // have a painter draw out this line by drawing the pixmap along the path
    QPainter painter(a_tempCanvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    while (pos < length){
        percent = line.percentAtLength(pos);

        painter.drawPixmap(line.pointAtPercent(percent), brush);

        pos += .1;
    }
    //painter.drawPixmap(drawPoint, brush);

    painter.end();

    m_lastPoint = drawPoint;

    return 0;
}


