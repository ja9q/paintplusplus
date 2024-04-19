#include "ResizeDialog.h"

#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>

ResizeDialog::ResizeDialog(QSize a_size, QWidget *parent): QDialog(parent) {
    QFormLayout *layout = new QFormLayout(this);

    QString labels[FIELDCNT] = {"Width: ", "Height: "};

    layout->addWidget(new QLabel("Enter new dimensions: ", this));

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

    QDialogButtonBox *buttons = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &ResizeDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &ResizeDialog::reject);

    //setLayout(layout);
}

QSize ResizeDialog::promptUser(QSize a_size, QWidget *a_parent) {
    ResizeDialog *temp = new ResizeDialog(a_size, a_parent);

    const int ret = temp->exec();

    if (ret) {
        return QSize(temp->m_fields[WIDTH]->value(),temp->m_fields[HEIGHT]->value());
    }

    return QSize();

}
