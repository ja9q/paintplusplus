//
// Implementation for the FillTool class
//

#include "FillTool.h"
#include <QQueue>
#include <QImage>
#include <QPainter>


/**/
/*
FillTool::FillTool(QString a_name, int a_color, QVector<int> a_moreProperties)

NAME

    FillTool::FillTool(a_name, int a_color, QVector<int> a_moreProperties) - constructor

SYNOPSIS

    FillTool::FillTool(QString a_name, int a_color, QVector<int> a_moreProperties);
        a_name --> The name of the tool
        a_color --> The color the tool uses
        a_moreProperties --> additional properties

DESCRIPTION

    constructor for the fill tool

RETURNS

    The constructed FillTool

*/
/**/
FillTool::FillTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
    BaseTool(a_name, {(int)ToolProperty::OPACITY}),
    m_color(a_color), m_opacity(100) {
    if (!a_moreProperties.empty()) {
        addProperties(a_moreProperties);
    }
}

/**/
/*
int FillTool::getProperty(const int a_propId)

NAME

    FillTool::getProperty(const int a_propId) - getter for tool properties

SYNOPSIS

    int FillTool::getProperty(const int a_propId);
        a_propId --> the id of the property to retrieve

DESCRIPTION

    retrieves a tool's property

RETURNS

    The value of the property if the fill tool has it. Otherwise a -1.

*/
/**/
int FillTool::getProperty(const int a_propId) {
    switch((ToolProperty)a_propId) {
    case ToolProperty::OPACITY:
        return m_opacity;
        break;
    default:
        return -1;
        break;
    }
}

/**/
/*
int FillTool::setProperty(const int a_propId, const int a_newValue)

NAME

    FillTool::setProperty(const int a_propId, const int a_newValue) - setter for a property

SYNOPSIS

    int FillTool::setProperty(const int a_propId, const int a_newValue);
        a_propId --> The id of the property to update
        a_newValue --> the new value for the property

DESCRIPTION

    changes a tool's property

RETURNS

    0 if the tool updated the property successfully. Otherwise, -1

*/
/**/
int FillTool::setProperty(const int a_propId, const int a_newValue) {
    switch((ToolProperty)a_propId) {
    case ToolProperty::OPACITY:
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
int FillTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    FillTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) -
        react to a click;

SYNOPSIS

    int FillTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas; used as reference to define the fill area
        a_tempCanvas --> the temporary canvas; where the fill is first placed
        a_event --> the mouse event that triggered the click
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    React to a click and fill an area of one color with a different color

RETURNS

    0 to not immediately finalize the change

*/
/**/
int FillTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    // get the current point
    QPoint point = a_event->position().toPoint();

    // get the color the mouse clicked on (the color to replace/overlay on)
    QColor origColor = a_canvas->pixelColor(point);

    // set the user-set color to the brush color
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;

    // only actually fill if the original and current colors are different and the opacity is not 0
    if (drawColor != origColor && m_opacity > 0.0) {
        if (m_opacity != 100.0)
        {
            drawColor.setAlpha((m_opacity*2.55));
        }


        fill(a_canvas, a_tempCanvas, point, drawColor, origColor);
    }

    return 0;
}

/**/
/*
int FillTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    FillTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) -
        react to a drag;

SYNOPSIS

    int FillTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas;
        a_tempCanvas --> the temporary canvas;
        a_event --> the mouse event that triggered the drag
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    The drag does nothing

RETURNS

    0 to not immediately finalize the change

*/
/**/
int FillTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    // do nothing, but void all the parameters to suppress any warnings
    (void) a_canvas;
    (void) a_tempCanvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;
    return 0;
}

/**/
/*
void FillTool::fill(QImage* a_canvas, QImage* a_tempCanvas, const QPoint a_startPos, const QColor a_fillColor, const QColor a_origColor)

NAME

    FillTool::fill(QImage* a_canvas, QImage* a_tempCanvas, const QPoint a_startPos, const QColor a_fillColor, const QColor a_origColor) - fill
        a single region of one color with another color

SYNOPSIS

    void FillTool::fill(QImage* a_canvas, QImage* a_tempCanvas, const QPoint a_startPos, const QColor a_fillColor, const QColor a_origColor);
        a_canvas --> the permanent canvas; referenced to define the fill area
        a_tempCanvas --> the temporary canvas; where the fill area is first drawn
        a_startPos --> the starting point of the fill; the pixel color of this area is the original color
        a_fillColor --> the color that will replace the original color
        a_origColor --> the original color that will be replaced

DESCRIPTION

    fills an area of one color with another color

RETURNS

    None

*/
/**/
void FillTool::fill(QImage* a_canvas, QImage* a_tempCanvas, const QPoint a_startPos, const QColor a_fillColor, const QColor a_origColor) {
    // store all the nodes to check in a queue. also have a current node and temporary node
    QQueue<QPoint> nodes;
    QPoint currentNode;
    QPoint tempNode;

    int height = a_canvas->height();
    int width = a_canvas->width();

    // create the painter and set it to have consistent opacity
    QPainter painter(a_tempCanvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(QPen(a_fillColor, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));


    int xShift[] = {0,0,1,-1};
    int yShift[] = {1,-1,0,0};

    nodes.enqueue(a_startPos);
    painter.drawPoint(a_startPos);


    while (!nodes.isEmpty()) {
        currentNode = nodes.dequeue();

        for (int i = 0; i < 4; i++) {
            // look at the nodes above, left, below, and right of the current pixel
            tempNode = QPoint(currentNode.x() + xShift[i], currentNode.y() + yShift[i]);
            // if this pixel is in bounds + is of the orignal color + hasn't been drawn over yet,
            // add it to the list of pixels to check and draw the new color over it
            if (tempNode.x() >= 0 && tempNode.x() < width && tempNode.y() >= 0 && tempNode.y() < height) {
                if (a_canvas->pixelColor(tempNode) == a_origColor && a_tempCanvas->pixelColor(tempNode) == QColor(Qt::transparent)) {
                    nodes.enqueue(tempNode);
                    painter.drawPoint(tempNode);
                }
            }
        }

    }

    painter.end();

}
