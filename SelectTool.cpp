#include "SelectTool.h"
#include <QPainter>
#include <QRegion>
#include <QBitmap>

SelectTool::SelectTool(QString a_name, QVector<int> a_moreProperties):
    BaseTool(a_name, {(int)ToolProperty::MASKBACK}), m_selectArea(), m_maskBack(false), m_fromCanvas(true)
{
    addProperties(a_moreProperties);
}

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

// Modify a tool's property (e.g. size, opacity)
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

// reset the editor (remove any uncommited shapes)
void SelectTool::resetEditor() {
    m_selectArea.reset();
    m_fromCanvas = true;
}

QImage SelectTool::getEditable(QImage* a_canvas, const QColor a_color, bool a_cuts) {
    if(m_selectArea.isEditing() && (m_selectArea.getEditMode() == Editable::EditMode::END || m_selectArea.getEditMode() == Editable::EditMode::NONE)) {
        QImage temp = m_selection;

        // scale the shape first because it displaces the center + has to be before rotation
        temp = temp.transformed(QTransform().scale(m_selectArea.getScale().x(), m_selectArea.getScale().y()));

        // rotate the shape with the new center
        temp = temp.transformed(QTransform().rotate(m_selectArea.getRotation()));

        if (a_cuts) {
            QPainter painter(a_canvas);
            painter.setPen(Qt::transparent);
            painter.setBrush(a_color);
            painter.drawPolygon(m_selectArea.getShape());
            painter.end();
        }

        return temp;
    }
    return QImage();
}

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


// react to a click
int SelectTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void)a_canvas;
    (void)a_tempCanvas;
    (void)a_color1;
    (void)a_color2;

    m_selectArea.setPrevPoint(a_event->position().toPoint());

    if (m_selectArea.isEditing()) {
        m_selectArea.identifyEdit();
    }

    if (m_selectArea.getEditMode() == Editable::EditMode::END) {
        m_selectArea.setEditMode(Editable::EditMode::NONE);
    }

    return 0;
}

// react to a drag
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

// react to a mouse release
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

// react to a double click
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

void SelectTool::drawSelection(QImage* a_canvas) {
    QImage temp = m_selection;

    QPainter painter(a_canvas);

    // scale the shape first because it displaces the center + has to be before rotation
    temp = temp.transformed(QTransform().scale(m_selectArea.getScale().x(), m_selectArea.getScale().y()));

    // rotate the shape with the new center
    temp = temp.transformed(QTransform().rotate(m_selectArea.getRotation()));

    QRect extraOffset = m_selectArea.getTransformedBoundRect().boundingRect();

    painter.drawPixmap(extraOffset.left(), extraOffset.top(), QPixmap::fromImage(temp));
}

// Draw the bounds on the temporary canvas
void SelectTool::drawBounds(QImage* a_canvas, const QColor a_color) {
    QPainter painter(a_canvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    if (!m_selectArea.isEditing()) {
        painter.setPen(Qt::white);
        painter.drawPolyline(m_selectArea.getShape().translated(-1,-1));

        painter.setPen(Qt::black);
        painter.drawPolyline(m_selectArea.getShape());
    } else {
        painter.setPen(Qt::transparent);
        painter.setBrush(a_color);
        painter.drawPolygon(m_selectArea.getShape());
    }

}

// Calculate the border of the selection as it is being drawn by the mouse
void SelectTool::calcBounds(const QMouseEvent* a_event) {
    QPolygon shape = m_selectArea.getShape();

    if (shape.isEmpty() || shape.back() != a_event->position().toPoint()) {
        shape.append(a_event->position().toPoint());
    }


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
