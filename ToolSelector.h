#ifndef TOOLSELECTOR_H
#define TOOLSELECTOR_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QVBoxLayout>

#include "BaseTool.h"

class ToolSelector : public QWidget
{
    Q_OBJECT
public:
    explicit ToolSelector(QList<BaseTool*> a_tools, int a_currentTool, QWidget *parent = nullptr);

public slots:
    void generateTools(QList<BaseTool*> a_tools, int a_currentTool);

signals:
    void updateTool(int a_newTool);

private:
    QWidget* m_container;
    QVBoxLayout* m_layout;
    QList<QPushButton*> m_tools;

    void changeTool(const int a_newTool);

    void clearTools();

};

#endif // TOOLSELECTOR_H
