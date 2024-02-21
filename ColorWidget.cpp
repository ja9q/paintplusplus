#include "ColorWidget.h"

#include <QLineEdit>
#include <QIntValidator>
#include <QGridLayout>
#include <QGroupBox>
#include <QString>
#include <QLabel>

ColorWidget::ColorWidget(QWidget *parent)
    : QWidget{parent}
{
    setMinimumWidth(170);
    setMaximumWidth(200);



    QGridLayout *layout = new QGridLayout;


    QString colorLabels[3] = {"R:", "G:", "B:"};

    // Create and label the textfields for the RGB values
    for (int i = 0 ; i < 3; i++)
    {
        m_rgbEdit[i] = new QLineEdit("0");
        m_rgbEdit[i]->setValidator(new QIntValidator(0,255, this));
        layout->addWidget(new QLabel(colorLabels[i]), 0, 2*i, 1, 1);
        layout->addWidget(m_rgbEdit[i], 0, (2*i)+1, 1, 1);
        connect(m_rgbEdit[i], &QLineEdit::editingFinished, this, &ColorWidget::updateColor);
    }

    setLayout(layout);


}

void ColorWidget::updateColor() {
    emit valueChanged(getColor());
}



QColor ColorWidget::getColor() {
    return QColor(m_rgbEdit[0]->displayText().toInt(), m_rgbEdit[1]->displayText().toInt(), m_rgbEdit[2]->displayText().toInt());
}
