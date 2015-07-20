#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    connect(ui->lineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableFindButton(const QString &)));
    connect(ui->findButton, SIGNAL(clicked()),
            this, SLOT(findClicked()));
    connect(ui->closeButton, SIGNAL(clicked()),
            this, SLOT(close()));

}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::findClicked()
{
    QString text = ui->lineEdit->text();
    Qt::CaseSensitivity cs =
            ui->caseCheckBox->isChecked() ? Qt::CaseSensitive
                                          : Qt::CaseInsensitive;
    if (ui->backwardCheckBox->isChecked()){
        emit findPrevious(text, cs);
    }
    else{
        emit findNext(text, cs);
    }
}

void FindDialog::enableFindButton(const QString &text)
{
    ui->findButton->setEnabled(!text.isEmpty());
}
