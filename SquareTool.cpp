#include "SquareTool.h"

#include <QPainter>
#include <QRectF>
#include <QMouseEvent>

SquareTool::SquareTool() {}


// parametric constructor
SquareTool::SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    ShapeTool(a_name, a_color, a_moreProperties)
{}



// Draw the shape
void SquareTool::drawShape(QImage* a_canvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2){
    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_canvas->fill(Qt::transparent);

    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    drawColor.setAlpha((m_opacity*2.55));

    // create the painter and set it to have consistent opacity
    QPainter painter(a_canvas);

    // set the composition mode to have consistent opacity
    painter.setCompositionMode(QPainter::CompositionMode_Source);

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

    // if the shift button is held down, then make this a square; otherwise, draw the rectangle as is
    if (a_event->modifiers() & Qt::ShiftModifier) {
        QPointF fixedPoint = a_event->position();
        qreal x_distance = qFabs(fixedPoint.x()-m_lastPoint.x());
        qreal y_distance = qFabs(fixedPoint.y()-m_lastPoint.y());
        // set the square's length to be whatever is shorter between the height and width
        if (x_distance > y_distance) {
            fixedPoint.rx() = (fixedPoint.x() > m_lastPoint.x()) ? m_lastPoint.x() + y_distance : m_lastPoint.x() - y_distance;
        } else {
            fixedPoint.ry() = (fixedPoint.y() > m_lastPoint.y()) ? m_lastPoint.y() + x_distance : m_lastPoint.y() - x_distance;
        }
        painter.drawRect(QRectF(m_lastPoint, fixedPoint));
    } else {
        painter.drawRect(QRectF(m_lastPoint, a_event->position()));
    }


}

