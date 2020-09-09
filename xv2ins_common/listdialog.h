#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class ListDialog;
}

enum class ListMode
{
    SELECT_PORTRAIT,
    BATTLE_PORTRAIT,
    CHARASEL_FULL,
    CHARASEL_PARTIAL,
    CMS,
    CUS,
    CSO,
    PSC,
    AUR,
    SEV,
    CML,
    HCI,
    PAL,
    CAC_FULL,
    CAC_PARTIAL,
    SKILL,
    SKILL_IDB,
    PUP,
    PUP_SKILL,
    SKILL_TRANS,
    AURA,
    AURA_SKILL,
    TALISMAN,
    BCS,
    COSTUME_IDB,
    ACCESSORY_IDB,
    STAGE,
    STAGE_LIGHTING,
    STAGE_SS,
    X2M_SLOT,
    TTB,
    TTC,
    TTC_FILES,
    CNC
};

#define SKILL_FLAG_SUPER        1
#define SKILL_FLAG_ULTIMATE     2
#define SKILL_FLAG_EVASIVE      4
#define SKILL_FLAG_BLAST        8
#define SKILL_FLAG_AWAKEN       0x10

#define SKILL_FLAG_ALL          (SKILL_FLAG_SUPER|SKILL_FLAG_ULTIMATE|SKILL_FLAG_EVASIVE|SKILL_FLAG_BLAST|SKILL_FLAG_AWAKEN)

#define FLAG_USE_CUS_AURA   1

class ListDialog : public QDialog
{
    Q_OBJECT

public:
    ListDialog(ListMode mode, QWidget *parent = 0, void *extra=nullptr, int flags=0);
    ~ListDialog();

    inline QString GetResult() const { return result; }
    inline int GetResultInteger() const { return result_integer; }
    inline int GetResultData() const { return result_data; }
    inline QString GetResultDataString() const { return result_data_str; }

private slots:
    void on_buttonBox_accepted();

    void on_listWidget_itemActivated(QListWidgetItem *item);

private:
    Ui::ListDialog *ui;

    ListMode mode;
    void *extra;
    int flags;

    QString result;
    int result_integer=-1;
    int result_data=-1;
    QString result_data_str;

    static bool BattlePortraitVisitor(const std::string &path, bool, void *param);
    static bool StageLightingVisitor(const std::string &path, bool, void *param);
    static bool TtcLlFilesVisitor(const std::string &path, bool, void *param);

    void PopulateList();
};

#endif // LISTDIALOG_H
