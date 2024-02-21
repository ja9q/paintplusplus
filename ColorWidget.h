//
// The ColorWidget is a GUI element that allows user to choose
// a color either by using the square color wheel or entering the RGB values.
//

#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QLineEdit>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    // constructor
    explicit ColorWidget(QWidget *parent = nullptr);

public slots:
    // React to when an individual field has been changed
    void signalNewColor();

    // React to when a color has been changed elsewhere.
    void updateColor(QColor a_newColor);


signals:
    // Alert other objects when the color has changed
    void valueChanged(QColor a_newColor);

private:

    static const int RGB_SIZE = 3; // The number of color parameters

    const int RED = 0;      // Index for red
    const int GREEN = 1;    // Index for green
    const int BLUE = 2;     // index


    // Three text fields that allow for the color to be changed
    QLineEdit* m_rgbEdit[RGB_SIZE];

    // Calculate the current color
    QColor getColor();

};

#endif // COLORWIDGET_H
