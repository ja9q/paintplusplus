

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include "User.h"

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(User* user, QWidget *parent = 0);

    void fillCanvas(QColor a_color);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

private:
    QImage m_canvas;
    User* m_user;

    void drawLine(const QPoint pt);
    void drawPoint(const QPoint pt);
};

#endif // CANVASWIDGET_H
