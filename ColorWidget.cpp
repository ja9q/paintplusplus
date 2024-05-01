//
// Implementation for the ColorWidget class
//

#include "ColorWidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QIntValidator>
#include <QGridLayout>
#include <QGroupBox>
#include <QString>
#include <QPainter>
#include <QImage>
#include <QLabel>

#include "ColorPicker.h"
#include "PaintModel.h"

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
ColorWidget::ColorWidget(PaintModel *a_model, QWidget *parent)
    : QWidget{parent}, m_model(a_model), m_whichColor(0)
{
    // initialize the colorpicker and connect the appropriate slots/signals between the widget, the picker, and the model
    m_colorPicker = new ColorPicker(a_model);
    connect(m_colorPicker, &ColorPicker::changedColor, this, &ColorWidget::updateColor);
    connect((m_model->getCanvas()), &CanvasWidget::colorChanged, m_colorPicker, &ColorPicker::updateColor);
    connect(this, &ColorWidget::valueChanged, m_colorPicker, [=](QColor a_color){m_colorPicker->updateColor(a_color);});

    // Set up a layout
    m_layout = new QVBoxLayout(this);

    resize(200,200);

    // Add the color picker and rgb control to the layout
    m_layout->addWidget(m_colorPicker);
    m_layout->addWidget(createRgbEdit());

    update();
}

/**/
/*
void ColorWidget::updateColor(QColor a_newColo, bool a_noSignalr)

NAME

    ColorWidget::updateColor(QColor a_newColor, bool a_noSignal) - react to when the color has been changed
        outside of the widget

SYNOPSIS

    void ColorWidget::updateColor(QColor a_newColor, bool a_noSignal);
        a_newColor - the new color to display
        a_noSignal - whether to echo the fact that the color has been updated

DESCRIPTION

    Update the fields of this widget to reflect the new color

RETURNS

    None

*/
/**/
void ColorWidget::updateColor(QColor a_newColor, bool a_noSignal) {
    // if the signal is to not be echoed, then prevent it from emitting
    // the signal is naturally emitted when the rgb edit fields are changed
    m_isEmitting = !a_noSignal;

    // Copy over the red, green, and blue values from the given color
    m_rgbEdit[RED]->setValue(a_newColor.red());
    m_rgbEdit[GREEN]->setValue(a_newColor.green());
    m_rgbEdit[BLUE]->setValue(a_newColor.blue());

    // reset the signal to be emitting
    m_isEmitting = true;
}

/**/
/*
void ColorWidget::swapColor(int a_newColor)

NAME

    ColorWidget::swapColor(int a_newColor) - swap the color that is being edited

SYNOPSIS

    void ColorWidget::swapColor(int a_newColor);
        a_newColor --> the new color type (either 0 or 1)

DESCRIPTION

    swaps the color that is being edited; this is triggered by the inner
    color picker only.

RETURNS

    None

*/
/**/
void ColorWidget::swapColor(int a_newColor) {
    // change the current color and display the new color's rgb values
    m_whichColor = a_newColor;
    updateColor(m_model->getColor(a_newColor));
}

/**/
/*
void ColorWidget::resizeEvent(QResizeEvent *event)

NAME

    ColorWidget::resizeEvent(QResizeEvent *event) - react to when the widget is resized

SYNOPSIS

    void ColorWidget::resizeEvent(QResizeEvent *event);
        event - the resize event; contains the new dimensions

DESCRIPTION

    the resize event has been overridden to keep the spacing between the
    color picker and rgb fields more stable

RETURNS

    None

*/
/**/
void ColorWidget::resizeEvent(QResizeEvent *event) {
    setMaximumHeight(event->size().height()+10);

    // if there is excess height, then update the spacer to fill in this excess spacing
    if (event->size().height() > m_colorPicker->height()+80) {
        m_layout->removeItem(m_layout->itemAt(2));
        m_layout->addSpacing(event->size().height() - (m_colorPicker->height()+100));
    }


}

/**/
/*
QWidget* ColorWidget::createRgbEdit()

NAME

    ColorWidget::createRgbEdit() - initialize and arrange the
        rgb editing fields

SYNOPSIS

    QWidget* ColorWidget::createRgbEdit();

DESCRIPTION

    Initialize the rgb edits and arrange them into a widget.

RETURNS

    A pointer to the widget that contains all of the rgb edit fields

*/
/**/
QWidget* ColorWidget::createRgbEdit() {
    QWidget *container = new QWidget();

    // Set up a layout
    QGridLayout *layout = new QGridLayout(container);


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
        connect(m_rgbEdit[i], &QSpinBox::valueChanged, this, [=](){if(m_isEmitting) {emit valueChanged(getColor(), m_whichColor);}});
    }

    return container;
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
