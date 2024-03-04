//
// Implementation for the CanvasWidget class
//

#include "CanvasWidget.h"

#include "User.h"
#include "BaseTool.h"

#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <QPixmap>
#include <QPainter>

// Constructor
CanvasWidget::CanvasWidget(User* user, QWidget *parent)
    : QWidget(parent), m_user(user)
{

    // initialize the canvas and make it white
    m_canvas = QPixmap(width(), height()).toImage();
    fillCanvas(QColor(Qt::white));
}

/**/
/*
void CanvasWidget::fillCanvas(QColor a_color)

NAME

    CanvasWidget::fillCanvas(QColor a_color) - fill the canvas with a color

SYNOPSIS

    void CanvasWidget::fillCanvas(QColor a_color);
        a_color --> the color to fill the canvas with

DESCRIPTION

    This fills the canvas with a given color

RETURNS

    None

*/
/**/
void CanvasWidget::fillCanvas(QColor a_color) {
    m_canvas.fill(a_color);
    update();
}

/**/
/*
void CanvasWidget::mousePressEvent(QMouseEvent *event)

NAME

    CanvasWidget::mousePressEvent(QMouseEvent *event) - react to a mouse click

SYNOPSIS

    void CanvasWidget::mousePressEvent(QMouseEvent *event);
        event --> the mouse even that happened, a click

DESCRIPTION

    If the left button was pressed, the event is sent to the current tool.
    If the right button was presssed, the current color is copied.

RETURNS

    None

*/
/**/
void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton){
        // If this was a left mouse click, send the event, canvas, and colors to the current tool
        BaseTool* currentTool = m_user->getCurrentTool();
        currentTool->processClick(&m_canvas, event->position(), m_user->getColor(0), m_user->getColor(1));
        // Rerender the component
        repaint();
    }
    else if (event->buttons() & Qt::RightButton){
        QColor newColor = m_canvas.pixelColor(event->pos());
        // If this was a right mouse click, copy the color the click was on.
        m_user->setColor(newColor, 0);
        emit colorChanged(newColor);
    }
}

/**/
/*
void CanvasWidget::mouseMoveEvent(QMouseEvent *event)

NAME

    CanvasWidget::mouseMoveEvent(QMouseEvent *event) - react to a mouse drag

SYNOPSIS

    void CanvasWidget::mouseMoveEvent(QMouseEvent *event);
        event --> the mouse event that happened, a drag

DESCRIPTION

    If the left button was pressed, the event is sent to the current tool.

RETURNS

    None

*/
/**/
void CanvasWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        // If this was a left mouse click, send the event, canvas, and colors to the current tool
        BaseTool* currentTool = m_user->getCurrentTool();
        currentTool->processDrag(&m_canvas, event->position(), m_user->getColor(0), m_user->getColor(1));
        // Rerender the component
        repaint();
    }
}

/**/
/*
void CanvasWidget::paintEvent(QPaintEvent *event)

NAME

    CanvasWidget::paintEvent(QPaintEvent *event) - react to a paint event

SYNOPSIS

    void CanvasWidgetpaintEvent(QPaintEvent *event);
        event --> the paint event that happened, usually a repaint or update

DESCRIPTION

    When the component is updated, copy the values on the stored canvas
    into the display

RETURNS

    None

*/
/**/
void CanvasWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    // Copy the values on the canvas onto the display
    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));

    painter.end();
}
