#include "gotocelldialog.h"
#include "ui_gotocelldialog.h"

GoToCellDialog::GoToCellDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToCellDialog)
{
    ui->setupUi(this);

    // Initial point for public call
    lineEdit = ui->lineEdit;

    QRegExp reg("[A-Za-z][1-9][0-9]{0,2}");
    ui->lineEdit->setValidator(new QRegExpValidator(reg, this));

    connect(ui->okButton, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(ui->cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()));

}

GoToCellDialog::~GoToCellDialog()
{
    delete ui;
}

void GoToCellDialog::on_lineEdit_textChanged()
{
    ui->okButton->setEnabled(ui->lineEdit->hasAcceptableInput());
}
