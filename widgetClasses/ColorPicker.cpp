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

    ColorPicker::ColorPicker(*a_model, QWidget *parent) - the parameter constructor

SYNOPSIS

    ColorPicker::ColorPicker(PaintModel *a_model, QWidget *parent);
        a_model --> the paint model the object will reference and manipulate
        parent --> the parent widget that the

DESCRIPTION

    parameter constructor

RETURNS

    The constructed ColorPicker

*/
/**/
ColorPicker::ColorPicker(PaintModel *a_model, QWidget *parent)
    : QWidget{parent}, m_model(a_model), m_cursor(generateCursor()),
    m_colorWheel(QImage(200, 200, QImage::Format_ARGB32)),
    m_colorSquare(QImage(118, 118, QImage::Format_ARGB32)),
    m_whichColor(0)
{
    // set dimensions + a minimum height so the outer widget renders correctly
    resize(200,200);
    setMinimumHeight(200);

    // initialize the visual components
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
    // only recalculate the square/wheel positions if the user isn't
    // actively editing them already (prevent signal loops)
    if (m_editFlag != EditFlag::EDITWHEEL) {
        calculateSquarePos(a_color);
    }
    if (m_editFlag != EditFlag::EDITSQUARE) {
        calculateWheelPos(a_color);
    }

    // Rerender the color square with the new hue
    renderColorSquare(m_colorWheel.pixelColor(QPoint(m_wheelPos.x()+5, m_wheelPos.y()+5)));

    // update the color in the model for the case the signal is from the outer widget
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

    // calculate the position of the color square
    const int squarePos = (m_colorWheel.width() - m_colorSquare.width())/2;
    // the color the cursor refers to
    const int squareCurOff = squarePos-5;

    // if this is a left click
    if(event->buttons() & Qt::LeftButton){
        // if this is a click on the color wheel, then change the color of the square
        if (m_colorWheel.pixelColor(event->pos()) != QColor(Qt::transparent)) {
            m_editFlag = EditFlag::EDITWHEEL;
            // update the position of the wheel cursor
            calculateWheelPos(event->pos());

            // update the color square + change the color in the outer widget and model
            renderColorSquare(m_colorWheel.pixelColor(event->pos()));
            emit changedColor(m_colorSquare.pixelColor(m_squarePos.x()-squareCurOff, m_squarePos.y()-squareCurOff), true);
            m_model->setColor(m_colorSquare.pixelColor(m_squarePos.x()-squareCurOff, m_squarePos.y()-squareCurOff), m_whichColor);

            // rerender + set the flag
            repaint();
        }
        // if the color square was clicked
        else if (mouseX >= squarePos && mouseY >= squarePos && mouseX<height()-squarePos && mouseY <height()-squarePos) {

            m_editFlag = EditFlag::EDITSQUARE;
            // change the color in the outer widget and model
            emit changedColor(m_colorSquare.pixelColor(mouseX-squarePos, mouseY-squarePos), true);
            m_model->setColor(m_colorSquare.pixelColor(mouseX-squarePos, mouseY-squarePos), m_whichColor);

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
    const int length = height();

    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    const int squarePos = (m_colorWheel.width() - m_colorSquare.width())/2;
    const int squareCurOff = squarePos-5;

    // if this is a left click within the range of the widget
    if((event->buttons() & Qt::LeftButton) && mouseX >=0 && mouseX < length && mouseY >=0 && mouseY < length){
        // if the user is editing the wheel, recalculate the wheel cursor and update the color accordingly
        if (m_editFlag == EditFlag::EDITWHEEL) {
            calculateWheelPos(event->pos());
            renderColorSquare(m_colorWheel.pixelColor(QPoint(m_wheelPos.x()+5, m_wheelPos.y()+5)));
            emit changedColor(m_colorSquare.pixelColor(m_squarePos.x()-squareCurOff, m_squarePos.y()-squareCurOff), true);
            m_model->setColor(m_colorSquare.pixelColor(m_squarePos.x()-squareCurOff, m_squarePos.y()-squareCurOff), m_whichColor);
            repaint();
        }
        // if the user is editing the square and is within the bounds of the square, then update the color and square cursor position
        else if (m_editFlag == EditFlag::EDITSQUARE && mouseX >= squarePos && mouseY >= squarePos && mouseX< (length-squarePos-2) && mouseY < (length-squarePos-2)) {
            emit changedColor(m_colorSquare.pixelColor(mouseX-squarePos, mouseY-squarePos), true);
            m_model->setColor(m_colorSquare.pixelColor(mouseX-squarePos, mouseY-squarePos), m_whichColor);
            m_squarePos = QPointF(mouseX-5, mouseY-5);
            repaint();
        // if the user is editing the square, but is out of bounds of the actual color square, then adjust the position before calculating the new color/cursor
        } else if (m_editFlag == EditFlag::EDITSQUARE && (mouseX < squarePos || mouseY < squarePos || mouseX >= (length-squarePos-2) || mouseY >= (length-squarePos-2))) {
            if (mouseX < squarePos) {
                mouseX = squarePos;
            }
            else if (mouseX >= length-squarePos-2 ) {
                mouseX = length-squarePos-3;
            }
            if (mouseY < squarePos) {
                mouseY = squarePos;
            }
            else if (mouseY >= length-squarePos-2 ) {
                mouseY = length-squarePos-2;

            }

            emit changedColor(m_colorSquare.pixelColor(mouseX-squarePos, mouseY-squarePos), true);
            m_model->setColor(m_colorSquare.pixelColor(mouseX-squarePos, mouseY-squarePos), m_whichColor);
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

    // calculate the position of the wheel
    int squarePos = (m_colorWheel.width() - m_colorSquare.width())/2;

    // draw the square and wheel
    painter.drawPixmap(squarePos, squarePos, QPixmap::fromImage(m_colorSquare));
    painter.drawPixmap(0,0,QPixmap::fromImage(m_colorWheel));

    // draw the two cursors
    painter.drawPixmap(m_squarePos,QPixmap::fromImage(m_cursor));
    painter.drawPixmap(m_wheelPos,QPixmap::fromImage(m_cursor));

    // draw the two color squares
    if (m_whichColor == 1) {
        painter.fillRect(15,height()-20,20,20, QColor(Qt::black));
    } else {
        painter.fillRect(15,height()-20,20,20, QColor(Qt::darkGray));
    }
    painter.fillRect(17,height()-18,16,16, m_model->getColor(1));

    if (m_whichColor == 0) {
        painter.fillRect(2,height()-33,20,20, QColor(Qt::black));
    } else {
        painter.fillRect(2,height()-33,20,20, QColor(Qt::darkGray));
    }
    painter.fillRect(4,height()-31,16,16, m_model->getColor(0));
}

/**/
/*
void ColorPicker::resizeEvent(QResizeEvent *event)

NAME

    ColorPicker::resizeEvent(QResizeEvent *event) - react to when the widget is resized

SYNOPSIS

    void ColorPicker::resizeEvent(QResizeEvent *event);
        event --> the resize event to react to

DESCRIPTION

    resize the widget according to the new given dimensions

RETURNS

    None

*/
/**/
void ColorPicker::resizeEvent(QResizeEvent *event) {
    (void) event;

    // don't use the event because it gives an inaccurate height. instead use the dimensions of the parent widget
    // find the length of the widget based off of the free vertical/horizontal space (choose the smaller of the two)
    int length = (parentWidget()->height()-80 < parentWidget()->width()-20) ? parentWidget()->height()-80 : parentWidget()->width()-20;

    // have a minimum height of 200
    if (length < 200) {
        length = 200;
    }

    // resize the actual widget
    resize(length,length);
    setMinimumHeight(length);

    // resize the color wheel
    m_colorWheel = QImage(length, length, QImage::Format_ARGB32_Premultiplied);
    m_colorWheel.fill(Qt::transparent);

    // calculate the size of the color square and resize it
    int squareLen = sqrt(((length-36)*(length-36))/2);
    m_colorSquare = QImage(squareLen, squareLen, QImage::Format_ARGB32_Premultiplied);

    // rerender the widget
    m_colorSquare.fill(Qt::transparent);
    calculateWheelPos(m_model->getColor(m_whichColor));
    calculateSquarePos(m_model->getColor(m_whichColor));
    renderColorWheel();
    renderColorSquare();
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
    QConicalGradient hueGradient((width()/2),(width()/2),90);
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
    painter.drawEllipse(8, 8, height()-16,height()-16);

    painter.end();
}

/**/
/*
void ColorPicker::renderColorSquare(QColor a_hue)

NAME

    ColorPicker::renderColorSquare(QColor a_hue) - create a color squre with a given color

SYNOPSIS

    void ColorPicker::renderColorSquare(QColor a_hue);
        a_hue --> the color of the new color square; this is black by default to flag
            that the color should be based off of the model

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
    const int len = m_colorSquare.height();

    // if the color is black (default), then use the color from the model
    if (a_hue == QColor(Qt::black)) {
        a_hue = m_model->getColor(m_whichColor);
        a_hue.toHsv();
        a_hue.setHsv(a_hue.hue(), 255, 255);
        if (a_hue.hue() == -1) {
            a_hue.setHsv(0, 255, 255);
        }
    }

    // form the gradient that goes from hue->black
    QLinearGradient hueGradient(1,1,0,len-2);
    hueGradient.setColorAt(0.0, a_hue);
    hueGradient.setColorAt(0.001, a_hue);
    hueGradient.setColorAt(1.0, Qt::black);

    // apply the gradient to the main square
    QPainter painter(&m_colorSquare);
    painter.fillRect(0,0,len,len, hueGradient);

    // form the gradient that goes from white->black
    QImage monoSlice = QImage(1, len,  QImage::Format_ARGB32);
    QLinearGradient monoGradient(1,1,0, len-1);
    monoGradient.setColorAt(0.0, Qt::white);
    monoGradient.setColorAt(1.0, Qt::black);

    // apply this gradient to a 1-pixel wide slice of the square
    QPainter tempPainter(&monoSlice);
    tempPainter.fillRect(0,0,1,len, monoGradient);
    tempPainter.end();

    // draw this gradient slice at decreasing opacity into the main square
    qreal temp = len;
    for(int i = 0; i < len-1; i++) {
        painter.setOpacity((len-i)/temp);
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

    // draw the cursor by making a white circle around a black circle
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
    const qreal CENTER = (width()/2.0);


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
    const qreal CENTER = (width()/2.0);


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

    const int squarePos = ((m_colorWheel.width() - m_colorSquare.width())/2)-5;

    // set the values proportionate to the color square's width.
    // also invert the X because more saturation should be farther away
    qreal newX = a_color.saturationF() * m_colorSquare.width();
    qreal newY = m_colorSquare.width()-(a_color.valueF() * m_colorSquare.width());


    // translate these values onto the square + adjust for cursor position
    m_squarePos = QPointF(newX + squarePos, newY + squarePos);
}
