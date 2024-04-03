//
// Implementation for the PaintModel class
//

#include "PaintModel.h"

#include "DrawTool.h"
#include "SquareTool.h"
#include "CircleTool.h"
#include "PolygonTool.h"
#include "LineTool.h"
#include "FillTool.h"
#include "User.h"

/**/
/*
PaintModel::PaintModel(QWidget *parent)

NAME

    PaintModel::PaintModel(QWidget *parent) - Construct the paint model

SYNOPSIS

    PaintModel::PaintModel(QWidget *parent)
        parent - the parent for the canvas widget

DESCRIPTION

    Construct the paint model

RETURNS

    The constructed paint model

*/
/**/
PaintModel::PaintModel(QWidget *parent) :
    QObject{parent},
    m_currentTool{0},
    m_currentToolType(ToolType::DRAWTOOL),
    m_user(nullptr),
    m_canvas(CanvasWidget(&m_user, parent)),
    m_historyPos(0)
{
    initTools();
    m_user.setCurrentTool(m_tools[(int)ToolType::DRAWTOOL][0]);

    connect(&m_canvas, &CanvasWidget::canvasChanged, this, &PaintModel::updateHistory);
    m_canvas.fillCanvas(QColor(Qt::white));
}

// Destructor
PaintModel::~PaintModel()
{
}

/**/
/*
CanvasWidget* PaintModel::getCanvas()

NAME

    PaintModel::getCanvas() - getter for the canvas

SYNOPSIS

    PaintModel::getCanvas()

DESCRIPTION

    Getter for the canvas

RETURNS

    The internal canvas

*/
/**/
CanvasWidget* PaintModel::getCanvas()
{
    return &m_canvas;
}

/**/
/*
BaseTool* PaintModel::getCurrentTool() const

NAME

    PaintModel::getCurrentTool() - getter for the current tool

SYNOPSIS

    PaintModel::getCurrentTool()

DESCRIPTION

    Getter for the current tool

RETURNS

    The current tool

*/
/**/
BaseTool* PaintModel::getCurrentTool() const
{
    return m_user.getCurrentTool();
}

/**/
/*
QList<BaseTool*> PaintModel::getToolSet() const
NAME

    PaintModel::getToolSet() const - getter for the current tools

SYNOPSIS

    QList<BaseTool*> PaintModel::getToolSet() const;

DESCRIPTION

    getter for the list of current subtools

RETURNS

    The list of the current tools related to the current tool type

*/
/**/
QList<BaseTool*> PaintModel::getToolSet() const {
    return m_tools[(int)m_currentToolType];
}

/**/
/*
int PaintModel::getCurrentToolInd() const
NAME

    PaintModel::getCurrentToolInd() const - getter for the index of the current tool

SYNOPSIS

    int PaintModel::getCurrentToolInd() const;

DESCRIPTION

    getter for the index of the current tool; this is for initializing the tool picker

RETURNS

    The index that reflects the current tool in the current tool list

*/
/**/
int PaintModel::getCurrentToolInd() const {
    return m_currentTool[(int)m_currentToolType];
}

/**/
/*
void PaintModel::setTool(int a_newTool)
NAME

    PaintModel::setTool(int a_newTool) -

SYNOPSIS

    void PaintModel::setTool(int a_newTool);
        a_newTool -->

DESCRIPTION

    description

RETURNS

    NONE

*/
/**/
void PaintModel::setTool(int a_newTool) {
    m_user.getCurrentTool()->resetEditor();
    m_canvas.flushTemp();
    m_currentTool[(int)m_currentToolType] = a_newTool;
    m_user.setCurrentTool(m_tools[(int)m_currentToolType][m_currentTool[(int)m_currentToolType]]);
}


/**/
/*
void PaintModel::setToolType(int a_typeId)
NAME

    PaintModel::setToolType(int a_typeId) - setter for the tool type

SYNOPSIS

    void PaintModel::setToolType(int a_typeId);
        a_typeId --> The id of the new tool type

DESCRIPTION

    Setter for the tool type; it also updates the current tool to be the current tool

RETURNS

    None

*/
/**/
void PaintModel::setToolType(int a_typeId) {
    m_user.getCurrentTool()->resetEditor();
    m_canvas.flushTemp();
    m_currentToolType = (ToolType)a_typeId;
    m_user.setCurrentTool(m_tools[a_typeId][m_currentTool[a_typeId]]);
}

/**/
/*
QColor PaintModel::getColor(int a_which)
NAME

    PaintModel::getColor(int a_which) - getter for one of the user's colors

SYNOPSIS

    QColor PaintModel::getColor(int a_which);
        a_which --> whether to get color 1 or color 2

DESCRIPTION

    Retrieves one of the color

RETURNS

    The selected color

*/
/**/
QColor PaintModel::getColor(int a_which) {
    return m_user.getColor(a_which);
}

/**/
/*
void PaintModel::setColor(QColor a_color, int a_which)
NAME

    PaintModel::setColor(QColor a_color, int a_which) - setter for one of the user's colors

SYNOPSIS

    void PaintModel::setColor(QColor a_color, int a_which);
        a_color --> the new value for the color
        a_which --> specifies which color to change

DESCRIPTION

    Setter for the user's colors; one color can be changed at the time

RETURNS

    None

*/
/**/
void PaintModel::setColor(QColor a_color, int a_which) {
    m_user.setColor(a_color, a_which);
}

/**/
/*
void PaintModel::fillCanvas()
NAME

    PaintModel::fillCanvas() - fills the canvas with color 1

SYNOPSIS

    void PaintModel::fillCanvas();

DESCRIPTION

    Fills the canvas with color 1

RETURNS

    None

*/
/**/
void PaintModel::fillCanvas() {
    m_canvas.fillCanvas(m_user.getColor(0));
}

/**/
/*
void PaintModel::clearCanvas()

NAME

   PaintModel::clearCanvas() - clears the canvas

SYNOPSIS

   void PaintModel::clearCanvas();

DESCRIPTION

    Fills the canvas with the second color (if layers/transparency is implemented this might be revised to be clearing the canvas entirely)

RETURNS

    None

*/
/**/
void PaintModel::clearCanvas() {
    m_canvas.fillCanvas(m_user.getColor(1));
}

/**/
/*
void PaintModel::updateToolSetting(const int a_settingid, const int a_newValue)

NAME

   PaintModel::updateToolSetting(const int a_settingid, const int a_newValue) - update one of the current tool's settings

SYNOPSIS

    void PaintModel::updateToolSetting(const int a_settingid, const int a_newValue);
        a_settingid --> the id of the setting to be changed
        a_newValue --> the new value of the setting

DESCRIPTION

    Changes one of the settings in the current tool; this is called by the tool setting widget

RETURNS

    None

*/
/**/
void PaintModel::updateToolSetting(const int a_settingid, const int a_newValue) {
    BaseTool* currentTool = m_user.getCurrentTool();
    currentTool->setProperty(a_settingid, a_newValue);
}


/**/
/*
void PaintModel::undo()

NAME

    PaintModel::undo() - undos a change in the canvas

SYNOPSIS

    void PaintModel::undo();

DESCRIPTION

    uses the undo history to revert a change in the canvas

RETURNS

    None

*/
/**/
void PaintModel::undo() {

    m_user.getCurrentTool()->resetEditor();

    // replace the canvas with the next most recent part of the history and remove it
    if (m_historyPos < UNDO_LIMIT-1) {
        // update the history position;
        m_historyPos++;
        // set the canvas to the current part of history
        m_canvas.setCanvas(m_history[m_historyPos]);
    }
}

/**/
/*
void PaintModel::redo()

NAME

    PaintModel::redo() - redos a change in the canvas

SYNOPSIS

    void PaintModel::redo();

DESCRIPTION

    uses the history to revert a change in the canvas (if possible)

RETURNS

    None

*/
/**/
void PaintModel::redo() {

    m_user.getCurrentTool()->resetEditor();

    // replace the canvas with the next most recent part of the history and remove it
    if (m_historyPos > 0) {
        // update the history position;
        m_historyPos--;
        // set the canvas to the newer part of history
        m_canvas.setCanvas(m_history[m_historyPos]);
    }
}


/**/
/*
void PaintModel::updateHistory(QImage a_canvas)

NAME

    PaintModel::updateHistory(QImage a_canvas) - to update the undo history

SYNOPSIS

    void PaintModel::updateHistory(QImage a_canvas);
        a_canvas --> the canvas to insert into the history

DESCRIPTION

    updates the undo history with the most recent version of the canvas.
    It also clears any possible redos because they are now impossible

RETURNS

    None

*/
/**/
void PaintModel::updateHistory(QImage a_canvas) {
    // if there are possible redos to flush out, remove them and reset the history position
    while (m_historyPos != 0) {
        m_history.removeFirst();
        m_historyPos--;
    }


    // insert the most recent version
    m_history.insert(0, a_canvas);

    while(m_history.length() > UNDO_LIMIT + 1) {
        m_history.removeLast();
    }
}

/**/
/*
void PaintModel::initTools()

NAME

   PaintModel::initTools() - initializes all of the tools the user can use

SYNOPSIS

   void PaintModel::initTools();

DESCRIPTION

    Initializes the tool sets the user can use.

RETURNS

    None

*/
/**/
void PaintModel::initTools() {
    // init the drawing tools
    m_tools[(int)ToolType::DRAWTOOL].append(new DrawTool(QString::fromStdString("Pencil"), 0));

    // init the erase tools
    m_tools[(int)ToolType::ERASETOOL].append(new DrawTool(QString::fromStdString("Eraser"), 1));

    // init the rest of the tools (they don't exist yet);
    m_tools[(int)ToolType::SELECTTOOL].append(new DrawTool(QString::fromStdString("Eraser"), 1));

    // init the shapetools
    m_tools[(int)ToolType::SHAPETOOL].append(new SquareTool(QString::fromStdString("Square"), 0));
    m_tools[(int)ToolType::SHAPETOOL].append(new CircleTool(QString::fromStdString("Circle"), 0));
    m_tools[(int)ToolType::SHAPETOOL].append(new PolygonTool(QString::fromStdString("Polygon"), 0));
    m_tools[(int)ToolType::SHAPETOOL].append(new LineTool(QString::fromStdString("Line"), 0));

    // init the fill tools
    m_tools[(int)ToolType::FILLTOOL].append(new FillTool("Fill Bucket", 0));

}
