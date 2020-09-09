#ifndef X2MCONVDIALOG_H
#define X2MCONVDIALOG_H

#include <QDialog>

namespace Ui {
class X2mConvDialog;
}

class X2mConvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit X2mConvDialog(QWidget *parent = 0);
    ~X2mConvDialog();

    QString GetCode() const;
    int GetGuidMode() const;
    bool UseCssVoice() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_codeButton_clicked();

private:
    Ui::X2mConvDialog *ui;
};

#endif // X2MCONVDIALOG_H
