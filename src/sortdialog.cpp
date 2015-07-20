#include <QtGui>

#include "sortdialog.h"
#include "ui_sortdialog.h"

SortDialog::SortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SortDialog)
{
    ui->setupUi(this);

    primaryColumnCombo = ui->primaryColumnCombo;
    primaryOrderCombo = ui->primaryOrderCombo;
    secondaryColumnCombo = ui->secondaryColumncombo;
    secondaryOrderCombo = ui->secondaryOrderCombo;
    tertiaryColumnCombo = ui->tertiaryColumnCombo;
    tertiaryOrderCombo = ui->tertiaryOrderCombo;

    ui->secondaryGroup->hide();
    ui->tertiaryGroup->hide();
    this->setFixedSize(301, 133);

    setColumnRange('A', 'Z');

    connect(ui->moreButton, SIGNAL(toggled(bool)), this, SLOT(showMore(bool)));

}

SortDialog::~SortDialog()
{
    delete ui;
}

void SortDialog::showMore(bool more){
    if(more)
        this->setFixedSize(301, 359);
    else
        this->setFixedSize(301, 133);
}

void SortDialog::setColumnRange(QChar first, QChar last)
{
    ui->primaryColumnCombo->clear();
    ui->secondaryColumncombo->clear();
    ui->tertiaryColumnCombo->clear();

    ui->secondaryColumncombo->addItem(tr("None"));
    ui->tertiaryColumnCombo->addItem(tr("None"));
    ui->primaryColumnCombo->setMinimumSize(
                ui->secondaryColumncombo->sizeHint());

    QChar ch = first;
    while(ch <= last){
        ui->primaryColumnCombo->addItem(QString(ch));
        ui->secondaryColumncombo->addItem(QString(ch));
        ui->tertiaryColumnCombo->addItem(QString(ch));
        ch = ch.unicode() + 1;
    }
}
