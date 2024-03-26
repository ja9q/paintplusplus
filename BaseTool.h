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

    virtual int getProperty(const int a_propId) = 0;

    // Modify a tool's property (e.g. size, opacity)
    virtual int setProperty(const int a_propId, const int a_newValue) = 0;

    virtual void resetEditor();

    // React to a click on the canvas
    virtual int processClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) = 0;

    // React to a drag on the canvas
    virtual int processDrag(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2) = 0;

    // React to when the mouse is no longer clicking/dragging on the canvas
    virtual int processMouseRelease(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

    // react to a double click (only relevant to the polyline tool)
    virtual int processDoubleClick(QImage* a_canvas, QImage* a_tempCanvas, const QMouseEvent* a_event, const QColor a_color1, const QColor a_color2);

protected:
    // Add new properties
    void addProperties(QVector<int> a_extraProperties);

private:
    // The name of the tool
    QString m_name;

    // List of properties that are processed to turn in a unique settings
    QVector<int> m_properties;

};

#endif // BASETOOL_H
