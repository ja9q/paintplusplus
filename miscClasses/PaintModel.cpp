//
// Implementation for the PaintModel class
//

#include "PaintModel.h"

#include <QFileDialog>
#include <QGuiApplication>
#include <QClipboard>
#include <QPainter>
#include <QMimeData>

#include "./../toolClasses/DrawTool.h"
#include "./../toolClasses/SelectTool.h"
#include "./../toolClasses/SquareSelectTool.h"
#include "./../toolClasses/SquareTool.h"
#include "./../toolClasses/CircleTool.h"
#include "./../toolClasses/PolygonTool.h"
#include "./../toolClasses/LineTool.h"
#include "./../toolClasses/FillTool.h"
#include "./../toolClasses/TextTool.h"
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
    m_fileName(""),
    m_historyPos(0),
    m_saved(true)
{
    initTools();
    m_user.setCurrentTool(m_tools[(int)ToolType::DRAWTOOL][0]);

    // set it such that when the canvas says it updated, the model updates its history
    connect(&m_canvas, &CanvasWidget::canvasChanged, this, &PaintModel::updateHistory);
    // thus, a blank white canvas becomes the first part of the history
    m_canvas.fillCanvas(QColor(Qt::white));

    // but, this is not an unsaved change (or any change at all), so reset that the canvas is "saved"
    m_saved = true;
}

/**/
/*
PaintModel::~PaintModel()

NAME

    PaintModel::~PaintModel() - Destructor

SYNOPSIS

    PaintModel::~PaintModel()
DESCRIPTION

    Destructs the PaintModel.

RETURNS

    None

*/
/**/
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
bool PaintModel::isSaved() const

NAME

    PaintModel::isSaved() const - getter for if the model is saved

SYNOPSIS

    bool PaintModel::isSaved() const;

DESCRIPTION

    getter for if the model is saved; used for checking for unsaved changes
    when the window is closed.

RETURNS

    Whether the model is saved or not

*/
/**/
bool PaintModel::isSaved() const {
    return m_saved;
}

/**/
/*
void PaintModel::setTool(int a_newTool)
NAME

    PaintModel::setTool(int a_newTool) -setter for tools

SYNOPSIS

    void PaintModel::setTool(int a_newTool);
        a_newTool --> the id of the new tool

DESCRIPTION

    changes the new tool

RETURNS

    NONE

*/
/**/
void PaintModel::setTool(int a_newTool) {
    // reset the editor of the current tool to ensure no changes are left behind
    m_user.getCurrentTool()->resetEditor();
    // clear out the temporary canvas
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
    // because the tool type changed, the current tool also changed so update accordingly
    m_user.setCurrentTool(m_tools[a_typeId][m_currentTool[a_typeId]]);
}

/**/
/*
void PaintModel::setCanvasSize(QSize a_size)

NAME

    PaintModel::setCanvasSize(QSize a_size) - resize the canvas

SYNOPSIS

    void PaintModel::setCanvasSize(QSize a_size);
        a_size --> the canvas's new size

DESCRIPTION

    change the canvas's size. the scaling is anchored in the top-left,
    so material is added/removed from the bottom and the right sides

RETURNS

    None

*/
/**/
void PaintModel::setCanvasSize(QSize a_size) {

    QImage newCanvas(a_size, QImage::Format_ARGB32_Premultiplied);
    newCanvas.fill(Qt::white);

    // redraw the old canvas into the new canvas
    QPainter painter(&newCanvas);
    painter.drawPixmap(0,0, QPixmap::fromImage(*(m_canvas.getCanvas())));
    painter.end();

    m_canvas.setCanvas(newCanvas);

    updateHistory(newCanvas);
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
void PaintModel::openFile()

NAME

    PaintModel::openFile() - prompt the user to choose a file and open it

SYNOPSIS

    void PaintModel::openFile();

DESCRIPTION

    Creates a file dialog to let the user choose a file to open. Then open that file.

RETURNS

    None

*/
/**/
void PaintModel::openFile() {

    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open File"), tr("./"), tr("Images (*.png *.jpg)"));
    if (fileName.isEmpty()) {
        return;
    }

    m_fileName = fileName;

    QImage file(m_fileName);

    // reset the history because this is a new file
    m_history.clear();
    m_history.append(file);
    m_historyPos = 0;

    m_canvas.setCanvas(file);
}

/**/
/*
bool PaintModel::saveFile()

NAME

    PaintModel::saveFile() - Save the file

SYNOPSIS

    bool PaintModel::saveFile();

DESCRIPTION

    If the file already has a name, save that named file.
    Otherwise, save this file as a new file.

RETURNS

    True if the file was sucessfully saved

*/
/**/
bool PaintModel::saveFile() {
    if(m_fileName != "") {
        QImage canvas = *(m_canvas.getCanvas());
        canvas.save(m_fileName);
        m_saved = true;
    } else {
        return saveNewFile();
    }
    return true;
}

/**/
/*
bool PaintModel::saveFile()

NAME

    PaintModel::saveNewFile() - Save the file as a new file

SYNOPSIS

    bool PaintModel::saveNewFile();

DESCRIPTION

    Open a dialog for the user to choose the file's name and location.
    Save the file under those parameters.

RETURNS

    True if the file was sucessfully saved (i.e. false when the user cancels the prompt)

*/
/**/
bool PaintModel::saveNewFile() {
    QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"), tr("./"), tr("Images (*.png *.jpg)"));
    if (fileName.isEmpty()) {
        return false;
    }

    QImage canvas = *(m_canvas.getCanvas());

    m_fileName = fileName;
    canvas.save(m_fileName);
    m_saved = true;
    return true;
}

/**/
/*
void PaintModel::copy()

NAME

    PaintModel::copy() - copy a tool's selection

SYNOPSIS

    void PaintModel::copy();

DESCRIPTION

    if the tool has a non-blank selection, copy it to the clipboard

RETURNS

    None

*/
/**/
void PaintModel::copy() {
    QImage selection = m_user.getCurrentTool()->getEditable();

    // if the selection is not empty, copy it to the clipboard
    if (!selection.isNull()) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setImage(selection);
    }
}

/**/
/*
void PaintModel::cut()

NAME

    PaintModel::cut() - cut a tool's selection from the canvas

SYNOPSIS

    void PaintModel::cut();

DESCRIPTION

    if the tool has a non-blank selection, copy it to the clipboard and remove it from the canvas

RETURNS

    None

*/
/**/
void PaintModel::cut() {
    // cut the selection from the canvas
    QImage selection = m_user.getCurrentTool()->getEditable(m_canvas.getCanvas(), m_user.getColor(1), true);

    if (!selection.isNull()) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setImage(selection);
        m_canvas.getTempCanvas()->fill(Qt::transparent);

        // this is a change to the canvas, so update the history
        updateHistory(*m_canvas.getCanvas());

        m_canvas.repaint();
    }
}

/**/
/*
void PaintModel::paste()

NAME

    PaintModel::paste() - paste the clipboard's contents into the canvas

SYNOPSIS

    void PaintModel::paste();

DESCRIPTION

    pastes the clipboards contents into the canvas. Assumes the tool is already the select tool

RETURNS

    None

*/
/**/
void PaintModel::paste() {
    const QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimedata = clipboard->mimeData();

    // only update the editable, if the item in the clipboard is an image
    if (mimedata->hasImage()) {
        m_user.getCurrentTool()->setEditable(qvariant_cast<QImage>(mimedata->imageData()), m_canvas.getCanvas(), m_canvas.getTempCanvas());
    }
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
    // if the tool is editing something, then just cancel the editing rather than actually undoing something
    if (!(m_user.getCurrentTool()->getEditable().isNull())) {
        m_user.getCurrentTool()->resetEditor();
        m_canvas.setCanvas(m_history[m_historyPos]);
    } else if (m_history.length() > m_historyPos+1 && m_historyPos < UNDO_LIMIT-1) {
        m_user.getCurrentTool()->resetEditor();
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
    m_saved = false;

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
    m_tools[(int)ToolType::DRAWTOOL].append(new DrawTool(QString::fromStdString("Pencil"), 0, {6}));

    // init the erase tools
    m_tools[(int)ToolType::ERASETOOL].append(new DrawTool(QString::fromStdString("Eraser"), 1));

    // init the rest of the tools (they don't exist yet);
    m_tools[(int)ToolType::SELECTTOOL].append(new SelectTool(QString::fromStdString("Lasso")));
    m_tools[(int)ToolType::SELECTTOOL].append(new SquareSelectTool(QString::fromStdString("Square Select")));

    // init the shapetools
    m_tools[(int)ToolType::SHAPETOOL].append(new SquareTool(QString::fromStdString("Square"), 0));
    m_tools[(int)ToolType::SHAPETOOL].append(new CircleTool(QString::fromStdString("Circle"), 0));
    m_tools[(int)ToolType::SHAPETOOL].append(new PolygonTool(QString::fromStdString("Polygon"), 0));
    m_tools[(int)ToolType::SHAPETOOL].append(new LineTool(QString::fromStdString("Line"), 0));

    // init the fill tools
    m_tools[(int)ToolType::FILLTOOL].append(new FillTool("Fill Bucket", 0));

    // init the text tools
    m_tools[(int)ToolType::TEXTTOOL].append(new TextTool(QString::fromStdString("Text"), &m_canvas));

}
