#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include "PaintModel.h"

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    static const int LENGTH = 200;

    explicit ColorPicker(PaintModel *a_model, QWidget *parent = nullptr);

protected:

    // React to a mouse click
    void mousePressEvent(QMouseEvent *event);

    // React to a mouse move (only a drag so far)
    void mouseMoveEvent(QMouseEvent *event);

    // React to when the mouse is released
    void mouseReleaseEvent(QPaintEvent *event);

    // React to when something is painted
    void paintEvent(QPaintEvent *event);

signals:
    void changedColor(QColor a_color);

private:
    const int EDITNONE = 0;
    const int EDITWHEEL = 1;
    const int EDITSQUARE = 2;

    PaintModel* m_model;

    // positions to place the cursors.
    QPointF m_squarePos;
    QPointF m_wheelPos;

    // a cursor that is drawn to display current color and hue
    QImage m_cursor;

    // Visual components needed to display the color wheel and square
    QImage m_colorWheel;
    QImage m_colorSquare;

    // Indicates what is being edited (if anything)
    int m_editFlag;

    // Draw the color wheel (happens once when intializing)
    void renderColorWheel();
    // Draw the color square (happens every time wheel is clicked/ hue changes)
    void renderColorSquare(QColor a_hue);

    // Draw the cursor for the wheel and square
    QImage generateCursor();
};

#endif // COLORWHEEL_H
