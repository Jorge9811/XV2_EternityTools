#ifndef CSSDIALOG_H
#define CSSDIALOG_H

#include <QDialog>
#include <QToolButton>
#include <QTimer>
#include "Xenoverse2.h"

enum class CssMode
{
    SELECT,
    EDIT
};

enum class CssDragScroll
{
    UNDEFINED,
    LEFT,
    RIGHT
};

namespace Ui {
class CssDialog;
}

class CssDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CssDialog(CssMode mode, QWidget *parent = 0);
    ~CssDialog();

    inline int GetResultInteger() const { return result_integer; }
    EmbFile *GetEditedEmb() { return edit_emb; }
    std::vector<CharaListSlot> &GetEditedSlots() { return edit_slots; }

    bool eventFilter(QObject* object, QEvent* event);

    static QString GetCharaName(const CharaListSlotEntry &entry);
    static QString GetCharaAndCostumeName(const CharaListSlotEntry &entry);

public slots:

    void onDragUpdateScroll();
    void onSwapPositions(int src, int dst);
    void onMoveSlot(int src, int dst);
    void onMergeSlot(int src, int dst);

signals:

private slots:

    void onCharacterSelect();
    void onCharacterEditAction(QAction *action);

    void on_cancelButton_clicked();

    void on_addButton_clicked();

private:
    Ui::CssDialog *ui;
    QTimer scroll_timer;
    CssDragScroll scroll_direction;

    int result_integer = -1;
    std::vector<CharaListSlot> edit_slots;
    EmbFile *edit_emb;

    CharaListSlotEntry *selected_entry = nullptr;
    size_t selected_entry_slot_index;
    size_t selected_entry_entry_index;

    void PopulateSlots(const std::vector<CharaListSlot> &c_slots, CssMode mode);

    void AddMenuAction(const QString &title, int type, QMenu *menu, size_t entry_index, bool enabled=true, bool bold=false);
    void AddMenuAction2(const QString &title, int type, QMenu *menu, size_t slot_index, bool enabled=true, bool bold=false);
    void AddEntry(const CharaListSlotEntry &entry, size_t i, size_t max, QMenu *menu, bool insert_before);

    void SetCharaIcon(QAbstractButton *button, const CharaListSlotEntry &entry);
    void SetCharaImage(const std::string &str, QPixmap &pixmap);
    bool SetRandomAvatarImage(QPixmap &pixmap);

    void EnableClipboard(bool enable);
    void UpdateClipboard();

    void Swap(size_t slot_index1, size_t entry_index1, size_t slot_index2, size_t entry_index2);
    void RebuildEntry(size_t slot_index, size_t entry_index);
};

#endif // CSSDIALOG_H
