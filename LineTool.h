//
// A line that can be drawn and edited by the user
//

#ifndef LINETOOL_H
#define LINETOOL_H

#include "DrawTool.h"
#include <QMouseEvent>

class LineTool : public DrawTool
{
public:
    // parametric constructor
    LineTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

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

    enum class EditMode {
        NONE, TRANSLATE, ROTATE, SCALE, END
    };

    // the points of the shape
    QLine m_line;

    // variables for editing the shape after it is made
    // if the shape exists and is being edited
    bool m_isEditing;
    // the type of transformation that is being performed on the shape
    EditMode m_editMode;

    // parameters of the transformation
    QPoint m_translation;
    qreal m_rotation;
    QPointF m_scale;

    // which point of the line should stay still when "scaling" it
    int m_pivot;

    // Draw the line on the temporary canvas
    virtual void drawLine(QImage* a_canvas, const QColor a_color1, const QColor a_color2);

    // Calculate the line as it is first being dragged by the mouse
    virtual void calcLine(const QMouseEvent* a_event);

    // Draw the pivots on the ends of the line
    void drawPivots(QImage* a_canvas);

    // identify the edit the user wants based off of the mouse click position
    void identifyEdit(QImage* a_canvas);

    // Move the shape
    void translate(const QMouseEvent* a_event);

    // rotate the line
    void rotate(const QMouseEvent* a_event);

    // resize the shape (move only one point)
    void scale(const QMouseEvent* a_event);
};

#endif // LINETOOL_H
