//
// A tool that is capable of creating lines on the canvas
//

#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "BaseTool.h"
#include <QMouseEvent>

class DrawTool : public BaseTool
{
public:
    // parametric constructor
    DrawTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // get which color the tool uses
    int getColor() const;

    // Get the current value of a tool's property
    int getProperty(const int a_propId);

    // Modify a tool's property (e.g. size, opacity)
    int setProperty(const int a_propId, const int a_newValue);

    // react to a click
    int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a drag
    int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

protected:
    int m_size;     // the thickness of the line
    int m_color;    // which color (1 or 2) the brush uses
    int m_opacity;  // the opacity (transparency/alpha value) of the line

    QPointF m_lastPoint;    // The last point drawn on the canvas; needed for drawing dragged lines

};

#endif // DRAWTOOL_H
