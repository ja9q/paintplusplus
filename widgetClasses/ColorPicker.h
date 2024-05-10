//
// GUI for the color square wheel part of the color widget
//

#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <QWidget>
#include "./../miscClasses/PaintModel.h"

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    // constructor
    explicit ColorPicker(PaintModel *a_model, QWidget *parent = nullptr);

public slots:
    // setter for color
    void updateColor(QColor a_color);

protected:

    // React to a mouse click
    void mousePressEvent(QMouseEvent *event);

    // React to a mouse move (only a drag so far)
    void mouseMoveEvent(QMouseEvent *event);

    // React to when the mouse is released
    void mouseReleaseEvent(QMouseEvent *event);

    // React to when something is painted
    void paintEvent(QPaintEvent *event);

    // React to when the widget is resized
    void resizeEvent(QResizeEvent *event);

signals:
    // alert other components that the current color values is different
    void changedColor(QColor a_color, bool a_blockSignal);

    // alert other components that the current color is different (e.g. now using color2)
    void swappedColor(int a_whichColor);

private:
    // modes for the edit flag
    enum class EditFlag {
            EDITNONE, EDITWHEEL, EDITSQUARE
    };

    // Copy the model because it needs to display the model's state (colors 1 and 2)
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
    EditFlag m_editFlag;

    // Indicates which color is being modified
    int m_whichColor;

    // Draw the color wheel (happens once when intializing)
    void renderColorWheel();

    // Draw the color square (happens every time wheel is clicked/ hue changes)
    void renderColorSquare(QColor a_hue = QColor(Qt::black));

    // Draw the cursor for the wheel and square
    QImage generateCursor();

    // Calculate the position of the wheel position to be centered
    void calculateWheelPos(QPointF a_pos);

    // Calculate the position of the wheel position to be centered
    void calculateWheelPos(QColor a_color);

    // Calculate the position of the wheel position to be centered
    void calculateSquarePos(QColor a_color);
};

#endif // COLORWHEEL_H
