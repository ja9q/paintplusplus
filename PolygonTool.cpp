//
// Implementation for the PolygonTool class
//

#include "PolygonTool.h"

#include <QPainter>

/**/
/*
PolygonTool::PolygonTool(QString a_name, int a_color, QVector<int> a_moreProperties)

NAME

    PolygonTool::PolygonTool(a_name, int a_color, QVector<int> a_moreProperties) - parametric constructor

SYNOPSIS

    PolygonTool::PolygonTool(QString a_name, int a_color, QVector<int> a_moreProperties);
        a_name --> the name of the tool
        a_color --> the color of the tool's outline
        a_moreProperties --> additional properties

DESCRIPTION

    Constructs the polygon

RETURNS

    The constructed polygon

*/
/**/
PolygonTool::PolygonTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
    ShapeTool(a_name, a_color, a_moreProperties)
{}


/**/
/*
void PolygonTool::calcShape(const QMouseEvent* a_event)

NAME

    PolygonTool::calcShape(const QMouseEvent* a_event) - calculate the shape
        as the mouse is dragged

SYNOPSIS

    void PolygonTool::calcShape(const QMouseEvent* a_event);
        a_event --> the mouse event that triggered this

DESCRIPTION

    Because the polygon tool is a collection of lines, a click will add a new point
    but dragging it will move this new point around.

RETURNS

    None

*/
/**/
void PolygonTool::calcShape(const QMouseEvent* a_event) {
    QPolygon shape = m_shape.getShape();

    // if there is only one point, then make this a new point on the shape
    if (shape.length() == 1) {
        shape.append(a_event->position().toPoint());
    } else {
        // otherwise, change the latest point to the event position
        shape.back() = a_event->position().toPoint();
    }

    m_shape.setShape(shape);
}

/**/
/*
int PolygonTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    PolygonTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a click

SYNOPSIS

    int PolygonTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas
        a_tempCanvas --> the temporary canvas; where the polygon is placed before it is done editing
        a_event --> the mouse event that triggered the click
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    React to a click. This is unique because polygons are initialized by many clicks rather than a single click and drag

RETURNS

    0 because the undo history should not be updated

*/
/**/
int PolygonTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    // process the click first in terms of the original ShapeTool
    ShapeTool::processClick(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

    QPolygon shape = m_shape.getShape();
    // if the shpae is not editing + either the shape is empty or the click is not equal to the shape's last point (not a double click), add a new point
    if (!m_shape.isEditing() && (shape.isEmpty() || shape.back() != a_event->position().toPoint())) {
        shape.append(m_lastPoint.toPoint());
        m_shape.setShape(shape);
        // if the shape has more than one point, display it ont he canvas
        if (shape.length() > 1) {
            drawShape(a_tempCanvas, a_color1, a_color2);
        }

    }

    return 0;
}

/**/
/*
int PolygonTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    PolygonTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a mouse release

SYNOPSIS

    int PolygonTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas
        a_tempCanvas --> the temporary canvas; where the polygon is placed before it is done editing
        a_event --> the mouse event that triggered the click
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    React to a mouse release.

RETURNS

    0 because the undo history should not be updated

*/
/**/
int PolygonTool::processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;

    // only do something when the shape is editing or the shape is done editing
    if (m_shape.getEditMode() == Editable::EditMode::END || m_shape.isEditing()) {
        ShapeTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);
    }

    return 0;
}

/**/
/*
int PolygonTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    PolygonTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a doubleclick

SYNOPSIS

    int PolygonTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas
        a_tempCanvas --> the temporary canvas; where the polygon is placed before it is done editing
        a_event --> the mouse event that triggered the click
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    React to a doubleclick. Either starts or ends editing.

RETURNS

    0 if the tool begins editing. if the tool ends editing, return 1 to commit the changes to the undo history

*/
/**/
int PolygonTool::processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    // if the tool was editing, end the editing
    if (m_shape.isEditing()) {
        ShapeTool::processDoubleClick(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

        return 1;
    }
    // if the tool was not editing, start editing
    else {
        m_shape.setIsEditing(true);
        drawShape(a_tempCanvas, a_color1, a_color2);
        m_shape.initBoundingRect();
        m_shape.drawBoundingRect(a_tempCanvas);
        return 0;
    }
}

/**/
/*
void PolygonTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2)

NAME

    PolygonTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2) - draw the shape on a canvas

SYNOPSIS

    void PolygonTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas to draw the shape on
        a_color1 --> the user's first color
        a_color2 --> the user's second color

DESCRIPTION

    Draw the shape on the canvas.

RETURNS

    None

*/
/**/
void PolygonTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2) {
    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    if (m_opacity < 100) {
        drawColor.setAlpha((m_opacity*2.55));
    }

    // create the painter and set it to have consistent opacity
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    // ready the fill portion (if it exists)
    if (m_fillMode != FillMode::NOFILL) {
        QColor fillColor = (m_fillMode == FillMode::FILLC1) ? a_color1 : a_color2;
        if (m_opacity < 100) {
            fillColor.setAlpha((m_opacity*2.55));
        }

        painter.setBrush(fillColor);
    }

    // if the shape has been initialized (and in editing mode), draw the shape whole
    if (m_shape.isEditing()) {
        // ready the outline portion (to exist or not exist)
        if (m_outline) {
            painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        } else {
            painter.setPen(QColor(Qt::transparent));
        }

        // draw the shape
        painter.drawPolygon(m_shape.getTransformedShape());
    } else {
        // otherwise, draw the shape's line section as incomplete
        painter.setPen(QColor(Qt::transparent));
        painter.drawPolygon(m_shape.getTransformedShape());

        if (m_outline) {
            painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        }

        painter.drawPolyline(m_shape.getShape());
    }

}
