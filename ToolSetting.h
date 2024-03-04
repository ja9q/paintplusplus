#ifndef TOOLSETTING_H
#define TOOLSETTING_H

#include <QString>

class ToolSetting
{
public:

    enum class DisplayType {
        SLIDER,
        TOGGLE,
        DROPDOWN
    };

    // Flags that mark certain settings
    static const int SIZE = 0;      // Setting for size
    static const int OPACITY = 1;   // Setting for opacity
    static const int ANTIALIAS = 2; // Setting for antialiasing

    // default constructor
    ToolSetting();

    // parametric constructor
    ToolSetting(QString a_name, int a_min, int a_max, DisplayType a_format);

    QString getName() const; // getter for setting name
    int getMinValue() const; // getter for setting min
    int getMaxValue() const; // getter for setting max
    DisplayType getDisplayType() const; // getter for if the setting is a toggle

private:

    QString m_name; // the name of the setting
    int m_minValue; // the minimum value of the setting
    int m_maxValue; // the maximum value of the setting
    DisplayType m_format;  // how the setting should be presented (e.g. checkbox, slider)
};

#endif // TOOLSETTING_H
