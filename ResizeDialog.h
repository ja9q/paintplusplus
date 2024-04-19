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
    ResizeDialog(QSize a_size, QWidget *parent = nullptr);

    static QSize promptUser(QSize a_size, QWidget *a_parent);

private:
    const static int WIDTH = 0;
    const static int HEIGHT = 1;
    const static int FIELDCNT = 2;

    QSpinBox* m_fields[FIELDCNT];
};

#endif // RESIZEDIALOG_H
