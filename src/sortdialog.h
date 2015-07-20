#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class SortDialog;
}

class SortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SortDialog(QWidget *parent = 0);
    ~SortDialog();

    void setColumnRange(QChar first, QChar last);

    QComboBox *primaryColumnCombo;
    QComboBox *primaryOrderCombo;
    QComboBox *secondaryColumnCombo;
    QComboBox *secondaryOrderCombo;
    QComboBox *tertiaryColumnCombo;
    QComboBox *tertiaryOrderCombo;

private slots:
    void showMore(bool more);

private:
    Ui::SortDialog *ui;
};

#endif // SORTDIALOG_H
