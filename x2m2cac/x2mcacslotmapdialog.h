#ifndef X2MCACSLOTMAPDIALOG_H
#define X2MCACSLOTMAPDIALOG_H

#include <QDialog>
#include <map>
#include "X2mFile.h"

namespace Ui {
class X2mCacSlotMapDialog;
}

class X2mCacSlotMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit X2mCacSlotMapDialog(X2mFile *x2m, std::map<uint32_t, uint32_t> *costume_maps, QWidget *parent = 0);
    ~X2mCacSlotMapDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::X2mCacSlotMapDialog *ui;

    std::map<uint32_t, uint32_t> *costume_maps;
    int num_slots;
};

#endif // X2MCACSLOTMAPDIALOG_H
