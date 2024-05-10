//
// Implementation for the BaseTool class
//

#include "BaseTool.h"
#include <QDebug>
#include <QPainter>

/**/
/*
BaseTool::BaseTool(QString a_name, QVector<int> a_properties)

NAME

    a_name, QVector<int> a_properties) - parametric constructor

SYNOPSIS

    BaseTool::BaseTool(QString a_name, QVector<int> a_properties);
        a_name --> the name of the tool
        a_properties --> the properties of the tool that the user can edit

DESCRIPTION

    Constructs a base tool

RETURNS

    The constructed BaseTool

*/
/**/
BaseTool::BaseTool(QString a_name, QVector<int> a_properties):
    m_name(a_name),
    m_properties(a_properties)
{}

/**/
/*
QString BaseTool::getName() const

NAME

    BaseTool::getName() const - getter for the name

SYNOPSIS

    QString BaseTool::getName() const;

DESCRIPTION

    Getter for the tool's name

RETURNS

    the tool's name

*/
/**/
QString BaseTool::getName() const {
    return m_name;
}

/**/
/*
QVector<int> BaseTool::getProperties() const

NAME

    BaseTool::getProperties() - gets the tool's properties

SYNOPSIS

    QVector<int> BaseTool::getProperties() const;

DESCRIPTION

    This function returns the tools properties, which are used to parsed to
    create tool settings in the GUI

RETURNS

    Returns the tool's properties a QVector of integers that are like
    id's to certain

*/
/**/
QVector<int> BaseTool::getProperties() const {
    return m_properties;
}

/**/
/*
void BaseTool::resetEditor()

NAME

    BaseTool::resetEditor() - reset the editor (if there is one)

SYNOPSIS

    void BaseTool::resetEditor();

DESCRIPTION

    If the tool has an editable (e.g. a shape tool or select tool), reset the editor if it is active

RETURNS

    None

*/
/**/
void BaseTool::resetEditor() {
    // do nothing because there is no editable
}

/**/
/*
QImage BaseTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts)

NAME

    BaseTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts) -
        (for tools with editors, get the edited item)

SYNOPSIS

    QImage BaseTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts);
        a_canvas --> (if this is a cut) the canvas to draw the cut area on
        a_color --> (if this is a cut) the color to draw the cut area with
        a_cuts --> whether this operation will cut out the editable from the canvas

DESCRIPTION

    Grabs the edited shape that a shape is currently editing (if there is one)

RETURNS

    The currently edited shape or a null shape if there is none

*/
/**/
QImage BaseTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts) {
    // do nothing because there is no editable
    (void)a_canvas;
    (void)a_color;
    (void)a_cuts;
    return QImage();
}

/**/
/*
void BaseTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas)

NAME

    BaseTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas) - setter for the editor (if there is one)

SYNOPSIS

    void BaseTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas);
        a_image --> the new image to edit
        a_canvas --> needed to reset the editor if needed
        a_tempCanvas --> needed to reset the editor if needed

DESCRIPTION

    setter for the editor's image. It is only called in the context that the current shape is a SelectTool

RETURNS

    None

*/
/**/
void BaseTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas) {
    // do nothing because there is no editable
    (void)a_image;
    (void)a_canvas;
    (void)a_tempCanvas;
}

/**/
/*
int BaseTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    BaseTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)
        - react to when the mouse is no longer clicking/dragging

SYNOPSIS

    int processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas - the actual canvas; the tool either references or transforms this area
        a_tempCanvas - a temporary canvas; changes to the canvas are temporarily displayed here before they are commited
        a_event - the related mouse event; stores position and active modifier keys
        a_color1 - the current color 1; from the PaintModel
        a_color2 - the current color 2; from the PaintModel

DESCRIPTION

    This function processes what to do when the mouse is no clicking/dragging; most drawing tools
    use this to transfer the contents of the temp canvas into the actual canvas, but other tools don't do this

RETURNS

    0 if the canvas doesn't have to do any further work
    However, this returns a 1 because the canvas needs to update the undo menu.

*/
/**/
int BaseTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_event;
    (void) a_color1;
    (void) a_color2;
    // If this was a left mouse button release, then paint the temp canvas onto the actual canvas and rerender
    QPainter painter(a_canvas);
    painter.drawPixmap(0,0,QPixmap::fromImage(*a_tempCanvas));
    // also clear the temporary canvas
    a_tempCanvas->fill(QColor(Qt::transparent));

    return 1;
}

/**/
/*
int BaseTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    BaseTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)
        - react to when the mouse double clicks

SYNOPSIS

    int processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas - the actual canvas; the tool either references or transforms this area
        a_tempCanvas - a temporary canvas; changes to the canvas are temporarily displayed here before they are commited
        a_event - the related mouse event; stores position and active modifier keys
        a_color1 - the current color 1; from the PaintModel
        a_color2 - the current color 2; from the PaintModel

DESCRIPTION

    This function processes what to do when the mouse double clicks; most tools do nothing, but the polygon uses this
    to complete and finalize the shape

RETURNS

    0 if the canvas doesn't have to do any further work

*/
/**/
int BaseTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_tempCanvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;
    return 0;
}

/**/
/*
void BaseTool::addProperties(QVector<int> a_extraProperties)

NAME

    BaseTool::addProperties(QVector<int> a_extraProperties) - adds new properties

SYNOPSIS

    void BaseTool::addProperties(QVector<int> a_extraProperties)
        a_extraProperties --> a list of new properties to be added

DESCRIPTION

    Adds new properties to the properties list.

RETURNS

    None

*/
/**/
void BaseTool::addProperties(QVector<int> a_extraProperties) {
    m_properties.append(a_extraProperties);
}
