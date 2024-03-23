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

    // Draw shape on the temp canvas (this is overwritten because circles are different from polygons)
    virtual void drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2);

    // Calculate the shape
    void calcShape(const QMouseEvent* a_event);


};

#endif // CIRCLETOOL_H
