//
// Implementation for the ColorWidget class
//

#include "ColorWidget.h"

#include <QWidget>
#include <QSpinBox>
#include <QIntValidator>
#include <QGridLayout>
#include <QGroupBox>
#include <QString>
#include <QLabel>

/**/
/*
ColorWidget::ColorWidget(QWidget *parent)

NAME

    ColorWidget::ColorWidget(QWidget *parent) - construct a color widget

SYNOPSIS

    ColorWidget::ColorWidget(QWidget *parent);
        parent --> the widget the object will be placed in

DESCRIPTION

    Construct the color widget and its visuals

RETURNS

    The constructed color widget

*/
/**/
ColorWidget::ColorWidget(QWidget *parent)
    : QWidget{parent}
{
    // Set the width (height has to be adjusted in the main window)
    setMinimumWidth(200);
    setMaximumWidth(200);

    QWidget *container = new QWidget(this);

    // Set up a layout
    QGridLayout *layout = new QGridLayout;


    QString colorLabels[RGB_SIZE] = {"R:", "G:", "B:"};

    // Create and label the textfields for the RGB values
    for (int i = 0 ; i < 3; i++)
    {
        m_rgbEdit[i] = new QSpinBox();
        m_rgbEdit[i]->setMaximum(255);
        m_rgbEdit[i]->setMinimum(0);
        m_rgbEdit[i]->setValue(0);
        layout->addWidget(new QLabel(colorLabels[i]), 0, 2*i, 1, 1);
        layout->addWidget(m_rgbEdit[i], 0, (2*i)+1, 1, 1);

        // Connect the text field to update the color
        connect(m_rgbEdit[i], &QSpinBox::valueChanged, this, [=](){emit valueChanged(getColor());});
    }

    container->setLayout(layout);
}

/**/
/*
void ColorWidget::updateColor(QColor a_newColor)

NAME

    ColorWidget::updateColor(QColor a_newColor) - react to when the color has been changed
        outside of the widget

SYNOPSIS

    void ColorWidget::updateColor(QColor a_newColor);
        a_newColor - the new color to display

DESCRIPTION

    Update the fields of this widget to reflect the new color

RETURNS

    None

*/
/**/
void ColorWidget::updateColor(QColor a_newColor) {
    // Copy over the red, green, and blue values from the given color
    m_rgbEdit[RED]->setValue(a_newColor.red());
    m_rgbEdit[GREEN]->setValue(a_newColor.green());
    m_rgbEdit[BLUE]->setValue(a_newColor.blue());
}

/**/
/*
QColor ColorWidget::getColor()

NAME

    QColor ColorWidget::getColor() - calculate the new color

SYNOPSIS

    QColor ColorWidget::getColor();

DESCRIPTION

    Calculate the new value from the displayed text fields

RETURNS

    The QColor that is displayed

*/
/**/
QColor ColorWidget::getColor() {
    return QColor(m_rgbEdit[RED]->value(), m_rgbEdit[GREEN]->value(), m_rgbEdit[BLUE]->value());
}
