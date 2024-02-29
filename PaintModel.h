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

    // Getter for the current tool
    BaseTool* getCurrentTool() const;

    // set the color
    void setColor(QColor a_color, int a_which);
    void fillCanvas();
    void clearCanvas();

public slots:
    void updateToolSetting(const int a_settingid, const int a_newValue);

private:
    QVector<BaseTool*> m_tools; // The implemented tools
    User m_user;                // The user's chosen tool and colors
    CanvasWidget m_canvas;      // The canvas
};

#endif // PAINTMODEL_H
