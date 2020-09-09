#include "cssdialog.h"
#include "ui_cssdialog.h"

#include <QPushButton>
#include <QToolButton>
#include <QScrollBar>
#include <QMenu>
#include <QMessageBox>
#include "Xenoverse2.h"
#include "sloteditdialog.h"
#include "debug.h"

#include "csstoolbutton.h"
#include "slotreorderdialog.h"

#define CSS_SEL_LEFT 125
#define CSS_SEL_TOP 45
#define CSS_SEL_WIDTH   125
#define CSS_SEL_HEIGHT 100

#define CSS_ICON_WIDTH  87
#define CSS_ICON_HEIGHT 69

#pragma warning(disable : 4258)

enum CssAction
{
    CSS_COSTUME = 0x300,
    CSS_SELECT,
    CSS_SWAP,
    CSS_UP,
    CSS_DOWN,
    CSS_ADD,
    CSS_COPY,
    CSS_MANUAL,
    CSS_EDIT,
    CSS_REMOVE,
    CSS_MOVETO,
    CSS_CUT,
};

CssDialog::CssDialog(CssMode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CssDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    edit_emb = nullptr;
    PopulateSlots(chara_list->GetSlots(), mode);

    if (mode == CssMode::SELECT)
    {
        ui->buttonBox->setVisible(false);
        ui->addButton->setVisible(false);
        ui->addButton->setDisabled(true);
        setWindowTitle("Select character");
        ui->tipLabel->setVisible(false);
        edit_emb = nullptr;
    }
    else if (mode == CssMode::EDIT)
    {
        ui->cancelButton->setVisible(false);
        setWindowTitle("Edit slots");

        // Copy slots, we will be working on this ones from now on (commit is made by caller on dialog accept)
        edit_slots = chara_list->GetSlots();
        // copy sel emb too
        edit_emb = new EmbFile(*game_sel_port);

        // Handling scroll in drag and drop mode
        scroll_timer.setSingleShot(false);
        scroll_timer.setInterval(0);
        scroll_direction = CssDragScroll::UNDEFINED;

        connect(&scroll_timer, SIGNAL(timeout()), this, SLOT(onDragUpdateScroll()));
    }
}

CssDialog::~CssDialog()
{
    if (edit_emb)
        delete edit_emb;

    delete ui;
}

bool CssDialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::DragMove)
    {
        QDragMoveEvent *drag_event = static_cast<QDragMoveEvent *>(event);
        CssToolButton *button = static_cast<CssToolButton *>(object);

        QPoint point = button->mapTo(ui->scrollArea, drag_event->pos());

        if (point.x() > ui->scrollArea->width()-50)
        {
            if (scroll_direction != CssDragScroll::RIGHT)
            {
                scroll_direction = CssDragScroll::RIGHT;
                scroll_timer.start();
            }
        }
        else if (point.x() < 50)
        {
            if (scroll_direction != CssDragScroll::LEFT)
            {
                scroll_direction = CssDragScroll::LEFT;
                scroll_timer.start();
            }
        }
        else
        {
            scroll_timer.stop();
            scroll_direction = CssDragScroll::UNDEFINED;
        }
    }
    else if (event->type() == QEvent::DragLeave || event->type() == QEvent::Drop)
    {
        scroll_timer.stop();
        scroll_direction = CssDragScroll::UNDEFINED;
    }

    return false;
}

void CssDialog::onDragUpdateScroll()
{
    if (scroll_direction == CssDragScroll::RIGHT)
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value()+8);

    else if (scroll_direction == CssDragScroll::LEFT)
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value()-8);
}

void CssDialog::PopulateSlots(const std::vector<CharaListSlot> &c_slots, CssMode mode)
{
    QLayoutItem *item;

    while ((item = ui->gridLayout->takeAt(0)))
    {
        QWidget *widget = item->widget();
        QToolButton *button = dynamic_cast<QToolButton *>(widget);

        if (button)
        {
            for (QAction *action : button->actions())
            {
                button->removeAction(action);
                delete action;
            }

            disconnect(button);
        }

        delete item;
        ui->gridLayout->removeWidget(widget);
        widget->deleteLater();
    }

    QList<QAction *> actions = findChildren<QAction *>();

    for (QAction *action : actions)
    {
        if (action->property("action_type") >= CSS_COSTUME)
        {
            action->deleteLater();
        }
    }

    QList<QMenu *> menus = findChildren<QMenu *>();

    for (QMenu *menu : menus)
    {
        menu->deleteLater();
    }

    int row = 0, column = 0;

    for (size_t i = 0; i < c_slots.size(); i++)
    {
        const CharaListSlot &slot = c_slots[i];

        if (row == 3)
        {
            row = 0;
            column++;
        }

        if (slot.entries.size() == 0)
        {
            DPRINTF("%s: Internal error.\n", FUNCNAME);
            exit(-1);
        }

        const CharaListSlotEntry &entry = slot.entries[0];

        QAbstractButton *button;

        if (mode == CssMode::SELECT)
            button = new QPushButton(ui->scrollAreaWidgetContents);
        else
            button = new CssToolButton(ui->scrollAreaWidgetContents); //QToolButton(ui->scrollAreaWidgetContents);

        SetCharaIcon(button, entry);
        button->setFixedSize(CSS_ICON_WIDTH, CSS_ICON_HEIGHT);
        button->setIconSize(QSize(CSS_ICON_WIDTH, CSS_ICON_HEIGHT));

        button->setToolTip(GetCharaName(entry));
        button->setProperty("slot_index", QVariant((uint)i));

        if (mode == CssMode::SELECT)
        {
            if (slot.entries.size() == XV2_MAX_SUBSLOTS)
                button->setDisabled(true);

            connect(button, SIGNAL(clicked(bool)), this, SLOT(onCharacterSelect()));
        }

        else if (mode == CssMode::EDIT)
        {
            CssToolButton *tbutton = dynamic_cast<CssToolButton *>(button);

            QMenu *menu = new QMenu(this);

            for (size_t i = 0; i < slot.entries.size(); i++)
            {
                AddEntry(slot.entries[i], i, slot.entries.size(), menu, false);
            }

            {
                QMenu *submenu = new QMenu("Add", this);

                AddMenuAction2("Copy selected", CSS_COPY, submenu, i, false);
                AddMenuAction2("Cut selected", CSS_CUT, submenu, i, false);
                AddMenuAction2("Manual", CSS_MANUAL, submenu, i, (slot.entries.size() < XV2_MAX_SUBSLOTS));

                QAction *add = menu->addMenu(submenu);
                add->setProperty("action_type", CSS_ADD);
            }

            tbutton->setMenu(menu);
            tbutton->setPopupMode(QToolButton::InstantPopup);

            tbutton->installEventFilter(this);
            connect(tbutton, SIGNAL(triggered(QAction*)), this, SLOT(onCharacterEditAction(QAction*)));
            connect(tbutton, SIGNAL(swapSlot(int,int)), this, SLOT(onSwapPositions(int,int)));
            connect(tbutton, SIGNAL(moveSlot(int,int)), this, SLOT(onMoveSlot(int,int)));
            connect(tbutton, SIGNAL(mergeSlot(int,int)), this, SLOT(onMergeSlot(int,int)));
        }

        ui->gridLayout->addWidget(button, row, column);
        row++;        
    }
}

QString CssDialog::GetCharaName(const CharaListSlotEntry &entry)
{
    std::string name;

    if (entry.code == "AvatarCode" || entry.code == "\"HUM\"")
    {
        name = "Avatar";
    }
    else if (entry.code.length() == 5 && entry.code[0] == '"' && entry.code[4] == '"')
    {
        if (!Xenoverse2::GetCharaName(entry.code.substr(1, 3), name, XV2_LANG_ENGLISH, entry.costume_index))
            name = "????";
    }
    else
    {
        name = "????";
    }

    return Utils::StdStringToQString(name, false);
}

QString CssDialog::GetCharaAndCostumeName(const CharaListSlotEntry &entry)
{
    std::string name, costume;

    if (entry.code == "AvatarCode" || entry.code == "\"HUM\"")
    {
        name = "Avatar";
        costume = Utils::ToString(entry.costume_index);
    }
    else if (entry.code.length() == 5 && entry.code[0] == '"' && entry.code[4] == '"')
    {
        if (!Xenoverse2::GetCharaName(entry.code.substr(1, 3), name, XV2_LANG_ENGLISH, entry.costume_index))
            name = "????";

        if (!Xenoverse2::GetCharaCostumeName(entry.code.substr(1, 3), entry.costume_index, entry.model_preset, costume))
            costume = "????";
    }
    else
    {
        name = "????";
        costume = "????";
    }

    return Utils::StdStringToQString(name + " - " + costume, false);
}

void CssDialog::SetCharaIcon(QAbstractButton *button, const CharaListSlotEntry &entry)
{
    QPixmap pixmap;
    std::string name;

    if (entry.code.length() == 5 && entry.code[0] == '"' && entry.code[4] == '"')
    {
        name = entry.code.substr(1, 3) + "_" + Utils::ToStringAndPad(entry.costume_index, 3) + ".dds";
    }
    else
    {
        name = entry.code;
    }

    //printf("%s\n", name.c_str());
    SetCharaImage(name, pixmap);

    button->setIcon(QIcon(pixmap));
    button->setIconSize(QSize(CSS_ICON_WIDTH, CSS_ICON_HEIGHT));
}

void CssDialog::SetCharaImage(const std::string &str, QPixmap &pixmap)
{
    EmbFile *emb;

    if (edit_emb)
        emb = edit_emb;
    else
        emb = game_sel_port;

    if ((str == "AvatarCode" || Utils::BeginsWith(str, "HUM")) && SetRandomAvatarImage(pixmap))
        return;

    uint16_t idx;

    if (Utils::ToUpperCase(str) == "VGT_001.DDS")
        idx = emb->FindIndexByName("VGT_008.dds");   
    else
        idx = emb->FindIndexByName(str);

    if (idx == 0xFFFF)
    {
        if (Utils::ToUpperCase(str) == "TRX_007.DDS")
        {
            idx = emb->FindIndexByName("TRX_004.dds"); // Not sure waht logic this follows...
        }
        else
        {
           static const std::vector<std::string> states = { "SSG", "SS1", "GK2", "SS4", "KY1" };
           if (Utils::EndsWith(str, ".dds", false))
           {
               std::string base = str.substr(0, str.length()-4);

               for (const std::string &state : states)
               {
                   idx = emb->FindIndexByName(base + "_" + state + ".dds");
                   if (idx != 0xFFFF)
                       break;
               }
           }

           if (idx == 0xFFFF)
           {
               for (int i = 0; i < 10 && idx == 0xFFFF; i++)
               {
                   std::string name0 = str.substr(0, 3) + "_" + Utils::ToStringAndPad(i, 3) + ".dds";
                   idx = emb->FindIndexByName(name0);
               }

               if (idx == 0xFFFF)
               {
                   for (int i = 0; i < 10 && idx == 0xFFFF; i++)
                   {
                       std::string base = str.substr(0, 3) + "_" + Utils::ToStringAndPad(i, 3);

                       for (const std::string &state : states)
                       {
                           idx = emb->FindIndexByName(base + "_" + state + ".dds");
                           if (idx != 0xFFFF)
                               break;
                       }
                   }
               }
           }
        }
    }

    if (idx != 0xFFFF)
    {
        const EmbContainedFile &texture = (*emb)[idx];

        pixmap.loadFromData(texture.GetData(), (uint)texture.GetSize());

        if (pixmap.width() != 384 || pixmap.height() != 192)
        {
            pixmap = pixmap.scaled(384, 192);
        }

        pixmap = pixmap.copy(CSS_SEL_LEFT, CSS_SEL_TOP, CSS_SEL_WIDTH, CSS_SEL_HEIGHT);
    }
    else
    {
        //DPRINTF("Not found: %s\n", str.c_str());
        pixmap.load(":/unknown.dds");
        pixmap = pixmap.copy(CSS_SEL_LEFT / 2, CSS_SEL_TOP / 2, CSS_SEL_WIDTH / 2, CSS_SEL_HEIGHT / 2);
    }
}

bool CssDialog::SetRandomAvatarImage(QPixmap &pixmap)
{
    static const std::vector<std::string> avatar_images =
    {
        "HUM.dds", "HUF.dds",
        "SYM.dds", "SYF.dds",
        "NMC.dds",
        "MAM.dds", "MAF.dds",
        "FRI.dds"
    };

    static size_t random = (size_t)-1;

    if (random == (size_t)-1)
        random = (size_t)Utils::RandomInt(0, avatar_images.size());

    if (random >= avatar_images.size())
        random = avatar_images.size();

    size_t idx = game_sel_port->FindIndexByName(avatar_images[random]);
    if (idx == 0xFFFF)
        return false;

    const EmbContainedFile &texture = (*game_sel_port)[idx];

    pixmap.loadFromData(texture.GetData(), (uint)texture.GetSize());
    pixmap = pixmap.copy(CSS_SEL_LEFT, CSS_SEL_TOP, CSS_SEL_WIDTH, CSS_SEL_HEIGHT);

    return true;
}

void CssDialog::onCharacterSelect()
{    
    QPushButton *button = dynamic_cast<QPushButton *>(QObject::sender());

    if (!button)
        return;

    size_t slot_index = (size_t)button->property("slot_index").toUInt();
    result_integer = (int)slot_index;
    accept();
}

void CssDialog::AddMenuAction(const QString &title, int type, QMenu *menu, size_t entry_index, bool enabled, bool bold)
{
    QAction *action = new QAction(title, this);
    action->setProperty("action_type", QVariant(type));
    action->setProperty("entry_index", QVariant((uint)entry_index));
    action->setEnabled(enabled);

    if (bold)
    {
        QFont font = action->font();
        font.setBold(true);
        action->setFont(font);
    }

    menu->addAction(action);
}

void CssDialog::AddMenuAction2(const QString &title, int type, QMenu *menu, size_t slot_index, bool enabled, bool bold)
{
    QAction *action = new QAction(title, this);
    action->setProperty("action_type", QVariant(type));
    action->setProperty("slot_index", QVariant((uint)slot_index));
    action->setEnabled(enabled);

    if (bold)
    {
        QFont font = action->font();
        font.setBold(true);
        action->setFont(font);
    }

    menu->addAction(action);
}

void CssDialog::AddEntry(const CharaListSlotEntry &entry, size_t i, size_t max, QMenu *menu, bool insert_before)
{
    QAction *add_action = nullptr;

    if (insert_before)
    {
        for (QAction *action : menu->actions())
        {
            if (action->property("action_type") == CSS_ADD)
            {
                add_action = action;
                break;
            }
        }
    }

    QMenu *submenu = new QMenu(GetCharaAndCostumeName(entry), this);

    AddMenuAction("Select", CSS_SELECT, submenu, i);
    AddMenuAction("Swap with selected", CSS_SWAP, submenu, i, false);
    AddMenuAction("Move up", CSS_UP, submenu, i, (i != 0));
    AddMenuAction("Move down", CSS_DOWN, submenu, i, (i != (max-1)));
    AddMenuAction("Move before/after", CSS_MOVETO, submenu, i, (max>1));
    AddMenuAction("Edit", CSS_EDIT, submenu, i);
    AddMenuAction("Delete", CSS_REMOVE, submenu, i);

    QAction *costume = (add_action) ? menu->insertMenu(add_action, submenu): menu->addMenu(submenu);

    costume->setProperty("action_type", CSS_COSTUME);
    costume->setProperty("entry_index", QVariant((uint)i));

    /*static const char * green_xpm[] = {
    "32 32 1 1",
    "a      c #00FF00",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    };

    costume->setIcon(QPixmap((const char**)green_xpm ));*/
}

void CssDialog::EnableClipboard(bool enable)
{
    QList<QAction *> actions = findChildren<QAction *>();

    for (QAction *action : actions)
    {
        int type = action->property("action_type").toInt();

        if (type == CSS_SWAP || type == CSS_COPY || type == CSS_CUT)
        {
            action->setEnabled(enable);
        }
    }
}

void CssDialog::UpdateClipboard()
{
    QList<QAction *> actions = findChildren<QAction *>();

    for (QAction *action : actions)
    {
        int type = action->property("action_type").toInt();
        size_t slot_index = (size_t)action->property("slot_index").toUInt();

        if (type == CSS_SWAP || type == CSS_COPY || type == CSS_CUT)
        {
            QString text;

            if (type == CSS_SWAP)
                text = "Swap with ";
            else if (type == CSS_COPY)
                text = "Copy ";
            else
                text = "Cut ";

            if (selected_entry)
            {
                const CharaListSlotEntry &entry = edit_slots[selected_entry_slot_index].entries[selected_entry_entry_index];

                if ((type == CSS_COPY || type == CSS_CUT) && edit_slots[slot_index].entries.size() >= XV2_MAX_SUBSLOTS)
                    action->setEnabled(false);
                else
                    action->setEnabled(true);

                action->setText(text + GetCharaAndCostumeName(entry));
            }
            else
            {
                action->setEnabled(false);
                action->setText(text + "selected");
            }
        }
    }
}

void CssDialog::Swap(size_t slot_index1, size_t entry_index1, size_t slot_index2, size_t entry_index2)
{
    if (slot_index1 == slot_index2 && entry_index1 == entry_index2)
    {
        //DPRINTF("No op");
        return;
    }

    CharaListSlotEntry temp = edit_slots[slot_index1].entries[entry_index1];

    edit_slots[slot_index1].entries[entry_index1] = edit_slots[slot_index2].entries[entry_index2];
    edit_slots[slot_index2].entries[entry_index2] = temp;

    RebuildEntry(slot_index1, entry_index1);
    RebuildEntry(slot_index2, entry_index2);
}

void CssDialog::RebuildEntry(size_t slot_index, size_t entry_index)
{
    QList<QToolButton *> buttons = findChildren<QToolButton *>();

    for (QToolButton *button : buttons)
    {
        if (button->property("slot_index") == slot_index)
        {
            QList<QAction *> actions = button->menu()->actions();

            for (QAction *action : actions)
            {
                if (action->property("action_type") == CSS_COSTUME)
                {
                    if (action->property("entry_index") == entry_index)
                    {
                        const CharaListSlotEntry &entry = edit_slots[slot_index].entries[entry_index];
                        action->setText(GetCharaAndCostumeName(entry));

                        if (entry_index == 0)
                        {
                            SetCharaIcon(button, entry);
                            button->setToolTip(GetCharaName(entry));
                        }

                        QMenu *submenu = action->menu();

                        for (QAction *sub_action : submenu->actions())
                        {
                            if (sub_action->property("action_type") == CSS_DOWN)
                            {
                                if (sub_action->property("entry_index") == entry_index)
                                {
                                    sub_action->setEnabled(entry_index != (edit_slots[slot_index].entries.size()-1));
                                }
                            }
                            else if (sub_action->property("action_type") == CSS_UP)
                            {
                                if (sub_action->property("entry_index") == entry_index)
                                {
                                    sub_action->setEnabled(entry_index != 0);
                                }
                            }
                            else if (sub_action->property("action_type") == CSS_MOVETO)
                            {
                                if (sub_action->property("entry_index") == entry_index)
                                {
                                    sub_action->setEnabled(edit_slots[slot_index].entries.size() > 1);
                                }
                            }
                        }

                        break;
                    }
                } // action_type == CSS_COSTUME
            }

            break;
        }
    }
}

void CssDialog::onCharacterEditAction(QAction *action)
{
    QToolButton *button = dynamic_cast<QToolButton *>(QObject::sender());

    if (!button)
        return;

    size_t slot_index = (size_t)button->property("slot_index").toUInt();
    if (slot_index >= edit_slots.size())
    {
        DPRINTF("%s: Internal error, slot_index out of bounds.\n", FUNCNAME);
        return;
    }

    CharaListSlot &slot = edit_slots[slot_index];

    int type = action->property("action_type").toInt();

    if (type == CSS_ADD)
    {
        //DPRINTF("Add clicked.\n");
    }
    else if (type == CSS_COPY || type == CSS_CUT)
    {
        if (!selected_entry)
            return;

        if (edit_slots[slot_index].entries.size() >= XV2_MAX_SUBSLOTS)
            return; // Should not happen

        size_t new_idx = edit_slots[slot_index].entries.size();

        edit_slots[slot_index].entries.push_back(*selected_entry);
        // Update selected_entry pointer
        selected_entry = &edit_slots[selected_entry_slot_index].entries[selected_entry_entry_index];

        AddEntry(edit_slots[slot_index].entries[new_idx], new_idx, new_idx+1, button->menu(), true);

        if (type == CSS_CUT)
        {
            CharaListSlot &cut_slot = edit_slots[selected_entry_slot_index];
            bool delete_whole_slot = (cut_slot.entries.size() == 1);

            selected_entry = nullptr;

            if (delete_whole_slot)
            {
                edit_slots.erase(edit_slots.begin()+selected_entry_slot_index);
                PopulateSlots(edit_slots, CssMode::EDIT);
            }
            else
            {
                cut_slot.entries.erase(cut_slot.entries.begin()+selected_entry_entry_index);

                QToolButton *sel_button = nullptr;
                QList<QToolButton *> buttons = this->findChildren<QToolButton *>();

                for (QToolButton *b : buttons)
                {
                    if (b->property("slot_index").toUInt() == selected_entry_slot_index)
                    {
                        sel_button = b;
                        break;
                    }
                }

                if (!sel_button)
                {
                    DPRINTF("Fatal error, cannot find source button.\nProgram will exit now.\n");
                    exit(-1);
                }

                for (QAction *action : sel_button->menu()->actions())
                {
                    if (action->property("action_type").toInt() == CSS_COSTUME)
                    {
                        uint i = action->property("entry_index").toUInt();
                        QMenu *menu = action->menu();

                        if (i == selected_entry_entry_index)
                        {
                            for (QAction *menu_action : menu->actions())
                            {
                                menu->removeAction(menu_action);
                                menu_action->deleteLater();
                            }

                            sel_button->menu()->removeAction(action);
                            action->deleteLater();
                            menu->deleteLater();
                        }
                        else if (i > selected_entry_entry_index)
                        {
                            action->setProperty("entry_index", QVariant(i-1));

                            for (QAction *menu_action : menu->actions())
                            {
                                menu_action->setProperty("entry_index", QVariant(i-1));
                            }
                        }
                    }

                    if (selected_entry_entry_index == 0 || selected_entry_entry_index == cut_slot.entries.size() || cut_slot.entries.size() == 1)
                    {
                        for (size_t i = 0; i < cut_slot.entries.size(); i++)
                        {
                            RebuildEntry(selected_entry_slot_index, i);
                        }
                    }
                }

                if (selected_entry_entry_index == 0)
                    RebuildEntry(selected_entry_slot_index, 0);

            }

        } // END CSS_CUT

        UpdateClipboard();
    }
    else if (type == CSS_MANUAL)
    {
        CharaListSlotEntry new_entry;
        SlotEditDialog dialog(&new_entry, edit_emb, this);

        dialog.setWindowTitle("Add entry");

        if (dialog.exec())
        {
            size_t new_idx = edit_slots[slot_index].entries.size();

            edit_slots[slot_index].entries.push_back(new_entry);
            // Update selected_entry pointer
            if (selected_entry)
                selected_entry = &edit_slots[selected_entry_slot_index].entries[selected_entry_entry_index];

            AddEntry(edit_slots[slot_index].entries[new_idx], new_idx, new_idx+1, button->menu(), true);
            UpdateClipboard();
        }
    }
    else
    {
        size_t entry_index = (size_t)action->property("entry_index").toUInt();
        if (entry_index >= slot.entries.size())
        {
            DPRINTF("%s: Internal error, entry_index out of bounds (%Id).\n", FUNCNAME, entry_index);
            return;
        }

        CharaListSlotEntry &entry = slot.entries[entry_index];

        if (type == CSS_SELECT)
        {
            selected_entry = &entry;
            selected_entry_slot_index = slot_index;
            selected_entry_entry_index = entry_index;
            UpdateClipboard();
        }

        else if (type == CSS_SWAP)
        {
            if (!selected_entry)
                return; // This shouldh happen

            Swap(slot_index, entry_index, selected_entry_slot_index, selected_entry_entry_index);

            selected_entry = nullptr;
            UpdateClipboard();
        }

        else if (type == CSS_UP)
        {
            if (entry_index == 0)
                return; // shouldn't happen

            Swap(slot_index, entry_index, slot_index, entry_index-1);
        }
        else if (type == CSS_DOWN)
        {
            if (entry_index >= (slot.entries.size()-1))
                return; // shouldn't happen

            Swap(slot_index, entry_index, slot_index, entry_index+1);
        }
        else if (type == CSS_MOVETO)
        {
            CharaListSlot copy = slot;
            SlotReorderDialog dialog(&copy, entry_index, this);

            dialog.exec();

            if (copy != slot)
            {
                slot = copy;

                if (selected_entry && selected_entry_slot_index == slot_index)
                {
                    selected_entry = nullptr;
                    UpdateClipboard();
                }

                for (size_t i = 0; i < slot.entries.size(); i++)
                {
                    RebuildEntry(slot_index, i);
                }
            }
        }
        else if (type == CSS_EDIT)
        {
            CharaListSlotEntry edited_entry = entry; // Make copy
            SlotEditDialog dialog(&edited_entry, edit_emb, this);

            dialog.setWindowTitle("Edit entry");

            if (dialog.exec())
            {
                entry = edited_entry; // Commit
                RebuildEntry(slot_index, entry_index);
            }

        }
        else if (type == CSS_REMOVE)
        {
            bool delete_whole_slot = false;
            QString message;
            QMessageBox::StandardButton default_button = QMessageBox::Yes;

            if (slot.entries.size() == 1)
            {
                delete_whole_slot = true;
                message = "This is the last entry in the slot. If you delete it, the whole slot will be deleted.\n"
                          "Are you sure you want to continue?";

                default_button = QMessageBox::No;
            }
            else
            {
                message = "Do you really want to delete this entry?";
            }

            if (QMessageBox::warning(this, "Delete?", message,
                                      QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No), default_button)
                    != QMessageBox::Yes)
            {
                return;
            }

            if (delete_whole_slot)
            {
                if (selected_entry && slot_index == selected_entry_slot_index)
                {
                    selected_entry = nullptr;
                }

                edit_slots.erase(edit_slots.begin()+slot_index);
                PopulateSlots(edit_slots, CssMode::EDIT);

                if (selected_entry)
                {
                    if (selected_entry_slot_index > slot_index)
                    {
                        selected_entry_slot_index--;
                    }

                    selected_entry = &edit_slots[selected_entry_slot_index].entries[selected_entry_entry_index];
                }
            }
            else
            {
                if (selected_entry && slot_index == selected_entry_slot_index && entry_index == selected_entry_entry_index)
                {
                    selected_entry = nullptr;                    
                }

                slot.entries.erase(slot.entries.begin()+entry_index);

                for (QAction *action : button->menu()->actions())
                {
                    if (action->property("action_type").toInt() == CSS_COSTUME)
                    {
                        uint i = action->property("entry_index").toUInt();
                        QMenu *menu = action->menu();

                        if (i == entry_index)
                        {
                            for (QAction *menu_action : menu->actions())
                            {
                                menu->removeAction(menu_action);
                                menu_action->deleteLater();
                            }

                            button->menu()->removeAction(action);
                            action->deleteLater();
                            menu->deleteLater();
                        }
                        else if (i > entry_index)
                        {
                            action->setProperty("entry_index", QVariant(i-1));

                            for (QAction *menu_action : menu->actions())
                            {
                                menu_action->setProperty("entry_index", QVariant(i-1));
                            }
                        }
                    }

                    if (entry_index == 0 || entry_index == slot.entries.size() || slot.entries.size() == 1)
                    {
                        for (size_t i = 0; i < slot.entries.size(); i++)
                        {
                            RebuildEntry(slot_index, i);
                        }
                    }
                }

                if (selected_entry)
                {
                    if (selected_entry_slot_index == slot_index && selected_entry_entry_index > entry_index)
                    {
                        selected_entry_entry_index--;                        
                    }

                    selected_entry = &edit_slots[selected_entry_slot_index].entries[selected_entry_entry_index];
                }

                if (entry_index == 0)
                    RebuildEntry(slot_index, 0);

            }

            UpdateClipboard();
        }        
    }
}

void CssDialog::onSwapPositions(int src, int dst)
{
    if (src < 0 || src >= (int)edit_slots.size())
        return;

    if (dst < 0 || dst >= (int)edit_slots.size())
        return;

    if (src == dst)
        return;

    CharaListSlot temp = edit_slots[src];

    edit_slots[src] = edit_slots[dst];
    edit_slots[dst] = temp;

    PopulateSlots(edit_slots, CssMode::EDIT);
}

void CssDialog::onMoveSlot(int src, int dst)
{
    if (src < 0 || src >= (int)edit_slots.size())
        return;

    if (dst < 0 || dst > (int)edit_slots.size()) // >, not >=
        return;

    if (src == dst)
        return;

    CharaListSlot temp = edit_slots[src];

    if (dst == (int)edit_slots.size())
    {
        edit_slots.push_back(temp);
    }
    else
    {
        edit_slots.insert(edit_slots.begin()+dst, temp);
    }

    if (dst < src)
        src++;

    edit_slots.erase(edit_slots.begin()+src);
    PopulateSlots(edit_slots, CssMode::EDIT);
}

void CssDialog::onMergeSlot(int src, int dst)
{
    if (src < 0 || src >= (int)edit_slots.size())
        return;

    if (dst < 0 || dst >= (int)edit_slots.size())
        return;

    if (src == dst)
        return;

    CharaListSlot src_slot = edit_slots[src]; // copy
    CharaListSlot &dst_slot = edit_slots[dst]; // reference

    if (src_slot.entries.size() + dst_slot.entries.size() > XV2_MAX_SUBSLOTS)
    {
        DPRINTF("Cannot merge because it would exceed %d entries.\n", XV2_MAX_SUBSLOTS);
        return;
    }

    for (size_t i = 0; i < src_slot.entries.size(); i++)
    {
        dst_slot.entries.push_back(src_slot.entries[i]);
    }

    edit_slots.erase(edit_slots.begin()+src);
    PopulateSlots(edit_slots, CssMode::EDIT);
}

void CssDialog::on_addButton_clicked()
{
    CharaListSlot slot;
    slot.entries.resize(1);

    SlotEditDialog dialog(&slot.entries[0], edit_emb, this);
    dialog.setWindowTitle("Add first entry of new slot");

    if (dialog.exec())
    {
        edit_slots.push_back(slot);

        if (selected_entry)
            selected_entry = &edit_slots[selected_entry_slot_index].entries[selected_entry_entry_index];

        PopulateSlots(edit_slots, CssMode::EDIT);
    }
}

void CssDialog::on_cancelButton_clicked()
{
    reject();
}


