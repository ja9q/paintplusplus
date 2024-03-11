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
    updateSetting(const int a_setting, int a_newValue);

private:
    QVector<ToolSetting> m_settings;

    QGridLayout* m_layout;
    QWidget* m_container;

    void initSettingData();

    void formSetting(int a_settingid, int a_hposition, int a_initValue);

    void createSlider(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);

    void createCheckbox(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);

    void createDropdown(int a_settingid, ToolSetting a_setting, int a_hposition, int a_initValue);

    void clearSettings();

};

#endif // TOOLSETTINGWIDGET_H
