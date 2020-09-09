#ifndef SLOTEDITDIALOG_H
#define SLOTEDITDIALOG_H

#include <QDialog>
#include "Xenoverse2.h"


namespace Ui {
class SlotEditDialog;
}

class SlotEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlotEditDialog(CharaListSlotEntry *entry, EmbFile *edit_emb, QWidget *parent = 0);
    ~SlotEditDialog();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::SlotEditDialog *ui;

    CharaListSlotEntry *entry;
    EmbFile *edit_emb;    

    bool GuiToEntry();
    void EntryToGui();
};

#endif // SLOTEDITDIALOG_H
