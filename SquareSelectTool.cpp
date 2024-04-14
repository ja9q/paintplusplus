#include "SquareSelectTool.h"

#include <QPainter>

SquareSelectTool::SquareSelectTool(QString a_name, QVector<int> a_moreProperties):
    SelectTool(a_name)
{
    addProperties(a_moreProperties);
}

void SquareSelectTool::drawSelection(QImage* a_canvas) {
    QImage temp = m_selection;

    QPainter painter(a_canvas);

    // scale the shape first because it displaces the center + has to be before rotation
    temp = temp.transformed(QTransform().scale(m_selectArea.getScale().x(), m_selectArea.getScale().y()));

    // rotate the shape with the new center
    temp = temp.transformed(QTransform().rotate(m_selectArea.getRotation()));

    QRect extraOffset = m_selectArea.getTransformedBoundRect().boundingRect();

    painter.drawPixmap(extraOffset.left(), extraOffset.top(), QPixmap::fromImage(temp));
}

// Draw the bounds on the temporary canvas
void SquareSelectTool::drawBounds(QImage* a_canvas, const QColor a_color) {
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    if (!m_selectArea.isEditing()) {
        painter.setPen(Qt::white);
        painter.drawPolygon(m_selectArea.getShape().translated(-1,-1));

        painter.setPen(Qt::black);
        painter.drawPolygon(m_selectArea.getShape());
    } else {
        painter.setPen(Qt::transparent);
        painter.setBrush(a_color);
        painter.drawPolygon(m_selectArea.getShape());
    }

}

// Calculate the border of the selection as it is being drawn by the mouse
void SquareSelectTool::calcBounds(const QMouseEvent* a_event) {
    QPolygon shape = m_selectArea.getShape();

    if (shape.isEmpty() || shape.back() != a_event->position().toPoint()) {
        shape.append(a_event->position().toPoint());
    }

    QRect newShape(shape.at(0), a_event->position().toPoint());


    m_selectArea.setShape(QPolygon(newShape));
}
