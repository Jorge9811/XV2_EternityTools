#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>

#include "Xenoverse2.h"
#include "Xv2SavFile.h"
#include "X2mFile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_entriesComboBox_currentIndexChanged(int index);

    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_nameEdit_textEdited(const QString &arg1);

    void on_langComboBox_currentIndexChanged(int index);

    void on_copyButton_triggered(QAction *arg1);

    void on_infoCopyButton_triggered(QAction *arg1);

    void on_equipCopyButton_triggered(QAction *arg1);

    void on_statsCopyButton_triggered(QAction *arg1);

    void on_applyButton_clicked();

    void on_allLangCheck_clicked();

    void on_actionConvert2X2M_triggered();

    void on_actionExportCac_triggered();

    void on_skinColor1Button_clicked();

    void on_skinColor2Button_clicked();

    void on_skinColor3Button_clicked();

    void on_skinColor4Button_clicked();

    void on_hairColorButton_clicked();

    void on_eyesColorButton_clicked();

    void on_topColor1Button_clicked();

    void on_topColor2Button_clicked();

    void on_topColor3Button_clicked();

    void on_topColor4Button_clicked();

    void on_bottomColor1Button_clicked();

    void on_bottomColor2Button_clicked();

    void on_bottomColor3Button_clicked();

    void on_bottomColor4Button_clicked();

    void on_glovesColor1Button_clicked();

    void on_glovesColor2Button_clicked();

    void on_glovesColor3Button_clicked();

    void on_glovesColor4Button_clicked();

    void on_shoesColor1Button_clicked();

    void on_shoesColor2Button_clicked();

    void on_shoesColor3Button_clicked();

    void on_shoesColor4Button_clicked();

    void on_makeupColor1Button_clicked();

    void on_makeupColor2Button_clicked();

    void on_makeupColor3Button_clicked();

    void on_tabWidget_currentChanged(int index);

protected:

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    int prev_entry_index=-1;

    void GameRequirements();
    bool ProcessShutdown();

    void FillCmsComboBox();
    void FillTopComboBox();
    void FillBottomComboBox();
    void FillGlovesComboBox();
    void FillShoesComboBox();
    void FillAccesoryComboBox();
    void FillTalismanComboBox();
    void FillSuperComboBox(QComboBox *comboBox);
    void FillUltimateComboBox(QComboBox *comboBox);
    void FillEvasiveComboBox();
    void FillBlastComboBox();
    void FillAwakenComboBox();

    void SetComboBox(QComboBox *comboBox, int16_t id);

    void InfoToGui(const PalEntry &entry);
    void GuiToInfo(PalEntry &entry);

    void BcsColorToButton(const std::string &part, uint16_t color, uint16_t cms_entry, QPushButton *button);
    uint16_t ButtonToBcsColor(QPushButton *button);

    void EquipmentToGui(const PalEquipment &entry, uint16_t cms_entry);
    void GuiToEquipment(PalEquipment &entry);

    void StatsToGui(const PalStats &stats);
    void GuiToStats(PalStats &stats);

    void EntryToGui(const PalEntry &entry);
    void GuiToEntry(PalEntry &entry);

    bool AddNewName(PalEntry &entry);

    bool CacToInfo(const XV2Cac *cac, PalEntry &entry);
    bool CacToEquipment(const XV2Cac *cac, uint32_t set, PalEquipment &equipment);
    bool CacToStats(const XV2Cac *cac, uint32_t set, PalStats &stats);
    bool CacToEntry(const XV2Cac *cac, uint32_t set, PalEntry &entry);

    bool InfoToCac(const PalEntry &entry, XV2Cac *cac);
    bool EquipmentToCac(const PalEquipment &equipment, XV2Cac *cac, uint32_t set, bool do_looks, bool do_equip);
    bool StatsToCac(const PalStats &stats, XV2Cac *cac, uint32_t set, bool do_stats, bool do_skills);
    bool EntryToCac(const PalEntry &entry, XV2Cac *cac, uint32_t set, bool do_info, bool do_looks, bool do_equip, bool do_stats, bool do_skills);

    bool EntryToX2m(const PalEntry &entry, const QString &file, const QString &code, const uint8_t *guid, bool css_voice);

    bool X2mToInfo(X2mFile *x2m, PalEntry &entry);
    bool X2mToEquipment(X2mFile *x2m, uint32_t slot_idx, PalEquipment &equipment, uint32_t current_race);
    bool X2mToStats(X2mFile *x2m, uint32_t slot_idx, PalStats &stats);
    bool X2mToEntry(X2mFile *x2m, uint32_t slot_idx, PalEntry &entry);

    void Load();

    bool GetBcsColorFromDialog(const std::string &part, uint16_t current_color, uint16_t cms_entry, uint16_t *out_color);
};

#endif // MAINWINDOW_H
