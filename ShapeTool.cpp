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
    DrawTool(a_name, a_color, {(int)ToolProperty::OUTLINE, (int)ToolProperty::FILLTYPE}), m_shape(), m_fillMode(FillMode::NOFILL), m_outline(true),
    m_isEditing(false), m_editMode(EditMode::NONE), m_translation(0,0), m_rotation(0), m_scale(1.0,1.0)
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
    m_shape.clear();
    m_boundRect.clear();

    // reset the editing parameters
    m_editMode = EditMode::END;
    m_translation = QPoint(0,0);
    m_rotation = 0.0;
    m_scale = QPointF(1,1);
    m_isEditing = false;
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

    if (m_isEditing) {
        identifyEdit();
    }

    if (m_editMode == EditMode::END) {
        m_editMode = EditMode::NONE;
    }

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
    // draw out the shape if the user is still dragging it for the first time (i.e. not in edit mode)
    if (!m_isEditing) {
        calcShape(a_event);
    }
    // otherwise the user is in edit mode; use the appropriate edit function
    else {
        switch (m_editMode) {
        case EditMode::ROTATE:
            rotate(a_event);
            break;
        case EditMode::TRANSLATE:
            translate(a_event);
            break;
        case EditMode::SCALE:
            scale(a_event);
            break;
        default:
            break;
        }
    }

    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_tempCanvas->fill(Qt::transparent);

    drawShape(a_tempCanvas, a_color1, a_color2);
    return 0;
}

// react to a mouse release
int ShapeTool::processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;

    // if the polygon is not empty and the tool is not editing, turn on editing mode + draw the bounding rectangle
    if (m_editMode == EditMode::END){
        // do nothing if the edit mode is set to stop, but reset the edit mode to none
        m_editMode = EditMode::NONE;
    }else if (!m_shape.isEmpty() && !m_isEditing) {
        m_isEditing = true;
        initBoundingRect();
        drawBoundingRect(a_tempCanvas);
    }
    else if (m_isEditing) {
        m_editMode = EditMode::NONE;
        drawBoundingRect(a_tempCanvas);
    }

    return 0;
}

int ShapeTool::processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    a_tempCanvas->fill(Qt::transparent);
    drawShape(a_tempCanvas, a_color1, a_color2);

    resetEditor();

    // call the original BaseTool's mouseRelease to copy the contents from the temporary canvas to the original canvas
    BaseTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

    return 1;
}

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
    painter.drawPolygon(transformShape(m_shape));
}

void ShapeTool::initBoundingRect() {
    QRect tempRect = m_shape.boundingRect();
    m_boundRect.clear();

    int midHeight = (tempRect.bottom() + tempRect.top()) / 2;
    int midWidth = (tempRect.right() + tempRect.left()) / 2;

    m_boundRect.append({tempRect.topLeft(), QPoint(midWidth, tempRect.top()), tempRect.topRight(), QPoint(tempRect.right(), midHeight)});
    m_boundRect.append({tempRect.bottomRight(), QPoint(midWidth, tempRect.bottom()), tempRect.bottomLeft(), QPoint(tempRect.left(), midHeight)});
}

void ShapeTool::drawBoundingRect(QImage* a_canvas) {
    // create a painter and set it up to draw the rectangle
    QPainter painter(a_canvas);

    QPolygon transformedRect = transformShape(m_boundRect);

    // make the pen white
    painter.setPen(QColor(Qt::white));
    painter.drawPolygon(transformedRect);

    // shift the rectangle and redraw it in black so it sticks out
    painter.setPen(QColor(Qt::black));
    painter.drawPolygon(transformedRect.translated(1,1));

    // get ready to draw the pivots
    painter.setBrush(QColor(Qt::white));
    QRect pivot = QRect(0,0,11,11);

    for (int i = 0; i < 8; i++) {
        pivot.moveCenter(transformedRect.at(i));
        painter.drawRect(pivot);
    }

}

void ShapeTool::identifyEdit() {
    QPolygon transformedRect = transformShape(m_boundRect);

    // if this click was outside of the bounding rectangle, then this is a rotation
    if (!transformedRect.containsPoint(m_lastPoint.toPoint(), Qt::OddEvenFill)) {
        m_editMode = EditMode::ROTATE;
        return;
    } else {
        // otherwise, check if the click was on one of the pivots

        QRect pivot = QRect(0,0,12,12);
        QPolygon temp;

        // if the click was on a pivot, then this is a scaling transformation (dilation)
        for (int i = 0; i < 8; i++) {
            pivot.moveCenter(transformedRect.at(i));
            if (pivot.contains(m_lastPoint.toPoint())) {
                // identify the pivot that was opposite to the clicked pivot (scaling anchor)
                m_anchor = (i+4) % 8;
                m_editMode = EditMode::SCALE;
                return;
            }
        }

        // otherwise, this is a translation
        m_editMode = EditMode::TRANSLATE;

    }
}

// Move the shape
void ShapeTool::translate(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();

    int offsetX = (eventPos.x() > m_lastPoint.x()) ? eventPos.x() - m_lastPoint.x() :  -(m_lastPoint.x() - eventPos.x());
    int offsetY = (eventPos.y() > m_lastPoint.y()) ? eventPos.y() - m_lastPoint.y() :  -(m_lastPoint.y() - eventPos.y());

    m_translation.rx() += offsetX;
    m_translation.ry() += offsetY;

    m_lastPoint = eventPos;

}

// rotate the shape
void ShapeTool::rotate(const QMouseEvent* a_event) {

    // get the required points
    QPoint center = m_shape.boundingRect().center();

    QPoint newPos = a_event->position().toPoint();

    // get the angle
    qreal angle = qAtan2(newPos.y() - center.y(), newPos.x() - center.x());
    qreal oldAngle = qAtan2(m_lastPoint.y() - center.y(), m_lastPoint.x() - center.x());
    angle = qRadiansToDegrees(angle);
    oldAngle = qRadiansToDegrees(oldAngle);

    m_rotation += (angle-oldAngle);

    if (m_rotation > 360) {
        m_rotation -= 360;
    } else if (m_rotation < -360) {
        m_rotation += 360;
    }

    m_lastPoint = newPos;

}

// Resize the shape
void ShapeTool::scale(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();
    QPoint oldAnchor = transformShape(m_boundRect).at(m_anchor);

    // find the line from the last point (i.e. the anchor)
    //   --> the vertical and horizontal component is adjusted for the rotation
    QLineF scaleLine(m_lastPoint, eventPos);
    // rotate the line so it is upright
    scaleLine = QTransform().rotate(-m_rotation).map(scaleLine);

    // divide the new dimensions by the old ones (form scaling ratio)
    qreal scaleX = (m_shape.boundingRect().width()+scaleLine.dx())/m_shape.boundingRect().width();
    qreal scaleY = (m_shape.boundingRect().height()+scaleLine.dy())/m_shape.boundingRect().height();

    // if the anchor (and thus the pivot) is on the top/bottom, disable x scaling
    if (m_anchor == 1 || m_anchor == 5) {
        scaleX = 1.0;
    }
    // if the anchor (and thus the pivot) is on the left/right, disable y scaling
    if (m_anchor == 3 || m_anchor == 7) {
        scaleY = 1.0;
    }
    // if the anchors are from the top-left/top/left/ bottom-left, inverse the y scaling
    if (m_anchor >= 3 && m_anchor <= 6) {
        scaleY = 1.0/scaleY;
    }
    // if the anchors are from top/
    if (m_anchor >= 2 && m_anchor <= 5) {
        scaleX = 1.0/scaleX;
    }

    // update the current scale
    m_scale.rx() += (scaleX-1.0);
    m_scale.ry() += (scaleY-1.0);

    // find the new position of the anchor
    QPoint newAnchor = transformShape(m_boundRect).at(m_anchor);
    int anchX = (newAnchor.x() > oldAnchor.x()) ? -(newAnchor.x() - oldAnchor.x()) :  oldAnchor.x() - newAnchor.x();
    int anchY = (newAnchor.y() > oldAnchor.y()) ? -(newAnchor.y() - oldAnchor.y()) :  oldAnchor.y() - newAnchor.y();

    // translate the shape so the anchor hasn't changed position
    m_translation.rx() += anchX;
    m_translation.ry() += anchY;

    m_lastPoint = eventPos;
}

QPolygon ShapeTool::transformShape(QPolygon a_shape) {
    QPolygon shape = a_shape;

    // scale the shape first because it displaces the center + has to be before rotation
    shape = QTransform().scale(m_scale.x(), m_scale.y()).map(shape);

    // rotate the shape with the new center
    QPoint center = shape.boundingRect().center();
    shape = QTransform().translate(center.x(), center.y()).rotate(m_rotation).translate(-center.x(), -center.y()).map(shape);

    // finally translate the shape
    shape = QTransform().translate(m_translation.x(), m_translation.y()).map(shape);

    return shape;
}
