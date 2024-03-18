//
// GUI component that changes the settings of the current brush
//

#ifndef TOOLSETTINGWIDGET_H
#define TOOLSETTINGWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "ToolSetting.h"
#include "BaseTool.h"

class ToolSettingWidget : public QWidget
{
    Q_OBJECT
public:
    // Constructor
    explicit ToolSettingWidget(BaseTool* a_tool, QWidget *parent = nullptr);

public slots:
    // Use a list sent from a tool to generate settings
    void generateSettings(BaseTool* a_tool);

signals:
    // alert other components when a setting has been updated
    updateSetting(const int a_setting, int a_newValue);

private:
    QVector<ToolSetting> m_settings; // information about each setting the widget can display

    QGridLayout* m_layout;  // the widget's layout
    QWidget* m_container;   // the widget's container

    // initialize the setting data
    void initSettingData();

    // form a layout for a single setting
    void formSetting(int a_settingid, int a_hposition, int a_initValue);

    // functions to create a certain GUI component for a setting
    void createSlider(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);
    void createCheckbox(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);
    void createDropdown(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);

    // remove all settings
    void clearSettings();

};

#endif // TOOLSETTINGWIDGET_H
