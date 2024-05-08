//
// Implementation for the ResizeDialog class
//

#include "ResizeDialog.h"

#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>

/**/
/*
ResizeDialog::ResizeDialog(QSize a_size, QWidget *parent)

NAME

    ResizeDialog::ResizeDialog(a_size, QWidget *parent) - parametric constructor

SYNOPSIS

    ResizeDialog::ResizeDialog(QSize a_size, QWidget *parent);
        a_size --> the initial size of the canvas; displayed as the default values of the fields
        parent --> the parent widget

DESCRIPTION

    Constructs the resize dialog, including the dialog format

RETURNS

    The constructed ResizeDialog

*/
/**/
ResizeDialog::ResizeDialog(QSize a_size, QWidget *parent): QDialog(parent) {
    QFormLayout *layout = new QFormLayout(this);

    QString labels[FIELDCNT] = {"Width: ", "Height: "};

    // add the label for the entire form
    layout->addWidget(new QLabel("Enter new dimensions: ", this));

    // add in the rows for the height and width
    for (int i = 0; i < FIELDCNT; i++) {
        m_fields[i] = new QSpinBox(this);
        m_fields[i]->setMinimum(1);
        m_fields[i]->setMaximum(9999);
        if (i == WIDTH) {
            m_fields[i]->setValue(a_size.width());
        } else {
            m_fields[i]->setValue(a_size.height());
        }
        m_fields[i]->setSuffix("px");
        layout->addRow(new QLabel(labels[i], this), m_fields[i]);
    }

    // add the buttons to the dialog
    QDialogButtonBox *buttons = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->addWidget(buttons);

    // connect the buttons to the rest of the widget
    connect(buttons, &QDialogButtonBox::accepted, this, &ResizeDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &ResizeDialog::reject);
}

/**/
/*
QSize ResizeDialog::promptUser(QSize a_size, QWidget *a_parent)

NAME

    ResizeDialog::promptUser(QSize a_size, QWidget *a_parent) - produce a resize dialog and return the results

SYNOPSIS

    QSize ResizeDialog::promptUser(QSize a_size, QWidget *a_parent);
        a_size --> the initial size of the canvas;
        a_parent --> the window that the popup will appear from

DESCRIPTION

    a static functiont that creates and displays a resize dialog

RETURNS

    The size that the user enters; If the user cancels the dialog, then return the original size.

*/
/**/
QSize ResizeDialog::promptUser(QSize a_size, QWidget *a_parent) {
    ResizeDialog *temp = new ResizeDialog(a_size, a_parent);

    const int ret = temp->exec();

    if (ret) {
        return QSize(temp->m_fields[WIDTH]->value(),temp->m_fields[HEIGHT]->value());
    }

    return a_size;

}
