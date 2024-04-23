//
// The ColorWidget is a GUI element that allows user to choose
// a color either by using the square color wheel or entering the RGB values.
//

#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QImage>
#include <QSpinBox>
#include <QVBoxLayout>
#include "PaintModel.h"

#include "ColorPicker.h"

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    // constructor
    explicit ColorWidget(PaintModel *a_model, QWidget *parent = nullptr);

public slots:
    // React to when a color has been changed elsewhere.
    void updateColor(QColor a_newColor, bool a_noSignal = false);

private slots:
    // react to when the color picker switches current color (e.g. switching to color2)
    void swapColor(int a_newColor);

signals:
    // Alert other objects when the color has changed
    void valueChanged(QColor a_newColor, int a_whichColor);

protected:
    void resizeEvent(QResizeEvent *event);

private:

    static const int RGB_SIZE = 3; // The number of color parameters

    const int RED = 0;      // Index for red
    const int GREEN = 1;    // Index for green
    const int BLUE = 2;     // Index for blue

    ColorPicker* m_colorPicker; // the GUI component of the color wheel square

    PaintModel* m_model; // the model it displays for

    int m_whichColor; // the color mode it is displaying

    bool m_isEmitting; // whether to emit to the canvas or not;

    // Three text fields that allow for the color to be changed
    QSpinBox* m_rgbEdit[RGB_SIZE];

    QVBoxLayout* m_layout;

    // Set up the text fields that manually the RGB values
    QWidget* createRgbEdit();

    // Calculate the current color
    QColor getColor();

};

#endif // COLORWIDGET_H
