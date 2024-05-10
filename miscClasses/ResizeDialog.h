//
// A dialog triggered when the user resized the canvas
//

#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class ResizeDialog : public QDialog
{
    Q_OBJECT
public:
    // constructor
    ResizeDialog(QSize a_size, QWidget *parent = nullptr);

    // create a dialog for the user and return the sizes they submit
    static QSize promptUser(QSize a_size, QWidget *a_parent);

private:
    // the indices for the width and height
    const static int WIDTH = 0;
    const static int HEIGHT = 1;

    // the number of fields the user has to fill out
    const static int FIELDCNT = 2;

    // the form fields for the height and width
    QSpinBox* m_fields[FIELDCNT];
};

#endif // RESIZEDIALOG_H
