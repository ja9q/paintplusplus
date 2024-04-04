#ifndef EDITABLE_H
#define EDITABLE_H

#include <QPolygon>
#include <QPoint>
#include <QImage>
#include <QMouseEvent>

class Editable
{
public:
    enum class EditMode {
        NONE, TRANSLATE, ROTATE, SCALE, END
    };


    Editable();

    QPolygon getShape();

    bool isEditing();

    EditMode getEditMode();

    QPolygon getTransformedShape();
    QPolygon getTransformedBoundRect();

    QPoint getTranslation();
    qreal getRotation();
    QPointF getScale();

    void setIsEditing(bool a_mode);

    void setEditMode(EditMode a_mode);

    void setShape(QPolygon a_shape);

    void setPrevPoint(QPoint a_point);

    // Initialize the bounding rectangle (custom shape)
    void initBoundingRect();

    // Draw the bounding rectangle
    void drawBoundingRect(QImage* a_canvas);

    // identify the edit the user wants based off of the mouse click position
    void identifyEdit();

    void processEdit(const QMouseEvent *a_event);

    void reset();

protected:

    // the points of the shape
    QPolygon m_shape;

    // the bounding rectangle of the shape
    QPolygon m_boundRect;

    // if the shape exists and is being edited
    bool m_isEditing;
    // the type of transformation that is being performed on the shape
    EditMode m_editMode;

    QPoint m_translation;
    qreal m_rotation;
    QPointF m_scale;

    int m_anchor;
    QPoint m_prevEditPoint;

    QPolygon transformShape(QPolygon a_shape);

    // Move the shape
    void translate(const QMouseEvent* a_event);

    // rotate the shape
    void rotate(const QMouseEvent* a_event);

    // Resize the shape
    void scale(const QMouseEvent* a_event);
};

#endif // EDITABLE_H
