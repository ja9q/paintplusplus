//
// An abstract class for tools that want to draw a shape with multiple points
//

#ifndef SHAPETOOL_H
#define SHAPETOOL_H

#include "DrawTool.h"
#include "Editable.h"
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

    // how to fill the shape
    FillMode m_fillMode;

    // whether to outline the shape or not
    bool m_outline;

    // the points of the shape
    Editable m_shape;

    // Calculate the shape as it is first being dragged by the mouse
    virtual void calcShape(const QMouseEvent* a_event) = 0;

    virtual void drawShape(QImage* a_canvas, const QColor a_color1, const QColor a_color2);

};

#endif // SHAPETOOL_H
