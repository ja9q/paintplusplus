#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);

protected:

    // React to a mouse click
    void mousePressEvent(QMouseEvent *event);

    // React to a mouse move (only a drag so far)
    void mouseMoveEvent(QMouseEvent *event);

    // React to when something is painted
    void paintEvent(QPaintEvent *event);

signals:

private:
    // positions to place the cursors.
    QPointF m_squarePos;
    QPointF m_wheelPos;

    // Visual components needed to display the color wheel and square
    QImage m_colorWheel;
    QImage m_colorSquare;

    // Draw the color wheel (happens once when intializing)
    void renderColorWheel();
    // Draw the color square (happens every time wheel is clicked/ hue changes)
    void renderColorSquare(QColor a_hue);
    // Draw the cursor for the wheel
    void drawWheelCursor();
    // Draw the cursor for the square
    void drawSquareCursor();
};

#endif // COLORWHEEL_H
