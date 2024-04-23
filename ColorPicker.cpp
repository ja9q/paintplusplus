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



/**/
/*
ColorPicker::ColorPicker(PaintModel *a_model, QWidget *parent)

NAME

    *a_model, QWidget *parent) - the parameter constructor

SYNOPSIS

    ColorPicker::ColorPicker(PaintModel *a_model, QWidget *parent);
        a_model -->

DESCRIPTION

    parameter constructor

RETURNS

    The constructed ColorPicker

*/
/**/
ColorPicker::ColorPicker(PaintModel *a_model, QWidget *parent)
    : QWidget{parent}, m_model(a_model), m_cursor(generateCursor()),
    m_colorWheel(QImage(LENGTH, LENGTH, QImage::Format_ARGB32)),
    m_colorSquare(QImage(LENGTH-82, LENGTH-82, QImage::Format_ARGB32)),
    m_whichColor(0)
{

    resize(LENGTH,LENGTH);

    renderColorWheel();
    calculateWheelPos(QColor(Qt::red));
    calculateSquarePos(QColor(Qt::black));
    renderColorSquare(QColor(Qt::red));
}

/**/
/*
void ColorPicker::updateColor(QColor a_color)

NAME

    ColorPicker::updateColor(QColor a_color) - updates the appearance of the
    color picker after the color is externally changed

SYNOPSIS

    void ColorPicker::updateColor(QColor a_color);
        a_color --> the new color that should be displayed

DESCRIPTION

    When an external component (i.e. the canvas color picker or outer color widget) change
    the color, this widget is also updated to reflect the new color

RETURNS

    None

*/
/**/
void ColorPicker::updateColor(QColor a_color) {
    if (m_editFlag != EditFlag::EDITWHEEL) {
        calculateSquarePos(a_color);
    }
    if (m_editFlag != EditFlag::EDITSQUARE) {
        calculateWheelPos(a_color);
    }
    renderColorSquare(m_colorWheel.pixelColor(QPoint(m_wheelPos.x()+5, m_wheelPos.y()+5)));

    m_model->setColor(a_color, m_whichColor);

    repaint();
}

/**/
/*
void ColorPicker::mousePressEvent(QMouseEvent *event)

NAME

    ColorPicker::mousePressEvent(QMouseEvent *event)) - react to a click

SYNOPSIS

    void ColorPicker::mousePressEvent(QMouseEvent *event);
        event --> the mouse event to be processed

DESCRIPTION

    This function handles when the presses any mouse button when the
    cursor is over any part of the color picker widget

RETURNS

    None

*/
/**/
void ColorPicker::mousePressEvent(QMouseEvent *event) {
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    // if this is a left click
    if(event->buttons() & Qt::LeftButton){
        // if this is a click on the color wheel, then change the color of the square
        if (m_colorWheel.pixelColor(event->pos()) != QColor(Qt::transparent)) {
            m_editFlag = EditFlag::EDITWHEEL;
            // update the position of the wheel cursor
            calculateWheelPos(event->pos());

            // update the color square + change the color in the outer widget and model
            renderColorSquare(m_colorWheel.pixelColor(event->pos()));
            emit changedColor(m_colorSquare.pixelColor(m_squarePos.x()-37, m_squarePos.y()-37), true);
            m_model->setColor(m_colorSquare.pixelColor(m_squarePos.x()-37, m_squarePos.y()-37), m_whichColor);

            // rerender + set the flag
            repaint();
        }
        // if the color square was clicked
        else if (mouseX >= 42 && mouseY >= 42 && mouseX<160 && mouseY <160) {
            m_editFlag = EditFlag::EDITSQUARE;
            // change the color in the outer widget and model
            emit changedColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), true);
            m_model->setColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), m_whichColor);

            // update the position of the square cursor, rerender, and set flag
            m_squarePos = QPointF(mouseX-5, mouseY-5);
            repaint();
        }
        // otherwise, if this was a click to the other square, then swap colors
        else if (this->grab().toImage().pixelColor(event->pos()) == m_model->getColor(1 + (-1 * m_whichColor))) {
            m_whichColor = 1 + (-1*m_whichColor);
            updateColor(m_model->getColor(m_whichColor));
            emit swappedColor(m_whichColor);
            repaint();
        }
    }

}

/**/
/*
void ColorPicker::mouseMoveEvent(QMouseEvent *event)

NAME

    ColorPicker::mouseMoveEvent(QMouseEvent *event)) - react to a movement

SYNOPSIS

    void ColorPicker::mouseMoveEvent(QMouseEvent *event);
        event --> the mouse event to be processed

DESCRIPTION

    This function handles when the moves the cursor when the
    cursor is over any part of the color picker widget

RETURNS

    None

*/
/**/
void ColorPicker::mouseMoveEvent(QMouseEvent *event) {
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    // if this is a left click within the range of the widget
    if((event->buttons() & Qt::LeftButton) && mouseX >=0 && mouseX < LENGTH && mouseY >=0 && mouseY < LENGTH){
        if (m_editFlag == EditFlag::EDITWHEEL) {
            calculateWheelPos(event->pos());
            renderColorSquare(m_colorWheel.pixelColor(QPoint(m_wheelPos.x()+5, m_wheelPos.y()+5)));
            emit changedColor(m_colorSquare.pixelColor(m_squarePos.x()-37, m_squarePos.y()-37), true);
            m_model->setColor(m_colorSquare.pixelColor(m_squarePos.x()-37, m_squarePos.y()-37), m_whichColor);
            repaint();
        }
        else if (m_editFlag == EditFlag::EDITSQUARE && mouseX >= 42 && mouseY >= 42 && mouseX<160 && mouseY <160) {
            emit changedColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), true);
            m_model->setColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), m_whichColor);
            m_squarePos = QPointF(mouseX-5, mouseY-5);
            repaint();
        } else if (m_editFlag == EditFlag::EDITSQUARE && (mouseX < 42 || mouseY < 42 || mouseX >= 160 || mouseY >= 160)) {
            if (mouseX < 42) {
                mouseX = 42;
            }
            else if (mouseX >= 160 ) {
                mouseX = 159;
            }
            if (mouseY < 42) {
                mouseY = 42;
            }
            else if (mouseY >= 160 ) {
                mouseY = 159;
            }

            emit changedColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), true);
            m_model->setColor(m_colorSquare.pixelColor(mouseX-42, mouseY-42), m_whichColor);
            m_squarePos = QPointF(mouseX-5, mouseY-5);
            repaint();
        }
    }
}

/**/
/*
void ColorPicker::mouseReleaseEvent(QMouseEvent *event)

NAME

    ColorPicker::mouseReleaseEvent(QMouseEvent *event)) - react to when a mouse
    button is released

SYNOPSIS

    void ColorPicker::mouseReleaseEvent(QMouseEvent *event);
        event --> the mouse event to be processed

DESCRIPTION

    This function handles when the user releases any mouse button from their mouse.
    This resets the editing flag for this widget.

RETURNS

    None

*/
/**/
void ColorPicker::mouseReleaseEvent(QMouseEvent *event) {
    (void) event;
    m_editFlag = EditFlag::EDITNONE;
}

/**/
/*
void ColorPicker::paintEvent(QPaintEvent *event)

NAME

    ColorPicker::paintEvent(QPaintEvent *event) - react to when the widget wants to rerender

SYNOPSIS

    void ColorPicker::paintEvent(QPaintEvent *event);
        event --> the paint event to be processed

DESCRIPTION

    This function handles when the program re-renders the visuals of this widget,
    which happens when repaint() or update() is called.

RETURNS

    None

*/
/**/
void ColorPicker::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);

    // draw the square and wheel
    painter.drawPixmap(42, 42,QPixmap::fromImage(m_colorSquare));
    painter.drawPixmap(0,0,QPixmap::fromImage(m_colorWheel));

    // draw the two cursors
    painter.drawPixmap(m_squarePos,QPixmap::fromImage(m_cursor));
    painter.drawPixmap(m_wheelPos,QPixmap::fromImage(m_cursor));

    // draw the two color squares
    if (m_whichColor == 1) {
        painter.fillRect(15,180,20,20, QColor(Qt::black));
    } else {
        painter.fillRect(15,180,20,20, QColor(Qt::darkGray));
    }
    painter.fillRect(17,182,16,16, m_model->getColor(1));

    if (m_whichColor == 0) {
        painter.fillRect(2,167,20,20, QColor(Qt::black));
    } else {
        painter.fillRect(2,167,20,20, QColor(Qt::darkGray));
    }
    painter.fillRect(4,169,16,16, m_model->getColor(0));
}

/**/
/*
void ColorPicker::renderColorWheel()

NAME

    ColorPicker::renderColorWheel() - create the color wheel section

SYNOPSIS

    void ColorPicker::renderColorWheel();

DESCRIPTION

    This function generates the color wheel, a rainbow-colored gradient
    that allows users to select a hue. Because the colors of the wheel
    are constant, this function is only called once in the constructor.

RETURNS

    None

*/
/**/
void ColorPicker::renderColorWheel() {
    // make sure the area is completely transparent beforehand
    m_colorWheel.fill(qRgba(0,0,0,0));

    // generate the rainbow gradient
    QConicalGradient hueGradient((LENGTH/2),(LENGTH/2),90);
    hueGradient.setColorAt(0.0, QColor(255,255,0));
    hueGradient.setColorAt((1.0/6), QColor(255,0,0));
    hueGradient.setColorAt((2.0/6), QColor(255,0,255));
    hueGradient.setColorAt((3.0/6), QColor(0,0,255));
    hueGradient.setColorAt((4.0/6), QColor(0,255,255));
    hueGradient.setColorAt((5.0/6), QColor(0,255,0));
    hueGradient.setColorAt(1.0, QColor(255,255,0));

    // draw a circle using the colors of the rainbow gradient
    QPainter painter(&m_colorWheel);
    painter.setPen(QPen(hueGradient, 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(8, 8, 184,184);

    painter.end();
}

/**/
/*
void ColorPicker::renderColorSquare(QColor a_hue)

NAME

    ColorPicker::renderColorSquare(QColor a_hue) - create a color squre with a given color

SYNOPSIS

    void ColorPicker::renderColorSquare(QColor a_hue);
        a_hue - the color of the new color square

DESCRIPTION

    This function generates the color square, a square that allows
    the user to choose the saturation and value of their color. Because,
    the color is not constant, this function is called every time the
    user interacts with the color wheel/changes the color outside this picker.

RETURNS

    None

*/
/**/
void ColorPicker::renderColorSquare(QColor a_hue) {
    // form the gradient that goes from hue->black
    QLinearGradient hueGradient(1,1,0,117);
    hueGradient.setColorAt(0.0, a_hue);
    hueGradient.setColorAt(0.001, a_hue);
    hueGradient.setColorAt(1.0, Qt::black);

    // apply the gradient to the main square
    QPainter painter(&m_colorSquare);
    painter.fillRect(0,0,118,118, hueGradient);

    // form the gradient that goes from white->black
    QImage monoSlice = QImage(1, 118,  QImage::Format_ARGB32);
    QLinearGradient monoGradient(1,1,0,117);
    monoGradient.setColorAt(0.0, Qt::white);
    hueGradient.setColorAt(0.001, Qt::white);
    monoGradient.setColorAt(1.0, Qt::black);

    // apply this gradient to a 1-pixel wide slice of the square
    QPainter tempPainter(&monoSlice);
    tempPainter.fillRect(0,0,1,118, monoGradient);
    tempPainter.end();

    // draw this gradient slice at decreasing opacity into the main square
    for(int i = 0; i < 117; i++) {
        painter.setOpacity((118-i)/118.0);
        painter.drawImage(i,0,monoSlice);
    }

}

/**/
/*
QImage ColorPicker::generateCursor()

NAME

    ColorPicker::generateCursor() - create the cursor markers for this widget

SYNOPSIS

    QImage ColorPicker::generateCursor();

DESCRIPTION

    This function generates the cursor markers for this widget, which mark the
    hue, saturation, and value of the current colors through the color wheel
    and square. Because these cursors are constant, this function is called once
    int the constructor

RETURNS

    the image of the cursor.

*/
/**/
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

/**/
/*
void ColorPicker::calculateWheelPos(QPointF a_pos)

NAME

    ColorPicker::calculateWheelPos(QPointF a_pos) - adjusts the wheel cursor to be centered

SYNOPSIS

    void ColorPicker::calculateWheelPos(QPointF a_pos);
        a_pos --> the unprocessed position of the wheel from the mouse movement

DESCRIPTION

    Recalculates the wheel cursor position after it is moved by the mouse to be something centered
    within the wheel's width

RETURNS

    None

*/
/**/
void ColorPicker::calculateWheelPos(QPointF a_pos) {
    // the center/radius of the circle the cursor wants to place itself on
    const qreal CENTER = (LENGTH/2.0);


    // translate these points so the center of the circle is 0,0
    int x = a_pos.x()-CENTER;
    int y = a_pos.y()-CENTER;


    // find the angle formed between it and a line coming from the origin in radians
    qreal angle = qAtan2(y,x);

    // find a point that follows the same angle but the hypotenuse is the length of the radius - 8.
    qreal newY = qSin(angle) * (CENTER-9);
    qreal newX = qSqrt(qPow((CENTER-9), 2) - qPow(newY, 2));


    if (qFabs(angle) > M_PI_2) {
        newX *= -1.0;
    }

    // translate the point back to the center
    m_wheelPos = QPointF(CENTER + newX -5, CENTER + newY -5);
}

/**/
/*
void ColorPicker::calculateWheelPos(QColor a_color)

NAME

    ColorPicker::calculateWheelPos(QColor a_color) - calculates a color's corresponding square cursor position

SYNOPSIS

    void ColorPicker::calculateWheelPos(QColor a_color);
        a_color --> the color the position will be calculated from

DESCRIPTION

    Changes the square cursor position from a given color. This is needed when the color is changed from outside
    of the this color picker

RETURNS

    None

*/
/**/
void ColorPicker::calculateWheelPos(QColor a_color){
    // the center/radius of the circle the cursor wants to place itself on
    const qreal CENTER = (LENGTH/2.0);


    a_color = a_color.toHsv();
    // note that hue is a value from 0-359 so it is already within angle range
    // shift the value by 150 because there is an offset the color ranges
    qreal hueAngle = (a_color.hue() - 150.0);

    // convert this value into radians
    if (hueAngle > 180.0) {
        hueAngle = ((-180.0) + (hueAngle - 180.0));
    }
    hueAngle = qDegreesToRadians(hueAngle);


    // find a point that follows the same angle but the hypotenuse is the length of the radius - 8.
    qreal newY = qSin(hueAngle) * (CENTER-9);
    qreal newX = qSqrt(qPow((CENTER-9), 2) - qPow(newY, 2));


    if (qFabs(hueAngle) > M_PI_2) {
        newX *= -1.0;
    }

    // translate the point back to the center
    m_wheelPos = QPointF(CENTER + newX -5, CENTER + newY -5);
}

/**/
/*
void ColorPicker::calculateSquarePos(QColor a_color)

NAME

    ColorPicker::calculateSquarePos(QColor a_color) - calculates a color's corresponding square cursor position

SYNOPSIS

    void ColorPicker::calculateSquarePos(QColor a_color);
        a_color --> the color the position will be calculated from

DESCRIPTION

    Changes the square cursor position from a given color. This is needed when the color is changed from outside
    of the this color picker

RETURNS

    None

*/
/**/
void ColorPicker::calculateSquarePos(QColor a_color){
    a_color = a_color.toHsv();

    // set the values proportionate to a 0-118 range.
    // also invert the X because more saturation should be farther away
    qreal newX = (a_color.saturationF() * 117);
    qreal newY = 117-(a_color.valueF() * 117);


    // translate these values onto the square + adjust for cursor position
    m_squarePos = QPointF(newX + 37, newY + 37);
}
