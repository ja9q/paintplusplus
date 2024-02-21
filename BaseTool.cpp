#include "BaseTool.h"

BaseTool::BaseTool() {}


BaseTool::BaseTool(QString a_name, QVector<int> a_properties):
    m_name(a_name),
    m_properties(a_properties)
{}

QVector<int> BaseTool::getProperties() const {
    return m_properties;
}


void BaseTool::addProperties(QVector<int> a_extraProperties) {
    m_properties.append(a_extraProperties);
}
