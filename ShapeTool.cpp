#include "ShapeTool.h"
#include "DrawTool.h"
#include "ToolSetting.h"

#include <QMouseEvent>

ShapeTool::ShapeTool() {}

// parametric constructor
ShapeTool::ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    DrawTool(a_name, a_color, {ToolSetting::OUTLINE, ToolSetting::FILLTYPE}), m_fillMode(NOFILL), m_outline(true)
{
    addProperties(a_moreProperties);
}

// Get a tool's property
int ShapeTool::getProperty(const int a_propId) {
    if (DrawTool::getProperty(a_propId) != -1) {
        return DrawTool::getProperty(a_propId);
    }

    switch (a_propId) {
    case ToolSetting::FILLTYPE:
        return m_fillMode;
        break;
    case ToolSetting::OUTLINE:
        return m_outline;
        break;
    default:
        break;
    }

    return -1;
}

// Modify a tool's property (e.g. size, opacity)
int ShapeTool::setProperty(const int a_propId, const int a_newValue) {
    if (DrawTool::setProperty(a_propId, a_newValue) == 0) {
        return 0;
    }

    switch (a_propId) {
    case ToolSetting::FILLTYPE:
        m_fillMode = a_newValue;
        return 0;
        break;
    case ToolSetting::OUTLINE:
        m_outline = a_newValue;
        return 0;
        break;
    default:
        break;
    }
    return -1;
}

// react to a click
int ShapeTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void)a_canvas;
    (void)a_tempCanvas;
    (void)a_color1;
    (void)a_color2;

    m_lastPoint = a_event->position();
    return 0;
}

int ShapeTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    drawShape(a_tempCanvas, a_event, a_color1, a_color2);
    return 0;
}
