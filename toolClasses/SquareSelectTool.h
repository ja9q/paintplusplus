//
// A select tool that only defines rectangular shapes
//

#ifndef SQUARESELECTTOOL_H
#define SQUARESELECTTOOL_H

#include "SelectTool.h"

class SquareSelectTool : public SelectTool
{
public:
    // parametric constructor
    SquareSelectTool(QString a_name, QVector<int> a_moreProperties = {});

protected:
    // draw the selection
    virtual void drawSelection(QImage* a_canvas);

    // Draw the bounds on the temporary canvas
    virtual void drawBounds(QImage* a_canvas, const QColor a_color);

    // Calculate the border of the selection as it is being drawn by the mouse
    virtual void calcBounds(const QMouseEvent* a_event);
};

#endif // SQUARESELECTTOOL_H
