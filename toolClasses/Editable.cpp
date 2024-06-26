//
// Implementation for the Editable class
//

#include "Editable.h"

#include <QPainter>

/**/
/*
Editable::Editable()

NAME

    Editable::Editable() - constructor

SYNOPSIS

    Editable::Editable();

DESCRIPTION

    constructor for an editable object

RETURNS

    The constructed editable

*/
/**/
Editable::Editable() :
    m_shape(), m_isEditing(false), m_editMode(EditMode::NONE), m_translation(0,0), m_rotation(0), m_scale(1.0,1.0)
{}

/**/
/*
QPolygon Editable::getShape()

NAME

    Editable::getShape() - getter for the shape

SYNOPSIS

    QPolygon Editable::getShape();

DESCRIPTION

    getter for the shape that the editable manipulates (with no transformations)

RETURNS

    The raw shape of the editable

*/
/**/
QPolygon Editable::getShape() {
    return m_shape;
}

/**/
/*
bool Editable::isEditing()

NAME

    Editable::isEditing() - getter for if object is editing something

SYNOPSIS

    bool Editable::isEditing();

DESCRIPTION

    getter for if the object is editing something and the initial shape has been
    defined already

RETURNS

    if the editable is editing

*/
/**/
bool Editable::isEditing() {
    return m_isEditing;
}

/**/
/*
Editable::EditMode Editable::getEditMode()

NAME

    Editable::getEditMode() - getter for the edit mode

SYNOPSIS

    Editable::EditMode Editable::getEditMode();

DESCRIPTION

    getter for the edit mode; irrelevant when the object is not editing, but
    refers to the type of transformation the user wants to perform (e.g. rotation, nothing, translation)

RETURNS

    the current edit mode of the object

*/
/**/
Editable::EditMode Editable::getEditMode() {
    return m_editMode;
}

/**/
/*
QPolygon Editable::getTransformedShape()

NAME

    Editable::getTransformedShape() - getter for the transformed shape

SYNOPSIS

    QPolygon Editable::getTransformedShape();

DESCRIPTION

    returns the shape as it has been transformed

RETURNS

    the transformed polygon

*/
/**/
QPolygon Editable::getTransformedShape() {
    return transformShape(m_shape);
}

/**/
/*
QPolygon Editable::getTransformedBoundRect()

NAME

    Editable::getTransformedBoundRect() - getter for the transformed object's bounding rectangle

SYNOPSIS

    QPolygon Editable::getTransformedBoundRect();

DESCRIPTION

    getter for the transformed object's bounding rectangle. more specifically, the bounding
    rectangle after being transformed, which is different from calling boundingRect() on the
    transformed shape

RETURNS

    The shape's bounding rectangle, transformed

*/
/**/
QPolygon Editable::getTransformedBoundRect() {
    return transformShape(m_boundRect);
}

/**/
/*
QPoint Editable::getTranslation()

NAME

    Editable::getTranslation() - getter for the shape's translation

SYNOPSIS

    QPoint Editable::getTranslation();

DESCRIPTION

    getter for the translation of the shape

RETURNS

    The x and y offset of the shape

*/
/**/
QPoint Editable::getTranslation() {
    return m_translation;
}

/**/
/*
qreal Editable::getRotation()

NAME

    Editable::getRotation() - getter for rotation

SYNOPSIS

    qreal Editable::getRotation();

DESCRIPTION

    getter for the shape's rotation

RETURNS

    the shape's rotation in degrees

*/
/**/
qreal Editable::getRotation() {
    return m_rotation;
}

/**/
/*
QPointF Editable::getScale()

NAME

    Editable::getScale() - getter for the scaling dimensions

SYNOPSIS

    QPointF Editable::getScale();

DESCRIPTION

    getter for the shape's scaling dimensions

RETURNS

    the x and y scaling factors for the shape; 1.0 means no scaling

*/
/**/
QPointF Editable::getScale() {
    return m_scale;
}

/**/
/*
void Editable::setIsEditing(bool a_mode)

NAME

    Editable::setIsEditing(bool a_mode) - setter for if the editor is editing

SYNOPSIS

    void Editable::setIsEditing(bool a_mode);
        a_mode --> whether the editor is editing or not

DESCRIPTION

    setter for if the shape is editing or not

RETURNS

    None

*/
/**/
void Editable::setIsEditing(bool a_mode) {
    m_isEditing = a_mode;
}

/**/
/*
void Editable::setEditMode(EditMode a_mode)

NAME

    Editable::setEditMode(EditMode a_mode) - setter for the editing mode

SYNOPSIS

    void Editable::setEditMode(EditMode a_mode);
        a_mode --> the editing mode the user set

DESCRIPTION

    setter for the editing mode (e.g. rotation, translation)

RETURNS

    None

*/
/**/
void Editable::setEditMode(EditMode a_mode) {
    m_editMode = a_mode;
}

/**/
/*
void Editable::setShape(QPolygon a_shape)

NAME

    Editable::setShape(QPolygon a_shape) - setter for the shape

SYNOPSIS

    void Editable::setShape(QPolygon a_shape);
        a_shape --> the editor's new shape

DESCRIPTION

    setter for the editor's shape.

RETURNS

    None

*/
/**/
void Editable::setShape(QPolygon a_shape) {
    m_shape = a_shape;
}

/**/
/*
void Editable::setPrevPoint(QPoint a_point)

NAME

    Editable::setPrevPoint(QPoint a_point) - setter for the previous point

SYNOPSIS

    void Editable::setPrevPoint(QPoint a_point);
        a_point --> the new previous point

DESCRIPTION

    setter for the previous point; needed for calculating some transformations

RETURNS

    None

*/
/**/
void Editable::setPrevPoint(QPoint a_point) {
    m_prevEditPoint = a_point;
}

/**/
/*
void Editable::initBoundingRect()

NAME

    Editable::initBoundingRect() - initialize the bounding rectangle

SYNOPSIS

    void Editable::initBoundingRect();

DESCRIPTION

    initialize the bounding rectangle from the current shape

RETURNS

    None

*/
/**/
void Editable::initBoundingRect() {
    QRect tempRect = m_shape.boundingRect();
    m_boundRect.clear();

    // define the midsections of the bounding rectangle
    int midHeight = (tempRect.bottom() + tempRect.top()) / 2;
    int midWidth = (tempRect.right() + tempRect.left()) / 2;

    // construct the bounding rectangle while adding midsections between each corner
    m_boundRect.append({tempRect.topLeft(), QPoint(midWidth, tempRect.top()), tempRect.topRight(), QPoint(tempRect.right(), midHeight)});
    m_boundRect.append({tempRect.bottomRight(), QPoint(midWidth, tempRect.bottom()), tempRect.bottomLeft(), QPoint(tempRect.left(), midHeight)});
}

/**/
/*
void Editable::drawBoundingRect(QImage* a_canvas)

NAME

    Editable::drawBoundingRect(QImage* a_canvas) - draw the bounding rectangle

SYNOPSIS

    void Editable::drawBoundingRect(QImage* a_canvas);
        a_canvas --> the image to draw the bounding rectangle on

DESCRIPTION

    draw the bounding rectangle on the given canvas. This includes the pivots
    that mark where to trigger scaling

RETURNS

    None

*/
/**/
void Editable::drawBoundingRect(QImage* a_canvas) {
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
    QRect pivot = QRect(0,0,7,7);

    for (int i = 0; i < 8; i++) {
        pivot.moveCenter(transformedRect.at(i));
        painter.drawRect(pivot);
    }

}

/**/
/*
void Editable::identifyEdit()

NAME

    Editable::identifyEdit() - use the previous point to identify the current edit

SYNOPSIS

    void Editable::identifyEdit();

DESCRIPTION

    uses the previous point to determine the type of transformation to calculate.

RETURNS

    None

*/
/**/
void Editable::identifyEdit() {
    QPolygon transformedRect = transformShape(m_boundRect);

    // if this click was outside of the bounding rectangle, then this is a rotation
    if (!transformedRect.containsPoint(m_prevEditPoint, Qt::OddEvenFill)) {
        m_editMode = EditMode::ROTATE;
        return;
    } else {
        // otherwise, check if the click was on one of the pivots

        QRect pivot = QRect(0,0,21,21);

        // if the click was on a pivot, then this is a scaling transformation (dilation)
        for (int i = 0; i < 8; i++) {
            pivot.moveCenter(transformedRect.at(i));
            if (((QPolygon)pivot).containsPoint(m_prevEditPoint, Qt::OddEvenFill)) {
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

/**/
/*
void Editable::processEdit(const QMouseEvent *a_event)

NAME

    Editable::processEdit(const QMouseEvent *a_event) - calculate a transformation based off of the
        current edit mode and the given mouse event

SYNOPSIS

    void Editable::processEdit(const QMouseEvent *a_event);
        a_event --> the mouse event to process; this is almost always a drag

DESCRIPTION

    calculates a transformation determined by the current transformation mode.

RETURNS

    None

*/
/**/
void Editable::processEdit(const QMouseEvent *a_event) {
    // calculate the transformation based on the edit mode
    switch(m_editMode) {
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

/**/
/*
void Editable::reset()

NAME

    Editable::reset() - reset the editor's parameters

SYNOPSIS

    void Editable::reset();

DESCRIPTION

    resets the editor's shape and transformation parameters

RETURNS

    None

*/
/**/
void Editable::reset() {
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
void Editable::translate(const QMouseEvent* a_event)

NAME

    Editable::translate(const QMouseEvent* a_event) - move the shape

SYNOPSIS

    void Editable::translate(const QMouseEvent* a_event);
        a_event --> the mouse event that triggered the translation

DESCRIPTION

    moves the shape based off of the mouse's current position

RETURNS

    None

*/
/**/
void Editable::translate(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();

    // calculate the offset
    int offsetX = (eventPos.x() > m_prevEditPoint.x()) ? eventPos.x() - m_prevEditPoint.x() :  -(m_prevEditPoint.x() - eventPos.x());
    int offsetY = (eventPos.y() > m_prevEditPoint.y()) ? eventPos.y() - m_prevEditPoint.y() :  -(m_prevEditPoint.y() - eventPos.y());

    // add this offset to the current translation
    m_translation.rx() += offsetX;
    m_translation.ry() += offsetY;

    m_prevEditPoint = eventPos;

}

/**/
/*
void Editable::rotate(const QMouseEvent* a_event)

NAME

    Editable::rotate(const QMouseEvent* a_event) - rotate the shape

SYNOPSIS

    void Editable::rotate(const QMouseEvent* a_event);
        a_event --> the mouse event that triggered the rotation

DESCRIPTION

    rotates the shaped based ont he mouse's movement

RETURNS

    None

*/
/**/
void Editable::rotate(const QMouseEvent* a_event) {

    // get the required points
    QPoint center = m_shape.boundingRect().center();

    QPoint newPos = a_event->position().toPoint();

    // get the angle
    qreal angle = qAtan2(newPos.y() - center.y(), newPos.x() - center.x());
    qreal oldAngle = qAtan2(m_prevEditPoint.y() - center.y(), m_prevEditPoint.x() - center.x());
    angle = qRadiansToDegrees(angle);
    oldAngle = qRadiansToDegrees(oldAngle);

    m_rotation += (angle-oldAngle);

    // keep the rotation within the bounds of -360 - 360
    if (m_rotation > 360) {
        m_rotation -= 360;
    } else if (m_rotation < -360) {
        m_rotation += 360;
    }

    m_prevEditPoint = newPos;

}

/**/
/*
void Editable::scale(const QMouseEvent* a_event)

NAME

    Editable::scale(const QMouseEvent* a_event) - scale the shape

SYNOPSIS

    void Editable::scale(const QMouseEvent* a_event);
        a_event --> the mouse event that the scaling is based off of

DESCRIPTION

    changes the x and y scaling of the shape based on the mouse's movement.
    this also translates the shape such that a certain pivot point looks stationary

RETURNS

    None

*/
/**/
void Editable::scale(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();
    QPoint oldAnchor = transformShape(m_boundRect).at(m_anchor);

    // find the line from the last point (i.e. the anchor)
    //   --> the vertical and horizontal component is adjusted for the rotation
    QLineF scaleLine(m_prevEditPoint, eventPos);
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

    m_prevEditPoint = eventPos;
}

/**/
/*
QPolygon Editable::transformShape(QPolygon a_shape)

NAME

    Editable::transformShape(QPolygon a_shape) - transform a given polygon

SYNOPSIS

    QPolygon Editable::transformShape(QPolygon a_shape);
        a_shape --> the polygon to transform

DESCRIPTION

    Apply all of the editable's recorded transformations onto a polygon

RETURNS

    The transformed polygon

*/
/**/
QPolygon Editable::transformShape(QPolygon a_shape) {
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
