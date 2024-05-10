//
// Implementation for the ToolSettingWidget class
//

#include "ToolSettingWidget.h"

#include <QWidget>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QComboBox>
#include "./../toolClasses/BaseTool.h"

/**/
/*
ToolSettingWidget::ToolSettingWidget(BaseTool* a_tool, QWidget *parent)

NAME

    ToolSettingWidget::ToolSettingWidget(a_tool, QWidget *parent) - constructor

SYNOPSIS

    ToolSettingWidget::ToolSettingWidget(BaseTool* a_tool, QWidget *parent);
        a_tool --> the tool to first make settings for
        parent --> the parent widget

DESCRIPTION

    parametric constructor

RETURNS

    the constructed tool setting widget

*/
/**/
ToolSettingWidget::ToolSettingWidget(BaseTool* a_tool, QWidget *parent)
    : QWidget{parent}
{
    initSettingData();
    resize(225,250);

    m_container = new QWidget();

    // first set up the layout that contains everything
    setContentsMargins(0,0,0,0);
    QVBoxLayout* area = new QVBoxLayout(this);
    area->setContentsMargins(0,0,0,0);

    // set up the layout the contains the settings
    m_layout = new QGridLayout();
    m_layout->setContentsMargins(5,5,25,5);

    // set up container that contains the settings layout + the spacer
    m_spacer = new QVBoxLayout(m_container);
    m_spacer->addLayout(m_layout);

    // initialize the settings for the provided tool
    generateSettings(a_tool);

    // create a scroll area for the widget
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(false);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(m_container);
    area->addWidget(scrollArea);

}

/**/
/*
void ToolSettingWidget::generateSettings(BaseTool* a_tool)

NAME

    ToolSettingWidget::generateSettings(BaseTool* a_tool) - render new set of settings for the tool

SYNOPSIS

    void ToolSettingWidget::generateSettings(BaseTool* a_tool);
        a_tool --> the tool to make settings for

DESCRIPTION

    render the settings for a given tool

RETURNS

    None

*/
/**/
void ToolSettingWidget::generateSettings(BaseTool* a_tool)
{
    clearSettings();
    m_spacer->removeItem(m_spacer->itemAt(1));

    int row = 0;
    int currentValue = 0;

    // for each setting, add it to a row of the settings widget
    for (int setting : a_tool->getProperties()) {
        currentValue = a_tool->getProperty(setting);

        formSetting(setting, row, currentValue);
        row++;
    }

    m_container->setMinimumHeight(row*60);

    if (m_container->minimumHeight() < height()) {
        m_spacer->removeItem(m_spacer->itemAt(1));
        m_spacer->addSpacing(height() - m_container->minimumHeight() - 10);
    }
}

/**/
/*
void ToolSettingWidget::resizeEvent(QResizeEvent *event)

NAME

    ToolSettingWidget::resizeEvent(QResizeEvent *event) - react to a resize event

SYNOPSIS

    void ToolSettingWidget::resizeEvent(QResizeEvent *event);

DESCRIPTION

    react to when the widget is resized; this is overridden to keep the spacing between
    settings consistent.

RETURNS

    None

*/
/**/
void ToolSettingWidget::resizeEvent(QResizeEvent *event) {
    m_container->resize(event->size().width(), event->size().height()-5);

    // if the height of the setting changed, then update the spacer to keep the spacing between
    if (m_container->minimumHeight() < height()) {
        m_spacer->removeItem(m_spacer->itemAt(1));
        m_spacer->addSpacing(height() - m_container->minimumHeight() - 10);
    }
}

/**/
/*
void ToolSettingWidget::initSettingData()

NAME

    ToolSettingWidget::initSettingData() - initialize settings data

SYNOPSIS

    void ToolSettingWidget::initSettingData();

DESCRIPTION

    initialize the parameters for the settings that the widget might need to load

RETURNS

    None

*/
/**/
void ToolSettingWidget::initSettingData() {
    m_settings.append(ToolSetting("Brush Size", 1, 200, ToolSetting::DisplayType::SLIDER)); // [0] = ToolSetting::SIZE
    m_settings.append(ToolSetting("Opacity", 0, 100, ToolSetting::DisplayType::SLIDER)); // [1] = ToolSetting::OPACITY
    m_settings.append(ToolSetting("Antialiasing", 0, 1, ToolSetting::DisplayType::TOGGLE)); // [2] = ToolSetting::ANTIALIAS
    m_settings.append(ToolSetting("Fill Type", 0, 2, ToolSetting::DisplayType::DROPDOWN, {"No Fill", "Fill with Color 1", "Fill with Color 2"}));
        // [3] = ToolSetting::FILLTYPE
    m_settings.append(ToolSetting("Has Outline", 0, 1, ToolSetting::DisplayType::TOGGLE)); // [4] = ToolSetting::OUTLINE
    m_settings.append(ToolSetting("Transparent Background", 0, 1, ToolSetting::DisplayType::TOGGLE)); // [5] = ToolSetting::MASKBACK
    m_settings.append(ToolSetting("Mask to Color 2", 0, 1, ToolSetting::DisplayType::TOGGLE)); // [6] = ToolSetting::MASKCOLOR2
    //[7]-[11] = FONT_SIZE, FONT, TEXT_BOLD, TEXT_ITALIC, TEXT_UNDERLINE
    m_settings.append(ToolSetting("Font Size", 0, 100, ToolSetting::DisplayType::SLIDER));
    QList<QString> fontList = QFontDatabase::families().toList();
    m_settings.append(ToolSetting("Font", 0, fontList.length(), ToolSetting::DisplayType::DROPDOWN, fontList));
    m_settings.append(ToolSetting("Bold", 0, 1, ToolSetting::DisplayType::TOGGLE));
    m_settings.append(ToolSetting("Italics", 0, 1, ToolSetting::DisplayType::TOGGLE));
    m_settings.append(ToolSetting("Underline", 0, 1, ToolSetting::DisplayType::TOGGLE));
}

/**/
/*
void ToolSettingWidget::formSetting(int a_settingid, int a_hposition, int a_initValue)

NAME

    ToolSettingWidget::formSetting(int a_settingid, int a_hposition, int a_initValue) - render and
        add a setting to a widget

SYNOPSIS

    void ToolSettingWidget::formSetting(int a_settingid, int a_hposition, int a_initValue);
        a_settingid --> the id of the setting
        a_hposition --> the row the setting is supposed to be in (relative to other widgets)
        a_initValue --> the initial value to display for the setting

DESCRIPTION

    render and add the setting to the tool setting widget

RETURNS

    None

*/
/**/
void ToolSettingWidget::formSetting(int a_settingid, int a_hposition, int a_initValue) {
    ToolSetting setting = m_settings[a_settingid];

    // label the name of the item
    m_layout->addWidget(new QLabel(setting.m_name), 2*a_hposition, 1, 1, 1, Qt::AlignBottom);

    // produce a different type of control depending on the display type
    switch (setting.m_format) {
    case ToolSetting::DisplayType::SLIDER:
    {
        createSlider(a_settingid, setting, a_hposition, a_initValue);
        break;
    }
    case ToolSetting::DisplayType::TOGGLE:
    {
        createCheckbox(a_settingid, setting, a_hposition, a_initValue);
        break;
    }
    case ToolSetting::DisplayType::DROPDOWN:
    {
        createDropdown(a_settingid, setting, a_hposition, a_initValue);
    }
    default:
    {
        break;
    }

    }

}

/**/
/*
void ToolSettingWidget::createSlider(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue)

NAME

    ToolSettingWidget::createSlider(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue) - render a slider

SYNOPSIS

    void ToolSettingWidget::createSlider(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);
        a_settingid --> the setting id
        a_setting --> the setting itself
        a_hposition --> the position of the setting in the widget relative to other settings
        a_initValue --> the setting's initial value

DESCRIPTION

    renders a slider display for a setting

RETURNS

    None

*/
/**/
void ToolSettingWidget::createSlider(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue) {
    // set up the slider and text field
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setRange(a_setting.m_minValue, a_setting.m_maxValue);
    QSpinBox* textField = new QSpinBox();
    textField->setMinimum(a_setting.m_minValue);
    textField->setMaximum(a_setting.m_maxValue);

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
}

/**/
/*
void ToolSettingWidget::createCheckbox(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue)

NAME

    ToolSettingWidget::createCheckbox(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue) - render a checkbox

SYNOPSIS

    void ToolSettingWidget::createCheckbox(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);
        a_settingid --> the setting id
        a_setting --> the setting itself
        a_hposition --> the position of the setting in the widget relative to other settings
        a_initValue --> the setting's initial value

DESCRIPTION

    renders a checkbox display for a setting

RETURNS

    None

*/
/**/
void ToolSettingWidget::createCheckbox(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue) {
    (void)a_setting;

    // otherwise, add a checkbox
    QCheckBox* checkbox = new QCheckBox();

    checkbox->setChecked(a_initValue == 1);

    // connect the checkbox and add it to the widget
    connect(checkbox, &QCheckBox::clicked, this, [=](){emit updateSetting(a_settingid, checkbox->isChecked());});

    m_layout->addWidget(checkbox, (2*a_hposition), 4, 1,1, Qt::AlignBottom);
}

/**/
/*
void ToolSettingWidget::createDropdown(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue)

NAME

    ToolSettingWidget::createDropdown(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue) - render a dropdown

SYNOPSIS

    void ToolSettingWidget::createDropdown(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);
        a_settingid --> the setting id
        a_setting --> the setting itself
        a_hposition --> the position of the setting in the widget relative to other settings
        a_initValue --> the setting's initial value

DESCRIPTION

    renders a dropdown display for a setting

RETURNS

    None

*/
/**/
void ToolSettingWidget::createDropdown(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue) {

    QComboBox* dropdown = new QComboBox();

    // initialize the dropdown menu items and the initial setting
    for (QString item : a_setting.m_listVals) {
        dropdown->addItem(item);
    }
    dropdown->setCurrentIndex(a_initValue);

    // connect the dropdown and add it to the widget
    connect(dropdown, &QComboBox::currentIndexChanged, this, [=](){emit updateSetting(a_settingid, dropdown->currentIndex());});

    m_layout->addWidget(dropdown, (2*a_hposition)+1, 1, 1, 4, Qt::AlignTop);
}

/**/
/*
void ToolSettingWidget::clearSettings()

NAME

    ToolSettingWidget::clearSettings() - remove all settings in the widget

SYNOPSIS

    void ToolSettingWidget::clearSettings();

DESCRIPTION

    removes all settings in the setting layout; needed to render a new list of settings

RETURNS

    None

*/
/**/
void ToolSettingWidget::clearSettings() {
    QLayoutItem* cell;

    // remove every widget in the layout
    while (m_layout->count() > 0) {
        cell = m_layout->itemAt(0);
        delete cell->widget();
    }
}
