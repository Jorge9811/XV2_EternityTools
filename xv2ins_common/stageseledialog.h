#ifndef STAGESELEDIALOG_H
#define STAGESELEDIALOG_H

#include <QDialog>
#include <QMenu>

#include "Xv2PatcherSlotsFileStage.h"
#include "Xv2StageDefFile.h"

namespace Ui {
class StageSeleDialog;
}

class StageSeleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StageSeleDialog(const std::vector<Xv2StageSlot> &ss_slots, QWidget *parent = 0);
    ~StageSeleDialog();

    std::vector<Xv2StageSlot> &GetEditedSlots() { return ss_slots; }

private slots:
    void on_pageComboBox_currentIndexChanged(int index);

    void onStageEditAction(QAction *action);

    void on_addButton_clicked();

private:
    Ui::StageSeleDialog *ui;

    std::vector<Xv2StageSlot> ss_slots;

    int clipboard=-1;

    void AddMenuAction(const QString &title, int type, QMenu *menu, size_t button_index, bool enabled=true);

    int GetNumPages() const;
    void LoadPage(int page);

    void UpdateClipboard();

    bool CanAddStage(Xv2Stage *stage);
};

#endif // STAGESELEDIALOG_H
