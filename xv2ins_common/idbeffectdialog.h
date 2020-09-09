#ifndef IDBEFFECTDIALOG_H
#define IDBEFFECTDIALOG_H

#include <QDialog>
#include "Xenoverse2.h"

namespace Ui {
class IdbEffectDialog;
}

class IdbEffectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IdbEffectDialog(QWidget *parent, const IdbEffect &effect);
    ~IdbEffectDialog();

    inline IdbEffect &GetEffect() { return effect; }
    inline const IdbEffect &GetEffect() const { return effect; }

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::IdbEffectDialog *ui;

    IdbEffect effect;

    bool Validate();

    void EffectToGui();
    void GuiToEffect();
};

#endif // IDBEFFECTDIALOG_H
