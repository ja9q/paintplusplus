//
// GUI component that changes the settings of the current brush
//

#ifndef TOOLSETTINGWIDGET_H
#define TOOLSETTINGWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "ToolSetting.h"

class ToolSettingWidget : public QWidget
{
    Q_OBJECT
public:
    // Constructor
    explicit ToolSettingWidget(QWidget *parent = nullptr);

public slots:
    // Use a list sent from a tool to generate settings
    void generateSettings(QVector<int> a_properties);

signals:
    updateSetting(const int a_setting, int a_newValue);

private:
    QVector<ToolSetting> m_settings;

    QGridLayout* m_layout;

    void initSettingData();

    void formSetting(int a_settingid, int a_hposition);

    void clearSettings();
};

#endif // TOOLSETTINGWIDGET_H
