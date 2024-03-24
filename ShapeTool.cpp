#include "ShapeTool.h"
#include "DrawTool.h"
#include "ToolSetting.h"

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
    DrawTool(a_name, a_color, {ToolSetting::OUTLINE, ToolSetting::FILLTYPE}), m_shape(), m_fillMode(NOFILL), m_outline(true),
    m_isEditing(false), m_editMode(NONE), m_translation(0,0), m_rotation(0), m_scaling(1)
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
    m_lastPoint = getTransform().inverted().map(m_lastPoint);

    if (m_isEditing) {
        identifyEdit();
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
        case ROTATE:
            rotate(a_event);
            break;
        case TRANSLATE:
            translate(a_event);
            break;
        case SCALE:
            scale(a_event);
            break;
        }
    }

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
    if (!m_shape.isEmpty() && !m_isEditing) {
        m_isEditing = true;
        initBoundingRect();
        drawBoundingRect(a_tempCanvas);
    }
    else if (m_isEditing) {
        m_editMode = NONE;
        drawBoundingRect(a_tempCanvas);
    }

    return 0;
}

void ShapeTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2) {
    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_canvas->fill(Qt::transparent);

    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    if (m_opacity < 100) {
        drawColor.setAlpha((m_opacity*2.55));
    }

    // create the painter and set it to have consistent opacity
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
    painter.setTransform(getTransform());

    // ready the outline portion (to exist or not exist)
    if (m_outline) {
        painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    } else {
        painter.setPen(QColor(Qt::transparent));
    }

    // ready the fill portion (if it exists)
    if (m_fillMode != NOFILL) {
        QColor fillColor = (m_fillMode == FILLC1) ? a_color1 : a_color2;
        fillColor.setAlpha((m_opacity*2.55));

        painter.setBrush(fillColor);
    }

    // draw the shape
    painter.drawPolygon(m_shape);
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

    painter.setTransform(getTransform());

    // make the pen white
    painter.setPen(QColor(Qt::white));
    painter.drawPolygon(m_boundRect);

    // shift the rectangle and redraw it in black so it sticks out
    painter.setPen(QColor(Qt::black));
    painter.drawPolygon(m_boundRect.translated(1,1));

    // get ready to draw the pivots
    painter.setBrush(QColor(Qt::white));
    QRect pivot = QRect(0,0,8,8);

    for (int i = 0; i < 8; i++) {
        pivot.moveCenter(m_boundRect.at(i));
        painter.drawRect(pivot);
    }

}

void ShapeTool::identifyEdit() {
    QRect boundRect = m_shape.boundingRect();
    // if this click was outside of the bounding rectangle, then this is a rotation
    if (!boundRect.contains(m_lastPoint.toPoint())) {
        m_editMode = ROTATE;
        return;
    } else {
        // otherwise, check if the click was on one of the pivots

        QRect pivot = QRect(0,0,8,8);

        // if the click was on a pivot, then this is a scaling transformation (dilation)
        for (int i = 0; i < 8; i++) {
            pivot.moveCenter(m_boundRect.at(i));
            if (pivot.contains(m_lastPoint.toPoint())) {
                m_editMode = SCALE;
                return;
            }
        }

        // otherwise, this is a translation
        m_editMode = TRANSLATE;

    }
}

// Move the shape
void ShapeTool::translate(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();
    eventPos = getTransform().inverted().map(eventPos);

    int offsetX = (eventPos.x() > m_lastPoint.x()) ? eventPos.x() - m_lastPoint.x() :  -(m_lastPoint.x() - eventPos.x());
    int offsetY = (eventPos.y() > m_lastPoint.y()) ? eventPos.y() - m_lastPoint.y() :  -(m_lastPoint.y() - eventPos.y());

    m_translation.rx() += offsetX;
    m_translation.ry() += offsetY;
}

// rotate the shape
void ShapeTool::rotate(const QMouseEvent* a_event) {

    // get the required points
    QPoint center = m_shape.boundingRect().center();
    QPoint newPos = a_event->position().toPoint();

    newPos = getTransform().inverted().map(newPos);

    // get the angle
    qreal angle = qAtan2(newPos.y() - center.y(), newPos.x() - center.x());
    angle = qRadiansToDegrees(angle);

    qDebug() << angle;

    m_rotation += angle;

    m_lastPoint = newPos;
}

// Resize the shape
void ShapeTool::scale(const QMouseEvent* a_event) {

}

QTransform ShapeTool::getTransform() {
    QTransform translates, rotates, resizes, temp;
    QPoint center = m_shape.boundingRect().center();

    translates.translate(0-center.x()-m_translation.x(), 0-center.y()-m_translation.y());

    rotates.rotate(m_rotation);

    center = temp.rotate(-m_rotation).translate(m_translation.x(), m_translation.y()).map(center);

    translates.translate(center.x(), center.y());

    translates.translate(m_translation.x(), m_translation.y());

    qDebug() << m_translation << (translates*rotates*resizes).map(center) << center;

    return (translates*rotates*resizes);
}
