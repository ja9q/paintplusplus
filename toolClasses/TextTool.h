//
// A tool that allows the user to make a textbox and draw text on the canvas
//

#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "BaseTool.h"

#include <QTextEdit>
#include <QTimer>
#include <QFontDatabase>
#include "Editable.h"
#include "./../widgetClasses/CanvasWidget.h"

class TextTool : public BaseTool
{
public:
    TextTool(QString a_name, CanvasWidget* a_canvas, QVector<int> a_moreProperties = {});

    int getProperty(const int a_propId);

    // Modify a tool's property (e.g. size, opacity)
    int setProperty(const int a_propId, const int a_newValue);

    // reset the parameters of the textbox and editor
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
    // the system's fonts
    QList<QString> FONTS;

    // the textbox the user will be using
    QTextEdit m_textbox;

    // stores the parameters of the text separate from the textbox
    QFont m_font;

    // the editor for the textbox and its dimensions
    Editable m_bounds;

    // a pointer to the canvas because a lot of rerendering is necessary
    CanvasWidget* m_canvas;

    // if the text editor is working or not
    bool m_isActive;

    // if the background of the textbox is opaque or not
    bool m_isOpaque;

    // the color of the text
    QColor m_textColor;

    // the background color for the textbox
    QColor m_bgColor;

    // draw the textbox
    void drawText(QImage* a_canvas, bool a_renderBounds = true);
};

#endif // TEXTTOOL_H
