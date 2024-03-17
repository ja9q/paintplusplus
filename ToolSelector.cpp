#include "ToolSelector.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QList>
#include "BaseTool.h"

ToolSelector::ToolSelector(QList<BaseTool*> a_tools, int a_currentTool, QWidget *parent)
    : QWidget{parent}
{
    // set up the container and layout
    m_container = new QWidget(this);
    m_container->setMinimumWidth(225);
    m_container->resize(225,200);
    m_layout = new QVBoxLayout(m_container);

    // intialize for the current toolset
    generateTools(a_tools, a_currentTool);
}

void ToolSelector::generateTools(QList<BaseTool*> a_tools, int a_currentTool) {
    clearTools();

    int toolIndex = 0;
    for (BaseTool* tool : a_tools) {
        QPushButton *button = new QPushButton(tool->getName());
        m_layout->addWidget(button);
        button->setCheckable(true);
        m_tools.append(button);

        connect(button, &QPushButton::clicked, this, [=](){changeTool(toolIndex);});
        toolIndex++;
    }

    m_tools[a_currentTool]->setChecked(true);
}

void ToolSelector::changeTool(const int a_newTool) {
    for (QPushButton *button : m_tools) {
        button->setChecked(false);
    }

    m_tools[a_newTool]->setChecked(true);
    emit updateTool(a_newTool);
}

void ToolSelector::clearTools() {

    while(m_tools.length() > 0) {
        delete (m_tools.takeAt(0));
    }
}
