//
// Implementation for the ColorWidget class
//

#include "ColorWidget.h"

#include <QLineEdit>
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
    setMinimumWidth(170);
    setMaximumWidth(200);

    // Set up a layout
    QGridLayout *layout = new QGridLayout;


    QString colorLabels[RGB_SIZE] = {"R:", "G:", "B:"};

    // Create and label the textfields for the RGB values
    for (int i = 0 ; i < 3; i++)
    {
        m_rgbEdit[i] = new QLineEdit("0");
        m_rgbEdit[i]->setValidator(new QIntValidator(0,255, this));
        layout->addWidget(new QLabel(colorLabels[i]), 0, 2*i, 1, 1);
        layout->addWidget(m_rgbEdit[i], 0, (2*i)+1, 1, 1);

        // Connect the text field to update the color
        connect(m_rgbEdit[i], &QLineEdit::editingFinished, this, &ColorWidget::signalNewColor);
    }

    setLayout(layout);
}

/**/
/*
void ColorWidget::signalNewColor()

NAME

    ColorWidget::signalNewColor - react to when the color has been changed here

SYNOPSIS

    void ColorWidget::signalNewColor()

DESCRIPTION

    Notify other objects that the color should be changed

RETURNS

    None

*/
/**/
void ColorWidget::signalNewColor() {
    emit valueChanged(getColor());
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
    m_rgbEdit[RED]->setText(QString::number(a_newColor.red()));
    m_rgbEdit[GREEN]->setText(QString::number(a_newColor.green()));
    m_rgbEdit[BLUE]->setText(QString::number(a_newColor.blue()));
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
    return QColor(m_rgbEdit[RED]->displayText().toInt(), m_rgbEdit[GREEN]->displayText().toInt(), m_rgbEdit[BLUE]->displayText().toInt());
}
