#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
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

    bool Initialize();

    void PlayTtbAudio(const std::string &event_name, bool japanese);

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_guidButton_clicked();

    void on_charaNameLangComboBox_currentIndexChanged(int index);

    void on_charaNameEdit_textEdited(const QString &arg1);

    void on_charNameCopyButton_clicked();

    void on_multNamesLangComboBox_currentIndexChanged(int index);

    void on_multNamesEdit_textEdited(const QString &arg1);

    void on_multNamesCheck_clicked();

    void on_multNamesComboBox_currentIndexChanged(int index);

    void on_multNamesAddButton_clicked();

    void on_multNamesRemoveButton_clicked();

    void on_multNamesCopyButton_clicked();

    void on_bodyShapeCheck_clicked();

    void on_charNameClearButon_triggered(QAction *arg1);

    void on_charaFilesButton_clicked();

    void on_selPortButton_triggered(QAction *arg1);

    void on_btlPortButton_triggered(QAction *arg1);

    void on_addEditButton_clicked();

    void on_cssSoundButton_clicked();

    void on_slotsComboBox_currentIndexChanged(int index);

    void on_slotsAddButton_clicked();

    void on_costumeNameLangComboBox_currentIndexChanged(int index);

    void on_costumeNameEdit_textEdited(const QString &arg1);

    void on_slotsRemoveButton_clicked();

    void on_slotsCopyButton_clicked();

    void on_voice1Check_clicked();

    void on_voice2Check_clicked();

    void on_costumeNameClearButton_triggered(QAction *arg1);

    void on_cmsCopyButton_triggered(QAction *arg1);

    void on_cusSlotsComboBox_currentIndexChanged(int index);

    void on_cusCopyButton_triggered(QAction *arg1);        

    void on_csoCheck_clicked();

    void on_csoCostComboBox_currentIndexChanged(int index);

    void on_csoAddButton_clicked();

    void on_csoRemoveButton_clicked();

    void on_csoCopyButton_triggered(QAction *arg1);

    void on_pscCheck_clicked();

    void on_pscCostComboBox_currentIndexChanged(int index);

    void on_pscAddButton_clicked();

    void on_pscRemoveButton_clicked();

    void on_pscCopyButton_triggered(QAction *arg1);

    void on_pscSuperSoulButton_clicked();

    void on_pscSuperSoulEdit_textChanged(const QString &arg1);

    void on_aurCheck_clicked();

    void on_aurCostComboBox_currentIndexChanged(int index);

    void on_aurAddButton_clicked();

    void on_aurRemoveButton_clicked();

    void on_aurCopyButton_triggered(QAction *arg1);

    void on_auraCustomCheck_clicked();

    void on_sevCheck_clicked();

    void on_sevModeComboBox_currentIndexChanged(int index);

    void on_sevHLCostComboBox_currentIndexChanged(int index);

    void on_sevHLAddButton_clicked();

    void on_sevHLRemoveButton_clicked();

    void on_sevLLCostComboBox_currentIndexChanged(int index);

    void on_sevLLAddButton_clicked();

    void on_sevLLRemoveButton_clicked();

    void on_sevLLCharComboBox_currentIndexChanged(int index);

    void on_sevLLCharAddButton_clicked();

    void on_sevLLCharRemoveButton_clicked();

    void on_sevLLEventsComboBox_currentIndexChanged(int index);

    void on_sevLLEventsAddButton_clicked();

    void on_sevLLEventsRemoveButton_clicked();

    void on_sevLLEvEntriesComboBox_currentIndexChanged(int index);

    void on_sevLLEvEntriesAddButton_clicked();

    void on_sevLLEvEntriesRemoveButton_clicked();

    void on_sevLLCopyButton_triggered(QAction *arg1);

    void on_cmlCheck_clicked();

    void on_cmlCostComboBox_currentIndexChanged(int index);

    void on_cmlAddButton_clicked();

    void on_cmlRemoveButton_clicked();

    void on_cmlCopyButton_triggered(QAction *arg1);

    void on_hciCheck_clicked();

    void on_hciEntryComboBox_currentIndexChanged(int index);

    void on_hciAddButton_clicked();

    void on_hciRemoveButton_clicked();

    void on_hciCopyButton_triggered(QAction *arg1);

    void on_hciGetEmbIndexButton_clicked();

    void on_cusDependsComboBox_currentIndexChanged(int index);

    void on_cusDependsAddButton_clicked();

    void on_cusDependsRemoveButton_clicked();

    void on_cusDependsUpdateButton_clicked();

    void on_voice1ListenButton_clicked();

    void on_voice2ListenButton_clicked();

    void on_sevListenButton_clicked();

    void on_sevListenResponseButton_clicked();

    void on_sevLLAudioComboBox_currentIndexChanged(int index);

    void on_sevLLAudioRespComboBox_currentIndexChanged(int index);

    void on_ttbCheck_clicked();

    void on_ttbModeComboBox_currentIndexChanged(int index);

    void on_ttbLLNumActorsComboBox_currentIndexChanged(int index);

    void on_ttbHLCostComboBox_currentIndexChanged(int index);

    void on_ttbLLCostComboBox_currentIndexChanged(int index);

    void on_ttbHLAddButton_clicked();

    void on_ttbLLAddButton_clicked();

    void on_ttbHLRemoveButton_clicked();

    void on_ttbLLRemoveButton_clicked();

    void on_ttbLLActor1Button_clicked();

    void on_ttbLLActor2Button_clicked();

    void on_ttbLLActor3Button_clicked();

    void on_ttbLLActor4Button_clicked();

    void on_ttbLLActor5Button_clicked();

    void on_ttbLLCopyButton_triggered(QAction *arg1);

    void on_ttcCheck_clicked();

    void on_ttcModeComboBox_currentIndexChanged(int index);

    void on_ttcHLCostComboBox_currentIndexChanged(int index);

    void on_ttcHLAddButton_clicked();

    void on_ttcHLRemoveButton_clicked();

    void on_ttcLLCostComboBox_currentIndexChanged(int index);

    void on_ttcLLAddButton_clicked();

    void on_ttcLLRemoveButton_clicked();

    void on_ttcLLListenButton_clicked();

    void on_ttcLLSubButton_clicked();

    void on_ttcLLVerifyButton_triggered(QAction *arg1);

    void on_ttcLLCopyButton_triggered(QAction *arg1);

    void on_ttcLLCopyFilesButton_triggered(QAction *arg1);

    void on_cncCheck_clicked();

    void on_cncComboBox_currentIndexChanged(int index);

    void on_cncAddButton_clicked();

    void on_cncRemoveButton_clicked();

    void on_cncCopyButton_triggered(QAction *arg1);

    void on_guidCopyButton_clicked();

    void on_vfxCopyButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    X2mFile *x2m = nullptr;
    std::string entry_name;

    int prev_mult_names_index=-1;
    int prev_slots_index=-1;
    int prev_cus_slots_index=-1;
    int prev_cso_cost_index=-1;
    int prev_psc_cost_index=-1;
    int prev_aur_cost_index=-1;
    int prev_sevhl_cost_index=-1;
    int prev_sevll_cost_index=-1;
    int prev_sevll_char_index=-1;
    int prev_sevll_ev_index=-1;
    int prev_sevll_ev_ent_index=-1;
    int prev_cml_cost_index=-1;
    int prev_hci_ent_index=-1;
    int prev_ttbhl_cost_index=-1;
    int prev_ttbll_cost_index=-1;
    int prev_ttchl_cost_index=-1;
    int prev_ttcll_cost_index=-1;
    int prev_cnc_index=-1;

    bool audio_playing=false;
    bool show_wait_onsave=false;

    bool ProcessShutdown();

    void ProcessX2m();
    bool Validate();
    uint8_t *GetGameSelPortrait(const QString &name, size_t *psize);
    uint8_t *GetGameBtlPortrait(const QString &file, size_t *psize);
    bool Build();

    void NameEntryToGui(const X2mCharaName &entry);
    void GuiToNameEntry(X2mCharaName &entry);

    void SetCssAudio(const QString &dir);

    void VoiceToComboBox(QComboBox *comboBox, QCheckBox *check, QLineEdit *edit, const std::string &voice_name);
    void ComboBoxToVoice(QComboBox *comboBox, QCheckBox *check, std::string &voice_name);
    void SlotEntryToGui(const X2mSlotEntry &entry);
    void GuiToSlotEntry(X2mSlotEntry &entry);

    void CmsEntryToGui(const CmsEntryXV2 &entry);
    void GuiToCmsEntry(CmsEntryXV2 &entry);

    void SkillToComboBox(uint16_t id, QComboBox *comboBox, const QString &name);
    void SkillSetToGui(const CusSkillSet &entry);
    void ComboBoxToSkill(QComboBox *comboBox, uint16_t *skill);
    void GuiToSkillSet(CusSkillSet &entry);

    void FillSuperComboBox(QComboBox *comboBox);
    void FillUltimateComboBox(QComboBox *comboBox);
    void FillEvasiveComboBox(QComboBox *comboBox);
    void FillBlastComboBox(QComboBox *comboBox);
    void FillAwakenComboBox(QComboBox *comboBox);

    void AddCustomSkill(QComboBox *comboBox, uint16_t id, const QString &name);
    void RemoveSkill(QComboBox *comboBox, uint16_t id);

    bool FindCustomSkillType(uint16_t id, X2mSkillType *type);

    void FillCustomSuper(QComboBox *comboBox);
    void FillCustomUltimate(QComboBox *comboBox);
    void FillCustomEvasive(QComboBox *comboBox);
    void FillCustomBlast(QComboBox *comboBox);
    void FillCustomAwaken(QComboBox *comboBox);

    void EraseCustomSkills(QComboBox *comboBox);

    int LinkOrEmbed(X2mFile *skill_x2m);

    void CsoEntryToGui(const CsoEntry& entry);
    void GuiToCsoEntry(CsoEntry &entry);

    void PscEntryToGui(const PscSpecEntry &entry);
    void GuiToPscEntry(PscSpecEntry &entry);

    void AurEntryToGui(const AurCharaLink &entry, const AurAura &aura);
    void GuiToAurEntry(AurCharaLink &entry, AurAura &aura);

    void FillSevHLComboBox();

    void SevEntryHLToGui(const SevEntryHL &entry);
    void GuiToSevEntryHL(SevEntryHL &entry);

    void SevEventEntryToGui(const SevEventEntry &entry);
    void GuiToSevEventEntry(SevEventEntry &entry);

    void SevEventToGui(const SevEvent &entry);
    void GuiToSevEvent(SevEvent &entry);

    void SevEventsLLToGui(const SevEventsLL &entry);
    void GuiToSevEventsLL(SevEventsLL &entry);

    void SevEntryLLToGui(const SevEntryLL &entry);
    void GuiToSevEntryLL(SevEntryLL &entry);

    void RebuildSevEnable(bool enable_hl, bool enable_ll);

    void CmlEntryToGui(const CmlEntry &entry);
    void GuiToCmlEntry(CmlEntry &entry);

    void HciEntryToGui(const HciEntry &entry);
    void GuiToHciEntry(HciEntry &entry);

    void PlayAudio(HcaFile *hca);
    void PlayAudio(AcbFile *acb, AwbFile *awb, uint32_t cue_id);

    void PlayCssAudio(QLineEdit *cue_id_edit, QCheckBox *custom_check, QComboBox *custom_combobox);
    void PlaySevAudio(QLineEdit *cue_id_edit, QLineEdit *file_id_edit, QComboBox *custom_cb);
    void PlayTtcAudio();

    void FillTtbHLComboBox();

    void TtbEntryHLToGui(const TtbEntryHL &entry);
    void GuiToTtbEntryHL(TtbEntryHL &entry);

    void TtbEntryLLToGui(const TtbEntryLL &entry, bool dummy);
    void GuiToTtbEntryLL(TtbEntryLL &entry);

    void RebuildTtbEnable(bool enable_hl, bool enable_ll);
    void EditTtbChar(int index);

    void FillTtcHLComboBox();

    void TtcEntryHLToGui(const TtcEntryHL &entry);
    void GuiToTtcEntryHL(TtcEntryHL &entry);

    void TtcEntryLLToGui(const TtcEntryLL &entry);
    void GuiToTtcEntryLL(TtcEntryLL &entry);

    void RebuildTtcEnable(bool enable_hl, bool enable_ll);

    bool VerifyTtcLLCommon(AcbFile *acbs, Afs2File *awbs, MsgFile &voice, MsgFile *subs);
    bool VerifyTtcLLEntry(const TtcEntryLL &entry, AcbFile *acbs, MsgFile &voice, MsgFile *subs, const char *err_message);

    bool CopyTtcLLFiles(const std::string &src_char, const std::string &out_path, bool internal);

    void FillDualSkillComboBox(QComboBox *cb);
    void DualSkillToComboBox(uint16_t dual_skill_id, QComboBox *cb);
    uint16_t ComboBoxToDualSkill(QComboBox *cb);

    void CncEntryToGui(const CncEntry &entry);
    void GuiToCncEntry(CncEntry &entry);
};

#endif // MAINWINDOW_H
