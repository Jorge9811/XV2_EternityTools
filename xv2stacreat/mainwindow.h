#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
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

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_guidButton_clicked();

    void on_stageNameLangComboBox_currentIndexChanged(int index);

    void on_stageNameEdit_textEdited(const QString &arg1);

    void on_stageNameCopyButton_clicked();

    void on_stageFilesButton_clicked();

    void on_lightingButton_triggered(QAction *arg1);

    void on_selPortButton_triggered(QAction *arg1);

    void on_questPortButton_triggered(QAction *arg1);

    void on_selBackButton_triggered(QAction *arg1);

    void on_addDataButton_clicked();

    void on_defCopyButton_clicked();

    void on_bgmButton_clicked();

    void on_gate1Button_clicked();

    void on_gate2Button_clicked();

    void on_gate3Button_clicked();

    void on_gate4Button_clicked();

    void on_gate5Button_clicked();

    void on_gate6Button_clicked();

    void on_gate7Button_clicked();

    void on_slotsCheck_clicked();

    void on_guidCopyButton_clicked();

    void on_vfxCopyButton_clicked();

    void on_gate8Button_clicked();

    void on_gate9Button_clicked();

private:
    Ui::MainWindow *ui;
    X2mFile *x2m;
    std::string temp_path;

    bool show_wait_onsave=false;
    bool audio_playing=false;

    bool ProcessShutdown();

    void ProcessX2m();
    bool Validate();
    bool Build();

    static bool ValidateFilesVisitor(const std::string &path, bool, void *param);

    uint8_t *GetGameLighting(const QString &file, size_t *psize);
    uint8_t *GetGameSelPortrait(const QString &name, size_t *psize);
    uint8_t *GetGameQstPortrait(const QString &name, size_t *psize);
    uint8_t *GetGameSelBackground(const QString &name, size_t *psize);

    void DefToGui(const Xv2Stage &stage);
    void GuiToDef(Xv2Stage &stage);

    void PlayAudio(HcaFile *hca);
    void PlayAudio(AcbFile *acb, AwbFile *awb, uint32_t cue_id);

    void ExecGateDialog(Xv2StageGate *gate);
};

#endif // MAINWINDOW_H
