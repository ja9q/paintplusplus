#ifndef USER_H
#define USER_H

#include "BaseTool.h"
#include <QColor>

class User
{
public:
    User();
    User(BaseTool* a_tool);

    const QColor getColor(const int a_which) const;
    BaseTool* getCurrentTool() const;

    void setCurrentTool(BaseTool* a_newTool);

    void setColor(QColor a_color, int a_which);

private:
    BaseTool* m_currentTool;
    QColor m_colors[2];
};

#endif // USER_H
