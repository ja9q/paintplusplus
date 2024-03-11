#include "SquareTool.h"

#include <QPainter>
#include <QRectF>

SquareTool::SquareTool() {}


// parametric constructor
SquareTool::SquareTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    ShapeTool(a_name, a_color, a_moreProperties)
{}



// Draw the shape
void SquareTool::drawShape(QImage* a_canvas, QPointF a_endPoint, const QColor a_color1, const QColor a_color2){
    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_canvas->fill(Qt::transparent);

    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    drawColor.setAlpha((m_opacity*2.55));

    // create the painter and set it to have consistent opacity
    QPainter painter(a_canvas);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    if (m_outline) {
        painter.setPen(QPen(drawColor, m_size, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    } else {
        painter.setPen(QColor(Qt::transparent));
    }

    if (m_fillMode != NOFILL) {
        QColor fillColor = (m_fillMode == FILLC1) ? a_color1 : a_color2;
        fillColor.setAlpha((m_opacity*2.55));

        painter.setBrush(fillColor);
    }

    painter.drawRect(QRectF(m_lastPoint, a_endPoint));

}

