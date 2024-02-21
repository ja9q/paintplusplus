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

    static const int MOD_SIZE = 0;
    static const int MOD_OPACITY = 1;
    static const int MOD_ANTIALIAS = 2;

    BaseTool();

    BaseTool(QString a_name, QVector<int> a_properties);

    QVector<int> getProperties() const;

    void setProperty(const int a_propId, const int a_newValue);

    virtual int processClick(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) = 0;

    virtual int processDrag(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2) = 0;

protected:
    void addProperties(QVector<int> a_extraProperties);

private:
    QString m_name;

    // List of properties that are processed to turn in a unique settings
    QVector<int> m_properties;

};

#endif // BASETOOL_H
