#ifndef SQUARETOOL_H
#define SQUARETOOL_H

#include "ShapeTool.h"
#include <QMouseEvent>

class SquareTool : public ShapeTool
{
public:
    SquareTool();

    // parametric constructor
    SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // Draw the shape
    void drawShape(QImage* a_canvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

};

#endif // SQUARETOOL_H
