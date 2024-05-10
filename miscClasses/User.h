//
// User stores how the user will interact with the canvas
//

#ifndef USER_H
#define USER_H

#include "./../toolClasses/BaseTool.h"
#include <QColor>

class User
{
public:

    // parametric constructor
    User(BaseTool* a_tool);

    // getter for one of the colors
    const QColor getColor(const int a_which) const;

    // getter for the current tool
    BaseTool* getCurrentTool() const;

    // setter for the current tool
    void setCurrentTool(BaseTool* a_newTool);

    // setter for one of the colors
    void setColor(QColor a_color, int a_which);

private:
    BaseTool* m_currentTool;    // Pointer to the user's current tool; Stored in the PaintModel
    QColor m_colors[2];         // The user's chosen colors
};

#endif // USER_H
