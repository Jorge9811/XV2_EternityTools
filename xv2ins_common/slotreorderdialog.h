#ifndef SLOTREORDERDIALOG_H
#define SLOTREORDERDIALOG_H

#include <QDialog>
#include "CharaListFile.h"

namespace Ui {
class SlotReorderDialog;
}

class SlotReorderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlotReorderDialog(CharaListSlot *cslot, size_t idx, QWidget *parent = 0);
    ~SlotReorderDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SlotReorderDialog *ui;
    CharaListSlot *cslot;
    size_t idx;

    static bool remember_before;
};

#endif // SLOTREORDERDIALOG_H
