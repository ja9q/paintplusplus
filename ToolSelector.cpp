#include "ToolSelector.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QList>
#include "BaseTool.h"

ToolSelector::ToolSelector(QList<BaseTool*> a_tools, int a_currentTool, QWidget *parent)
    : QListWidget{parent}
{

    connect(this, &ToolSelector::itemClicked, this, [=](){changeTool(currentRow());});

    // intialize for the current toolset
    generateTools(a_tools, a_currentTool);
}

void ToolSelector::generateTools(QList<BaseTool*> a_tools, int a_currentTool) {
    clearTools();

    //int toolIndex = 0;
    for (BaseTool* tool : a_tools) {
        addItem(tool->getName());
        // QPushButton *button = new QPushButton(tool->getName());
        // m_layout->addWidget(button);
        // button->setCheckable(true);
        // m_tools.append(button);

        // connect(button, &QPushButton::clicked, this, [=](){changeTool(toolIndex);});
        // toolIndex++;
    }

    setCurrentRow(a_currentTool);
    // m_tools[a_currentTool]->setChecked(true);
}

void ToolSelector::changeTool(const int a_newTool) {
    // for (QPushButton *button : m_tools) {
    //     button->setChecked(false);
    // }

    // m_tools[a_newTool]->setChecked(true);
    emit updateTool(a_newTool);
}

void ToolSelector::clearTools() {

    clear();
}
