#include "PolygonTool.h"

#include <QPainter>

// parametric constructor
PolygonTool::PolygonTool(QString a_name, int a_color, QVector<int> a_moreProperties) :
    ShapeTool(a_name, a_color, a_moreProperties)
{}


// Draw the shape
void PolygonTool::calcShape(const QMouseEvent* a_event) {
    if (m_shape.length() == 1) {
        m_shape.append(a_event->position().toPoint());
    } else {
        m_shape.back() = a_event->position().toPoint();
    }

}

int PolygonTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    ShapeTool::processClick(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

    if (!m_isEditing && (m_shape.isEmpty() || m_shape.back() != a_event->position().toPoint())) {
        m_shape.append(m_lastPoint.toPoint());
        if (m_shape.length() > 1) {
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

    if (m_editMode == EditMode::END || m_isEditing) {
        ShapeTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);
    }

    return 0;
}

// react to a double click
int PolygonTool::processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    // if the tool was editing, end the editing
    if (m_isEditing) {
        ShapeTool::processDoubleClick(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

        return 1;
    }
    // if the tool was not editing, start editing
    else {
        m_isEditing = true;
        drawShape(a_tempCanvas, a_color1, a_color2);
        initBoundingRect();
        drawBoundingRect(a_tempCanvas);
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

    if (m_isEditing) {
        // ready the outline portion (to exist or not exist)
        if (m_outline) {
            painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        } else {
            painter.setPen(QColor(Qt::transparent));
        }

        // draw the shape
        painter.drawPolygon(transformShape(m_shape));
    } else {
        painter.setPen(QColor(Qt::transparent));
        painter.drawPolygon(transformShape(m_shape));

        painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        painter.drawPolyline(m_shape);
    }

}
