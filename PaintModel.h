//
// The paint model keeps information about the available tools, the user
// and what is one the canvas
//

#ifndef PAINTMODEL_H
#define PAINTMODEL_H

#include "CanvasWidget.h"
#include "User.h"
#include "BaseTool.h"
#include <QVector>

class PaintModel : public QObject
{
    Q_OBJECT
public:
    // The constructor
    explicit PaintModel(QWidget *parent = nullptr);

    // The destructor
    ~PaintModel();

    // Getter for the canvas
    CanvasWidget* getCanvas();

    // set the color
    void setColor(QColor a_color, int a_which);
    void fillCanvas();
    void clearCanvas();

private:
    QVector<BaseTool*> m_tools; // The implemented tools
    User m_user;                // The user's chosen tool and colors
    CanvasWidget m_canvas;      // The canvas
};

#endif // PAINTMODEL_H
