#include "ToolSetting.h"

ToolSetting::ToolSetting() {}


// parametric constructor
ToolSetting::ToolSetting(QString a_name, int a_min, int a_max, DisplayType a_format) :
    m_name(a_name), m_minValue(a_min), m_maxValue(a_max), m_format(a_format) {}

QString ToolSetting::getName() const {
    return m_name;
}
int ToolSetting::getMinValue() const {
    return m_minValue;
}
int ToolSetting::getMaxValue() const {
    return m_maxValue;
}
ToolSetting::DisplayType ToolSetting::getDisplayType() const {
    return m_format;
}
