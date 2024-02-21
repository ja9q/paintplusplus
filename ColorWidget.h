//
// The ColorWidget is a GUI element that allows user to choose
// a color either by using the square color wheel or entering the RGB values.
//

#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QLineEdit>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = nullptr);

public slots:
    void updateColor();

signals:
    void valueChanged(QColor a_newColor);

private:
    QLineEdit* m_rgbEdit[3];
    QColor getColor();

};

#endif // COLORWIDGET_H
