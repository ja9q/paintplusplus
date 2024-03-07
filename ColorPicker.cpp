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

ColorPicker::ColorPicker(PaintModel *a_model, QWidget *parent)
    : QWidget{parent}, m_model(a_model), m_squarePos(38,154), m_wheelPos(30,30), m_cursor(generateCursor()),
    m_colorWheel(QImage(LENGTH, LENGTH, QImage::Format_ARGB32)),
    m_colorSquare(QImage(LENGTH-82, LENGTH-82, QImage::Format_ARGB32))
{

    resize(LENGTH,LENGTH);

    renderColorWheel();
    renderColorSquare(QColor(Qt::red));
}

// React to a mouse click
void ColorPicker::mousePressEvent(QMouseEvent *event) {
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    // if this is a left click
    if(event->buttons() & Qt::LeftButton){
        // if this is a click on the color wheel, then change the color of the square
        if (m_colorWheel.pixelColor(event->pos()) != QColor(Qt::transparent)) {
            m_wheelPos = QPointF(mouseX-5, mouseY-5);
            renderColorSquare(m_colorWheel.pixelColor(event->pos()));
            emit changedColor(m_colorSquare.pixelColor(m_squarePos.x()-42, m_squarePos.y()-42));
            m_model->setColor(m_colorSquare.pixelColor(m_squarePos.x()-42, m_squarePos.y()-42), 0);
            m_editFlag = EDITWHEEL;
        }
        else if (mouseX >= 42 && mouseY >= 42 && mouseX<160 && mouseY <160) {
            emit changedColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42));
            m_model->setColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), 0);
            m_squarePos = QPointF(mouseX-5, mouseY-5);
            repaint();
            m_editFlag = EDITSQUARE;
        }
    }

}

// React to a mouse move (only a drag so far)
void ColorPicker::mouseMoveEvent(QMouseEvent *event) {
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    // if this is a left click within the range of the widget
    if((event->buttons() & Qt::LeftButton) && mouseX >=0 && mouseX < LENGTH && mouseY >=0 && mouseY < LENGTH){
        if (m_editFlag == EDITWHEEL && m_colorWheel.pixelColor(event->pos()) != QColor(Qt::transparent)) {
            renderColorSquare(m_colorWheel.pixelColor(event->pos()));
            emit changedColor(m_colorSquare.pixelColor(m_squarePos.x()-42, m_squarePos.y()-42));
            m_model->setColor(m_colorSquare.pixelColor(m_squarePos.x()-42, m_squarePos.y()-42), 0);
            m_wheelPos = QPointF(mouseX-5, mouseY-5);
        }
        else if (m_editFlag == EDITSQUARE && mouseX >= 42 && mouseY >= 42 && mouseX<160 && mouseY <160) {
            emit changedColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42));
            m_model->setColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), 0);
            m_squarePos = QPointF(mouseX-5, mouseY-5);
            repaint();
        }
    }
}

// React to when the mouse is released
void ColorPicker::mouseReleaseEvent(QPaintEvent *event) {
    m_editFlag = EDITNONE;
}

// React to when something is painted
void ColorPicker::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    // Copy the values on the canvas onto the display
    painter.drawPixmap(42, 42,QPixmap::fromImage(m_colorSquare));

    painter.drawPixmap(0,0,QPixmap::fromImage(m_colorWheel));
    painter.drawPixmap(m_squarePos,QPixmap::fromImage(m_cursor));
    painter.drawPixmap(m_wheelPos,QPixmap::fromImage(m_cursor));

}

// Draw the color wheel (happens once when intializing)
void ColorPicker::renderColorWheel() {
    // make sure the area is completely transparent beforehand
    m_colorWheel.fill(qRgba(0,0,0,0));

    // generate the rainbow gradient
    QConicalGradient hueGradient(LENGTH/2,LENGTH/2,90);
    hueGradient.setColorAt(0.0, QColor(255,255,0));
    hueGradient.setColorAt(0.1, QColor(255,0,0));
    hueGradient.setColorAt(0.3, QColor(255,0,255));
    hueGradient.setColorAt(0.5, QColor(0,0,255));
    hueGradient.setColorAt(0.7, QColor(0,255,255));
    hueGradient.setColorAt(0.9, QColor(0,255,0));
    hueGradient.setColorAt(1.0, QColor(255,255,0));

    // draw a circle using the colors of the rainbow gradient
    QPainter painter(&m_colorWheel);
    painter.setPen(QPen(hueGradient, 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(8, 8, 184,184);

    painter.end();

    repaint();
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

    // form the gradient that goes from white->black
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

    repaint();

}

// Draw the cursor for the wheel and squre
QImage ColorPicker::generateCursor() {
    QImage cursor = QImage(11,11, QImage::Format_ARGB32);

    cursor.fill(QColor(Qt::transparent));

    QPainter painter(&cursor);
    painter.setPen(QPen(QColor(Qt::white), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(0, 0, 10,10);
    painter.setPen(QPen(QColor(Qt::black), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(1,1, 8,8);

    painter.end();

    return cursor;
}
