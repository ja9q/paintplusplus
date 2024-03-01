//
// Implementation for the PaintModel class
//

#include "PaintModel.h"

#include "DrawTool.h"
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
    m_canvas(CanvasWidget(&m_user, parent)),
    m_currentToolType(PaintModel::DRAWTOOL)
{
    initTools();

    for(int i = 0; i < TOOLCOUNT; i++) {
        m_currentTool[i] = 0;
    }

    m_user.setCurrentTool(m_tools[m_currentToolType][0]);
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

BaseTool* PaintModel::getCurrentTool() const
{
    return m_user.getCurrentTool();
}

void PaintModel::setColor(QColor a_color, int a_which) {
    m_user.setColor(a_color, a_which);
}

void PaintModel::fillCanvas() {
    m_canvas.fillCanvas(m_user.getColor(0));
}

void PaintModel::clearCanvas() {
    m_canvas.fillCanvas(m_user.getColor(1));
}

void PaintModel::updateToolSetting(const int a_settingid, const int a_newValue) {
    BaseTool* currentTool = m_user.getCurrentTool();
    currentTool->setProperty(a_settingid, a_newValue);
}

void PaintModel::initTools() {
    // init the drawing tools
    m_tools[PaintModel::DRAWTOOL].append(new DrawTool(QString::fromStdString("Pencil"), 0, {}));

    // init the erase tools
    m_tools[PaintModel::ERASETOOL].append(new DrawTool(QString::fromStdString("Eraser"), 1, {}));

    // init the rest of the tools (they don't exist yet);

}
