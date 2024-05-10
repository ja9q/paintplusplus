//
// Implementation for the ToolSelector class
//

#include "ToolSelector.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QList>
#include "./../toolClasses/BaseTool.h"

/**/
/*
ToolSelector::ToolSelector(QList<BaseTool*> a_tools, int a_currentTool, QWidget *parent)

NAME

    ToolSelector::ToolSelector(a_tools, int a_currentTool, QWidget *parent) - constructor

SYNOPSIS

    ToolSelector::ToolSelector(QList<BaseTool*> a_tools, int a_currentTool, QWidget *parent);
        a_tools --> the tools to display
        a_currentTool --> the index of the current tool
        parent --> the parent widget

DESCRIPTION

    parametric constructor

RETURNS

    The constructed ToolSelector

*/
/**/
ToolSelector::ToolSelector(QList<BaseTool*> a_tools, int a_currentTool, QWidget *parent)
    : QListWidget{parent}
{
    // whenever a listed tool is clicked, update the tool
    connect(this, &ToolSelector::itemClicked, this, [=](){emit updateTool(currentRow());});

    // intialize for the current toolset
    generateTools(a_tools, a_currentTool);
}

/**/
/*
void ToolSelector::generateTools(QList<BaseTool*> a_tools, int a_currentTool)

NAME

    ToolSelector::generateTools(QList<BaseTool*> a_tools, int a_currentTool) - load the options as available tools

SYNOPSIS

    void ToolSelector::generateTools(QList<BaseTool*> a_tools, int a_currentTool);
        a_tools --> the list of tools to display
        a_currentTool --> the current tool to show as selected

DESCRIPTION

    displays the possible tools to use of a certain tool type within a given list

RETURNS

    None

*/
/**/
void ToolSelector::generateTools(QList<BaseTool*> a_tools, int a_currentTool) {
    clear();

    // add an entry for each tool
    for (BaseTool* tool : a_tools) {
        addItem(tool->getName());
    }

    setCurrentRow(a_currentTool);
}
