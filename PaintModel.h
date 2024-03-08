//
// The paint model keeps information about the available tools, the user
// and what is one the canvas
//

#ifndef PAINTMODEL_H
#define PAINTMODEL_H

#include "CanvasWidget.h"
#include "User.h"
#include "BaseTool.h"
#include <QVector>

class PaintModel : public QObject
{
    Q_OBJECT
public:
    const static int TOOLCOUNT = 3;        // The number of selectable tools

    const static int DRAWTOOL = 0;
    const static int ERASETOOL = 1;
    const static int SELECTTOOL = 2;

    // The constructor
    explicit PaintModel(QWidget *parent = nullptr);

    // The destructor
    ~PaintModel();

    // Getter for the canvas
    CanvasWidget* getCanvas();

    // Getter for the current tool
    BaseTool* getCurrentTool() const;

    // Getter for one of the colors
    QColor getColor(int a_which);

    // set the toolType
    void setToolType(int a_typeId);

    // set the color
    void setColor(QColor a_color, int a_which);

    // fill the canvas with the first color
    void fillCanvas();

    // fill the canvas with the second color
    void clearCanvas();

public slots:
    void updateToolSetting(const int a_settingid, const int a_newValue);

private:
    QVector<BaseTool*> m_tools[TOOLCOUNT]; // The implemented tools, each vector is a different category

    int m_currentTool[TOOLCOUNT];   // The indices of the selected tools for each tool type
    int m_currentToolType;          // The index that reflects the current tool type

    User m_user;                // The user's chosen tool and colors
    CanvasWidget m_canvas;      // The canvas

    void initTools();
};

#endif // PAINTMODEL_H
