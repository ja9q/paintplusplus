#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "BaseTool.h"

class DrawTool : public BaseTool
{
public:
    DrawTool();
    DrawTool(QString a_name, int a_color, QVector<int> a_moreProperties = {});

    int getColor() const;

    void setOpacity(const int a_newOpacity);

    int processClick(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2);
    int processDrag(QImage* a_canvas, const QPointF a_point, const QColor a_color1, const QColor a_color2);

private:
    int m_size;
    int m_color;
    int m_opacity;

    QPointF m_lastPoint;
};

#endif // DRAWTOOL_H
