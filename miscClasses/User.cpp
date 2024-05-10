//
// Implementation for the User class
//

#include "User.h"

#include "./../toolClasses/BaseTool.h"
#include "./../toolClasses/DrawTool.h"
#include <QColor>

/**/
/*
User::User(BaseTool* a_tool)

NAME

    User(BaseTool* a_tool) - parametric constructor

SYNOPSIS

    User::User(BaseTool* a_tool);
        a_tool --> the starting tool the user will start with

DESCRIPTION

    parametric constructor for the user object

RETURNS

    The constructed user

*/
/**/
User::User(BaseTool* a_tool) {
    m_colors[0] = QColor(Qt::black);
    m_colors[1] = QColor(Qt::white);

    m_currentTool = a_tool;
}

/**/
/*
const QColor User::getColor(const int a_which) const

NAME

    User::getColor(const int a_which) - get one of the colors

SYNOPSIS

    User::getColor(const int a_which);
        a_which - either 0 or 1, which reflects to get either color 1 or 2


DESCRIPTION

    Getter for one of the colors

RETURNS

    Either color 1 or color 2

*/
/**/
const QColor User::getColor(const int a_which) const {
    return m_colors[a_which];
}

/**/
/*
BaseTool* User::getCurrentTool() const

NAME

    User::getCurrentTool() - get the current tool

SYNOPSIS

    getCurrentTool();


DESCRIPTION

    Getter for the current tool

RETURNS

    A pointer to the current tool

*/
/**/
BaseTool* User::getCurrentTool() const {
    return m_currentTool;
}

/**/
/*
void User::setCurrentTool(BaseTool* a_newTool)

NAME

    User::setCurrentTool(BaseTool* a_newTool) - set the current tool

SYNOPSIS

    User::setCurrentTool(BaseTool* a_newTool);
        a_newTool --> the new tool the user will use


DESCRIPTION

    Setter for the current tool

RETURNS

    None

*/
/**/
void User::setCurrentTool(BaseTool* a_newTool) {
    m_currentTool = a_newTool;
}

/**/
/*
void User::setColor(QColor a_color, int a_which)

NAME

    User::setColor(QColor a_color, int a_which) - set one of the colors

SYNOPSIS

    User::setColor(QColor a_color, int a_which);
        a_color --> the new tool the user will use
        a_which --> either 0 or 1, which reflects to set either color 1 or 2


DESCRIPTION

    Setter for one of the colors

RETURNS

    None

*/
/**/
void User::setColor(QColor a_color, int a_which) {
    m_colors[a_which] = a_color;
}
