#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "BaseTool.h"

#include <QTextEdit>
#include <QTimer>
#include <QFontDatabase>
#include "Editable.h"
#include "CanvasWidget.h"

class TextTool : public BaseTool
{
public:
    TextTool(QString a_name, CanvasWidget* a_canvas, QVector<int> a_moreProperties = {});

    int getProperty(const int a_propId);

    // Modify a tool's property (e.g. size, opacity)
    int setProperty(const int a_propId, const int a_newValue);

    void resetEditor();

    // React to a click on the canvas
    int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // React to a drag on the canvas
    int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // React to when the mouse is no longer clicking/dragging on the canvas
    int processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a double click (only relevant to the polyline tool)
    int processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

private:

    QList<QString> FONTS;

    QTextEdit m_textbox;

    Editable m_bounds;

    QTimer m_timer; // used to automatically rerender the textbox and show the blinking cursor

    CanvasWidget* m_canvas;

    bool m_isActive;

    bool m_isOpaque;

    QColor m_bgColor;

    void drawText(QImage* a_canvas, bool a_renderBounds = true);

    void adjustSize();
};

#endif // TEXTTOOL_H
