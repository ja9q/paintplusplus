//
// The paint model keeps information about the available tools, the user
// and what is one the canvas
//

#ifndef PAINTMODEL_H
#define PAINTMODEL_H

#include "CanvasWidget.h"
#include "User.h"
#include "BaseTool.h"
#include <QList>
#include <QImage>

class PaintModel : public QObject
{
    Q_OBJECT
public:
    const static int TOOLCOUNT = 5;        // The number of selectable tools

    // the indices of the tool types in the tool arrays
    enum class ToolType {
        DRAWTOOL, ERASETOOL, SELECTTOOL, SHAPETOOL, FILLTOOL
    };

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

    // Getter for the list of the current tools
    QList<BaseTool*> getToolSet() const;

    // Getter for the current tool id
    int getCurrentToolInd() const;

    void setTool(int a_newTool);

    // set the toolType
    void setToolType(int a_typeId);

    // fill the canvas with the first color
    void fillCanvas();

    // fill the canvas with the second color
    void clearCanvas();

public slots:
    void openFile();

    // update one of the settings of the current tool
    void updateToolSetting(const int a_settingid, const int a_newValue);

    // undo within the canvas
    void undo();

    // redo within the canvas
    void redo();

    // add to the undo history
    void updateHistory(QImage a_canvas);

    // set the color
    void setColor(QColor a_color, int a_which);

private:
    const int UNDO_LIMIT = 20; // max length for history

    QList<BaseTool*> m_tools[TOOLCOUNT]; // The implemented tools, each vector is a different category

    int m_currentTool[TOOLCOUNT];   // The indices of the selected tools for each tool type
    ToolType m_currentToolType;          // The index that reflects the current tool type

    User m_user;                // The user's chosen tool and colors
    CanvasWidget m_canvas;      // The canvas widget that is to be displayed

    int m_historyPos;   // tracks the undos and redos of the history
    QList<QImage> m_history;    // the limited history of the canvas

    void initTools();   // initialize all the tools that the user can use
};

#endif // PAINTMODEL_H
