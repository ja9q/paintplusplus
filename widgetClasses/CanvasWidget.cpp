//
// Implementation for the CanvasWidget class
//

#include "CanvasWidget.h"

#include "./../miscClasses/User.h"
#include "./../toolClasses/BaseTool.h"

#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QPainter>

/**/
/*
CanvasWidget::CanvasWidget(User* user, QWidget *parent)

NAME

    CanvasWidget::CanvasWidget(User* user, QWidget *parent) - constructor
SYNOPSIS

    CanvasWidget::CanvasWidget(User* user, QWidget *parent)
        user --> a pointer to the user that interfaces with the canvas
        parent --> the widget that contains the canvas; this is a nullptr by default

DESCRIPTION

    Constructs a CanvasWidget

RETURNS

    The constructed CanvasWidget

*/
/**/
CanvasWidget::CanvasWidget(User* user, QWidget *parent)
    : QWidget(parent), m_user(user)
{
    resize(1000,750);

    // initialize the canvases
    m_canvas = QImage(width(), height(), QImage::Format_ARGB32);
    m_tempCanvas = QImage(width(), height(), QImage::Format_ARGB32);

    // make the temporary canvas clear and the actual canvas white
    flushTemp();
    m_canvas.fill(QColor(Qt::white));
    update();
}

/**/
/*
QImage* CanvasWidget::getCanvas()

NAME

    CanvasWidget::getCanvas() - getter for the permanent canvas

SYNOPSIS

    QImage* CanvasWidget::getCanvas();

DESCRIPTION

    getter for the canvas (not to be confused with the temporary canvas

RETURNS

    A pointer to the canvas

*/
/**/
QImage* CanvasWidget::getCanvas() {
    return &m_canvas;
}

/**/
/*
QImage* CanvasWidget::getTempCanvas()

NAME

    CanvasWidget::getTempCanvas() - getter for the temporary canvas

SYNOPSIS

    QImage* CanvasWidget::getTempCanvas();

DESCRIPTION

    getter for the temporary canvas

RETURNS

    A pointer to the temporary canvas

*/
/**/
QImage* CanvasWidget::getTempCanvas() {
    return &m_tempCanvas;
}

/**/
/*
void CanvasWidget::setCanvas(QImage a_newCanvas)

NAME

    CanvasWidget::setCanvas(QImage a_newCanvas) - setter for the canvas

SYNOPSIS

    void CanvasWidget::setCanvas(QImage a_newCanvas);
        a_newCanvas --> the new canvas to be displayed

DESCRIPTION

    updates the canvas to be a separate image

RETURNS

    None

*/
/**/
void CanvasWidget::setCanvas(QImage a_newCanvas) {
    // blank out the temporary canvas
    flushTemp();

    // change the canvas
    m_canvas = a_newCanvas;

    // if the new canvas has different dimensions, change the dimensions of the canvas
    if (a_newCanvas.height() != height() || a_newCanvas.width() != width()) {
        resize(a_newCanvas.width(), a_newCanvas.height());
        a_newCanvas.fill(Qt::transparent);
        m_tempCanvas = a_newCanvas;
    }

    update();
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
    emit canvasChanged(m_canvas);
}

/**/
/*
void CanvasWidget::flushTemp()

NAME

    CanvasWidget::flushTemp() - clear out the temporary canvas;

SYNOPSIS

    void CanvasWidget::flushTemp();

DESCRIPTION

    clears out the temporary canvas. this is necessary for rerendering it with new contents.

RETURNS

    None

*/
/**/
void CanvasWidget::flushTemp() {
    m_tempCanvas.fill(Qt::transparent);
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
    // set the keyboard focus (needed for keyboard shortcuts)
    setFocus();

    if(event->buttons() & Qt::LeftButton){
        // If this was a left mouse click, send the event, canvas, and colors to the current tool
        BaseTool* currentTool = m_user->getCurrentTool();
        currentTool->processClick(&m_canvas, &m_tempCanvas, event, m_user->getColor(0), m_user->getColor(1));
        // Rerender the component
        repaint();
    }
    else if (event->buttons() & Qt::RightButton){
        QColor newColor = m_canvas.pixelColor(event->pos());
        // If this was a right mouse click, copy the color the click was on.
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
        // If this was a left mouse click, send the event, canvases, and colors to the current tool
        BaseTool* currentTool = m_user->getCurrentTool();
        currentTool->processDrag(&m_canvas, &m_tempCanvas, event, m_user->getColor(0), m_user->getColor(1));
        // Rerender the component
        repaint();
    }
    else if (event->buttons() & Qt::RightButton){
        QColor newColor = m_canvas.pixelColor(event->pos());
        // If this was a right mouse click, copy the color the click was on.
        emit colorChanged(newColor);
    }
}

/**/
/*
void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)

NAME

    CanvasWidget::mouseReleaseEvent(QMouseEvent *event) - react to a mouse release

SYNOPSIS

    void CanvasWidget::mouseReleaseEvent(QMouseEvent *event);
        event --> the mouse event that happened, a button release

DESCRIPTION

    If the left button was released, the event is sent to the current tool.

RETURNS

    None

*/
/**/
void CanvasWidget::mouseReleaseEvent(QMouseEvent *event){
    // When drawing with a tool, the tool first applies it to a temporary canvas, and the changes to the temp canvas
    // are made permanent when the user stops drawing (releases the left mouse button).

    // some other tools might have different behaviors (e.g. select and shapes) so tell the tool to deal with it
    if(event->button() == Qt::LeftButton){
        BaseTool* currentTool = m_user->getCurrentTool();
        if (currentTool->processMouseRelease(&m_canvas, &m_tempCanvas, event, m_user->getColor(0), m_user->getColor(1)) == 1) {
            emit canvasChanged(m_canvas);
        }
        repaint();
    }
}

/**/
/*
void CanvasWidget::mouseDoubleClickEvent(QMouseEvent *event)

NAME

    CanvasWidget::mouseDoubleClickEvent(QMouseEvent *event) - react to a doubleclick

SYNOPSIS

    void CanvasWidget::mouseDoubleClickEvent(QMouseEvent *event);
        event --> the mouse event that happened, a doubleclick

DESCRIPTION

    description

RETURNS

    None

*/
/**/
void CanvasWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    // some other tools might have different behaviors (e.g. select and shapes) so tell the tool to deal with it
    if(event->button() == Qt::LeftButton){
        BaseTool* currentTool = m_user->getCurrentTool();
        if (currentTool->processDoubleClick(&m_canvas, &m_tempCanvas, event, m_user->getColor(0), m_user->getColor(1)) == 1) {
            emit canvasChanged(m_canvas);
        }
        repaint();
    }
}

/**/
/*
void CanvasWidget::paintEvent(QPaintEvent *event)

NAME

    CanvasWidget::paintEvent(QPaintEvent *event) - react to a paint event release

SYNOPSIS

    void CanvasWidget::paintEvent(QPaintEvent *event);
        event --> the paint event; a trigger to rerender the canvas

DESCRIPTION

    Rerenders the canvas.

RETURNS

    None

*/
/**/
void CanvasWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Copy the values on the canvas then the temporary canvas onto the display
    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
    painter.drawPixmap(0,0,QPixmap::fromImage(m_tempCanvas));
    painter.end();
}
