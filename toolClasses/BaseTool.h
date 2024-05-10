//
// Base class for all tools. It is abstract because the implementation
// of click and drag vary wildly among the tools.
//

#ifndef BASETOOL_H
#define BASETOOL_H

#include <QIcon>
#include <QString>
#include <QMouseEvent>

class BaseTool
{
public:
    // Parametric constructor
    BaseTool(QString a_name, QVector<int> a_properties);

    // Getter for name
    QString getName() const;

    // Getter for properties list
    QVector<int> getProperties() const;

    // Getter for a single property
    virtual int getProperty(const int a_propId) = 0;

    // Modify a tool's property (e.g. size, opacity)
    virtual int setProperty(const int a_propId, const int a_newValue) = 0;

    // (for tools with editable features) reset the editor
    virtual void resetEditor();

    // (for tools with editable features) get the edited image
    virtual QImage getEditable(QImage* a_canvas = NULL, const QColor a_color = QColor(), bool a_cuts = false);

    // (for tools with editable features) set the editable image
    virtual void setEditable(QImage a_image, QImage *a_canvas, QImage* a_tempCanvas);

    // React to a click on the canvas
    virtual int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) = 0;

    // React to a drag on the canvas
    virtual int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) = 0;

    // React to when the mouse is no longer clicking/dragging on the canvas
    virtual int processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a double click (only relevant to the polyline tool)
    virtual int processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

protected:
    // A list of possible tool properties a tool can have
    enum class ToolProperty {
        SIZE, OPACITY, ANTIALIAS, FILLTYPE, OUTLINE, MASKBACK, MASKCOLOR2,
        FONT_SIZE, FONT, TEXT_BOLD, TEXT_ITALIC, TEXT_UNDERLINE
    };

    // Add new properties
    void addProperties(QVector<int> a_extraProperties);

private:
    // The name of the tool
    QString m_name;

    // List of properties that are processed to turn in a unique settings
    QVector<int> m_properties;

};

#endif // BASETOOL_H
