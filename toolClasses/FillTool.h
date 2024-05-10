//
// Tool that replaces a section of one color with another color
//

#ifndef FILLTOOL_H
#define FILLTOOL_H

#include "BaseTool.h"

class FillTool : public BaseTool
{
public:
    // parametric constructor
    FillTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    // Get the current value of a tool's property
    int getProperty(const int a_propId);

    // Modify a tool's property (e.g. size, opacity)
    int setProperty(const int a_propId, const int a_newValue);

    // react to a click
    int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a drag
    int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

protected:
    int m_color;    // which color (1 or 2) the tool uses
    int m_opacity;  // the opacity of the color

    // the fill algorithm
    void fill(QImage* a_canvas, QImage* a_tempCanvas, const QPoint a_startPos, const QColor a_fillColor, const QColor a_origColor);
};

#endif // FILLTOOL_H
