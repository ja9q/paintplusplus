#include "ShapeTool.h"
#include "DrawTool.h"
#include "ToolSetting.h"

#include <QMouseEvent>

/**/
/*
ShapeTool::ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties);

NAME

    ShapeTool::ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties) - parameter constructor

SYNOPSIS

    ShapeTool::ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties);
        a_name - the name of the tool
        a_color - the color that is used to draw the outline
        a_moreProperties - additional properties the tool can use

DESCRIPTION

    Constructor for the SquareTool

RETURNS

    The constructed SquareTool

*/
/**/
ShapeTool::ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    DrawTool(a_name, a_color, {ToolSetting::OUTLINE, ToolSetting::FILLTYPE}), m_fillMode(NOFILL), m_outline(true)
{
    addProperties(a_moreProperties);
}

/**/
/*
int ShapeTool::getProperty(const int a_propId)

NAME

    ShapeTool::getProperty(const int a_propId) - property getter

SYNOPSIS

    int ShapeTool::getProperty(const int a_propId);
        a_propId - the id of the prop to be retrieved

DESCRIPTION

    Getter for the property; If the property is not present in this object, then
    it is assumed to be in the derived version of this function

RETURNS

    The value of the property, or -1 if the property is not identified in this tool

*/
/**/
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

/**/
/*
int ShapeTool::setProperty(const int a_propId, const int a_newValue)

NAME

    ShapeTool::ShapeTool::setProperty(const int a_propId, const int a_newValue)  - setter for property

SYNOPSIS

    int ShapeTool::setProperty(const int a_propId, const int a_newValue) ;
        a_propId - the id of the property to be changed
        a_newValue - the new value of the property

DESCRIPTION

    Setter for the property; If the property is not present in this object, then
    it is assumed to be in the derived version of this function

RETURNS

    0 if the property was changed, or -1 if the property is not identified in this tool

*/
/**/
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

/**/
/*
int ShapeTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)
NAME

    ShapeTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) -
        React to when a click happens

SYNOPSIS

    int ShapeTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas where the drawing is eventually applied; this is used by some tools as a reference for colors, but not this one
        a_tempCanvas --> the canvas where the stroke is temporarily applied; this is to ensure the opacity works well
        a_event --> the related mouse event; contains the position and modifier keys
        a_color1 --> the user's first color
        a_color2 --> the user's second color

DESCRIPTION

    A shape tool shouldn't draw anything with a single click because there is no height and width to a click.
    Instead, clicking prepares for a drag by saving the position of the click as one of the corners of the shape (by default)

RETURNS

    0 because the canvas doesn't have to do any further work

*/
/**/
int ShapeTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void)a_canvas;
    (void)a_tempCanvas;
    (void)a_color1;
    (void)a_color2;

    m_lastPoint = a_event->position();
    return 0;
}

/**/
/*
int ShapeTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)
NAME

    ShapeTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) -

SYNOPSIS

    int ShapeTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas where the drawing is eventually applied; this is used by some tools as a reference for colors, but not this one
        a_tempCanvas --> the canvas where the stroke is temporarily applied; this is to ensure the opacity works well
        a_event --> the related mouse event; contains the position and modifier keys
        a_color1 --> the user's first color
        a_color2 --> the user's second color

DESCRIPTION

    Draw the shape onto the temporary canvas

RETURNS

    0 because the canvas doesn't have to do any further work

*/
/**/
int ShapeTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    drawShape(a_tempCanvas, a_event, a_color1, a_color2);
    return 0;
}
