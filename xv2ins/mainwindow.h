#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QLabel>

#include "X2mFile.h"
#include "Xv2QuestCompiler.h"
#include "logdialog.h"

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
    bool ClearInstallation(bool special_mode);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_modsList_itemSelectionChanged();

    void on_actionUninstall_triggered();

    void on_actionAssociate_x2m_extension_triggered();

    void on_actionSlot_editor_triggered();

    void on_actionFind_and_delete_dead_ids_triggered();

    void on_actionClear_installation_triggered();

    void on_actionRemoveEmptyDirs_triggered();

    void on_actionChange_3_letter_code_of_x2m_triggered();

    void on_actionConvert_charalist_triggered();

    void on_actionCompile_INTERNAL_folder_triggered();

    void on_actionSlot_editor_stages_triggered();

    void on_actionSlot_editor_stages_local_mode_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *statusLabel;
    bool invisible_mode;
    QMutex ui_mutex;
    bool process_selection_change = false;

    std::vector<SevTableEntryHL> sev_hl_table;
    std::vector<SevTableEntryLL> sev_ll_table;

    std::vector<TtbTableEntryHL> ttb_hl_table;
    std::vector<TtbTableEntryLL> ttb_ll_table;

    struct ModEntry
    {
        std::string package_path;
        QString name;
        QString author;
        float version;
        QString guid;
        QString entry_name;
        X2mType type;

        // These two needs to update skills of characters "after"
        std::vector<X2mDepends> depends;
        std::vector<CusSkillSet> skill_sets;  // Only for chara mods with skill depends

        // This one needed along depends to update costume of skills "after"
        std::vector<std::pair<CusSkill, X2mSkillType>> skill_entry; // Only for skill mods with costume depends. This should be a 0 or 1 sized vector.

        int index;
        QTreeWidgetItem *item;
    };

    QVector<ModEntry> installed_mods;
    Xv2QuestCompiler qc;

    size_t temp_empty_found; // For delete empty dir
    size_t temp_empty_deleted; // For delete empty dir
    bool temp_empty_yesall; // For delete empty dir

    bool ProcessShutdown();
    void UpdateStatus();

    bool CheckCharaSele();
    void CheckRegistryAssociation();

    void ModToGui(ModEntry &entry);
    ModEntry *GetSelectedMod();

    static void AddModToQuestCompiler(X2mFile &x2m, Xv2QuestCompiler &qc, const std::string &path);
    static bool LoadVisitor(const std::string &path, bool, void *param);
    uint8_t *GetAudio(AcbFile *acb, AwbFile *awb, uint32_t cue_id, size_t *psize);   
    void LoadInstalledMods();    

    bool ExtractQuestFiles(X2mFile &x2m, std::string &qs, std::string &cs, std::string &ds, std::string &ps, std::vector<std::string> &svec, std::vector<std::string> &sfvec);

    ModEntry *FindModByGuid(const QString &guid);
    ModEntry *FindModByEntry(const QString &entry);

    bool CanUpdateSlots(const X2mFile &new_mod, const X2mFile &old_mod) const;
    bool CanUpdateCostume(const X2mFile &new_mod, const X2mCostumeEntry *entry) const;
    void ClearAttachments(std::vector<X2mFile *> &attachments);
    void PostProcessSkill(X2mFile *x2m);
    void PostProcessCostume(X2mFile *x2m);
    ModEntry *InstallMod(const QString &path, ModEntry *reinstall_mod, bool silent_mode);

    void RemoveDeadItem();
    void PostProcessCostumeUninstall(X2mFile *x2m, const X2mCostumeEntry &costume);
    bool UninstallMod(const ModEntry &mod, bool remove_empty_dir);

    bool MultipleModsQuestion();

    static bool DeleteEmptyDirVisitor(const std::string &path, bool, void *param);

    bool ConvertModEntry(X2mFile *x2m, const std::string &new_entry);
};

#endif // MAINWINDOW_H
