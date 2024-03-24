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
    m_tempCanvas.fill(QColor(Qt::transparent));
    m_canvas.fill(QColor(Qt::white));
    update();
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

    updates the canvas. This is called by the undo action

RETURNS

    None

*/
/**/
void CanvasWidget::setCanvas(QImage a_newCanvas) {
    m_tempCanvas.fill(QColor(Qt::transparent));
    m_canvas = a_newCanvas;
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
        // If this was a left mouse click, send the event, canvases, and colors to the current tool
        BaseTool* currentTool = m_user->getCurrentTool();
        currentTool->processDrag(&m_canvas, &m_tempCanvas, event, m_user->getColor(0), m_user->getColor(1));
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
