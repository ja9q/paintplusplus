#ifndef TOOLSETTING_H
#define TOOLSETTING_H

#include <QString>
#include <QList>

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
    static const int FILLTYPE = 3; // Setting for fill-type
    static const int OUTLINE = 4; // Setting for if a shape is outlined or not

    // default constructor
    ToolSetting();

    // parametric constructor
    ToolSetting(QString a_name, int a_min, int a_max, DisplayType a_format, QList<QString> a_listVals = {});

    QString getName() const; // getter for setting name
    int getMinValue() const; // getter for setting min
    int getMaxValue() const; // getter for setting max
    DisplayType getDisplayType() const; // getter for how the setting should be displayed by the settings widget

    QList<QString> getListValues() const; //get for the list values (used in dropdown only)


private:

    QString m_name; // the name of the setting
    int m_minValue; // the minimum value of the setting
    int m_maxValue; // the maximum value of the setting
    DisplayType m_format;  // how the setting should be presented (e.g. checkbox, slider)

    QList<QString> m_listVals; // for a dropdown, the values to display in the menu
};

#endif // TOOLSETTING_H
