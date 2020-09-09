#include "stageseledialog.h"
#include "ui_stageseledialog.h"

#include <QToolButton>

#include "VirtualStageEmb.h"
#include "listdialog.h"

#define INITIAL_X   20
#define INITIAL_Y   60

#define ICON_WIDTH  110
#define ICON_HEIGHT 60

#define SPACING_X   4
#define SPACING_Y   4

#define PAGE_SIZE   33

enum SsAction
{
    SS_SELECT = 0x300,
    SS_SWAP,
    SS_BEFORE,
    SS_AFTER,
    SS_DELETE
};

StageSeleDialog::StageSeleDialog(const std::vector<Xv2StageSlot> &ss_slots, QWidget *parent) :
    ss_slots(ss_slots),
    QDialog(parent),
    ui(new Ui::StageSeleDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if (VirtualStageEmb::portraits.GetNumFiles() == 0)
        VirtualStageEmb::Build();

    for (int i = 0; i < PAGE_SIZE; i++)
    {
        QToolButton *button = new QToolButton(this);

        int col = i / 3;
        int row = i % 3;
        int x = INITIAL_X + col*(ICON_WIDTH+SPACING_X);
        int y = INITIAL_Y + row*(ICON_HEIGHT+SPACING_Y);

        button->setFixedSize(ICON_WIDTH, ICON_HEIGHT);
        button->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT));

        button->setGeometry(x, y, ICON_WIDTH, ICON_HEIGHT);
        button->setObjectName("ssbutton" + QString("%1").arg(i));

        QMenu *menu = new QMenu(this);

        AddMenuAction("Select", SS_SELECT, menu, i);
        AddMenuAction("Swap selection with this", SS_SWAP, menu, i, false);
        AddMenuAction("Move selection before this", SS_BEFORE, menu, i, false);
        AddMenuAction("Move selection after this", SS_AFTER, menu, i, false);
        AddMenuAction("Delete", SS_DELETE, menu, i);

        button->setMenu(menu);
        button->setPopupMode(QToolButton::InstantPopup);

        connect(button, SIGNAL(triggered(QAction*)), this, SLOT(onStageEditAction(QAction*)));
    }

    int num_pages = GetNumPages();

    for (int i = 0; i < num_pages; i++)
        ui->pageComboBox->addItem("Page " + QString("%1").arg(i+1));
}

StageSeleDialog::~StageSeleDialog()
{
    delete ui;
}

void StageSeleDialog::AddMenuAction(const QString &title, int type, QMenu *menu, size_t button_index, bool enabled)
{
    QAction *action = new QAction(title, this);
    action->setProperty("action_type", QVariant(type));
    action->setProperty("button_index", QVariant((uint)button_index));
    action->setEnabled(enabled);

    menu->addAction(action);
}

int StageSeleDialog::GetNumPages() const
{
    int num_pages = (int)ss_slots.size() / PAGE_SIZE;

    if ((ss_slots.size() % PAGE_SIZE) != 0)
        num_pages++;

    return num_pages;
}

void StageSeleDialog::LoadPage(int page)
{
    if (page >= GetNumPages())
        return;

    QList<QToolButton *> buttons = this->findChildren<QToolButton *>(QRegExp("ssbutton", Qt::CaseSensitive, QRegExp::Wildcard));
    if (buttons.size() != PAGE_SIZE)
    {
        UPRINTF("Internal error");
        exit(-1);
    }

    auto first = ss_slots.begin() + (page*PAGE_SIZE);
    size_t page_size = ss_slots.size() - (page*PAGE_SIZE);

    if (page_size > PAGE_SIZE)
        page_size = PAGE_SIZE;

    auto end = first + page_size;

    std::vector<Xv2StageSlot> page_slots(first, end);

    int i;

    for (i = 0; i < (int)page_slots.size(); i++)
    {
        buttons[i]->setEnabled(true);
        buttons[i]->setVisible(true);

        const Xv2StageSlot &ss_slot = page_slots[i];
        QPixmap pixmap;
        QString tooltip;

        Xv2Stage *stage = game_stage_def->GetStageBySsid(ss_slot.stage);
        if (stage)
        {
            uint16_t idx = VirtualStageEmb::portraits.FindIndexByName(stage->code + ".dds");

            if (idx != 0xFFFF)
            {
                const EmbContainedFile &texture = VirtualStageEmb::portraits[idx];
                pixmap.loadFromData(texture.GetData(), (uint)texture.GetSize());

                if (pixmap.width() != ICON_WIDTH || pixmap.height() != ICON_HEIGHT)
                {
                    pixmap = pixmap.scaled(ICON_WIDTH, ICON_HEIGHT);
                }
            }

            std::string name;

            if (Xenoverse2::GetStageName(stage->code, name))
            {
                tooltip = Utils::StdStringToQString(name, false);
            }
        }

        buttons[i]->setIcon(QIcon(pixmap));
        buttons[i]->setToolTip(tooltip);
    }

    for (; i < PAGE_SIZE; i++)
    {
        buttons[i]->setEnabled(false);
        buttons[i]->setVisible(false);
    }

    UpdateClipboard();
}

void StageSeleDialog::UpdateClipboard()
{
    QList<QAction *> actions = findChildren<QAction *>();

    for (QAction *action : actions)
    {
        int type = action->property("action_type").toInt();
        size_t button_index = (size_t)action->property("button_index").toUInt();
        size_t page_index = (ui->pageComboBox->currentIndex()*PAGE_SIZE);
        size_t ss_index = page_index + button_index;

        if (type == SS_SWAP || type == SS_BEFORE || type == SS_AFTER)
        {
            if (clipboard < 0)
            {
                action->setEnabled(false);
            }
            else
            {
                action->setEnabled(ss_index != clipboard);
            }
        }
    }
}

bool StageSeleDialog::CanAddStage(Xv2Stage *stage)
{
    if (VirtualStageEmb::portraits.FindIndexByName(stage->code + ".dds") == 0xFFFF)
    {
        DPRINTF("Cannot add that stage because it doesn't have a portrait.\n", FUNCNAME);
        return false;
    }

    if (VirtualStageEmb::backgrounds.FindIndexByName(stage->code + ".dds") == 0xFFFF)
    {
        DPRINTF("Cannot add that stage because it doesn't have selection background.\n", FUNCNAME);
        return false;
    }

    return true;
}

void StageSeleDialog::on_pageComboBox_currentIndexChanged(int index)
{
    LoadPage(index);
}

void StageSeleDialog::onStageEditAction(QAction *action)
{
    int type = action->property("action_type").toInt();
    size_t button_index = (size_t)action->property("button_index").toUInt();
    int page = ui->pageComboBox->currentIndex();
    size_t page_index = (page*PAGE_SIZE);
    size_t ss_index = page_index + button_index;

    if (ss_index >= ss_slots.size())
    {
        //UPRINTF("Internal error, index out of bounds.\n");
        return;
    }

    bool clipboard_valid = (clipboard >= 0 && clipboard < (int)ss_slots.size() && clipboard != ss_index);

    if (type == SS_SELECT)
    {
        clipboard = (int)ss_index;
        UpdateClipboard();
    }
    else if (type == SS_SWAP)
    {
        if (!clipboard_valid)
            return;

        Xv2StageSlot temp = ss_slots[ss_index];
        ss_slots[ss_index] = ss_slots[clipboard];
        ss_slots[clipboard] = temp;

        clipboard = -1;
        LoadPage(page);
    }
    else if (type == SS_BEFORE)
    {
        if (!clipboard_valid)
            return;

        size_t erase_index = (size_t)clipboard;

        if (erase_index > ss_index)
            erase_index++;

        ss_slots.insert(ss_slots.begin()+ss_index, ss_slots[clipboard]);
        ss_slots.erase(ss_slots.begin()+erase_index);

        clipboard = -1;
        LoadPage(page);
    }
    else if (type == SS_AFTER)
    {
        if (!clipboard_valid)
            return;

        size_t erase_index = (size_t)clipboard;

        if (erase_index > ss_index)
            erase_index++;

        if (ss_index == ss_slots.size()-1)
        {
            ss_slots.push_back(ss_slots[clipboard]);
        }
        else
        {
            ss_slots.insert(ss_slots.begin()+ss_index+1, ss_slots[clipboard]);
        }

        ss_slots.erase(ss_slots.begin()+erase_index);

        clipboard = -1;
        LoadPage(page);
    }

    else if (type == SS_DELETE)
    {
        if (ss_slots.size() == 1)
        {
            DPRINTF("Cannot remove last slot, fucktard.\n");
            return;
        }

        int num_pages = GetNumPages();

        ss_slots.erase(ss_slots.begin()+ss_index);

        if (num_pages != GetNumPages())
        {
            bool update = (page != (num_pages-1));

            clipboard = -1;
            ui->pageComboBox->removeItem(num_pages-1);

            if (update)
            {
                LoadPage(page);
            }
        }
        else
        {
            clipboard = -1;
            LoadPage(page);
        }
    }
}

void StageSeleDialog::on_addButton_clicked()
{
    ListDialog dialog(ListMode::STAGE_SS, this);

    if (!dialog.exec())
        return;

    Xv2Stage *stage = game_stage_def->GetStageBySsid(dialog.GetResultInteger());
    if (!stage || !CanAddStage(stage))
        return;

    int num_pages = GetNumPages();

    Xv2StageSlot new_slot;
    new_slot.stage = stage->ssid;
    new_slot.dlc = 0;

    ss_slots.push_back(new_slot);

    if (num_pages != GetNumPages())
    {
        ui->pageComboBox->addItem("Page " + QString("%1").arg(GetNumPages()));
    }

    clipboard = -1;
    LoadPage(ui->pageComboBox->currentIndex());
}
