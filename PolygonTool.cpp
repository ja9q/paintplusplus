#include "PolygonTool.h"

#include <QPainter>

// parametric constructor
PolygonTool::PolygonTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
    ShapeTool(a_name, a_color, a_moreProperties)
{}


// Draw the shape
void PolygonTool::calcShape(const QMouseEvent* a_event) {
    QPolygon shape = m_shape.getShape();

    if (shape.length() == 1) {
        shape.append(a_event->position().toPoint());
    } else {
        shape.back() = a_event->position().toPoint();
    }

    m_shape.setShape(shape);
}

int PolygonTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    ShapeTool::processClick(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);
    QPolygon shape = m_shape.getShape();

    if (!m_shape.isEditing() && (shape.isEmpty() || shape.back() != a_event->position().toPoint())) {
        shape.append(m_lastPoint.toPoint());
        m_shape.setShape(shape);
        if (shape.length() > 1) {
            drawShape(a_tempCanvas, a_color1, a_color2);
        }

    }

    return 0;
}

// react to a mouse release
int PolygonTool::processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;

    if (m_shape.getEditMode() == Editable::EditMode::END || m_shape.isEditing()) {
        ShapeTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);
    }

    return 0;
}

// react to a double click
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
        painter.setPen(QColor(Qt::transparent));
        painter.drawPolygon(m_shape.getTransformedShape());

        painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        painter.drawPolyline(m_shape.getShape());
    }

}
