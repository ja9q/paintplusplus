#include "CircleTool.h"

#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QPainterPath>

/**/
/*
CircleTool::CircleTool(QString a_name, int a_color, QVector<int> a_moreProperties);

NAME

    CircleTool::CircleTool(QString a_name, int a_color, QVector<int> a_moreProperties) - parameter constructor

SYNOPSIS

    CircleTool::CircleTool(QString a_name, int a_color, QVector<int> a_moreProperties);
        a_name - the name of the tool
        a_color - the color that is used to draw the outline
        a_moreProperties - additional properties the tool can use

DESCRIPTION

    Constructor for the CircleTool

RETURNS

    The constructed CircleTool

*/
/**/
CircleTool::CircleTool(QString a_name, int a_color, QVector<int> a_moreProperties):
    ShapeTool(a_name, a_color, a_moreProperties)
{}

/**/
/*
void CircleTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2)

NAME

    CircleTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2) - draws the shape

SYNOPSIS

    void CircleTool::drawShape(QImage* a_canvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the canvas where the shape is drawn; this is always the temporary canvas
        a_color1 --> the user's color 1
        a_color2 --> the user's color 2

DESCRIPTION

    Draws the circle onto the temporary canvas.

RETURNS

    None

*/
/**/
void CircleTool::drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2) {

    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_canvas->fill(Qt::transparent);

    // set the user-set color to the brush color and opacity
    QColor drawColor = (m_color == 0) ? a_color1 : a_color2;
    if (m_opacity < 100) {
    }

    // create the painter and set it to have consistent opacity
    QPainter painter(a_canvas);

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

    // Make the circle a painterpath in order to apply transformations
    QPainterPath ellipse;
    ellipse.addEllipse(m_shape.getShape().boundingRect());

    // scale the shape first because it displaces the center + has to be before rotation
    ellipse = QTransform().scale(m_shape.getScale().x(), m_shape.getScale().y()).map(ellipse);

    // rotate the shape with the new center
    QPoint center = ellipse.boundingRect().center().toPoint();
    ellipse = QTransform().translate(center.x(), center.y()).rotate(m_shape.getRotation()).translate(-center.x(), -center.y()).map(ellipse);

    // finally translate the shape
    ellipse = QTransform().translate(m_shape.getTranslation().x(), m_shape.getTranslation().y()).map(ellipse);

    // draw the shape
    painter.drawPath(ellipse);
}


/**/
/*
void CircleTool::calcShape(const QMouseEvent* a_event)

NAME

    CircleTool::calcShape(const QMouseEvent* a_event) - calculate the circle based off of the mouse

SYNOPSIS

    void CircleTool::calcShape(const QMouseEvent* a_event);
        a_event --> the mouse drag that triggered this event.

DESCRIPTION

    The user defines the initial dimensions of the circle by clicking and dragging. This
    function calculates these dimensions.

RETURNS

    None

*/
/**/
void CircleTool::calcShape(const QMouseEvent* a_event){
    // if the shift button is held down, then make this a perfect circle; otherwise, draw the ellipse as is
    // the ellipse is stored as a rectangle with the same corners.
    if (a_event->modifiers() & Qt::ShiftModifier) {
        QPoint fixedPoint = a_event->position().toPoint();
        int x_distance = qFabs(fixedPoint.x()-m_lastPoint.x());
        int y_distance = qFabs(fixedPoint.y()-m_lastPoint.y());
        // set the circle's diameter to be whatever is shorter between the height and width
        if (x_distance > y_distance) {
            fixedPoint.rx() = (fixedPoint.x() > m_lastPoint.x()) ? m_lastPoint.x() + y_distance : m_lastPoint.x() - y_distance;
        } else {
            fixedPoint.ry() = (fixedPoint.y() > m_lastPoint.y()) ? m_lastPoint.y() + x_distance : m_lastPoint.y() - x_distance;
        }
        m_shape.setShape(QPolygon(QRect(m_lastPoint.toPoint(), fixedPoint)));
    } else {
        m_shape.setShape(QPolygon(QRect(m_lastPoint.toPoint(), a_event->pos())));
    }

}
