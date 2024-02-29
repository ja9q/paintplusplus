//
// Implementation for the ToolSettingWidget class
//

#include "ToolSettingWidget.h"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>

ToolSettingWidget::ToolSettingWidget(QWidget *parent)
    : QWidget{parent}
{
    initSettingData();

    QWidget* container = new QWidget(this);

    m_layout = new QGridLayout();
    m_layout->setVerticalSpacing(5);

    generateSettings({ToolSetting::SIZE, ToolSetting::OPACITY, ToolSetting::ANTIALIAS});

    container->setLayout(m_layout);
}

void ToolSettingWidget::generateSettings(QVector<int> a_properties)
{
    clearSettings();

    int row = 0;

    for (int setting : a_properties) {
        formSetting(setting, row);
        row++;
    }
}

void ToolSettingWidget::initSettingData() {
    m_settings.append(ToolSetting("Brush Size", 0, 200, false)); // [0] = ToolSetting::SIZE
    m_settings.append(ToolSetting("Opacity", 0, 100, false)); // [1] = ToolSetting::OPACITY
    m_settings.append(ToolSetting("Antialiasing", 0, 1, true)); // [1] = ToolSetting::ANTIALIAS
}

void ToolSettingWidget::formSetting(int a_settingid, int a_hposition) {
    ToolSetting setting = m_settings[a_settingid];

    // label the name of the item
    m_layout->addWidget(new QLabel(setting.getName()), 2*a_hposition, 1, 1, 1, Qt::AlignBottom);

    // if this setting is not a toggle, then create a slider and text field
    if (!setting.getIsToggle()) {
        // set up the slider and text field
        QSlider* slider = new QSlider(Qt::Horizontal);
        slider->setRange(setting.getMinValue(), setting.getMaxValue());
        QSpinBox* textField = new QSpinBox();
        textField->setMinimum(setting.getMinValue());
        textField->setMaximum(setting.getMaxValue());

        // sync the text field with the slider and vice versa
        connect(slider, &QSlider::valueChanged, textField, &QSpinBox::setValue);
        connect(textField, &QSpinBox::valueChanged, slider, &QSlider::setValue);

        // connect the setting to the rest of the program
        connect(slider, &QSlider::valueChanged, this, [=](){emit updateSetting(a_settingid, slider->value());});

        // add them to the widget
        m_layout->addWidget(slider, (2*a_hposition)+1, 1, 1, 3, Qt::AlignTop);
        m_layout->addWidget(textField, (2*a_hposition)+1, 4, 1, 1, Qt::AlignTop);

    } else {
        // otherwise, add a checkbox
        QCheckBox* checkbox = new QCheckBox();
        connect(checkbox, &QCheckBox::clicked, this, [=](){emit updateSetting(a_settingid, checkbox->isChecked());});
        m_layout->addWidget(checkbox, (2*a_hposition), 4, 1,1, Qt::AlignBottom);
    }


}

void ToolSettingWidget::clearSettings() {
    QList< QWidget* > children;

    // find child widgets and delete them until there are no more children
    do
    {
        children = m_layout->findChildren<QWidget*>();
        if (children.count() == 0)
            break;
        delete children.at(0);
    }
    while ( true );

}
