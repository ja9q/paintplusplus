#include "LineTool.h"
#include <QPainter>

LineTool::LineTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    DrawTool(a_name, a_color, {}), m_line(), m_isEditing(false), m_editMode(EditMode::NONE),
   m_translation(0,0), m_rotation(0), m_scale(1.0,1.0)
{
    addProperties(a_moreProperties);
}

int LineTool::getProperty(const int a_propId) {
    if (DrawTool::getProperty(a_propId) != -1) {
        return DrawTool::getProperty(a_propId);
    }

    return -1;
}

int LineTool::setProperty(const int a_propId, const int a_newValue) {
    return DrawTool::setProperty(a_propId, a_newValue);
}

void LineTool::resetEditor() {
    // reset the line
    m_line = QLine();

    // reset the editing parameters
    m_editMode = EditMode::END;
    m_translation = QPoint(0,0);
    m_rotation = 0.0;
    m_scale = QPointF(1,1);
    m_isEditing = false;
}

int LineTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void)a_canvas;
    (void)a_tempCanvas;
    (void)a_color1;
    (void)a_color2;

    m_lastPoint = a_event->position();

    if (m_isEditing) {
        identifyEdit(a_tempCanvas);
    } else {
        m_line.setP1(m_lastPoint.toPoint());
    }

    if (m_editMode == EditMode::END) {
        m_editMode = EditMode::NONE;
    }

    return 0;
}

int LineTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    // draw out the shape if the user is still dragging it for the first time (i.e. not in edit mode)
    if (!m_isEditing) {
        calcLine(a_event);
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

    drawLine(a_tempCanvas, a_color1, a_color2);
    return 0;
}

int LineTool::processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;

    // if the polygon is not empty and the tool is not editing, turn on editing mode + draw the bounding rectangle
    if (m_editMode == EditMode::END){
        // do nothing if the edit mode is set to stop, but reset the edit mode to none
        m_editMode = EditMode::NONE;
    }else if (!m_line.isNull() && !m_isEditing) {
        m_isEditing = true;
        drawPivots(a_tempCanvas);
    }
    // if the edit mode is alread on, draw the pivots
    else if (m_isEditing) {
        m_editMode = EditMode::NONE;
        drawPivots(a_tempCanvas);
    }

    return 0;
}

int LineTool::processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    a_tempCanvas->fill(Qt::transparent);
    drawLine(a_tempCanvas, a_color1, a_color2);

    resetEditor();

    // call the original BaseTool's mouseRelease to copy the contents from the temporary canvas to the original canvas
    BaseTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

    return 1;
}

void LineTool::drawLine(QImage* a_canvas, const QColor a_color1, const QColor a_color2) {
    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    if (m_opacity < 100) {
        drawColor.setAlpha((m_opacity*2.55));
    }

    // create the painter and set it to have consistent opacity
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    // ready the line
    painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));

    // draw the shape
    painter.drawLine(m_line);
}

void LineTool::calcLine(const QMouseEvent* a_event) {
    m_line.setP2(a_event->position().toPoint());
}

void LineTool::drawPivots(QImage* a_canvas) {
    // create a painter and set it up to draw the rectangle
    QPainter painter(a_canvas);

    // get ready to draw the pivots
    painter.setBrush(QColor(Qt::white));
    painter.setPen(QColor(Qt::black));
    QRect pivot = QRect(0,0,11,11);

    pivot.moveCenter(m_line.p1());
    painter.drawRect(pivot);
    pivot.moveCenter(m_line.p2());
    painter.drawRect(pivot);
}

void LineTool::identifyEdit(QImage* a_canvas) {

    // if this click was outside of the bounding rectangle, then this is a rotation
    if (a_canvas->pixelColor(m_lastPoint.toPoint()) == QColor(Qt::transparent) ) {
        m_editMode = EditMode::ROTATE;
        return;
    } else {
        // otherwise, check if the click was on one of the pivots

        QRect pivot = QRect(0,0,12,12);

        pivot.moveCenter(m_line.p1());
        if (pivot.contains(m_lastPoint.toPoint())) {
            m_editMode = EditMode::SCALE;
            m_pivot = 1;
            return;
        }
        pivot.moveCenter(m_line.p2());
        if (pivot.contains(m_lastPoint.toPoint())) {
            m_editMode = EditMode::SCALE;
            m_pivot = 2;
            return;
        }

        // otherwise, this is a translation
        m_editMode = EditMode::TRANSLATE;

    }
}

// translate the shape
void LineTool::translate(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();

    int offsetX = (eventPos.x() > m_lastPoint.x()) ? eventPos.x() - m_lastPoint.x() :  -(m_lastPoint.x() - eventPos.x());
    int offsetY = (eventPos.y() > m_lastPoint.y()) ? eventPos.y() - m_lastPoint.y() :  -(m_lastPoint.y() - eventPos.y());

    m_line.translate(offsetX, offsetY);

    m_lastPoint = eventPos;

}

// rotate the shape
void LineTool::rotate(const QMouseEvent* a_event) {

    // get the required points
    QPoint center = m_line.center();

    QPoint newPos = a_event->position().toPoint();

    // get the angle
    qreal angle = qAtan2(newPos.y() - center.y(), newPos.x() - center.x());
    qreal oldAngle = qAtan2(m_lastPoint.y() - center.y(), m_lastPoint.x() - center.x());
    angle = qRadiansToDegrees(angle);
    oldAngle = qRadiansToDegrees(oldAngle);


    m_line = QTransform().translate(center.x(), center.y()).rotate(angle-oldAngle).translate(-center.x(), -center.y()).map(m_line);

    m_lastPoint = newPos;

}

// Resize the shape
void LineTool::scale(const QMouseEvent* a_event) {
    QPoint eventPos = a_event->position().toPoint();

    if (m_pivot == 1) {
        m_line.setP1(eventPos);
    } else {
        m_line.setP2(eventPos);
    }
}
