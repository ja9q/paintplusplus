#ifndef SHAPETOOL_H
#define SHAPETOOL_H

#include "DrawTool.h"
#include <QMouseEvent>

class ShapeTool : public DrawTool
{

public:
    // parametric constructor
    ShapeTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // Get a tool's property
    int getProperty(const int a_propId);

    // Modify a tool's property (e.g. size, opacity)
    int setProperty(const int a_propId, const int a_newValue);

    // reset the editor (remove any uncommited shapes)
    void resetEditor();

    // react to a click
    int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a drag
    int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a mouse release
    int processMouseRelease(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2);

    // react to a double click
    int processDoubleClick(QImage *a_canvas, QImage *a_tempCanvas, const QMouseEvent *a_event, const QColor a_color1, const QColor a_color2);

protected:
    enum class FillMode {
        NOFILL, FILLC1, FILLC2
    };

    enum class EditMode {
        NONE, TRANSLATE, ROTATE, SCALE, END
    };

    // the points of the shape
    QPolygon m_shape;

    // the bounding rectangle of the shape
    QPolygon m_boundRect;

    // how the tool should fill the shape or not
    FillMode m_fillMode;

    // if the tool should outline the shape
    bool m_outline;

    // variables for editing the shape after it is made
    // if the shape exists and is being edited
    bool m_isEditing;
    // the type of transformation that is being performed on the shape
    EditMode m_editMode;

    QPoint m_translation;
    qreal m_rotation;
    QPointF m_scale;

    int m_anchor;

    // Draw the shape on the temporary canvas
    virtual void drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2);

    // Calculate the shape as it is first being dragged by the mouse
    virtual void calcShape(const QMouseEvent* a_event) = 0;

    // Initialize the bounding rectangle (custom shape)
    void initBoundingRect();

    // Initialize the bounding rectangle (custom shape)
    void resetBoundingRect();

    // Draw the bounding rectangle
    void drawBoundingRect(QImage* a_canvas);

    // identify the edit the user wants based off of the mouse click position
    void identifyEdit();

    QPolygon transformShape(QPolygon a_shape);

    // Move the shape
    void translate(const QMouseEvent* a_event);

    // rotate the shape
    void rotate(const QMouseEvent* a_event);

    // Resize the shape
    void scale(const QMouseEvent* a_event);
};

#endif // SHAPETOOL_H
