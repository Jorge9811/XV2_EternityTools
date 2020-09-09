#ifndef REQUERIMENTSDIALOG_H
#define REQUERIMENTSDIALOG_H

#include <QDialog>

namespace Ui {
class RequerimentsDialog;
}

class RequerimentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RequerimentsDialog(const QString &game_exe, QWidget *parent = 0);
    ~RequerimentsDialog();

    inline QString &GetGameExe() { return game_exe; }    

private slots:
    void on_gameButton_clicked();    

    void on_buttonBox_accepted();

private:
    Ui::RequerimentsDialog *ui;

    QString game_exe;    
};

#endif // REQUERIMENTSDIALOG_H
