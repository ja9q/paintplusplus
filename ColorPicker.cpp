//
// Implementation for the ColorPicker class
//

#include "ColorPicker.h"
#include <QPainter>
#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QWidget>
#include <QPixmap>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget{parent}, m_squarePos(0,0), m_wheelPos(0,0),
    m_colorWheel(QImage(LENGTH, LENGTH, QImage::Format_ARGB32)),
    m_colorSquare(QImage(LENGTH-82, LENGTH-82, QImage::Format_ARGB32))
{
    qDebug() << "building something..." << width();
    resize(LENGTH,LENGTH);

    renderColorWheel();
    renderColorSquare(QColor(Qt::red));
}

// React to a mouse click
void ColorPicker::mousePressEvent(QMouseEvent *event) {
    qDebug() << height() << width();
    qDebug() << event->pos().rx() << event->pos().ry();
}

// React to a mouse move (only a drag so far)
void ColorPicker::mouseMoveEvent(QMouseEvent *event) {

}

// React to when something is painted
void ColorPicker::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    // Copy the values on the canvas onto the display
    painter.drawPixmap(42, 42,QPixmap::fromImage(m_colorSquare));

    painter.drawPixmap(0,0,QPixmap::fromImage(m_colorWheel));

}

// Draw the color wheel (happens once when intializing)
void ColorPicker::renderColorWheel() {
    qDebug() << m_colorWheel.depth();
    m_colorWheel.fill(qRgba(0,0,0,0));

    QConicalGradient monoGradient(LENGTH/2,LENGTH/2,90);
    monoGradient.setColorAt(0.0, QColor(255,255,0));
    monoGradient.setColorAt(0.1, QColor(255,0,0));
    monoGradient.setColorAt(0.3, QColor(255,0,255));
    monoGradient.setColorAt(0.5, QColor(0,0,255));
    monoGradient.setColorAt(0.7, QColor(0,255,255));
    monoGradient.setColorAt(0.9, QColor(0,255,0));
    monoGradient.setColorAt(1.0, QColor(255,255,0));

    QPainter painter(&m_colorWheel);
    painter.setPen(QPen(monoGradient, 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    painter.drawEllipse(8, 8, 184,184);


    painter.end();
}

// Draw the color square (happens every time wheel is clicked/ hue changes)
void ColorPicker::renderColorSquare(QColor a_hue) {
    // form the gradient that goes from hue->black
    QLinearGradient hueGradient(0,0,0,118);
    hueGradient.setColorAt(0.0, a_hue);
    hueGradient.setColorAt(1.0, Qt::black);

    // apply the gradient to the main square
    QPainter painter(&m_colorSquare);
    painter.fillRect(0,0,118,118, hueGradient);

    // form the slice of gradient that goes from white->black
    QImage monoSlice = QImage(1, 118,  QImage::Format_ARGB32);
    QLinearGradient monoGradient(0,0,0,118);
    monoGradient.setColorAt(0.0, Qt::white);
    monoGradient.setColorAt(1.0, Qt::black);

    // apply this gradient to a 1-pixel wide slice of the square
    QPainter tempPainter(&monoSlice);
    tempPainter.fillRect(0,0,1,118, monoGradient);
    tempPainter.end();

    // draw this gradient slice at decreasing opacity into the main square
    for(int i = 0; i < 118; i++) {
        painter.setOpacity((118-i)/118.0);
        painter.drawImage(i,0,monoSlice);
    }

}

// Draw the cursor for the wheel
void ColorPicker::drawWheelCursor() {

}

// Draw the cursor for the square
void ColorPicker::drawSquareCursor() {

}
