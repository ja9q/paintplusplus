#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "BaseTool.h"

#include <QTextEdit>
#include "Editable.h"

class TextTool : public BaseTool
{
public:
    TextTool(QString a_name, QVector<int> a_moreProperties = {});

    int getProperty(const int a_propId) = 0;

    // Modify a tool's property (e.g. size, opacity)
    int setProperty(const int a_propId, const int a_newValue) = 0;

    void resetEditor();

    // React to a click on the canvas
    int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) = 0;

    // React to a drag on the canvas
    int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) = 0;

    // React to when the mouse is no longer clicking/dragging on the canvas
    int processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a double click (only relevant to the polyline tool)
    int processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

private:

    QTextEdit m_textbox;

    Editable m_bounds;

    bool m_isOpaque;
};

#endif // TEXTTOOL_H
