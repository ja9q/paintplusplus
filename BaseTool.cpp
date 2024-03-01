//
// Implementation for the BaseTool class
//

#include "BaseTool.h"
#include <QDebug>

// Base tool default constructor
BaseTool::BaseTool() {}

// Parametric constructor for the base tool; called in the constructors of
// derived classes
BaseTool::BaseTool(QString a_name, QVector<int> a_properties):
    m_name(a_name),
    m_properties(a_properties)
{}

/**/
/*
QVector<int> BaseTool::getProperties() const

NAME

    BaseTool::getProperties() - gets the tool's properties

SYNOPSIS

    QVector<int> BaseTool::getProperties() const;

DESCRIPTION

    This function returns the tools properties, which are used to parsed to
    create tool settings in the GUI

RETURNS

    Returns the tool's properties a QVector of integers that are like
    id's to certain

*/
/**/
QVector<int> BaseTool::getProperties() const {
    return m_properties;
}

/**/
/*
void BaseTool::addProperties(QVector<int> a_extraProperties)

NAME

    BaseTool::addProperties(QVector<int> a_extraProperties) - adds new properties

SYNOPSIS

    void BaseTool::addProperties(QVector<int> a_extraProperties)
        a_extraProperties --> a list of new properties to be added

DESCRIPTION

    Adds new properties to the properties list.

RETURNS

    None

*/
/**/
void BaseTool::addProperties(QVector<int> a_extraProperties) {
    m_properties.append(a_extraProperties);
}
