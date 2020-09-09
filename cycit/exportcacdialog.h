#ifndef EXPORTCACDIALOG_H
#define EXPORTCACDIALOG_H

#include <QDialog>

namespace Ui {
class ExportCacDialog;
}

class ExportCacDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportCacDialog(QWidget *parent = 0);
    ~ExportCacDialog();

    bool DoInfo();
    bool DoLooks();
    bool DoEquip();
    bool DoStats();
    bool DoSkills();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExportCacDialog *ui;
};

#endif // EXPORTCACDIALOG_H
