//
// Implementation for the ColorPicker class
//

#include "ColorPicker.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <QPixmap>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget{parent}, m_squarePos(0,0), m_wheelPos(0,0),
    m_colorWheel(QPixmap(220,220).toImage()), m_colorSquare(QPixmap(220,220).toImage())
{
    qDebug() << "building something..." << width();
    resize(220,220);

    renderColorWheel();
    renderColorSquare(QColor(Qt::red));
}

// React to a mouse click
void ColorPicker::mousePressEvent(QMouseEvent *event) {
    qDebug() << "i'm existing";
}

// React to a mouse move (only a drag so far)
void ColorPicker::mouseMoveEvent(QMouseEvent *event) {

}

// React to when something is painted
void ColorPicker::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    // Copy the values on the canvas onto the display
    painter.drawPixmap(0,0,QPixmap::fromImage(m_colorSquare));

}

// Draw the color wheel (happens once when intializing)
void ColorPicker::renderColorWheel() {

}

// Draw the color square (happens every time wheel is clicked/ hue changes)
void ColorPicker::renderColorSquare(QColor a_hue) {
    m_colorSquare.fill(a_hue);
}

// Draw the cursor for the wheel
void ColorPicker::drawWheelCursor() {

}

// Draw the cursor for the square
void ColorPicker::drawSquareCursor() {

}
