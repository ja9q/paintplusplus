#ifndef POLYGONTOOL_H
#define POLYGONTOOL_H

#include "ShapeTool.h"

class PolygonTool : public ShapeTool
{
public:
    // parametric constructor
    PolygonTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // Draw the shape
    void calcShape(const QMouseEvent* a_event);

    int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a mouse release
    int processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2);

    // react to a double click
    int processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2);

protected:
    void drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2);
};

#endif // POLYGONTOOL_H
