#include "ShapeTool.h"

ShapeTool::ShapeTool() {}

// parametric constructor
ShapeTool::ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    DrawTool(a_name, a_color, a_moreProperties) {}

// Get a tool's property
int ShapeTool::getProperty(const int a_propId) {
    return -1;
}

// Modify a tool's property (e.g. size, opacity)
int ShapeTool::setProperty(const int a_propId, const int a_newValue) {
    return -1;
}
