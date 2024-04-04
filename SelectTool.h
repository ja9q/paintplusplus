#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "BaseTool.h"
#include "Editable.h"

class SelectTool : public BaseTool
{
public:
    // parametric constructor
    SelectTool(QString a_name, QVector<int> a_moreProperties = {});

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

    // the selected part of the canvas
    QImage m_selection;

    Editable m_selectArea;

    // whether to remove the background from the selection or not
    bool m_maskBack;


    virtual void drawSelection(QImage* a_canvas);

    // Draw the bounds on the temporary canvas
    virtual void drawBounds(QImage* a_canvas, const QColor a_color);

    // Calculate the border of the selection as it is being drawn by the mouse
    virtual void calcBounds(const QMouseEvent* a_event);

    void renderSelection(QImage* a_canvas, const QColor a_color);

};

#endif // SELECTTOOL_H
