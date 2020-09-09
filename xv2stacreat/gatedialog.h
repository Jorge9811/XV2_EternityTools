#ifndef GATEDIALOG_H
#define GATEDIALOG_H

#include <QDialog>
#include "Xv2StageDefFile.h"

namespace Ui {
class GateDialog;
}

class GateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GateDialog(Xv2StageGate *gate, QWidget *parent = 0);
    ~GateDialog();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::GateDialog *ui;
    Xv2StageGate *gate;

    void GateToGui(const Xv2StageGate &gate);
    void GuiToGate(Xv2StageGate &gate);
};

#endif // GATEDIALOG_H
