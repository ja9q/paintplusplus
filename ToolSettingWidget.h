//
// GUI component that changes the settings of the current brush
//

#ifndef TOOLSETTINGWIDGET_H
#define TOOLSETTINGWIDGET_H

#include <QWidget>

class ToolSettingWidget : public QWidget
{
    Q_OBJECT
public:
    // Constructor
    explicit ToolSettingWidget(QWidget *parent = nullptr);

signals:
};

#endif // TOOLSETTINGWIDGET_H
