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

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_guidButton_clicked();

    void on_humButton_clicked();

    void on_hufButton_clicked();

    void on_nmcButton_clicked();

    void on_friButton_clicked();

    void on_mamButton_clicked();

    void on_mafButton_clicked();

    void on_addEditButton_clicked();

    void on_humEnableCheck_clicked();

    void on_humComboBox_currentIndexChanged(int index);

    void on_humAddButton_clicked();

    void on_humRemoveButton_clicked();

    void on_hufEnableCheck_clicked();

    void on_hufComboBox_currentIndexChanged(int index);

    void on_hufAddButton_clicked();

    void on_hufRemoveButton_clicked();

    void on_nmcEnableCheck_clicked();

    void on_nmcComboBox_currentIndexChanged(int index);

    void on_nmcAddButton_clicked();

    void on_nmcRemoveButton_clicked();

    void on_friEnableCheck_clicked();

    void on_friComboBox_currentIndexChanged(int index);

    void on_friAddButton_clicked();

    void on_friRemoveButton_clicked();

    void on_mamEnableCheck_clicked();

    void on_mamComboBox_currentIndexChanged(int index);

    void on_mamAddButton_clicked();

    void on_mamRemoveButton_clicked();

    void on_mafEnableCheck_clicked();

    void on_mafComboBox_currentIndexChanged(int index);

    void on_mafAddButton_clicked();

    void on_mafRemoveButton_clicked();

    void on_humCopyButton_triggered(QAction *arg1);

    void on_hufCopyButton_triggered(QAction *arg1);

    void on_nmcCopyButton_triggered(QAction *arg1);

    void on_friCopyButton_triggered(QAction *arg1);

    void on_mamCopyButton_triggered(QAction *arg1);

    void on_mafCopyButton_triggered(QAction *arg1);

    void on_idbEnableCheck_clicked();

    void on_idbComboBox_currentIndexChanged(int index);

    void on_idbAddButton_clicked();

    void on_idbRemoveButton_clicked();

    void on_idbEff1Button_clicked();

    void on_idbEff2Button_clicked();

    void on_idbEff3Button_clicked();

    void on_idbNameEdit_textEdited(const QString &arg1);

    void on_idbDescEdit_textChanged();

    void on_idbCopyButton_triggered(QAction *arg1);

    void on_idbNameCopyButton_triggered(QAction *arg1);

    void on_idbDescCopyButton_triggered(QAction *arg1);

    void on_idbNameLangComboBox_currentIndexChanged(int index);

    void on_idbDescLangComboBox_currentIndexChanged(int index);

    void on_guidCopyButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    X2mFile *x2m;

    QVector<QString> hum_bcs;
    QVector<QString> huf_bcs;
    QVector<QString> nmc_bcs;
    QVector<QString> fri_bcs;
    QVector<QString> mam_bcs;
    QVector<QString> maf_bcs;

    int prev_hum_index=-1;
    int prev_huf_index=-1;
    int prev_nmc_index=-1;
    int prev_fri_index=-1;
    int prev_mam_index=-1;
    int prev_maf_index=-1;
    int prev_idb_index=-1;

    bool ProcessShutdown();

    int GetMaxNumPartSets();

    static bool CheckDirVisitor(const std::string &path, bool, void *custom_param);
    bool CheckDirectoryContent(const std::string &dir);

    static bool AddDirVisitor(const std::string &path, bool, void *custom_param);
    bool AddDirectoryContent(const std::string &dir, uint8_t race);

    void ProcessX2m();
    bool Validate();
    bool Build();

    void UpdateIdbPartset();

    QString BcsToString(const BcsPartSet &bcs);
    bool StringToBcs(const QString &str, BcsPartSet &bcs, const std::string &error_prefix);

    void CopyBcs(QTextEdit *edit, BcsFile &bcs, uint8_t race_lock);

    void ItemToGui(const X2mItem &item);
    void GuiToItem(X2mItem &item);
    void EditIdbEffect(IdbEffect &effect);

    void CopyIdb(IdbFile *idb, X2mItemType type);
    void CopyName(IdbFile *idb, X2mItemType type);
    void CopyDesc(IdbFile *idb, X2mItemType type);
};

#endif // MAINWINDOW_H
