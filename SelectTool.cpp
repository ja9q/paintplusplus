//
// Implementation for the SelectTool class
//

#include "SelectTool.h"
#include <QPainter>
#include <QRegion>
#include <QBitmap>

/**/
/*
SelectTool::SelectTool(QString a_name, QVector<int> a_moreProperties)

NAME

    SelectTool::SelectTool(a_name, QVector<int> a_moreProperties) - constructor

SYNOPSIS

    SelectTool::SelectTool(QString a_name, QVector<int> a_moreProperties);
        a_name --> the name of the tool
        a_moreProperties --> additional properties

DESCRIPTION

    parametric constructor

RETURNS

    The constructed select tool

*/
/**/
SelectTool::SelectTool(QString a_name, QVector<int> a_moreProperties):
    BaseTool(a_name, {(int)ToolProperty::MASKBACK}), m_selectArea(), m_maskBack(false), m_fromCanvas(true)
{
    addProperties(a_moreProperties);
}

/**/
/*
int SelectTool::getProperty(const int a_propId)

NAME

    SelectTool::getProperty(const int a_propId) - get a certain property's value

SYNOPSIS

    int SelectTool::getProperty(const int a_propId);
        a_propId --> the ID of the property the tool wants to receive

DESCRIPTION

    Get a tool's current property value

RETURNS

    The value of the requested property

*/
/**/
// Get a tool's property
int SelectTool::getProperty(const int a_propId) {
    switch ((ToolProperty)a_propId) {
    case ToolProperty::MASKBACK:
        return (int)m_maskBack;
        break;
    default:
        break;
    }

    return -1;
}

/**/
/*
void SelectTool::setProperty(const int a_propId, const int a_newValue)

NAME

    SelectTool::setProperty(const int a_propId, const int a_newValue) - set a certain property's value

SYNOPSIS

    void SelectTool::setProperty(const int a_propId, const int a_newValue);
        a_propId --> the ID of the property to change
        a_newValue --> the new value of the changed property

DESCRIPTION

    Changes a tool's property to a new value.

RETURNS

    None

*/
/**/
int SelectTool::setProperty(const int a_propId, const int a_newValue) {
    switch ((ToolProperty)a_propId) {
    case ToolProperty::MASKBACK:
        m_maskBack = (bool)a_newValue;
        return 0;
        break;
    default:
        break;
    }
    return -1;
}

/**/
/*
void SelectTool::resetEditor()

NAME

    SelectTool::resetEditor() - reset the editor

SYNOPSIS

    void SelectTool::resetEditor();

DESCRIPTION

    reset the editable parameters in the editable

RETURNS

    None

*/
/**/
void SelectTool::resetEditor() {
    m_selectArea.reset();
    m_fromCanvas = true;
}

/**/
/*
QImage SelectTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts)

NAME

    SelectTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts) - get the image of the current selection

SYNOPSIS

    QImage SelectTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts);
        a_canvas --> the canvas to cover cut area if needed; this is the permanent canvas
        a_color --> the color used to cover the cut area if needed
        a_cuts --> whether to cut the selection

DESCRIPTION

    Get the selected image if there is one. The image will also have the transformations applied to it.
    The image can also be "cut from the canvas"

RETURNS

    The transformed selection

*/
/**/
QImage SelectTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts) {
    if(m_selectArea.isEditing() && (m_selectArea.getEditMode() == Editable::EditMode::END || m_selectArea.getEditMode() == Editable::EditMode::NONE)) {
        QImage temp = m_selection;

        // scale the shape first because it displaces the center + has to be before rotation
        temp = temp.transformed(QTransform().scale(m_selectArea.getScale().x(), m_selectArea.getScale().y()));

        // rotate the shape with the new center
        temp = temp.transformed(QTransform().rotate(m_selectArea.getRotation()));

        // if the image is to be cut, then draw a shape that covers the original selection area.
        if (a_cuts) {
            QPainter painter(a_canvas);
            painter.setPen(Qt::transparent);
            painter.setBrush(a_color);
            painter.drawPolygon(m_selectArea.getShape());
            painter.end();
        }

        return temp;
    }
    // if there is no selection, return a null image.
    return QImage();
}

/**/
/*
void SelectTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas)

NAME

    SelectTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas) - set the selection and
        activate editing mode

SYNOPSIS

    void SelectTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas);
        a_image --> The new selection
        a_canvas --> The permanent canvas
        a_tempCanvas --> The temporary canvas

DESCRIPTION

    Changes the selection of the tool; used when something is pasted in.

RETURNS

    None

*/
/**/
void SelectTool::setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas) {
    if(!m_selectArea.isEditing() || m_selectArea.getEditMode() == Editable::EditMode::END || m_selectArea.getEditMode() == Editable::EditMode::NONE) {
        if (m_selectArea.isEditing()) {
            processDoubleClick(a_canvas, a_tempCanvas, NULL, QColor(), QColor());
        }

        resetEditor();

        m_selection = a_image;

        QRect shape(QPoint(0,0), a_image.size());
        m_selectArea.setShape(QPolygon(shape));
        m_selectArea.initBoundingRect();
        m_selectArea.setIsEditing(true);

        m_fromCanvas = false;

        drawSelection(a_tempCanvas);
        m_selectArea.drawBoundingRect(a_tempCanvas);
    }
}


/**/
/*
int SelectTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    SelectTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a click

SYNOPSIS

    int SelectTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas
        a_tempCanvas --> the temporary canvas
        a_event --> the mouse click that triggered this
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    react to a click. record the position and edit mode if needed.

RETURNS

    0 because it doesn't need to be recorded in the undo history

*/
/**/
int SelectTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void)a_canvas;
    (void)a_tempCanvas;
    (void)a_color1;
    (void)a_color2;

    m_selectArea.setPrevPoint(a_event->position().toPoint());

    // identify the edit based on the click position if the tool is editing
    if (m_selectArea.isEditing()) {
        m_selectArea.identifyEdit();
    }

    if (m_selectArea.getEditMode() == Editable::EditMode::END) {
        m_selectArea.setEditMode(Editable::EditMode::NONE);
    }

    return 0;
}

/**/
/*
int SelectTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    SelectTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a drag

SYNOPSIS

    int SelectTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas
        a_tempCanvas --> the temporary canvas
        a_event --> the mouse click that triggered this
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    react to a drag. If not editing, update selection bounds. Otherwise, update transformation parameters

RETURNS

    0 because it doesn't need to be recorded in the undo history

*/
/**/
int SelectTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_color1;

    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_tempCanvas->fill(Qt::transparent);
    // draw out the shape if the user is still dragging it for the first time (i.e. not in edit mode)
    if (!m_selectArea.isEditing()) {
        calcBounds(a_event);

        drawBounds(a_tempCanvas, a_color2);
    }
    // otherwise the user is in edit mode; use the appropriate edit function
    else {
        m_selectArea.processEdit(a_event);

        if (m_fromCanvas) {
            drawBounds(a_tempCanvas, a_color2);
        }
        drawSelection(a_tempCanvas);
    }

    return 0;
}

/**/
/*
int SelectTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    SelectTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a mouse release

SYNOPSIS

    int SelectTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas
        a_tempCanvas --> the temporary canvas
        a_event --> the mouse click that triggered this
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    react to a mouse release. if bounds have been defined for the first time, turn on editing, otherwise, cancel the current edit.

RETURNS

    0 because it doesn't need to be recorded in the undo history

*/
/**/
int SelectTool::processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_event;
    (void) a_color1;
    (void) a_color2;


    if (m_selectArea.getEditMode() == Editable::EditMode::END) {
        m_selectArea.setEditMode(Editable::EditMode::NONE);
    // if the polygon is not empty and the tool is not editing, turn on editing mode + draw the bounding rectangle + define the selected area
    }else if (!m_selectArea.getShape().isEmpty() && !m_selectArea.isEditing()) {
        m_selectArea.setIsEditing(true);
        renderSelection(a_canvas, a_color2);
        m_selectArea.initBoundingRect();
        m_selectArea.drawBoundingRect(a_tempCanvas);
    }
    else if (m_selectArea.isEditing()) {
        m_selectArea.setEditMode(Editable::EditMode::NONE);
        m_selectArea.drawBoundingRect(a_tempCanvas);
    }

    return 0;
}

/**/
/*
int SelectTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2)

NAME

    SelectTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) - react to a double click

SYNOPSIS

    int SelectTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);
        a_canvas --> the permanent canvas
        a_tempCanvas --> the temporary canvas
        a_event --> the mouse click that triggered this
        a_color1 --> the user's color1
        a_color2 --> the user's color2

DESCRIPTION

    react to a double click by commiting the change to the permanent canvas

RETURNS

    1 to record it in the update history

*/
/**/
int SelectTool::processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2) {
    a_tempCanvas->fill(Qt::transparent);
    if (m_fromCanvas) {
        drawBounds(a_tempCanvas, a_color2);
    }
    drawSelection(a_tempCanvas);

    resetEditor();

    // call the original BaseTool's mouseRelease to copy the contents from the temporary canvas to the original canvas
    BaseTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);
    return 1;
}

/**/
/*
void SelectTool::drawSelection(QImage* a_canvas)

NAME

    SelectTool::drawSelection(QImage* a_canvas) - draw the transformed selection

SYNOPSIS

    void SelectTool::drawSelection(QImage* a_canvas);
        a_canvas --> where to draw the selection

DESCRIPTION

    Draw the transformed selection. Because this draws an image, the transformation has to be done in the function

RETURNS

    None

*/
/**/
void SelectTool::drawSelection(QImage* a_canvas) {
    QImage temp = m_selection;

    QPainter painter(a_canvas);

    // scale the shape first because it displaces the center + has to be before rotation
    temp = temp.transformed(QTransform().scale(m_selectArea.getScale().x(), m_selectArea.getScale().y()));

    // rotate the shape with the new center
    temp = temp.transformed(QTransform().rotate(m_selectArea.getRotation()));

    //images cannot be translated, so base it off of the position of the editable's bounding rectangle

    QRect extraOffset = m_selectArea.getTransformedBoundRect().boundingRect();

    painter.drawPixmap(extraOffset.left(), extraOffset.top(), QPixmap::fromImage(temp));
}

/**/
/*
void SelectTool::drawBounds(QImage* a_canvas, const QColor a_color)

NAME

    SelectTool::drawBounds(QImage* a_canvas, const QColor a_color) -

SYNOPSIS

    void SelectTool::drawBounds(QImage* a_canvas, const QColor a_color);
        a_canvas --> the canvas to draw the bounds on
        a_color --> the color to cover the previous selection area

DESCRIPTION

    draw the current selection border if the user is still drawing it out.
    if the tool is otherwise in editing mode, then block out the original selected area

RETURNS

    None

*/
/**/
void SelectTool::drawBounds(QImage* a_canvas, const QColor a_color) {
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    // if not editing, draw out the bounds
    if (!m_selectArea.isEditing()) {
        painter.setPen(Qt::white);
        painter.drawPolyline(m_selectArea.getShape().translated(-1,-1));

        painter.setPen(Qt::black);
        painter.drawPolyline(m_selectArea.getShape());
    } else {
        // otherwise, draw the original shape in a apaque blob
        painter.setPen(Qt::transparent);
        painter.setBrush(a_color);
        painter.drawPolygon(m_selectArea.getShape());
    }

}

/**/
/*
void SelectTool::calcBounds(const QMouseEvent* a_event)

NAME

    SelectTool::calcBounds(const QMouseEvent* a_event) - calculate the bounds as they are being drawn

SYNOPSIS

    void SelectTool::calcBounds(const QMouseEvent* a_event);
        a_event --> The mouse event that triggered this function

DESCRIPTION

    calculate the outline of the selection as it is being drawn out

RETURNS

    None

*/
/**/
void SelectTool::calcBounds(const QMouseEvent* a_event) {
    QPolygon shape = m_selectArea.getShape();

    // if the shape is empty or the mouse moved from the last position in the shape,
    // add it to the original shape
    if (shape.isEmpty() || shape.back() != a_event->position().toPoint()) {
        shape.append(a_event->position().toPoint());
    }


    // if there are 3+ in the shape, then check if the two newest sides have the same
    // slope. if they do, then merge them.
    if (shape.size() > 2) {
        QLine oldSide = QLine(shape.at(shape.size()-3), shape.at(shape.size()-2));
        QLine newSide = QLine(shape.at(shape.size()-2), shape.back());
        if (oldSide.dx()* newSide.dx() != 0) {
            qreal oldSlope = (1.0*oldSide.dy())/ oldSide.dx();
            qreal newSlope = (1.0*newSide.dy())/ newSide.dx();
            if (oldSlope == newSlope) {
                shape.remove(shape.size()-2);
            }
        } else if (oldSide.dx() == 0 && newSide.dx() == 0){
            shape.remove(shape.size()-2);
        }
    }

    m_selectArea.setShape(shape);
}

/**/
/*
void SelectTool::renderSelection(QImage* a_canvas, const QColor a_color)

NAME

    SelectTool::renderSelection(QImage* a_canvas, const QColor a_color) - initialize the selection
        after the bounds are first defined

SYNOPSIS

    void SelectTool::renderSelection(QImage* a_canvas, const QColor a_color);
        a_canvas --> the canvas from where to grab the selection from
        a_color --> the color to mask out (if required)

DESCRIPTION

    using the bounds defined by the user, create a copy of that region into the tool
    so it can be edited later. mask out the second color if it was required.

RETURNS

    None

*/
/**/
void SelectTool::renderSelection(QImage* a_canvas, const QColor a_color) {
    QRect boundingRect = m_selectArea.getShape().boundingRect();

    // initialize the selection to have the dimensions of the selected area
    m_selection = QImage(boundingRect.width(), boundingRect.height(), QImage::Format_ARGB32);
    QImage temp = QImage(boundingRect.width(), boundingRect.height(), QImage::Format_ARGB32);

    // if color2 will be masked out then make it the default background color so it can be later clipped out.
    // otherwise, make the backing transparent
    if(m_maskBack) {
        m_selection.fill(a_color);
    } else {
        m_selection.fill(Qt::transparent);
    }
    temp.fill(Qt::transparent);

    // draw the silhouette of the selection area onto the selection
    QPainter maskPainter(&temp);
    maskPainter.setPen(QColor(Qt::transparent));
    maskPainter.setBrush(a_color);
    maskPainter.drawPolygon(m_selectArea.getShape().translated(-boundingRect.left(), -boundingRect.top()));
    maskPainter.end();

    // draw the current selection while clipping onto the selection area
    QPainter painter(&temp);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(QPoint(0,0), QPixmap::fromImage(*a_canvas), boundingRect);
    painter.end();

    // if the settings want to remove the back color, draw a version without the color into the selection
    if(m_maskBack) {
        QPainter finalPainter(&m_selection);
        finalPainter.drawPixmap(0,0, QPixmap::fromImage(temp));
        QImage mask = (m_selection.createMaskFromColor(a_color.rgb(), Qt::MaskInColor));
        m_selection.fill(Qt::transparent);
        finalPainter.setClipRegion(QBitmap::fromImage(mask));
        finalPainter.drawPixmap(QPoint(0,0), QPixmap::fromImage(*a_canvas), boundingRect);
    } else {
        m_selection = temp;
    }

}
