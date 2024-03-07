//
// The ColorWidget is a GUI element that allows user to choose
// a color either by using the square color wheel or entering the RGB values.
//

#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QImage>
#include <QSpinBox>
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
    void updateColor(QColor a_newColor);

signals:
    // Alert other objects when the color has changed
    void valueChanged(QColor a_newColor);

private:

    static const int RGB_SIZE = 3; // The number of color parameters

    const int RED = 0;      // Index for red
    const int GREEN = 1;    // Index for green
    const int BLUE = 2;     // Index for blue

    ColorPicker* m_colorPicker;

    PaintModel* m_model;

    // Three text fields that allow for the color to be changed
    QSpinBox* m_rgbEdit[RGB_SIZE];

    // Set up the text fields that manually the RGB values
    QWidget* createRgbEdit();

    // Calculate the current color
    QColor getColor();

};

#endif // COLORWIDGET_H
