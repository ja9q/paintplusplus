#include "TextTool.h"

#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QApplication>



TextTool::TextTool(QString a_name, CanvasWidget* a_canvas, QVector<int> a_moreProperties):
    BaseTool(a_name, {(int)ToolProperty::FONT_SIZE, (int)ToolProperty::FONT, (int)ToolProperty::TEXT_BOLD, (int)ToolProperty::TEXT_ITALIC,
                        (int)ToolProperty::TEXT_UNDERLINE}),
    m_textbox(), m_bounds(), m_timer(), m_canvas(a_canvas), m_isActive(false), m_isOpaque(false), m_bgColor(QColor(Qt::white))
{
    FONTS = QFontDatabase::families().toList();

    addProperties(a_moreProperties);

    m_textbox.setFrameStyle(QFrame::NoFrame);

    QPalette palette = m_textbox.palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    m_textbox.setPalette(palette);

    m_textbox.setFontFamily(FONTS[0]);
    m_textbox.setFontPointSize(12);
    m_textbox.resize(500,30);
    m_textbox.setAttribute(Qt::WA_TranslucentBackground);
    m_textbox.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QWidget::connect(&m_textbox, &QTextEdit::textChanged, &m_textbox, [=](){drawText(a_canvas->getTempCanvas());});
    QWidget::connect(&m_textbox, &QTextEdit::cursorPositionChanged, &m_textbox, [=](){drawText(a_canvas->getTempCanvas());});
    QWidget::connect(&m_timer, &QTimer::timeout, &m_textbox, [=](){drawText(a_canvas->getTempCanvas());});


}

int TextTool::getProperty(const int a_propId) {



    switch ((ToolProperty)a_propId) {
    case ToolProperty::FONT_SIZE:
        return m_textbox.fontPointSize();
        break;
    case ToolProperty::FONT:
        return FONTS.indexOf(m_textbox.fontFamily());
        break;
    case ToolProperty::TEXT_BOLD:
        return (m_textbox.fontWeight() == QFont::Bold);
        break;
    case ToolProperty::TEXT_ITALIC:
        return m_textbox.fontItalic();
        break;
    case ToolProperty::TEXT_UNDERLINE:
        return m_textbox.fontUnderline();
        break;
    default:
        break;
    }


    return -1;
}

// Modify a tool's property (e.g. size, opacity)
int TextTool::setProperty(const int a_propId, const int a_newValue) {

    QTextCursor temp = m_textbox.textCursor();
    m_textbox.selectAll();

    switch ((ToolProperty)a_propId) {
    case ToolProperty::FONT_SIZE:
        m_textbox.setFontPointSize(a_newValue);
        break;
    case ToolProperty::FONT:
        m_textbox.setFontFamily(FONTS[a_newValue]);
        break;
    case ToolProperty::TEXT_BOLD:
        if (a_newValue == 0) {
            m_textbox.setFontWeight(QFont::Normal);
        } else {
            m_textbox.setFontWeight(QFont::Bold);
        }
        break;
    case ToolProperty::TEXT_ITALIC:
        m_textbox.setFontItalic(a_newValue);
        break;
    case ToolProperty::TEXT_UNDERLINE:
        m_textbox.setFontUnderline(a_newValue);
        break;
    default:
        return -1;
        break;
    }

    m_textbox.setTextCursor(temp);

    return 0;


}

void TextTool::resetEditor() {
    m_timer.stop();
    m_isActive = false;
    m_textbox.setText("");
    m_textbox.resize(500,m_textbox.fontPointSize()*2.5);
    m_textbox.releaseKeyboard();
    m_bounds.reset();
}

// React to a click on the canvas
int TextTool::processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {

    if (m_bgColor != a_color2) {
        m_bgColor = a_color2;
    }

    if (m_bounds.isEditing()) {
        m_bounds.setPrevPoint(a_event->position().toPoint());
        m_bounds.identifyEdit();
    }


    if (m_bounds.getEditMode() == Editable::EditMode::END) {
        m_bounds.setEditMode(Editable::EditMode::NONE);
    }

    return 0;
}

// React to a drag on the canvas
int TextTool::processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    (void) a_canvas;
    (void) a_color1;

    // clear the temp canvas from any previous marks (e.g. the previous rectangle)
    a_tempCanvas->fill(Qt::transparent);
    // draw out the shape if the user is still dragging it for the first time (i.e. not in edit mode)
    if (m_bounds.isEditing()) {
        m_bounds.processEdit(a_event);

        if (m_bounds.getEditMode() == Editable::EditMode::SCALE) {
            QPolygon newSize = m_bounds.getShape();
            newSize = QTransform().scale(m_bounds.getScale().x(), m_bounds.getScale().y()).map(newSize);

            m_textbox.resize(newSize.boundingRect().size());
        }

        drawText(a_tempCanvas);
    }

    return 0;
}

// React to when the mouse is no longer clicking/dragging on the canvas
int TextTool::processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {
    if (m_bounds.getEditMode() == Editable::EditMode::END) {
        m_bounds.setEditMode(Editable::EditMode::NONE);
        // if the polygon is not empty and the tool is not editing, turn on editing mode + draw the bounding rectangle + define the selected area
    }else if (!m_isActive && !m_bounds.isEditing()) {
        m_bounds.setShape(QPolygon(QRect(a_event->position().toPoint(), m_textbox.size())));
        m_bounds.initBoundingRect();
        m_timer.start(500);
        m_isActive = true;
        m_textbox.grabKeyboard();
        m_bounds.setIsEditing(true);

        drawText(a_tempCanvas);

    }
    else if (m_bounds.isEditing()) {
        m_bounds.setEditMode(Editable::EditMode::NONE);
    }

    return 0;
}

// react to a double click (only relevant to the polyline tool)
int TextTool::processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) {

    a_tempCanvas->fill(Qt::transparent);

    QTextCursor deselectAll = m_textbox.textCursor();
    deselectAll.setPosition(0);

    m_textbox.setTextCursor(deselectAll);
    m_textbox.setCursorWidth(0);
    drawText(a_tempCanvas, false);


    m_textbox.setCursorWidth(1);

    // call the original BaseTool's mouseRelease to copy the contents from the temporary canvas to the original canvas
    BaseTool::processMouseRelease(a_canvas, a_tempCanvas, a_event, a_color1, a_color2);

    resetEditor();

    return 1;
}

void TextTool::drawText(QImage* a_canvas, bool a_renderBounds) {
    if (m_isActive && !m_bounds.getShape().isEmpty()) {
        QImage box = QImage(m_textbox.size(), QImage::Format_ARGB32);

        box.fill(Qt::transparent);
        if (m_isOpaque) {
            box.fill(m_bgColor);
        }

        QPainter boxPainter(&box);
        m_textbox.render(&boxPainter);

        if (m_textbox.cursorWidth() != 0) {
            QRect fakeCursor = QRect(m_textbox.cursorRect());
            fakeCursor.setWidth(1);
            boxPainter.fillRect(fakeCursor, m_textbox.textColor());
        }

        boxPainter.end();

        box = box.transformed(QTransform().rotate(m_bounds.getRotation()));

        a_canvas->fill(Qt::transparent);
        QPainter painter(a_canvas);
        painter.drawPixmap(m_bounds.getTransformedBoundRect().boundingRect().topLeft(), QPixmap::fromImage(box));



        painter.end();

        if (a_renderBounds) {
            m_bounds.drawBoundingRect(a_canvas);
        }

        m_canvas->repaint();

    }
}

void TextTool::adjustSize() {

}
