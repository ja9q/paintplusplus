//
// GUI object that displays the current image
//

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include "User.h"

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    // Constructs the widget
    explicit CanvasWidget(User* user, QWidget *parent = 0);

    // Fills the canvas with one color
    void fillCanvas(QColor a_color);

protected:

    // React to a mouse click
    void mousePressEvent(QMouseEvent *event);

    // React to a mouse move (only a drag so far)
    void mouseMoveEvent(QMouseEvent *event);

    // React to when something is painted
    void paintEvent(QPaintEvent *event);

signals:
    void colorChanged(QColor a_newColor);

private:
    // The actual pixel values
    QImage m_canvas;

    // Pointer to the user; allows for access to their current colors and brush
    User* m_user;

    // Draw a line on the canvas
    void drawLine(const QPoint pt);

    // Draw a point on the canvas
    void drawPoint(const QPoint pt);
};

#endif // CANVASWIDGET_H
