//
// A shape tool that draws squares
//

#ifndef SQUARETOOL_H
#define SQUARETOOL_H

#include "ShapeTool.h"
#include <QMouseEvent>

class SquareTool : public ShapeTool
{
public:
    // parametric constructor
    SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // Draw the shape
    void calcShape(const QMouseEvent* a_event);

};

#endif // SQUARETOOL_H
