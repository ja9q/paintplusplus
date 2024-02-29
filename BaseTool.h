//
// Base class for all tools. It is abstract because the implementation
// of click and drag vary wildly among the tools.
//

#ifndef BASETOOL_H
#define BASETOOL_H

#include <QIcon>
#include <QString>

class BaseTool
{
public:



    // Default constructor
    BaseTool();

    // Parametric constructor
    BaseTool(QString a_name, QVector<int> a_properties);

    // Getter for properties list
    QVector<int> getProperties() const;

    // Modify a tool's property (e.g. size, opacity)
    virtual void setProperty(const int a_propId, const int a_newValue) = 0;

    // React to a click on the canvas
    virtual int processClick(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) = 0;

    // React to a drag on the canvas
    virtual int processDrag(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) = 0;

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
