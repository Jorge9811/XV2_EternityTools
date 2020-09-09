#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "Xenoverse2.h"
#include "Xv2SavFile.h"
#include "X2mFile.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    bool Initialize();

protected:

    void closeEvent(QCloseEvent *event);

private slots:
    void on_x2mButton_clicked();

    void on_savButton_clicked();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;

    void GameRequirements();
    bool ProcessShutdown();

    bool X2mSlotToCacSlot(XV2Cac *cac, X2mFile *x2m, uint32_t x2m_race, uint32_t cac_slot, uint32_t x2m_slot, bool basic,
                          bool body_components, bool keep_race, bool costumes, bool skills, bool stats,
                          bool &face_base_set, bool &face_forehead_set, bool &hair_set, bool &eyes_set,
                          bool &nose_set, bool &ears_set);
};

#endif // DIALOG_H
