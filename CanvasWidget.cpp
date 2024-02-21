#include "CanvasWidget.h"

#include "User.h"
#include "BaseTool.h"

#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPainterPath>
#include <QPainter>


CanvasWidget::CanvasWidget(User* user, QWidget *parent)
    : QWidget(parent), m_user(user)
{

    m_canvas = QPixmap(width(), height()).toImage();

    fillCanvas(QColor(Qt::white));
}

void CanvasWidget::fillCanvas(QColor a_color) {
    m_canvas.fill(a_color);
    update();
}


void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton){
        BaseTool* currentTool = m_user->getCurrentTool();
        currentTool->processClick(&m_canvas, event->position(), m_user->getColor(0), m_user->getColor(1));
        repaint();
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        BaseTool* currentTool = m_user->getCurrentTool();

        currentTool->processDrag(&m_canvas, event->position(), m_user->getColor(0), m_user->getColor(1));
        repaint();
    }
}





void CanvasWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
}
