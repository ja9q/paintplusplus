#ifndef SHAPETOOL_H
#define SHAPETOOL_H

#include "DrawTool.h"

class ShapeTool : public DrawTool
{

public:
    ShapeTool();

    // parametric constructor
    ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // Get a tool's property
    int getProperty(const int a_propId);

    // Modify a tool's property (e.g. size, opacity)
    int setProperty(const int a_propId, const int a_newValue);

    // react to a click
    int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QPointF a_point, const QColor a_color1, const QColor a_color2);

    int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QPointF a_point, const QColor a_color1, const QColor a_color2);

    // Draw the shape
    virtual void drawShape(QImage* a_canvas, QPointF a_endPoint, const QColor a_color1, const QColor a_color2) = 0;


protected:
    // the modes for fill mode
    static const int NOFILL = 0;    // don't fill
    static const int FILLC1 = 1;    // fill with color 1
    static const int FILLC2 = 2;    // fill with color 2

    // how the tool should fill the shape or not
    int m_fillMode;

    // if the tool should outline the shape
    bool m_outline;
};

#endif // SHAPETOOL_H
