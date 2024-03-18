//
// Shape tool for drawing circles
//

#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "ShapeTool.h"
#include <QMouseEvent>

class CircleTool : public ShapeTool
{
public:
    // parametric constructor
    CircleTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // Draw the shape
    void drawShape(QImage* a_canvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
};

#endif // CIRCLETOOL_H
