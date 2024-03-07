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
#include "BaseTool.h"

ToolSettingWidget::ToolSettingWidget(BaseTool* a_tool, QWidget *parent)
    : QWidget{parent}
{
    initSettingData();

    setMinimumWidth(225);
    setMaximumWidth(225);

    QWidget* container = new QWidget(this);


    m_layout = new QGridLayout();
    m_layout->setVerticalSpacing(5);

    generateSettings(a_tool);

    container->resize(200, m_layout->rowCount()*30);

    container->setLayout(m_layout);
}

void ToolSettingWidget::generateSettings(BaseTool* a_tool)
{
    clearSettings();

    int row = 0;
    int currentValue = 0;

    for (int setting : a_tool->getProperties()) {
        currentValue = a_tool->getProperty(setting);

        formSetting(setting, row, currentValue);
        row++;
    }
}

void ToolSettingWidget::initSettingData() {
    m_settings.append(ToolSetting("Brush Size", 1, 200, ToolSetting::DisplayType::SLIDER)); // [0] = ToolSetting::SIZE
    m_settings.append(ToolSetting("Opacity", 0, 100, ToolSetting::DisplayType::SLIDER)); // [1] = ToolSetting::OPACITY
    m_settings.append(ToolSetting("Antialiasing", 0, 1, ToolSetting::DisplayType::TOGGLE)); // [2] = ToolSetting::ANTIALIAS
}

void ToolSettingWidget::formSetting(int a_settingid, int a_hposition, int a_initValue) {
    ToolSetting setting = m_settings[a_settingid];

    // label the name of the item
    m_layout->addWidget(new QLabel(setting.getName()), 2*a_hposition, 1, 1, 1, Qt::AlignBottom);

    // produce a different type of control depending on the display type
    switch (setting.getDisplayType()) {
    case ToolSetting::DisplayType::SLIDER:
    {
        // set up the slider and text field
        QSlider* slider = new QSlider(Qt::Horizontal);
        slider->setRange(setting.getMinValue(), setting.getMaxValue());
        QSpinBox* textField = new QSpinBox();
        textField->setMinimum(setting.getMinValue());
        textField->setMaximum(setting.getMaxValue());

        // sync the text field with the slider and vice versa
        connect(slider, &QSlider::valueChanged, textField, &QSpinBox::setValue);
        connect(textField, &QSpinBox::valueChanged, slider, &QSlider::setValue);

        // set the current value for the slider (the textfield will automatically adjust)
        slider->setValue(a_initValue);

        // connect the setting to the rest of the program
        connect(slider, &QSlider::valueChanged, this, [=](){emit updateSetting(a_settingid, slider->value());});

        // add them to the widget
        m_layout->addWidget(slider, (2*a_hposition)+1, 1, 1, 3, Qt::AlignTop);
        m_layout->addWidget(textField, (2*a_hposition)+1, 4, 1, 1, Qt::AlignTop);
        break;
    }
    case ToolSetting::DisplayType::TOGGLE:
    {
        // otherwise, add a checkbox
        QCheckBox* checkbox = new QCheckBox();

        checkbox->setChecked(a_initValue == 1);

        // connect the checkbox and add it to the widget
        connect(checkbox, &QCheckBox::clicked, this, [=](){emit updateSetting(a_settingid, checkbox->isChecked());});

        m_layout->addWidget(checkbox, (2*a_hposition), 4, 1,1, Qt::AlignBottom);
        break;
    }
    default:
    {
        break;
    }

    }

}

void ToolSettingWidget::clearSettings() {
    QLayoutItem* cell;

    // remove every widget in the layout
    while (m_layout->count() > 0) {
        cell = m_layout->itemAt(0);
        delete cell->widget();
    }
}
