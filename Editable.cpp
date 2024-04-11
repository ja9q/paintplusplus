#include "Editable.h"

#include <QPainter>

Editable::Editable() :
    m_shape(), m_isEditing(false), m_editMode(EditMode::NONE), m_translation(0,0), m_rotation(0), m_scale(1.0,1.0)
{}

QPolygon Editable::getShape() {
    return m_shape;
}

bool Editable::isEditing() {
    return m_isEditing;
}

Editable::EditMode Editable::getEditMode() {
    return m_editMode;
}

QPolygon Editable::getTransformedShape() {
    return transformShape(m_shape);
}

QPolygon Editable::getTransformedBoundRect() {
    return transformShape(m_boundRect);
}

QPoint Editable::getTranslation() {
    return m_translation;
}

qreal Editable::getRotation() {
    return m_rotation;
}

QPointF Editable::getScale() {
    return m_scale;
}

void Editable::setIsEditing(bool a_mode) {
    m_isEditing = a_mode;
}

void Editable::setEditMode(EditMode a_mode) {
    m_editMode = a_mode;
}

void Editable::setShape(QPolygon a_shape) {
    m_shape = a_shape;
}

void Editable::setPrevPoint(QPoint a_point) {
    m_prevEditPoint = a_point;
}

void Editable::initBoundingRect() {
    QRect tempRect = m_shape.boundingRect();
    m_boundRect.clear();

    int midHeight = (tempRect.bottom() + tempRect.top()) / 2;
    int midWidth = (tempRect.right() + tempRect.left()) / 2;

    m_boundRect.append({tempRect.topLeft(), QPoint(midWidth, tempRect.top()), tempRect.topRight(), QPoint(tempRect.right(), midHeight)});
    m_boundRect.append({tempRect.bottomRight(), QPoint(midWidth, tempRect.bottom()), tempRect.bottomLeft(), QPoint(tempRect.left(), midHeight)});
}

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

void Editable::identifyEdit() {
    QPolygon transformedRect = transformShape(m_boundRect);

    // if this click was outside of the bounding rectangle, then this is a rotation
    if (!transformedRect.containsPoint(m_prevEditPoint, Qt::OddEvenFill)) {
        m_editMode = EditMode::ROTATE;
        return;
    } else {
        // otherwise, check if the click was on one of the pivots

        QRect pivot = QRect(0,0,12,12);
        QPolygon temp;

        // if the click was on a pivot, then this is a scaling transformation (dilation)
        for (int i = 0; i < 8; i++) {
            pivot.moveCenter(transformedRect.at(i));
            if (pivot.contains(m_prevEditPoint)) {
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

void Editable::processEdit(const QMouseEvent *a_event) {
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


// Move the shape
void Editable::translate(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();

    int offsetX = (eventPos.x() > m_prevEditPoint.x()) ? eventPos.x() - m_prevEditPoint.x() :  -(m_prevEditPoint.x() - eventPos.x());
    int offsetY = (eventPos.y() > m_prevEditPoint.y()) ? eventPos.y() - m_prevEditPoint.y() :  -(m_prevEditPoint.y() - eventPos.y());

    m_translation.rx() += offsetX;
    m_translation.ry() += offsetY;

    m_prevEditPoint = eventPos;

}

// rotate the shape
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

    if (m_rotation > 360) {
        m_rotation -= 360;
    } else if (m_rotation < -360) {
        m_rotation += 360;
    }

    m_prevEditPoint = newPos;

}

// Resize the shape
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
