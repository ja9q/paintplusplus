#include "User.h"

#include "BaseTool.h"
#include "DrawTool.h"
#include <QColor>


User::User() {
    m_colors[0] = qRgb(0,0,0);
    m_colors[1] = qRgb(255,255,255);

    m_currentTool = new DrawTool();
}

User::User(BaseTool* a_tool) {
    m_colors[0] = qRgb(0,0,0);
    m_colors[1] = qRgb(255,255,255);

    m_currentTool = a_tool;
}

const QColor User::getColor(const int a_which) const {
    return m_colors[a_which];
}


BaseTool* User::getCurrentTool() const {
    return m_currentTool;
}

void User::setCurrentTool(BaseTool* a_newTool) {
    m_currentTool = a_newTool;
}

void User::setColor(QColor a_color, int a_which) {
    m_colors[a_which] = a_color;
}
