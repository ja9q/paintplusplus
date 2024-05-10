//
// A shape that can be edited by the user by interacting with a bounding rectangle
//

#ifndef EDITABLE_H
#define EDITABLE_H

#include <QPolygon>
#include <QPoint>
#include <QImage>
#include <QMouseEvent>

class Editable
{
public:
    // The possible actions (or lack of actions) the editable can assume
    enum class EditMode {
        NONE, TRANSLATE, ROTATE, SCALE, END
    };

    // constructor
    Editable();

    // getter for the shape
    QPolygon getShape();

    // getter for if the object is editing
    bool isEditing();

    // getter for the current edit mode
    EditMode getEditMode();

    // getter for the shape and its bound rectangle
    QPolygon getTransformedShape();
    QPolygon getTransformedBoundRect();

    // getters for the parameters of the transformation
    QPoint getTranslation();
    qreal getRotation();
    QPointF getScale();

    // setter for if the object is editing
    void setIsEditing(bool a_mode);

    // setter for the edit mode
    void setEditMode(EditMode a_mode);

    // setter for the shape
    void setShape(QPolygon a_shape);

    // setter for the last recorded point (for calculating transformations)
    void setPrevPoint(QPoint a_point);

    // Initialize the bounding rectangle (custom shape)
    void initBoundingRect();

    // Draw the bounding rectangle
    void drawBoundingRect(QImage* a_canvas);

    // identify the edit the user wants based off of the mouse click position
    void identifyEdit();

    // perform a transformation based off of the current edit mode
    void processEdit(const QMouseEvent *a_event);

    // reset the object's shape and parameters
    void reset();

private:
    // the shape that will be edited
    QPolygon m_shape;

    // the bounding rectangle of the shape
    QPolygon m_boundRect;

    // if the shape exists and is being edited
    bool m_isEditing;
    // the type of transformation that is being performed on the shape
    EditMode m_editMode;

    // parameters of the transformation
    QPoint m_translation;
    qreal m_rotation;
    QPointF m_scale;

    // (for scaling) which point of the bounding rectangle will not move when scaling
    int m_anchor;

    // the previous mouse point relevant for a transformation
    QPoint m_prevEditPoint;

    // Transform a shape
    QPolygon transformShape(QPolygon a_shape);

    // Move the shape
    void translate(const QMouseEvent* a_event);

    // rotate the shape
    void rotate(const QMouseEvent* a_event);

    // Resize the shape
    void scale(const QMouseEvent* a_event);
};

#endif // EDITABLE_H
