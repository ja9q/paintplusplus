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

    // Draw the shape
    virtual void drawShape(QPointF a_endPoint) = 0;

protected:
    // how the tool should fill the shape or not
    int m_fillMode;
};

#endif // SHAPETOOL_H
