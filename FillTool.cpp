#include "FillTool.h"
#include "ToolSetting.h"
#include <QQueue>
#include <QImage>
#include <QPainter>


FillTool::FillTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
    BaseTool(a_name, {(int)ToolProperty::OPACITY}),
    m_color(a_color), m_opacity(100) {
    if (!a_moreProperties.empty()) {
        addProperties(a_moreProperties);
    }
}

// Get the current value of a tool's property
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

// Modify a tool's property (e.g. size, opacity)
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

// react to a click
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

// react to a drag
int FillTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    // do nothing, but void all the parameters to suppress any warnings
    (void) a_canvas;
    (void) a_tempCanvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;
    return 0;
}

// the fill algorithm
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
            tempNode = QPoint(currentNode.x() + xShift[i], currentNode.y() + yShift[i]);
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
