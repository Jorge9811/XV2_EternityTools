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

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionImportSkillDir_triggered();

    void on_actionAbout_triggered();

    void on_guidButton_clicked();

    void on_skillTypeComboBox_currentIndexChanged(int index);

    void on_skillNameLangComboBox_currentIndexChanged(int index);

    void on_skillNameEdit_textEdited(const QString &arg1);

    void on_skillNameCopyButton_clicked();

    void on_skillTransNameEdit_textEdited(const QString &arg1);

    void on_skillTransLangComboBox_currentIndexChanged(int index);

    void on_skillTransComboBox_currentIndexChanged(int index);

    void on_skillTransAddButton_clicked();

    void on_skillTransRemoveButton_clicked();

    void on_skillTransCopyButton_clicked();

    void on_skillFilesButton_clicked();

    void on_addEditButton_clicked();

    void on_cusCopyButton_triggered(QAction *arg1);

    void on_skillDescLangComboBox_currentIndexChanged(int index);

    void on_skillDescEdit_textChanged();

    void on_skillDescCopyButton_clicked();    

    void on_cusAcbSeButton_clicked();

    void on_cusAcbVoxButton_clicked();

    void on_cusModelButton_clicked();

    void on_cusModelEdit_textChanged(const QString &arg1);

    void on_idbEnableCheck_clicked();

    void on_idbEff1Button_clicked();

    void on_idbEff2Button_clicked();

    void on_idbEff3Button_clicked();

    void on_idbCopyButton_triggered(QAction *arg1);

    void on_pupEnableCheck_clicked();

    void on_pupCopyButton_triggered(QAction *arg1);

    void on_pupComboBox_currentIndexChanged(int index);

    void on_pupAddButton_clicked();

    void on_pupRemoveButton_clicked();

    void on_auraEnableCheck_clicked();

    void on_auraCustomCheck_clicked();

    void on_auraComboBox_currentIndexChanged(int index);

    void on_auraAddButton_clicked();

    void on_auraRemoveButton_clicked();    

    void on_auraCopyButton_triggered(QAction *arg1);    

    void on_auraPatchCopyButton_triggered(QAction *arg1);

    void on_bcsAttachmentSetButton_clicked();

    void on_bcsAttachmentRemoveButton_clicked();

    void on_auraTeleportCheck_clicked();

    void on_auraGetHairColorButton_triggered(QAction *arg1);

    void on_bcsBodyCheck_clicked();

    void on_bcsBodyComboBox_currentIndexChanged(int index);

    void on_bcsBodyAddButton_clicked();

    void on_bcsBodyRemoveButton_clicked();

    void on_bcsBodyEdit_textChanged();

    void on_auraGetEyesColorButton_triggered(QAction *arg1);

    void on_guidCopyButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    X2mFile *x2m;

    std::string temp_skill_dir;
    std::string temp_skill_prefix;
    uint16_t temp_skill_id2;

    std::vector<QString> bcs_bodies;

    int prev_trans_index=-1;
    int prev_pup_index=-1;
    int prev_aura_index=-1;
    int prev_body_index=-1;

    static bool ImportSkillDirVisitor(const std::string &path, bool, void *param);

    bool ProcessShutdown();

    void ProcessX2m();
    bool Validate();
    bool Build();

    void TransNameToGui(const X2mSkillTransName &entry);
    void GuiToTransName(X2mSkillTransName &entry);

    void SkillToGui(const CusSkill &skill);
    void GuiToSkill(CusSkill &skill);

    void IdbEntryToGui(const IdbEntry &entry);
    void GuiToIdbEntry(IdbEntry &entry);

    void EditIdbEffect(IdbEffect &effect);

    void PupEntryToGui(const PupEntry &entry);
    void GuiToPupEntry(PupEntry &entry);

    void AuraToGui(const X2mSkillAura &aura);
    void GuiToAura(X2mSkillAura &aura);

    void BodyToGui(const X2mBody &body, const QString body_xml);
    void GuiToBody(X2mBody &body, QString &body_xml);

    QString BodyToString(const BcsBody &body);
    bool StringToBody(const QString &str, BcsBody &body, const std::string &error_prefix);

    bool OpenBcsColorDialog(BcsFile *bcs, const std::string &part_name, uint32_t current_color, uint32_t *out_color);

    int LinkOrEmbed(X2mFile *cost_x2m);
};

#endif // MAINWINDOW_H
