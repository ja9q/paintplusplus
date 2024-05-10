//
// Implementation for the ShapeTool class
//

#include "ShapeTool.h"
#include "DrawTool.h"

#include <QMouseEvent>
#include <QPainter>

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
    DrawTool(a_name, a_color, {(int)ToolProperty::OUTLINE, (int)ToolProperty::FILLTYPE}), m_fillMode(FillMode::NOFILL), m_outline(true)
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

    switch ((ToolProperty)a_propId) {
    case ToolProperty::FILLTYPE:
        return (int)m_fillMode;
        break;
    case ToolProperty::OUTLINE:
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

    switch ((ToolProperty)a_propId) {
    case ToolProperty::FILLTYPE:
        m_fillMode = (FillMode)a_newValue;
        return 0;
        break;
    case ToolProperty::OUTLINE:
        m_outline = a_newValue;
        return 0;
        break;
    default:
        break;
    }
    return -1;
}

void ShapeTool::resetEditor() {
    // reset the shape
    m_shape.reset();
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
    m_shape.setPrevPoint(m_lastPoint.toPoint());

    if (m_shape.getEditMode() == Editable::EditMode::END) {
        m_shape.setEditMode(Editable::EditMode::NONE);
    }

    if (m_shape.isEditing()) {
        m_shape.identifyEdit();
    }

    return 0;
}

/**/
/*
int ShapeTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)
NAME

    ShapeTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a drag

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
    // draw out the shape if the user is still dragging it for the first time (i.e. not in edit mode)
    if (!m_shape.isEditing()) {
        calcShape(a_event);
    }
    // otherwise the user is in edit mode; use the appropriate edit function
    m_shape.processEdit(a_event);

    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_tempCanvas->fill(Qt::transparent);

    drawShape(a_tempCanvas, a_color1, a_color2);
    return 0;
}

/**/
/*
int ShapeTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)
NAME

    ShapeTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a mouse release

SYNOPSIS

    int ShapeTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas where the drawing is eventually applied; this is used by some tools as a reference for colors, but not this one
        a_tempCanvas --> the canvas where the stroke is temporarily applied; this is to ensure the opacity works well
        a_event --> the related mouse event; contains the position and modifier keys
        a_color1 --> the user's first color
        a_color2 --> the user's second color

DESCRIPTION

    reacts to a mouse release. if the polygon is not editing, then start editing. Otherwise, set the edit mode to none.

RETURNS

    0 because the canvas doesn't have to do any further work

*/
/**/
int ShapeTool::processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;

    // if the polygon is not empty and the tool is not editing, turn on editing mode + draw the bounding rectangle
    if (m_shape.getEditMode() == Editable::EditMode::END){
        // do nothing if the edit mode is set to stop, but reset the edit mode to none
        m_shape.setEditMode(Editable::EditMode::NONE);
    }else if (!m_shape.getShape().isEmpty() && !m_shape.isEditing()) {
        m_shape.setIsEditing(true);
        m_shape.initBoundingRect();
        m_shape.drawBoundingRect(a_tempCanvas);
    }
    else if (m_shape.isEditing()) {
        m_shape.setEditMode(Editable::EditMode::NONE);
        m_shape.drawBoundingRect(a_tempCanvas);
    }

    return 0;
}

/**/
/*
int ShapeTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)
NAME

    ShapeTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a double click

SYNOPSIS

    int ShapeTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas where the drawing is eventually applied; this is used by some tools as a reference for colors, but not this one
        a_tempCanvas --> the canvas where the stroke is temporarily applied; this is to ensure the opacity works well
        a_event --> the related mouse event; contains the position and modifier keys
        a_color1 --> the user's first color
        a_color2 --> the user's second color

DESCRIPTION

    reacts to a double click. if the polygon is editing, commit the polygon to the final canvas.

RETURNS

    1 because the canvas needs to update the undo history

*/
/**/
int ShapeTool::processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    a_tempCanvas->fill(Qt::transparent);
    drawShape(a_tempCanvas, a_color1, a_color2);

    m_shape.reset();

    // call the original BaseTool's mouseRelease to copy the contents from the temporary canvas to the original canvas
    BaseTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

    return 1;
}

/**/
/*
void ShapeTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2)

NAME

    ShapeTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2) - draw the shape on a given canvas

SYNOPSIS

    void ShapeTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas to draw the shape on
        a_color1 --> the user's first color
        a_color2 --> the user's second color

DESCRIPTION

    draw the transformed shape on the given canvas with the provided settings

RETURNS

    None

*/
/**/
void ShapeTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2) {
    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    if (m_opacity < 100) {
        drawColor.setAlpha((m_opacity*2.55));
    }

    // create the painter and set it to have consistent opacity
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    // ready the outline portion (to exist or not exist)
    if (m_outline) {
        painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    } else {
        painter.setPen(QColor(Qt::transparent));
    }

    // ready the fill portion (if it exists)
    if (m_fillMode != FillMode::NOFILL) {
        QColor fillColor = (m_fillMode == FillMode::FILLC1) ? a_color1 : a_color2;
        if (m_opacity < 100) {
            fillColor.setAlpha((m_opacity*2.55));
        }

        painter.setBrush(fillColor);
    }

    // draw the shape
    painter.drawPolygon(m_shape.getTransformedShape());
}
