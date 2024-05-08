//
// A widget that lets the user choose a tool within a tool type
//

#ifndef TOOLSELECTOR_H
#define TOOLSELECTOR_H

#include <QListWidget>
#include <QList>
#include <QPushButton>
#include <QVBoxLayout>

#include "BaseTool.h"

class ToolSelector : public QListWidget
{
    Q_OBJECT
public:
    // constructor
    explicit ToolSelector(QList<BaseTool*> a_tools, int a_currentTool, QWidget *parent = nullptr);

public slots:
    // when the tool type has switched, update the list of available tools
    void generateTools(QList<BaseTool*> a_tools, int a_currentTool);

signals:
    // alert other components that the current tool has changed
    void updateTool(int a_newTool);

private:
    QWidget* m_container;   // container for the all the components
    QVBoxLayout* m_layout;  // layout for the components
    QList<QPushButton*> m_tools;    // the generated components

};

#endif // TOOLSELECTOR_H
