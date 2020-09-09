#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>

#include "Config.h"
#include "Xenoverse2.h"
#include "Cac2X2m.h"
#include "listdialog.h"
#include "x2mconvdialog.h"
#include "waitdialog.h"
#include "exportcacdialog.h"
#include "colordialog.h"
#include "debug.h"

#define PROGRAM_NAME    "Cycit"
#define PROGRAM_VERSION "1.21"

#define CUSTOM_MIN  5108
#define CUSTOM_MAX  9800

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    GameRequirements();

    // Actions
    ui->copyButton->addAction(ui->actionFromOtherEntry);
    ui->copyButton->addAction(ui->actionFromExternalPAL);
    ui->copyButton->addAction(ui->actionFromCAC);
    ui->copyButton->addAction(ui->actionFromX2m);
    ui->infoCopyButton->addAction(ui->actionFromOtherEntryInfo);
    ui->infoCopyButton->addAction(ui->actionFromExternalPAInfo);
    ui->infoCopyButton->addAction(ui->actionFromCACInfo);
    ui->infoCopyButton->addAction(ui->actionFromX2mInfo);
    ui->equipCopyButton->addAction(ui->actionFromOtherEntryEquip);
    ui->equipCopyButton->addAction(ui->actionFromExternalPALEquip);
    ui->equipCopyButton->addAction(ui->actionFromCACEquip);
    ui->equipCopyButton->addAction(ui->actionFromX2mEquip);
    ui->statsCopyButton->addAction(ui->actionFromOtherEntryStats);
    ui->statsCopyButton->addAction(ui->actionFromExternalPALStats);
    ui->statsCopyButton->addAction(ui->actionFromCACStats);
    ui->statsCopyButton->addAction(ui->actionFromX2mStats);

    // Info tab
    ui->voiceEdit->setValidator(new QIntValidator(-32768, 32767, this));
    FillCmsComboBox();

    // Equipment tab
    ui->faceTopEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->faceBottomEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->eyesEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->noseEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->earsEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->hairEdit->setValidator(new QIntValidator(-32768, 32767, this));
    FillTopComboBox();
    FillBottomComboBox();
    FillGlovesComboBox();
    FillShoesComboBox();
    ui->bodyShapeEdit->setValidator(new QIntValidator(-32768, 32767, this));
    FillAccesoryComboBox();
    FillTalismanComboBox();

    // Stats tab
    ui->levelEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->u0EEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->heaEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->kiEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->stmEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->atkEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->strEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->blaEdit->setValidator(new QIntValidator(-32768, 32767, this));
    FillSuperComboBox(ui->super1ComboBox);
    FillSuperComboBox(ui->super2ComboBox);
    FillSuperComboBox(ui->super3ComboBox);
    FillSuperComboBox(ui->super4ComboBox);
    FillUltimateComboBox(ui->ultimate1ComboBox);
    FillUltimateComboBox(ui->ultimate2ComboBox);
    FillEvasiveComboBox();
    FillBlastComboBox();
    FillAwakenComboBox();

    //
    Load();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GameRequirements()
{
    config.Load();

    if (!config.ConfigureRequirements())
    {
        DPRINTF("ConfigureRequirements failed.\n");
        exit(-1);
    }

    Xenoverse2::InitFs(Utils::QStringToStdString(config.game_directory));

    if (!Xenoverse2::InitSystemFiles())
    {
        DPRINTF("InitSystemFiles failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitSkillNames())
    {
        DPRINTF("InitSkillNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitLobbyText())
    {
        DPRINTF("InitLobbyText failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitCacCostumeNames())
    {
        DPRINTF("InitCacCostumeNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitTalismanNames())
    {
        DPRINTF("InitTalismanNames failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitIdb())
    {
        DPRINTF("InitIdb failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitLobby())
    {
        DPRINTF("InitLobby failed.\n");
        exit(-1);
    }

    if (!Xenoverse2::InitSelPort())
    {
        DPRINTF("InitSelPort failed.\n");
        exit(-1);
    }

    // Added in 0.7, needed for the color preview/selection
    if (!Xenoverse2::InitCac())
    {
        DPRINTF("InitCac failed.\n");
        exit(-1);
    }
}

bool MainWindow::ProcessShutdown()
{
    config.Save();
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ProcessShutdown())
        event->accept();
    else
        event->ignore();
}

void MainWindow::FillCmsComboBox()
{
    ui->cmsComboBox->addItem("HUM", QVariant(0x64));
    ui->cmsComboBox->addItem("HUF", QVariant(0x65));
    ui->cmsComboBox->addItem("SYM", QVariant(0x66));
    ui->cmsComboBox->addItem("SYF", QVariant(0x67));
    ui->cmsComboBox->addItem("NMC", QVariant(0x68));
    ui->cmsComboBox->addItem("FRI", QVariant(0x69));
    ui->cmsComboBox->addItem("MAM", QVariant(0x6a));
    ui->cmsComboBox->addItem("MAF", QVariant(0x6b));
}

void MainWindow::FillTopComboBox()
{
    ui->topComboBox->addItem("None", QVariant(-1));

    for (const IdbEntry &entry : *game_top_idb)
    {
        std::string name = Xenoverse2::GetCacTopName(entry.id);

        if (name.length() == 0)
            name = Utils::ToString(entry.id);

        ui->topComboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)entry.id));
    }
}

void MainWindow::FillBottomComboBox()
{
    ui->bottomComboBox->addItem("None", QVariant(-1));

    for (const IdbEntry &entry : *game_bottom_idb)
    {
        std::string name = Xenoverse2::GetCacBottomName(entry.id);

        if (name.length() == 0)
            name = Utils::ToString(entry.id);

        ui->bottomComboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)entry.id));
    }
}

void MainWindow::FillGlovesComboBox()
{
    ui->glovesComboBox->addItem("None", QVariant(-1));

    for (const IdbEntry &entry : *game_gloves_idb)
    {
        std::string name = Xenoverse2::GetCacGlovesName(entry.id);

        if (name.length() == 0)
            name = Utils::ToString(entry.id);

        ui->glovesComboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)entry.id));
    }
}

void MainWindow::FillShoesComboBox()
{
    ui->shoesComboBox->addItem("None", QVariant(-1));

    for (const IdbEntry &entry : *game_shoes_idb)
    {
        std::string name = Xenoverse2::GetCacShoesName(entry.id);

        if (name.length() == 0)
            name = Utils::ToString(entry.id);

        ui->shoesComboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)entry.id));
    }
}

void MainWindow::FillAccesoryComboBox()
{
    ui->accesoryComboBox->addItem("None", QVariant(-1));

    for (const IdbEntry &entry : *game_accesory_idb)
    {
        std::string name = Xenoverse2::GetCacAccesoryName(entry.id);

        if (name.length() == 0)
            name = Utils::ToString(entry.id);

        ui->accesoryComboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)entry.id));
    }
}

void MainWindow::FillTalismanComboBox()
{
    ui->talismanComboBox->addItem("None", QVariant(-1));

    for (const IdbEntry &entry : *game_talisman_idb)
    {
        std::string name = Xenoverse2::GetTalismanNameEx(entry.id);

        if (name.length() == 0)
            name = Utils::ToString(entry.id);

        ui->talismanComboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)entry.id));
    }
}

void MainWindow::FillSuperComboBox(QComboBox *comboBox)
{
    const std::vector<CusSkill> &skills = game_cus->GetSuperSkills();

    comboBox->addItem("None", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetSuperSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id2));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)skill.id2));
        }
    }
}

void MainWindow::FillUltimateComboBox(QComboBox *comboBox)
{
    const std::vector<CusSkill> &skills = game_cus->GetUltimateSkills();

    comboBox->addItem("None", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetUltimateSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id2));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)skill.id2));
        }
    }
}

void MainWindow::FillEvasiveComboBox()
{
    QComboBox *comboBox = ui->evasiveComboBox;

    const std::vector<CusSkill> &skills = game_cus->GetEvasiveSkills();

    comboBox->addItem("None", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetEvasiveSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id2));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)skill.id2));
        }
    }
}

void MainWindow::FillBlastComboBox()
{
    QComboBox *comboBox = ui->blastComboBox;

    const std::vector<CusSkill> &skills = game_cus->GetBlastSkills();

    comboBox->addItem("None", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id2));
    }
}

void MainWindow::FillAwakenComboBox()
{
    QComboBox *comboBox = ui->awakenComboBox;

    const std::vector<CusSkill> &skills = game_cus->GetAwakenSkills();

    comboBox->addItem("None", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetAwakenSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id2));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id2));
        }
    }
}

void MainWindow::SetComboBox(QComboBox *comboBox, int16_t id)
{
    for (int i = 0; i < comboBox->count(); i++)
    {
        if (comboBox->itemData(i).toInt() == id)
        {
            comboBox->setCurrentIndex(i);
            return;
        }
    }

    // Not found, let's add an entry and push it
    comboBox->addItem(QString("%1").arg(id), QVariant(id));
    comboBox->setCurrentIndex(comboBox->count()-1);
}

void MainWindow::InfoToGui(const PalEntry &entry)
{
    int lang = ui->langComboBox->currentIndex();

    if (lang >= 0 && lang < XV2_LANG_NUM)
    {
        std::string name;

        if (Xenoverse2::GetLobbyName(entry.name_id, name, lang))
        {
            ui->nameEdit->setText(Utils::StdStringToQString(name, false));
        }
        else
        {
            ui->nameEdit->setText("");
        }
    }
    else
    {
        ui->nameEdit->setText("");
    }

    SetComboBox(ui->cmsComboBox, entry.cms_entry);
    ui->voiceEdit->setText(QString("%1").arg((int16_t)entry.voice));
    //ui->teamMateCheck->setChecked((entry.team_mate != 0));

    if (entry.team_mate >= 3)
    {
        ui->typeComboBox->setCurrentIndex(3);
    }
    else
    {
        ui->typeComboBox->setCurrentIndex(entry.team_mate);
    }
}

void MainWindow::GuiToInfo(PalEntry &entry)
{
    // Name is done via text edited event, so we ignore it here
    entry.cms_entry = (uint16_t) ui->cmsComboBox->currentData().toInt();
    entry.voice = (uint16_t) ui->voiceEdit->text().toInt();
    //entry.team_mate = ui->teamMateCheck->isChecked();
    entry.team_mate = ui->typeComboBox->currentIndex();
}

void MainWindow::BcsColorToButton(const std::string &part, uint16_t color, uint16_t cms_entry, QPushButton *button)
{
    BcsFile *bcs;
    bool has_multiple_skin = false;

    switch (cms_entry)
    {
        case 0x64: case 0x66:
            bcs = game_hum_bcs;
        break;

        case 0x65: case 0x67:
            bcs = game_huf_bcs;
        break;

        case 0x68:
            bcs = game_nmc_bcs;
            has_multiple_skin = true;
        break;

        case 0x69:
            bcs = game_fri_bcs;
            has_multiple_skin = true;
        break;

        case 0x6A:
            bcs = game_mam_bcs;
        break;

        case 0x6B:
            bcs = game_maf_bcs;
        break;

        default:
            return;
    }

    std::string part_name = part;

    if (part_name == "SKIN_" && has_multiple_skin)
        part_name = "SKIN_A_";

    button->setProperty("bcs_color", QVariant(color));

    uint32_t preview_color;

    if (color >= 0x8000 || !bcs->GetPreviewColor(part_name, color, &preview_color))
    {
        button->setText("NO COLOR");
        button->setStyleSheet("");
        button->setToolTip("No color");
    }
    else
    {
        button->setText("");

        QString style = "background-color: " + Utils::StdStringToQString(Utils::ColorToString(preview_color)) + ";";
        button->setStyleSheet(style);
        button->setAutoFillBackground(true);

        QString tooltip = QString("%1").arg(color) + ": #" + QString("%1").arg((uint)preview_color, (int)6, (int)16, QChar('0'));
        button->setToolTip(tooltip);
    }

    button->update();
}

uint16_t MainWindow::ButtonToBcsColor(QPushButton *button)
{
    return button->property("bcs_color").toUInt();
}

void MainWindow::EquipmentToGui(const PalEquipment &entry, uint16_t cms_entry)
{
    ui->faceTopEdit->setText(QString("%1").arg((int16_t)entry.face_forehead));
    ui->faceBottomEdit->setText(QString("%1").arg((int16_t)entry.face_base));
    ui->eyesEdit->setText(QString("%1").arg((int16_t)entry.eyes));
    ui->noseEdit->setText(QString("%1").arg((int16_t)entry.nose));
    ui->earsEdit->setText(QString("%1").arg((int16_t)entry.ears));
    ui->hairEdit->setText(QString("%1").arg((int16_t)entry.hair));
    SetComboBox(ui->topComboBox, entry.top);
    SetComboBox(ui->bottomComboBox, entry.bottom);
    SetComboBox(ui->glovesComboBox, entry.gloves);
    SetComboBox(ui->shoesComboBox, entry.shoes);
    ui->bodyShapeEdit->setText(QString("%1").arg((int16_t)entry.body_shape));

    BcsColorToButton("SKIN_", entry.skin_color1, cms_entry, ui->skinColor1Button);
    BcsColorToButton("SKIN_B_", entry.skin_color2, cms_entry, ui->skinColor2Button);
    BcsColorToButton("SKIN_C_", entry.skin_color3, cms_entry, ui->skinColor3Button);
    BcsColorToButton("SKIN_D_", entry.skin_color4, cms_entry, ui->skinColor4Button);

    BcsColorToButton("HAIR_", entry.hair_color, cms_entry, ui->hairColorButton);
    BcsColorToButton("eye_", entry.eye_color, cms_entry, ui->eyesColorButton);

    BcsColorToButton("CC00_BUST_", entry.top_color1, cms_entry, ui->topColor1Button);
    BcsColorToButton("CC01_BUST_", entry.top_color2, cms_entry, ui->topColor2Button);
    BcsColorToButton("CC02_BUST_", entry.top_color3, cms_entry, ui->topColor3Button);
    BcsColorToButton("CC03_BUST_", entry.top_color4, cms_entry, ui->topColor4Button);

    BcsColorToButton("CC00_PANTS_", entry.bottom_color1, cms_entry, ui->bottomColor1Button);
    BcsColorToButton("CC01_PANTS_", entry.bottom_color2, cms_entry, ui->bottomColor2Button);
    BcsColorToButton("CC02_PANTS_", entry.bottom_color3, cms_entry, ui->bottomColor3Button);
    BcsColorToButton("CC03_PANTS_", entry.bottom_color4, cms_entry, ui->bottomColor4Button);

    BcsColorToButton("CC00_RIST_", entry.gloves_color1, cms_entry, ui->glovesColor1Button);
    BcsColorToButton("CC01_RIST_", entry.gloves_color2, cms_entry, ui->glovesColor2Button);
    BcsColorToButton("CC02_RIST_", entry.gloves_color3, cms_entry, ui->glovesColor3Button);
    BcsColorToButton("CC03_RIST_", entry.gloves_color4, cms_entry, ui->glovesColor4Button);

    BcsColorToButton("CC00_BOOTS_", entry.shoes_color1, cms_entry, ui->shoesColor1Button);
    BcsColorToButton("CC01_BOOTS_", entry.shoes_color2, cms_entry, ui->shoesColor2Button);
    BcsColorToButton("CC02_BOOTS_", entry.shoes_color3, cms_entry, ui->shoesColor3Button);
    BcsColorToButton("CC03_BOOTS_", entry.shoes_color4, cms_entry, ui->shoesColor4Button);

    BcsColorToButton("PAINT_A_", entry.makeup_color1, cms_entry, ui->makeupColor1Button);
    BcsColorToButton("PAINT_B_", entry.makeup_color2, cms_entry, ui->makeupColor2Button);
    BcsColorToButton("PAINT_C_", entry.makeup_color3, cms_entry, ui->makeupColor3Button);

    SetComboBox(ui->accesoryComboBox, entry.accesory);
    SetComboBox(ui->talismanComboBox, entry.talisman);
}

void MainWindow::GuiToEquipment(PalEquipment &entry)
{
    entry.unk_16 = 0;

    entry.face_forehead = (uint16_t) ui->faceTopEdit->text().toInt();
    entry.face_base = (uint16_t) ui->faceBottomEdit->text().toInt();
    entry.eyes = (uint16_t) ui->eyesEdit->text().toInt();
    entry.nose = (uint16_t) ui->noseEdit->text().toInt();
    entry.ears = (uint16_t) ui->earsEdit->text().toInt();
    entry.hair = (uint16_t) ui->hairEdit->text().toInt();
    entry.top = (uint16_t) ui->topComboBox->currentData().toInt();
    entry.bottom = (uint16_t) ui->bottomComboBox->currentData().toInt();
    entry.gloves = (uint16_t) ui->glovesComboBox->currentData().toInt();
    entry.shoes = (uint16_t) ui->shoesComboBox->currentData().toInt();
    entry.body_shape = (uint16_t) ui->bodyShapeEdit->text().toInt();
    entry.skin_color1 = ButtonToBcsColor(ui->skinColor1Button);
    entry.skin_color2 = ButtonToBcsColor(ui->skinColor2Button);
    entry.skin_color3 = ButtonToBcsColor(ui->skinColor3Button);
    entry.skin_color4 = ButtonToBcsColor(ui->skinColor4Button);
    entry.hair_color = ButtonToBcsColor(ui->hairColorButton);
    entry.eye_color = ButtonToBcsColor(ui->eyesColorButton);
    entry.top_color1 = ButtonToBcsColor(ui->topColor1Button);
    entry.top_color2 = ButtonToBcsColor(ui->topColor2Button);
    entry.top_color3 = ButtonToBcsColor(ui->topColor3Button);
    entry.top_color4 = ButtonToBcsColor(ui->topColor4Button);
    entry.bottom_color1 = ButtonToBcsColor(ui->bottomColor1Button);
    entry.bottom_color2 = ButtonToBcsColor(ui->bottomColor2Button);
    entry.bottom_color3 = ButtonToBcsColor(ui->bottomColor3Button);
    entry.bottom_color4 = ButtonToBcsColor(ui->bottomColor4Button);
    entry.gloves_color1 = ButtonToBcsColor(ui->glovesColor1Button);
    entry.gloves_color2 = ButtonToBcsColor(ui->glovesColor2Button);
    entry.gloves_color3 = ButtonToBcsColor(ui->glovesColor3Button);
    entry.gloves_color4 = ButtonToBcsColor(ui->glovesColor4Button);
    entry.shoes_color1 = ButtonToBcsColor(ui->shoesColor1Button);
    entry.shoes_color2 = ButtonToBcsColor(ui->shoesColor2Button);
    entry.shoes_color3 = ButtonToBcsColor(ui->shoesColor3Button);
    entry.shoes_color4 = ButtonToBcsColor(ui->shoesColor4Button);
    entry.makeup_color1 = ButtonToBcsColor(ui->makeupColor1Button);
    entry.makeup_color2 = ButtonToBcsColor(ui->makeupColor2Button);
    entry.makeup_color3 = ButtonToBcsColor(ui->makeupColor3Button);
    entry.accesory = (uint16_t) ui->accesoryComboBox->currentData().toInt();
    entry.talisman = (uint16_t) ui->talismanComboBox->currentData().toInt();
}

void MainWindow::StatsToGui(const PalStats &stats)
{
    ui->levelEdit->setText(QString("%1").arg((int16_t)stats.level));
    ui->u0EEdit->setText(QString("%1").arg((int16_t)stats.unk_0E));
    ui->heaEdit->setText(QString("%1").arg((int16_t)stats.hea));
    ui->kiEdit->setText(QString("%1").arg((int16_t)stats.ki));
    ui->stmEdit->setText(QString("%1").arg((int16_t)stats.stm));
    ui->atkEdit->setText(QString("%1").arg((int16_t)stats.atk));
    ui->strEdit->setText(QString("%1").arg((int16_t)stats.str));
    ui->blaEdit->setText(QString("%1").arg((int16_t)stats.bla));
    SetComboBox(ui->super1ComboBox, stats.super_skills[0]);
    SetComboBox(ui->super2ComboBox, stats.super_skills[1]);
    SetComboBox(ui->super3ComboBox, stats.super_skills[2]);
    SetComboBox(ui->super4ComboBox, stats.super_skills[3]);
    SetComboBox(ui->ultimate1ComboBox, stats.ult_skills[0]);
    SetComboBox(ui->ultimate2ComboBox, stats.ult_skills[1]);
    SetComboBox(ui->evasiveComboBox, stats.evasive_skill);
    SetComboBox(ui->blastComboBox, stats.blast_skill);
    SetComboBox(ui->awakenComboBox, stats.awaken_skill);
}

void MainWindow::GuiToStats(PalStats &stats)
{
    stats.level = (uint16_t) ui->levelEdit->text().toInt();
    stats.unk_0E = (uint16_t) ui->u0EEdit->text().toInt();
    stats.hea = (uint16_t) ui->heaEdit->text().toInt();
    stats.ki = (uint16_t) ui->kiEdit->text().toInt();
    stats.stm = (uint16_t) ui->stmEdit->text().toInt();
    stats.atk = (uint16_t) ui->atkEdit->text().toInt();
    stats.str = (uint16_t) ui->strEdit->text().toInt();
    stats.bla = (uint16_t) ui->blaEdit->text().toInt();
    stats.super_skills[0] = (uint16_t) ui->super1ComboBox->currentData().toInt();
    stats.super_skills[1] = (uint16_t) ui->super2ComboBox->currentData().toInt();
    stats.super_skills[2] = (uint16_t) ui->super3ComboBox->currentData().toInt();
    stats.super_skills[3] = (uint16_t) ui->super4ComboBox->currentData().toInt();
    stats.ult_skills[0] = (uint16_t) ui->ultimate1ComboBox->currentData().toInt();
    stats.ult_skills[1] = (uint16_t) ui->ultimate2ComboBox->currentData().toInt();
    stats.evasive_skill = (uint16_t) ui->evasiveComboBox->currentData().toInt();
    stats.blast_skill = (uint16_t) ui->blastComboBox->currentData().toInt();
    stats.awaken_skill = (uint16_t) ui->awakenComboBox->currentData().toInt();
}

void MainWindow::EntryToGui(const PalEntry &entry)
{
    InfoToGui(entry);
    EquipmentToGui(entry.equipment, entry.cms_entry);
    StatsToGui(entry.stats);
}

void MainWindow::GuiToEntry(PalEntry &entry)
{
    GuiToInfo(entry);
    GuiToEquipment(entry.equipment);
    GuiToStats(entry.stats);
}

bool MainWindow::AddNewName(PalEntry &entry)
{
    for (int i = CUSTOM_MIN; i < CUSTOM_MAX; i++)
    {
        if (game_pal->FindEntryByID(i))
            continue;

        bool select = true;

        for (int j = 0; j < XV2_LANG_NUM; j++)
        {
            MsgFile *msg = game_lobby_texts[j];
            std::string name = "name_" + Utils::ToStringAndPad(i, 4);

            if (msg->FindEntryByName(name))
            {
                select = false;
                break;
            }
        }

        if (select)
        {
            for (int j = 0; j < XV2_LANG_NUM; j++)
            {
                std::string name;

                Xenoverse2::GetLobbyName(entry.name_id, name, j);
                Xenoverse2::SetLobbyName(i, name, j);
            }

            entry.id = entry.name_id = i;
            return true;
        }
    }

    DPRINTF("You have reached the limit of characters to add, cannot add more!\n");
    return false;
}

bool MainWindow::CacToInfo(const XV2Cac *cac, PalEntry &entry)
{
    CmsEntry *cms_entry = nullptr;

    switch (cac->race)
    {
        case CAC_HUM:
            cms_entry = game_cms->FindEntryByName("HUM");
        break;

        case CAC_HUF:
            cms_entry = game_cms->FindEntryByName("HUF");
        break;

        case CAC_SYM:
            cms_entry = game_cms->FindEntryByName("SYM");
        break;

        case CAC_SYF:
            cms_entry = game_cms->FindEntryByName("SYF");
        break;

        case CAC_NMC:
            cms_entry = game_cms->FindEntryByName("NMC");
        break;

        case CAC_FRI:
            cms_entry = game_cms->FindEntryByName("FRI");
        break;

        case CAC_MAM:
            cms_entry = game_cms->FindEntryByName("MAM");
        break;

        case CAC_MAF:
            cms_entry = game_cms->FindEntryByName("MAF");
        break;
    }

    if (!cms_entry)
    {
        DPRINTF("%s: Cannot find cms entry or save had corrupted or unknown race data.\n", FUNCNAME);
        return false;
    }

    entry.cms_entry = cms_entry->id;
    entry.voice = cac->voice;

    return true;
}

bool MainWindow::CacToEquipment(const XV2Cac *cac, uint32_t set, PalEquipment &equipment)
{
    if (set >= XV2_NUM_PRESETS)
        return false;

    const XV2CacSet *cac_set = &cac->sets[set];

    equipment.face_base = cac->face_base;
    equipment.face_forehead = cac->face_forehead;
    equipment.eyes = cac->eyes;
    equipment.nose = cac->nose;
    equipment.ears = cac->ears;
    equipment.hair = cac->hair;
    equipment.top = cac_set->top;
    equipment.bottom = cac_set->bottom;
    equipment.gloves = cac_set->gloves;
    equipment.shoes = cac_set->shoes;

    equipment.body_shape = cac->body_shape;
    equipment.unk_16 = 0;
    equipment.skin_color1 = cac->skin_color1;
    equipment.skin_color2 = cac->skin_color2;
    equipment.skin_color3 = cac->skin_color3;
    equipment.skin_color4 = cac->skin_color4;
    equipment.hair_color = cac->hair_color;
    equipment.eye_color = cac->eye_color;

    equipment.top_color1 = cac_set->top_color1;
    equipment.top_color2 = cac_set->top_color2;
    equipment.top_color3 = cac_set->top_color3;
    equipment.top_color4 = cac_set->top_color4;
    equipment.bottom_color1 = cac_set->bottom_color1;
    equipment.bottom_color2 = cac_set->bottom_color2;
    equipment.bottom_color3 = cac_set->bottom_color3;
    equipment.bottom_color4 = cac_set->bottom_color4;
    equipment.gloves_color1 = cac_set->gloves_color1;
    equipment.gloves_color2 = cac_set->gloves_color2;
    equipment.gloves_color3 = cac_set->gloves_color3;
    equipment.gloves_color4 = cac_set->gloves_color4;
    equipment.shoes_color1 = cac_set->shoes_color1;
    equipment.shoes_color2 = cac_set->shoes_color2;
    equipment.shoes_color3 = cac_set->shoes_color3;
    equipment.shoes_color4 = cac_set->shoes_color4;

    equipment.makeup_color1 = cac->makeup_color1;
    equipment.makeup_color2 = cac->makeup_color2;
    equipment.makeup_color3 = cac->makeup_color3;
    equipment.accesory = cac_set->accesory;
    equipment.talisman = cac_set->talisman;

    return true;
}

bool MainWindow::CacToStats(const XV2Cac *cac, uint32_t set, PalStats &stats)
{
    if (set >= XV2_NUM_PRESETS)
        return false;

    const XV2CacSet *cac_set = &cac->sets[set];

    stats.level = cac->level;
    stats.hea = cac->hea;
    stats.ki = cac->ki;
    stats.stm = cac->stm;
    stats.atk = cac->atk;
    stats.str = cac->str;
    stats.bla = cac->bla;
    stats.unk_0E = cac->unk_9C[0]; // this is a wild guess, we don't know what this is

    for (int i = 0; i < 4; i++)
    {
        const CusSkill *skill = game_cus->FindSuperSkillByID(cac_set->super_skills[i]);

        if (!skill)
            stats.super_skills[i] = 0xFFFF;
        else
            stats.super_skills[i] = skill->id2;
    }    

    for (int i = 0; i < 2; i++)
    {
        const CusSkill *skill = game_cus->FindUltimateSkillByID(cac_set->ult_skills[i]);

        if (!skill)
            stats.ult_skills[i] = 0xFFFF;
        else
            stats.ult_skills[i] = skill->id2;
    }     

    {
        const CusSkill *skill = game_cus->FindEvasiveSkillByID(cac_set->evasive_skill);

        if (!skill)
            stats.evasive_skill = 0xFFFF;
        else
            stats.evasive_skill = skill->id2;
    }   

    {
        stats.blast_skill = 1081;

        if (cac_set->talisman != 0xFFFFFFFF)
        {
            uint32_t blast = Xenoverse2::GetBlastFromTalisman(cac_set->talisman, true);
            if (blast != (uint32_t)-1)
                stats.blast_skill = blast;

        }
    }

    {
        const CusSkill *skill = game_cus->FindAwakenSkillByID(cac_set->awaken_skill);

        if (!skill)
            stats.awaken_skill = 0xFFFF;
        else
            stats.awaken_skill = skill->id2;
    }

    return true;
}

bool MainWindow::CacToEntry(const XV2Cac *cac, uint32_t set, PalEntry &entry)
{
    if (!CacToInfo(cac, entry))
        return false;    

    if (!CacToEquipment(cac, set, entry.equipment))
        return false;    

    if (!CacToStats(cac, set, entry.stats))
        return false;    

    return true;
}

bool MainWindow::InfoToCac(const PalEntry &entry, XV2Cac *cac)
{
    if (entry.cms_entry == 0x64)
    {
        cac->race = CAC_HUM;
    }
    else if (entry.cms_entry == 0x65)
    {
        cac->race = CAC_HUF;
    }
    else if (entry.cms_entry == 0x66)
    {
        cac->race = CAC_SYM;
    }
    else if (entry.cms_entry == 0x67)
    {
        cac->race = CAC_SYF;
    }
    else if (entry.cms_entry == 0x68)
    {
        cac->race = CAC_NMC;
    }
    else if (entry.cms_entry == 0x69)
    {
        cac->race = CAC_FRI;
    }
    else if (entry.cms_entry == 0x6A)
    {
        cac->race = CAC_MAM;
    }
    else if (entry.cms_entry == 0x6B)
    {
        cac->race = CAC_MAF;
    }
    else
    {
        DPRINTF("%s: Unrecognized race 0x%x\n", entry.cms_entry);
        return false;
    }

    cac->voice = entry.voice;

    std::string name;

    int lang = ui->langComboBox->currentIndex();
    if (lang < 0 || lang >= XV2_LANG_NUM)
        lang = XV2_LANG_ENGLISH;

    if (!Xenoverse2::GetLobbyName(entry.name_id, name, lang))
        name = "PATROLLER";

    strncpy(cac->name, name.c_str(), sizeof(cac->name));
    cac->name[sizeof(cac->name)-1] = 0;

    return true;
}

bool MainWindow::EquipmentToCac(const PalEquipment &equipment, XV2Cac *cac, uint32_t set, bool do_looks, bool do_equip)
{
    if (!do_looks && !do_equip)
        return true;

    if (set >= XV2_NUM_PRESETS)
        return false;

    XV2CacSet *cac_set = &cac->sets[set];

    if (do_looks)
    {
        cac->face_base = equipment.face_base;
        cac->face_forehead = equipment.face_forehead;
        cac->eyes = equipment.eyes;
        cac->nose = equipment.nose;
        cac->ears = equipment.ears;
        cac->hair = equipment.hair;
        cac->body_shape = equipment.body_shape;
        cac->skin_color1 = equipment.skin_color1;
        cac->skin_color2 = equipment.skin_color2;
        cac->skin_color3 = equipment.skin_color3;
        cac->skin_color4 = equipment.skin_color4;
        cac->hair_color = equipment.hair_color;
        cac->eye_color = equipment.eye_color;
        cac->makeup_color1 = equipment.makeup_color1;
        cac->makeup_color2 = equipment.makeup_color2;
        cac->makeup_color3 = equipment.makeup_color3;
    }

    if (do_equip)
    {
        cac_set->top = equipment.top;
        cac_set->bottom = equipment.bottom;
        cac_set->gloves = equipment.gloves;
        cac_set->shoes = equipment.shoes;
        cac_set->top_color1 = equipment.top_color1;
        cac_set->top_color2 = equipment.top_color2;
        cac_set->top_color3 = equipment.top_color3;
        cac_set->top_color4 = equipment.top_color4;
        cac_set->bottom_color1 = equipment.bottom_color1;
        cac_set->bottom_color2 = equipment.bottom_color2;
        cac_set->bottom_color3 = equipment.bottom_color3;
        cac_set->bottom_color4 = equipment.bottom_color4;
        cac_set->gloves_color1 = equipment.gloves_color1;
        cac_set->gloves_color2 = equipment.gloves_color2;
        cac_set->gloves_color3 = equipment.gloves_color3;
        cac_set->gloves_color4 = equipment.gloves_color4;
        cac_set->shoes_color1 = equipment.shoes_color1;
        cac_set->shoes_color2 = equipment.shoes_color2;
        cac_set->shoes_color3 = equipment.shoes_color3;
        cac_set->shoes_color4 = equipment.shoes_color4;
        cac_set->accesory = equipment.accesory;
        cac_set->talisman = equipment.talisman;
    }

    return true;
}

bool MainWindow::StatsToCac(const PalStats &stats, XV2Cac *cac, uint32_t set, bool do_stats, bool do_skills)
{
    if (!do_stats && !do_skills)
        return true;

    if (set >= XV2_NUM_PRESETS)
        return false;

    XV2CacSet *cac_set = &cac->sets[set];

    if (do_stats)
    {
        cac->level = stats.level;
        cac->hea = stats.hea;
        cac->ki = stats.ki;
        cac->stm = stats.stm;
        cac->atk = stats.atk;
        cac->str = stats.str;
        cac->bla = stats.bla;
    }

    if (do_skills)
    {
        for (int i = 0; i < 4; i++)
        {
            cac_set->super_skills[i] = Cac2X2m::SuperSkillFromId2(stats.super_skills[i]);
        }

        for (int i = 0; i < 2; i++)
        {
            cac_set->ult_skills[i] = Cac2X2m::UltSkillFromId2(stats.ult_skills[i]);
        }

        cac_set->evasive_skill = Cac2X2m::EvaSkillFromId2(stats.evasive_skill);
        //cac_set->blast_skill = 0xFFFF; // lets leave it unprocessed
        cac_set->awaken_skill = Cac2X2m::AwakenSkillFromId2(stats.awaken_skill);
    }

    return true;
}

bool MainWindow::EntryToCac(const PalEntry &entry, XV2Cac *cac, uint32_t set, bool do_info, bool do_looks, bool do_equip, bool do_stats, bool do_skills)
{
    if (do_info)
    {
        if (!InfoToCac(entry, cac))
        {
            DPRINTF("InfoToCac failed.\n");
            return false;
        }
    }

    if (do_looks || do_equip)
    {
        if (!EquipmentToCac(entry.equipment, cac, set, do_looks, do_equip))
        {
            DPRINTF("EquipmentToCac failed.\n");
            return false;
        }
    }

    if (do_stats || do_skills)
    {
        if (!StatsToCac(entry.stats, cac, set, do_stats, do_skills))
        {
            DPRINTF("StatsToCac failed.\n");
            return false;
        }
    }

    return true;
}

bool MainWindow::EntryToX2m(const PalEntry &entry, const QString &file, const QString &code, const uint8_t *guid, bool css_voice)
{
    X2mFile x2m;
    std::string name;

    // Info
    if (!Xenoverse2::GetLobbyName(entry.name_id, name))
        name = Utils::ToString(entry.name_id);

    if (guid)
    {
        x2m.SetModGuid(Utils::GUID2String(guid));
    }

    x2m.SetType(X2mType::NEW_CHARACTER);
    x2m.SetModName(name);
    x2m.SetModVersion(1.0f);
    x2m.SetModAuthor(Utils::QStringToStdString(QString("%1 v%2").arg(PROGRAM_NAME).arg(PROGRAM_VERSION)));
    x2m.SetEntryName(Utils::QStringToStdString(code));

    for (int lang = 0; lang < XV2_LANG_NUM; lang++)
    {
        std::string name;

        if (!Xenoverse2::GetLobbyName(entry.name_id, name, lang))
            name = Utils::ToString(entry.name_id);

        x2m.SetCharaName(name, lang);
    }

    // Cms / Files
    BcsFile orig_bcs, new_bcs;
    EskFile new_esk;
    EanFile new_cam_ean;
    std::unordered_set<std::string> files;
    std::string base_dir;

    if (!Cac2X2m::SetCms(&x2m, entry.cms_entry, files, base_dir, orig_bcs, new_esk, new_cam_ean))
    {
        DPRINTF("SetCms failed.\n");
        return false;
    }

    new_bcs = orig_bcs;    

    const PalEquipment &equip = entry.equipment;
    const PalStats &stats = entry.stats;

    uint32_t bust = Cac2X2m::BustItem(equip.top);
    uint32_t pants = Cac2X2m::PantsItem(equip.bottom);
    uint32_t rist = Cac2X2m::RistItem(equip.gloves);
    uint32_t boots = Cac2X2m::BootsItem(equip.shoes);
    uint32_t accesory = Cac2X2m::AccesoryItem(equip.accesory);

    BcsPartSet &output_set = new_bcs.GetPartSets()[0];

    if (equip.body_shape != 0xFFFF)
        x2m.SetBodyShape(equip.body_shape);

    x2m.SetCanUseAnyDualSkill(true);

    if (!Cac2X2m::BuildPartSet(&x2m, output_set, orig_bcs, files, base_dir,
                               equip.face_base, equip.face_forehead, equip.eyes, equip.nose, equip.ears,
                               equip.hair, bust, pants, rist, boots, accesory))
    {
        DPRINTF("BuildPartSet failed.\n");
        return false;
    }

    if (!Cac2X2m::SetColors(output_set, orig_bcs,
                            equip.skin_color1, equip.skin_color2, equip.skin_color3, equip.skin_color4,
                            equip.hair_color, equip.eye_color,
                            equip.top_color1, equip.top_color2, equip.top_color3, equip.top_color4,
                            equip.bottom_color1, equip.bottom_color2, equip.bottom_color3, equip.bottom_color4,
                            equip.gloves_color1, equip.gloves_color2, equip.gloves_color3, equip.gloves_color4,
                            equip.shoes_color1, equip.shoes_color2, equip.shoes_color3, equip.shoes_color4,
                            equip.makeup_color1, equip.makeup_color2, equip.makeup_color3))
    {
        DPRINTF("SetColors failed.\n");
        return false;
    }

    if (!Cac2X2m::SetBody(new_esk, new_cam_ean, orig_bcs, equip.body_shape))
    {
        DPRINTF("SetBody failed.\n");
        return false;
    }

    if (entry.cms_entry >= 0x64 && entry.cms_entry <= 0x67) // HUM,HUF,SYM,SYF
    {
        if (!Cac2X2m::CopySSJ(new_bcs, orig_bcs, files, base_dir, x2m.GetEntryName()))
        {
            DPRINTF("CopySuperSaiyan failed.\n");
            return false;
        }
    }

    {
        uint16_t awa_skill_id = Cac2X2m::AwakenSkillFromId2(stats.awaken_skill);
        if (awa_skill_id < 0x8000)
        {
            CusSkill *awa_skill = game_cus->FindAwakenSkillByID(awa_skill_id);
            if (awa_skill)
            {
                if (awa_skill->partset != 0xFFFF)
                {
                    bool ssj = (awa_skill->partset >= 100 && awa_skill->partset <= 102) || (awa_skill->partset >= 297 && awa_skill->partset <= 299);

                    if (!ssj)
                    {
                        std::vector<uint32_t> extra_trans, def_sets;

                        uint32_t num_trans = 1;

                        if (awa_skill->num_transforms > 1 && awa_skill->num_transforms != 0xFFFFFFFF)
                            num_trans = awa_skill->num_transforms;

                        for (uint32_t i = 0; i < num_trans; i++)
                        {
                            extra_trans.push_back(awa_skill->partset+i);
                            def_sets.push_back(0);
                        }

                        if (!Cac2X2m::CopyTransforms(new_bcs, orig_bcs, extra_trans, def_sets, 1, files, base_dir, x2m.GetEntryName()))
                        {
                            DPRINTF("CopyTransforms failed.\n");
                            return false;
                        }
                    }
                }
            }
        }
    }

    // Cso
    CsoEntry cso_entry;

    if (!Cac2X2m::SetCso(entry.cms_entry, cso_entry, files, entry.voice))
    {
        DPRINTF("SetCso failed.\n");
        return false;
    }

    x2m.AddCsoEntry(cso_entry);

    if (!Cac2X2m::WriteFiles(&x2m, files))
    {
        DPRINTF("WriteFiles failed.\n");
        return false;
    }

    if (!Cac2X2m::WriteCustomFiles(&x2m, new_bcs, new_esk, new_cam_ean))
    {
        DPRINTF("WriteBcs failed.\n");
        return false;
    }

    if (!Cac2X2m::WriteSelPort(&x2m, entry.cms_entry))
    {
        DPRINTF("WriteSelPort failed.\n");
        return false;
    }

    // 0.5: no battle portrait = auto battle portrait
    /*if (!Cac2X2m::WriteBtlPort(&x2m, entry.cms_entry))
    {
        DPRINTF("WriteBtlPort failed.\n");
        return false;
    }*/

    if (entry.cms_entry >= 0x64 && entry.cms_entry <= 0x67) // HUM,HUF,SYM,SYF
    {
        if (!Cac2X2m::FixSSJ3(&x2m))
        {
            DPRINTF("FixSSJ3 failed.\n");
            return false;
        }
    }

    // Audio tracks
    if (css_voice)
    {
        if (!Cac2X2m::WriteCssVoice(&x2m, entry.cms_entry, entry.voice))
        {
            DPRINTF("WriteCssVoice failed.\n");
            return false;
        }
    }

    // Slots
    X2mSlotEntry slot;

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        std::string name = Xenoverse2::GetCacTopName(equip.top, i);
        if (name.length() > 0)
        {
            slot.costume_name[i] = name;
        }
        else
        {
            if (i == XV2_LANG_ENGLISH)
                slot.costume_name[i] = "0";
        }
    }

    if (css_voice)
    {
        slot.voices_id_list[0] = slot.voices_id_list[1] = X2M_DUMMY_ID;
        slot.audio_files[0] = slot.audio_files[1] = "VOICE";
    }

    x2m.AddSlotEntry(slot);

    // Cus
    CusSkillSet skill_set;

    for (int i = 0; i < 4; i++)
        skill_set.char_skills[i] = Cac2X2m::SuperSkillFromId2(stats.super_skills[i]);

    for (int i = 0; i < 2; i++)
        skill_set.char_skills[4+i] = Cac2X2m::UltSkillFromId2(stats.ult_skills[i]);

    skill_set.char_skills[6] = Cac2X2m::EvaSkillFromId2(stats.evasive_skill);
    skill_set.char_skills[7] = Cac2X2m::BlastSkillFromId2(stats.blast_skill);
    skill_set.char_skills[8] = Cac2X2m::AwakenSkillFromId2(stats.awaken_skill);

    Cac2X2m::ResolveCustomSkills(&x2m, skill_set);
    x2m.AddSkillSet(skill_set);

    // Psc
    PscSpecEntry psc_entry;

    if (!Cac2X2m::SetPsc(entry.cms_entry, psc_entry, equip.body_shape, stats.hea, stats.ki, stats.stm,
                         stats.atk, stats.str, stats.bla, equip.talisman))
    {
        DPRINTF("SetPsc failed.\n");
        return false;
    }

    x2m.AddPscEntry(psc_entry);

    // aur
    AurCharaLink aur_entry;

    if (!Cac2X2m::SetAura(entry.cms_entry, aur_entry))
    {
        DPRINTF("SetAura failed.\n");
        return false;
    }

    x2m.AddAurEntry(aur_entry);

    // Cml
    CmlEntry cml_entry;

    if (!Cac2X2m::SetCml(entry.cms_entry, cml_entry, equip.body_shape))
    {
        DPRINTF("SetCml failed.\n");
        return false;
    }

    x2m.AddCmlEntry(cml_entry);

    if (!x2m.SaveToFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Failed to write x2m.\n");
        return false;
    }

    return true;
}

bool MainWindow::X2mToInfo(X2mFile *x2m, PalEntry &entry)
{
    if (x2m->GetType() != X2mType::NEW_CHARACTER)
    {
        DPRINTF("This x2m is not a character one.\n");
        return false;
    }

    std::string race;
    if (!X2m2Cac::GetRace(x2m, race))
    {
        DPRINTF("This doesn't seem to be a Cac2X2m/Cycit x2m.\n");
        return false;
    }

    CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByName(race));
    if (!cms)
    {
        DPRINTF("Internal error, cannot find race \"%s\". This shouldn't normally happen.\n", race.c_str());
        return false;
    }

    entry.cms_entry = cms->id;

    int voice = X2m2Cac::GetVoice(x2m);
    if (voice < 0)
    {
        DPRINTF("Cannot get voice type, this is probably not a Cac2X2m/Cycit x2m.\n");
        return false;
    }

    entry.voice = (uint16_t)voice;
    return true;
}

bool MainWindow::X2mToEquipment(X2mFile *x2m, uint32_t slot_idx, PalEquipment &equipment, uint32_t current_race)
{
    std::string my_race, race;

    if (!x2m->UsesBodyShape())
    {
        DPRINTF("Doesn't seem like a Cac2X2m/Cycit x2m.\n");
        return false;
    }

    CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(current_race));
    if (!cms)
    {
        DPRINTF("Cannot find cms of current race, this shouldn't normally happen.\n");
        return false;
    }

    my_race = cms->name;

    if (!X2m2Cac::GetRace(x2m, race))
    {
        DPRINTF("This doesn't seem to be a Cac2X2m/Cycit x2m.\n");
        return false;
    }

    if (my_race != race)
    {
        bool allow = ((my_race == "HUM" && race == "SYM") || (my_race == "SYM" && race == "HUM") ||
                      (my_race == "HUF" && race == "SYF") || (my_race == "SYF" && race == "HUF"));

        if (!allow)
        {
            DPRINTF("The equipment copy requires current entry to be of same race than x2m (HUM/SYM and HUF/SYF combinations are allowed)\n"
                    "Current entry race: %s. X2M race: %s\n", my_race.c_str(), race.c_str());

            return false;
        }
    }

    BcsFile *bcs;

    if (race == "HUM" || race == "SYM")
    {
        bcs = game_hum_bcs;        
    }
    else if (race == "HUF" || race == "SYF")
    {
        bcs = game_huf_bcs;
    }
    else if (race == "NMC")
    {
        bcs = game_nmc_bcs;
    }
    else if (race == "FRI")
    {
        bcs = game_fri_bcs;
    }
    else if (race == "MAM")
    {
        bcs = game_mam_bcs;
    }
    else
    {
        bcs = game_maf_bcs;
    }

    bool accesory_set = false;
    bool accesory;
    uint32_t part;
    uint32_t colors[4];

    if (!X2m2Cac::GetPart(x2m, *bcs,  slot_idx, PART_FACE_BASE, &part, colors, &accesory))
    {
        DPRINTF("Failed to find face base.\n");
        return false;
    }

    equipment.face_base = (uint16_t)part;
    equipment.skin_color1 = (uint16_t)colors[0];

    if (race == "NMC" || race == "FRI")
    {
        equipment.skin_color2 = (uint16_t)colors[1];
        equipment.skin_color3 = (uint16_t)colors[2];

        if (race == "FRI")
        {
            equipment.skin_color4 = (uint16_t)colors[3];
        }
        else
        {
            equipment.skin_color4 = 0xFFFF;
        }
    }
    else
    {
        equipment.skin_color2 = equipment.skin_color3 = equipment.skin_color4 = 0xFFFF;
    }

    if (!X2m2Cac::GetPart(x2m, *bcs,  slot_idx, PART_FACE_FOREHEAD, &part, colors, &accesory))
    {
        DPRINTF("Failed to find face forehead.\n");
        return false;
    }

    equipment.face_forehead = (uint16_t)part;
    equipment.makeup_color1 = (uint16_t)colors[0];
    equipment.makeup_color2 = (uint16_t)colors[1];
    equipment.makeup_color3 = (uint16_t)colors[2];

    if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_FACE_EYE, &part, colors, &accesory))
    {
        DPRINTF("Failed to find eyes.\n");
        return false;
    }

    equipment.eyes = (uint16_t)part;
    equipment.eye_color = (uint16_t)colors[0];

    if (race == "MAM" || race == "MAF")
    {
        equipment.nose = 0xFFFF;
    }
    else
    {
        if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_FACE_NOSE, &part, colors, &accesory))
        {
            DPRINTF("Failed to find nose.\n");
            return false;
        }

         equipment.nose = (uint16_t)part;
    }

    if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_FACE_EAR, &part, colors, &accesory))
    {
        DPRINTF("Failed to find ear.\n");
        return false;
    }

    equipment.ears = (uint16_t)part;

    if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_HAIR, &part, colors, &accesory))
    {
        DPRINTF("Failed to find hair.\n");
        return false;
    }

    if (accesory)
    {
        if (accesory_set)
        {
            DPRINTF("Multiple merged accessory, cannot proceed.\n");
            return false;
        }

        accesory_set = true;
        equipment.hair = 0;
        equipment.accesory = (uint16_t)part;
    }
    else
    {
        equipment.hair = (uint16_t)part;
    }

    if (race == "HUM" || race == "HUF" || race == "SYM" || race == "SYF")
    {
        equipment.hair_color = (uint16_t)colors[0];
    }
    else
    {
        equipment.hair_color = 0xFFFF;
    }

    if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_BUST, &part, colors, &accesory))
    {
        DPRINTF("Failed to find bust.\n");
        return false;
    }

    equipment.top = (uint16_t)part;
    equipment.top_color1 = (uint16_t)colors[0];
    equipment.top_color2 = (uint16_t)colors[1];
    equipment.top_color3 = (uint16_t)colors[2];
    equipment.top_color4 = (uint16_t)colors[3];

    if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_PANTS, &part, colors, &accesory))
    {
        DPRINTF("Failed to find pants.\n");
        return false;
    }

    equipment.bottom = (uint16_t)part;
    equipment.bottom_color1 = (uint16_t)colors[0];
    equipment.bottom_color2 = (uint16_t)colors[1];
    equipment.bottom_color3 = (uint16_t)colors[2];
    equipment.bottom_color4 = (uint16_t)colors[3];

    if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_RIST, &part, colors, &accesory))
    {
        DPRINTF("Failed to find rist.\n");
        return false;
    }

    equipment.gloves = (uint16_t)part;
    equipment.gloves_color1 = (uint16_t)colors[0];
    equipment.gloves_color2 = (uint16_t)colors[1];
    equipment.gloves_color3 = (uint16_t)colors[2];
    equipment.gloves_color4 = (uint16_t)colors[3];

    if (!X2m2Cac::GetPart(x2m, *bcs, slot_idx, PART_BOOTS, &part, colors, &accesory))
    {
        DPRINTF("Failed to find boots.\n");
        return false;
    }

    equipment.shoes = (uint16_t)part;
    equipment.shoes_color1 = (uint16_t)colors[0];
    equipment.shoes_color2 = (uint16_t)colors[1];
    equipment.shoes_color3 = (uint16_t)colors[2];
    equipment.shoes_color4 = (uint16_t)colors[3];

    if (!accesory_set)
    {
        uint32_t accesory_id;

        if (X2m2Cac::FindAccessory(x2m, *bcs, slot_idx, &accesory_id))
        {
            equipment.accesory = (uint16_t)accesory_id;
        }
        else
        {
            equipment.accesory = 0xFFFF;
        }
    }

    equipment.body_shape = (uint16_t)x2m->GetBodyShape();
    equipment.talisman = (uint16_t)X2m2Cac::GetTalisman(x2m, slot_idx);

    return true;
}

bool MainWindow::X2mToStats(X2mFile *x2m, uint32_t slot_idx, PalStats &stats)
{
    if (!X2m2Cac::GetStats(x2m, slot_idx, &stats.hea, &stats.ki, &stats.stm, &stats.atk, &stats.str, &stats.bla))
    {
        DPRINTF("Cannot get stats. Maybe not a Cac2X2m/Cycyt x2m.\n");
        return false;
    }

    stats.super_skills[0] = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 0, true);
    stats.super_skills[1] = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 1, true);
    stats.super_skills[2] = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 2, true);
    stats.super_skills[3] = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 3, true);
    stats.ult_skills[0] = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 4, true);
    stats.ult_skills[1] = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 5, true);
    stats.evasive_skill= (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 6, true);
    stats.blast_skill = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 7, true);
    stats.awaken_skill = (uint16_t)X2m2Cac::GetSkill(x2m, slot_idx, 8, true);

    return true;
}

bool MainWindow::X2mToEntry(X2mFile *x2m, uint32_t slot_idx, PalEntry &entry)
{
    if (!X2mToInfo(x2m, entry))
        return false;

    if (!X2mToEquipment(x2m, slot_idx, entry.equipment, entry.cms_entry))
        return false;

    if (!X2mToStats(x2m, slot_idx, entry.stats))
        return false;

    return true;
}

void MainWindow::Load()
{
    if (game_pal->GetNumEntries() == 0)
    {
        DPRINTF("WTF, the pal file is empty. What have you done?\n");
        DPRINTF("Program will close.\n");
        exit(-1);
    }

    prev_entry_index = -1;
    ui->entriesComboBox->clear();

    for (const PalEntry &entry : *game_pal)
    {
        std::string name;
        Xenoverse2::GetLobbyName(entry.name_id, name);

        if (name.length() > 0)
        {
            ui->entriesComboBox->addItem(Utils::StdStringToQString(name, false));
        }
        else
        {
            ui->entriesComboBox->addItem(QString("%1").arg((int16_t)entry.id));
        }
    }

    ui->removeButton->setEnabled(game_pal->GetNumEntries() > 1);
    on_allLangCheck_clicked();
}

bool MainWindow::GetBcsColorFromDialog(const std::string &part, uint16_t current_color, uint16_t cms_entry, uint16_t *out_color)
{
    BcsFile *bcs;
    bool has_multiple_skin = false;

    switch (cms_entry)
    {
        case 0x64: case 0x66:
            bcs = game_hum_bcs;
        break;

        case 0x65: case 0x67:
            bcs = game_huf_bcs;
        break;

        case 0x68:
            bcs = game_nmc_bcs;
            has_multiple_skin = true;
        break;

        case 0x69:
            bcs = game_fri_bcs;
            has_multiple_skin = true;
        break;

        case 0x6A:
            bcs = game_mam_bcs;
        break;

        case 0x6B:
            bcs = game_maf_bcs;
        break;

        default:
            return false;
    }

    std::string part_name = part;

    if (part_name == "SKIN_" && has_multiple_skin)
        part_name = "SKIN_A_";

    std::vector<uint32_t> colors;

    if (!bcs->ListPreviewColors(part_name, colors))
    {
        DPRINTF("The color of this part cannot be set in this race.\n");
        return false;
    }

    bool was_no_color = (current_color >= colors.size());
    ColorDialog dialog(colors, current_color, this);

    if (dialog.exec())
    {
        uint16_t result = dialog.GetSelection();

        if (result >= colors.size())
        {
            if (was_no_color)
            {
                // Do nothing
                return false;
            }
            else
            {
                *out_color = 0xFFFF;
                return true;
            }
        }
        else
        {
            *out_color = result;
            return true;
        }
    }

    return false;
}

void MainWindow::on_actionExit_triggered()
{
    if (ProcessShutdown())
        qApp->exit();
}

#define Q_COMPILER_INITIALIZER_LISTS

void MainWindow::on_actionAbout_triggered()
{
    /*std::map<uint32_t, uint32_t> map;

    for (size_t i = 0; i < game_pal->GetNumEntries(); i++)
    {
        const PalEntry &entry = (*game_pal)[i];

        if (entry.cms_entry == 0x67)
        {
            auto it = map.find(entry.equipment.hair);

            if (it == map.end())
            {
                map[entry.equipment.hair] = 1;
            }
            else
            {
                map[entry.equipment.hair] = it->second + 1;
            }

            if (entry.equipment.hair == 33)
            {
                std::string name;

                Xenoverse2::GetLobbyName(entry.name_id, name);
                DPRINTF("%s\n", name.c_str());
            }
        }
    }

    for (auto it : map)
    {
        //DPRINTF("%d: %d times\n", it.first, it.second);
    }*/

    QMessageBox box;

    box.setWindowTitle(PROGRAM_NAME);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Icon::Information);
    //box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4").arg(PROGRAM_NAME).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));
    box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4<br><br>If you liked the program, you can support its development at<br><a href='https://www.patreon.com/eternity_tools'>https://www.patreon.com/eternity_tools</a>").arg(PROGRAM_NAME).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));


    box.exec();
    //UPRINTF("%s v%s by Eternity\n\nBuilt on %s %s\n", PROGRAM_NAME, PROGRAM_VERSION, __DATE__, __TIME__);
}

void MainWindow::on_entriesComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= game_pal->GetNumEntries())
        return;

    if (prev_entry_index >= 0 && prev_entry_index < game_pal->GetNumEntries())
    {
        PalEntry &entry = (*game_pal)[prev_entry_index];
        GuiToEntry(entry);
    }

    const PalEntry &entry = (*game_pal)[index];
    EntryToGui(entry);

    prev_entry_index = index;
}

void MainWindow::on_addButton_clicked()
{
    PalEntry entry;

    int idx = ui->entriesComboBox->currentIndex();

    if (idx >= 0 && idx < game_pal->GetNumEntries())
    {
        entry = (*game_pal)[idx];
    }

    if (idx >= 0 && idx < game_pal->GetNumEntries())
    {
        GuiToEntry(entry);
    }
    else
    {
        DPRINTF("Combobox out of bounds!");
    }

    if (!AddNewName(entry))
        return;

    size_t pos = game_pal->AddEntry(entry);
    std::string name;

    Xenoverse2::GetLobbyName(entry.name_id, name);

    if (name.length() > 0)
    {
        ui->entriesComboBox->addItem(Utils::StdStringToQString(name, false));
    }
    else
    {
        ui->entriesComboBox->addItem(QString("%1").arg((int16_t)entry.id));
    }

    ui->entriesComboBox->setCurrentIndex((int)pos);

    if (game_pal->GetNumEntries() > 1)
        ui->removeButton->setEnabled(true);
}

void MainWindow::on_removeButton_clicked()
{
    int index = ui->entriesComboBox->currentIndex();

    if (index < 0 || index >= game_pal->GetNumEntries())
        return;

    const PalEntry &entry = (*game_pal)[index];

    if (entry.name_id >= CUSTOM_MIN && entry.name_id < CUSTOM_MAX)
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
            Xenoverse2::RemoveLobbyName(entry.name_id, i);
    }

    game_pal->RemoveEntryByIndex(index);

    if (prev_entry_index > index)
        prev_entry_index--;
    else
        prev_entry_index = -1;

    ui->entriesComboBox->removeItem(index);

    if (game_pal->GetNumEntries() == 1)
        ui->removeButton->setDisabled(true);
}

void MainWindow::on_nameEdit_textEdited(const QString &arg1)
{
    int idx = ui->entriesComboBox->currentIndex();
    const PalEntry &entry = (*game_pal)[idx];

    if (idx < 0 || idx >= game_pal->GetNumEntries())
        return;

    if (ui->allLangCheck->isEnabled())
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::SetLobbyName(entry.name_id, Utils::QStringToStdString(arg1, false), i);
        }
    }
    else
    {
        int lang = ui->langComboBox->currentIndex();

        if (lang < 0 || lang >= XV2_LANG_NUM)
            return;

        Xenoverse2::SetLobbyName(entry.name_id, Utils::QStringToStdString(arg1, false), lang);
    }

    ui->entriesComboBox->setItemText(idx, arg1);
}

void MainWindow::on_langComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= XV2_LANG_NUM)
        return;

    int idx = ui->entriesComboBox->currentIndex();

    if (idx < 0 || idx >= game_pal->GetNumEntries())
        return;

    const PalEntry &entry = (*game_pal)[idx];
    std::string name;

    Xenoverse2::GetLobbyName(entry.name_id, name, index);
    ui->nameEdit->setText(Utils::StdStringToQString(name, false));

    ui->entriesComboBox->setItemText(idx, Utils::StdStringToQString(name, false));
}

void MainWindow::on_copyButton_triggered(QAction *arg1)
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    ListDialog *dialog = nullptr;
    PalFile *pal = nullptr;

    if (arg1 == ui->actionFromOtherEntry)
    {
        dialog = new ListDialog(ListMode::PAL, this);
        pal = game_pal;
    }
    else if (arg1 == ui->actionFromExternalPAL)
    {
        QString file = QFileDialog::getOpenFileName(this, "External PAL", config.lf_external_pal, "PAL Files (*.pal *.pal.xml)");

        if (file.isNull())
            return;

        config.lf_external_pal = file;

        pal = new PalFile();
        if (!pal->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete pal;
            return;
        }

        dialog = new ListDialog(ListMode::PAL, this, pal);
    }
    else if (arg1 == ui->actionFromCAC)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose save file", config.lf_external_sav, "XV2 SAV Files (DBXV2.sav)");

        if (file.isNull())
            return;

        config.lf_external_sav = file;
        Xv2SavFile sav;

        if (!sav.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        ListDialog cac_dialog(ListMode::CAC_FULL, this, &sav);

        if (cac_dialog.exec())
        {
            uint32_t result = cac_dialog.GetResultData();
            uint32_t cac = result / XV2_NUM_PRESETS;
            uint32_t set = result % XV2_NUM_PRESETS;

            const PalEntry &dst = (*game_pal)[idx_me];
            PalEntry new_entry = dst;

            if (CacToEntry(sav.GetCac(cac), set, new_entry))
            {
                (*game_pal)[idx_me] = new_entry;

                for (int i = 0; i < XV2_LANG_NUM; i++)
                {
                    Xenoverse2::SetLobbyName(new_entry.name_id, sav.GetCac(cac)->name, i);
                }

                EntryToGui(new_entry);
                on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
            }
            else
            {
               DPRINTF("Failed to convert cac to npc.\n");
            }
        }

        return;
    }
    else if (arg1 == ui->actionFromX2m)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose x2m file", config.lf_installer_open, "X2M Files (*.x2m)");

        if (file.isNull())
            return;

        config.lf_installer_open = file;
        X2mFile x2m;

        if (!x2m.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        uint32_t slot_idx = 0;

        if (x2m.GetNumSlotEntries() > 1)
        {
            ListDialog dialog(ListMode::X2M_SLOT, this, &x2m);

            if (!dialog.exec())
                return;

            if (dialog.GetResultInteger() < 0 || dialog.GetResultInteger() >= (int)x2m.GetNumSlotEntries())
            {
                DPRINTF("You didn't select a slot.\n");
                return;
            }

            slot_idx = (uint32_t)dialog.GetResultInteger();
        }

        const PalEntry &dst = (*game_pal)[idx_me];
        PalEntry new_entry = dst;

        WaitDialog wait;
        wait.show();
        qApp->processEvents();

        if (X2mToEntry(&x2m, slot_idx, new_entry))
        {
            (*game_pal)[idx_me] = new_entry;

            for (int i = 0; i < XV2_LANG_NUM; i++)
            {
                std::string name;

                X2m2Cac::GetName(&x2m, name, i);
                Xenoverse2::SetLobbyName(new_entry.name_id, name, i);
            }

            EntryToGui(new_entry);
            on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
        }

        wait.close();
        return;
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < pal->GetNumEntries())
            {
                const PalEntry &entry = (*pal)[idx];
                const PalEntry &dst = (*game_pal)[idx_me];

                for (int i = 0; i < XV2_LANG_NUM; i++)
                {
                    std::string name;

                    Xenoverse2::GetLobbyName(entry.name_id, name, i);
                    Xenoverse2::SetLobbyName(dst.name_id, name, i);
                }

                EntryToGui(entry);
                on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
            }
        }

        delete dialog;

        if (pal != game_pal)
            delete pal;
    }
}

void MainWindow::on_infoCopyButton_triggered(QAction *arg1)
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    ListDialog *dialog = nullptr;
    PalFile *pal = nullptr;

    if (arg1 == ui->actionFromOtherEntryInfo)
    {
        dialog = new ListDialog(ListMode::PAL, this);
        pal = game_pal;
    }
    else if (arg1 == ui->actionFromExternalPAInfo)
    {
        QString file = QFileDialog::getOpenFileName(this, "External PAL", config.lf_external_pal, "PAL Files (*.pal *.pal.xml)");

        if (file.isNull())
            return;

        config.lf_external_pal = file;

        pal = new PalFile();
        if (!pal->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete pal;
            return;
        }

        dialog = new ListDialog(ListMode::PAL, this, pal);
    }
    else if (arg1 == ui->actionFromCACInfo)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose save file", config.lf_external_sav, "XV2 SAV Files (DBXV2.sav)");

        if (file.isNull())
            return;

        config.lf_external_sav = file;
        Xv2SavFile sav;

        if (!sav.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        ListDialog cac_dialog(ListMode::CAC_FULL, this, &sav);

        if (cac_dialog.exec())
        {
            uint32_t result = cac_dialog.GetResultData();
            uint32_t cac = result / XV2_NUM_PRESETS;
            //uint32_t set = result % XV2_NUM_PRESETS;

            const PalEntry &dst = (*game_pal)[idx_me];
            PalEntry new_entry = dst;

            if (CacToInfo(sav.GetCac(cac), new_entry))
            {
                (*game_pal)[idx_me] = new_entry;

                for (int i = 0; i < XV2_LANG_NUM; i++)
                {
                    Xenoverse2::SetLobbyName(new_entry.name_id, sav.GetCac(cac)->name, i);
                }

                InfoToGui(new_entry);
                on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
            }
            else
            {
               DPRINTF("Failed to convert cac info to npc info.\n");
            }
        }

        return;
    }
    else if (arg1 == ui->actionFromX2mInfo)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose x2m file", config.lf_installer_open, "X2M Files (*.x2m)");

        if (file.isNull())
            return;

        config.lf_installer_open = file;
        X2mFile x2m;

        if (!x2m.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        const PalEntry &dst = (*game_pal)[idx_me];
        PalEntry new_entry = dst;

        if (X2mToInfo(&x2m, new_entry))
        {
            (*game_pal)[idx_me] = new_entry;

            for (int i = 0; i < XV2_LANG_NUM; i++)
            {
                std::string name;

                X2m2Cac::GetName(&x2m, name, i);
                Xenoverse2::SetLobbyName(new_entry.name_id, name, i);
            }

            InfoToGui(new_entry);
            on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
        }

        return;
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < pal->GetNumEntries())
            {
                const PalEntry &entry = (*pal)[idx];
                const PalEntry &dst = (*game_pal)[idx_me];

                for (int i = 0; i < XV2_LANG_NUM; i++)
                {
                    std::string name;

                    Xenoverse2::GetLobbyName(entry.name_id, name, i);
                    Xenoverse2::SetLobbyName(dst.name_id, name, i);
                }

                InfoToGui(entry);
                on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
            }
        }

        delete dialog;

        if (pal != game_pal)
            delete pal;
    }
}

void MainWindow::on_equipCopyButton_triggered(QAction *arg1)
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    ListDialog *dialog = nullptr;
    PalFile *pal = nullptr;

    if (arg1 == ui->actionFromOtherEntryEquip)
    {
        dialog = new ListDialog(ListMode::PAL, this);
        pal = game_pal;
    }
    else if (arg1 == ui->actionFromExternalPALEquip)
    {
        QString file = QFileDialog::getOpenFileName(this, "External PAL", config.lf_external_pal, "PAL Files (*.pal *.pal.xml)");

        if (file.isNull())
            return;

        config.lf_external_pal = file;

        pal = new PalFile();
        if (!pal->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete pal;
            return;
        }

        dialog = new ListDialog(ListMode::PAL, this, pal);
    }
    else if (arg1 == ui->actionFromCACEquip)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose save file", config.lf_external_sav, "XV2 SAV Files (DBXV2.sav)");

        if (file.isNull())
            return;

        config.lf_external_sav = file;
        Xv2SavFile sav;

        if (!sav.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        ListDialog cac_dialog(ListMode::CAC_FULL, this, &sav);

        if (cac_dialog.exec())
        {
            uint32_t result = cac_dialog.GetResultData();
            uint32_t cac = result / XV2_NUM_PRESETS;
            uint32_t set = result % XV2_NUM_PRESETS;

            const PalEntry &dst = (*game_pal)[idx_me];
            PalEntry new_entry = dst;

            if (CacToEquipment(sav.GetCac(cac), set, new_entry.equipment))
            {
                (*game_pal)[idx_me] = new_entry;

                EquipmentToGui(new_entry.equipment, new_entry.cms_entry);
                on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
            }
            else
            {
               DPRINTF("Failed to convert cac to npc.\n");
            }
        }

        return;
    }
    else if (arg1 == ui->actionFromX2mEquip)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose x2m file", config.lf_installer_open, "X2M Files (*.x2m)");

        if (file.isNull())
            return;

        config.lf_installer_open = file;
        X2mFile x2m;

        if (!x2m.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        uint32_t slot_idx = 0;

        if (x2m.GetNumSlotEntries() > 1)
        {
            ListDialog dialog(ListMode::X2M_SLOT, this, &x2m);

            if (!dialog.exec())
                return;

            if (dialog.GetResultInteger() < 0 || dialog.GetResultInteger() >= (int)x2m.GetNumSlotEntries())
            {
                DPRINTF("You didn't select a slot.\n");
                return;
            }

            slot_idx = (uint32_t)dialog.GetResultInteger();
        }

        const PalEntry &dst = (*game_pal)[idx_me];
        PalEntry new_entry = dst;

        WaitDialog wait;
        wait.show();
        qApp->processEvents();

        if (X2mToEquipment(&x2m, slot_idx, new_entry.equipment, new_entry.cms_entry))
        {
            (*game_pal)[idx_me] = new_entry;

            EquipmentToGui(new_entry.equipment, new_entry.cms_entry);
            on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
        }

        wait.close();

        return;
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < pal->GetNumEntries())
            {
                const PalEntry &entry = (*pal)[idx];
                EquipmentToGui(entry.equipment, (*game_pal)[idx_me].cms_entry);
            }
        }

        delete dialog;

        if (pal != game_pal)
            delete pal;
    }
}

void MainWindow::on_statsCopyButton_triggered(QAction *arg1)
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    ListDialog *dialog = nullptr;
    PalFile *pal = nullptr;

    if (arg1 == ui->actionFromOtherEntryStats)
    {
        dialog = new ListDialog(ListMode::PAL, this);
        pal = game_pal;
    }
    else if (arg1 == ui->actionFromExternalPALStats)
    {
        QString file = QFileDialog::getOpenFileName(this, "External PAL", config.lf_external_pal, "PAL Files (*.pal *.pal.xml)");

        if (file.isNull())
            return;

        config.lf_external_pal = file;

        pal = new PalFile();
        if (!pal->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete pal;
            return;
        }

        dialog = new ListDialog(ListMode::PAL, this, pal);
    }
    else if (arg1 == ui->actionFromCACStats)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose save file", config.lf_external_sav, "XV2 SAV Files (DBXV2.sav)");

        if (file.isNull())
            return;

        config.lf_external_sav = file;
        Xv2SavFile sav;

        if (!sav.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        ListDialog cac_dialog(ListMode::CAC_FULL, this, &sav);

        if (cac_dialog.exec())
        {
            uint32_t result = cac_dialog.GetResultData();
            uint32_t cac = result / XV2_NUM_PRESETS;
            uint32_t set = result % XV2_NUM_PRESETS;

            const PalEntry &dst = (*game_pal)[idx_me];
            PalEntry new_entry = dst;

            if (CacToStats(sav.GetCac(cac), set, new_entry.stats))
            {
                (*game_pal)[idx_me] = new_entry;

                StatsToGui(new_entry.stats);
                on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
            }
            else
            {
               DPRINTF("Failed to convert cac to npc.\n");
            }
        }

        return;
    }
    else if (arg1 == ui->actionFromX2mStats)
    {
        QString file = QFileDialog::getOpenFileName(this, "Choose x2m file", config.lf_installer_open, "X2M Files (*.x2m)");

        if (file.isNull())
            return;

        config.lf_installer_open = file;
        X2mFile x2m;

        if (!x2m.LoadFromFile(Utils::QStringToStdString(file)))
            return;

        uint32_t slot_idx = 0;

        if (x2m.GetNumSlotEntries() > 1)
        {
            ListDialog dialog(ListMode::X2M_SLOT, this, &x2m);

            if (!dialog.exec())
                return;

            if (dialog.GetResultInteger() < 0 || dialog.GetResultInteger() >= (int)x2m.GetNumSlotEntries())
            {
                DPRINTF("You didn't select a slot.\n");
                return;
            }

            slot_idx = (uint32_t)dialog.GetResultInteger();
        }

        const PalEntry &dst = (*game_pal)[idx_me];
        PalEntry new_entry = dst;

        if (X2mToStats(&x2m, slot_idx, new_entry.stats))
        {
            (*game_pal)[idx_me] = new_entry;

            StatsToGui(new_entry.stats);
            on_langComboBox_currentIndexChanged(ui->langComboBox->currentIndex());
        }

        return;
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < pal->GetNumEntries())
            {
                const PalEntry &entry = (*pal)[idx];
                StatsToGui(entry.stats);
            }
        }

        delete dialog;

        if (pal != game_pal)
            delete pal;
    }
}

void MainWindow::on_applyButton_clicked()
{
    int idx = ui->entriesComboBox->currentIndex();

    if (idx >= 0 && idx < game_pal->GetNumEntries())
    {
        PalEntry &entry = (*game_pal)[idx];
        GuiToEntry(entry);
    }

    if (!Xenoverse2::CommitLobbyText())
    {
        DPRINTF("CommitLobbyText failed.\n");
        return;
    }

    if (!Xenoverse2::CommitLobby())
    {
        DPRINTF("CommitLobby failed.\n");
        return;
    }

    UPRINTF("Data applied succesfully!");
}

void MainWindow::on_allLangCheck_clicked()
{
    ui->langComboBox->setEnabled(!ui->allLangCheck->isChecked());
}

void MainWindow::on_actionConvert2X2M_triggered()
{
    int idx = ui->entriesComboBox->currentIndex();
    if (idx < 0 || idx >= game_pal->GetNumEntries())
        return;

    int lang = ui->langComboBox->currentIndex();
    if (lang < 0 || lang >= XV2_LANG_NUM)
        return;

    PalEntry &entry = (*game_pal)[idx];
    GuiToEntry(entry);

    CmsEntry *cms = game_cms->FindEntryByID(entry.cms_entry);
    if (!cms)
    {
        DPRINTF("Cannot find cms of race 0x%x\n", entry.cms_entry);
        return;
    }

    X2mConvDialog dialog(this);

    if (!dialog.exec())
        return;

    QString file = QFileDialog::getSaveFileName(this, "Save", config.GetDirectory(config.lf_editor_save), "X2M Files (*.x2m)");
    if (file.isNull())
        return;

    config.lf_editor_save = file;

    uint8_t guid[16];
    bool random_guid = false;
    int guid_mode = dialog.GetGuidMode();

    if (guid_mode == 0)
    {
        // Md5 (name+race)
        uint8_t md5_race[16];
        std::string name;

        if (Xenoverse2::GetLobbyName(entry.name_id, name, lang))
        {
            Utils::Md5(name.c_str(), (uint32_t)name.length(), guid);
        }
        else
        {
            random_guid = true;
        }

        Utils::Md5(cms->name.c_str(), (uint32_t)cms->name.length(), md5_race);
        Utils::XorBuf(guid, md5_race, sizeof(guid));
    }
    else if (guid_mode == 1)
    {
        std::string name;

        if (Xenoverse2::GetLobbyName(entry.name_id, name, lang))
        {
            Utils::Md5(name.c_str(), (uint32_t)name.length(), guid);
        }
        else
        {
            random_guid = true;
        }
    }
    else
    {
        random_guid = true;
    }

    WaitDialog wait;

    wait.show();
    qApp->processEvents();

    bool ret= EntryToX2m(entry, file, dialog.GetCode(), (random_guid) ? nullptr : guid, dialog.UseCssVoice());
    wait.close();

    if (ret)
    {
        UPRINTF("Conversion done succesfully.\n");
    }
}

void MainWindow::on_actionExportCac_triggered()
{
    int idx = ui->entriesComboBox->currentIndex();
    if (idx < 0 || idx >= game_pal->GetNumEntries())
        return;

    PalEntry &entry = (*game_pal)[idx];
    GuiToEntry(entry);

    QString file = QFileDialog::getOpenFileName(this, "Choose save file", config.lf_external_sav, "XV2 SAV Files (DBXV2.sav)");

    if (file.isNull())
        return;

    config.lf_external_sav = file;
    Xv2SavFile sav;

    if (!sav.LoadFromFile(Utils::QStringToStdString(file)))
        return;

    ListDialog cac_dialog(ListMode::CAC_FULL, this, &sav);

    if (!cac_dialog.exec())
        return;

    uint32_t result = cac_dialog.GetResultData();
    uint32_t cac = result / XV2_NUM_PRESETS;
    uint32_t set = result % XV2_NUM_PRESETS;

    ExportCacDialog export_dialog(this);

    if (!export_dialog.exec())
        return;

    if (!EntryToCac(entry, sav.GetCac(cac), set, export_dialog.DoInfo(), export_dialog.DoLooks(),
        export_dialog.DoEquip(), export_dialog.DoStats(), export_dialog.DoSkills()))
    {
         return;
    }

    QMessageBox box(this);
    QCheckBox *check = new QCheckBox("I acknowledge that I have either made a backup of the save file or that I don't give a shit about the save.");

    box.addButton(QMessageBox::Yes);
    box.addButton(QMessageBox::No);
    box.setCheckBox(check);
    box.setIcon(QMessageBox::Icon::Information);
    box.setWindowTitle("Apply changes to sav file?");
    box.setText("Changes are about to be applied to the sav file. "
                "If you haven't made a backup, this is the last opportunity to do it "
                "or to cancel the operation.\n\nDo you want to apply the changes?\n");

    if (box.exec() != QMessageBox::Yes)
    {
        return;
    }

    if (!check->isChecked())
    {
        DPRINTF("Operation cancelled (you didn't acknowledge to the text).\n");
        return;
    }

    if (!sav.SaveToFile(Utils::QStringToStdString(file)))
        return;

    UPRINTF("Citizen exported to CaC succesfully!\n");
}

void MainWindow::on_skinColor1Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("SKIN_", ui->skinColor1Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.skin_color1))
    {
        BcsColorToButton("SKIN_", me.equipment.skin_color1, me.cms_entry, ui->skinColor1Button);
    }
}

void MainWindow::on_skinColor2Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("SKIN_B_", ui->skinColor2Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.skin_color2))
    {
        BcsColorToButton("SKIN_B_", me.equipment.skin_color2, me.cms_entry, ui->skinColor2Button);
    }
}

void MainWindow::on_skinColor3Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("SKIN_C_", ui->skinColor3Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.skin_color3))
    {
        BcsColorToButton("SKIN_C_", me.equipment.skin_color3, me.cms_entry, ui->skinColor3Button);
    }
}

void MainWindow::on_skinColor4Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("SKIN_D_", ui->skinColor4Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.skin_color4))
    {
        BcsColorToButton("SKIN_D_", me.equipment.skin_color4, me.cms_entry, ui->skinColor4Button);
    }
}

void MainWindow::on_hairColorButton_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("HAIR_", ui->hairColorButton->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.hair_color))
    {
        BcsColorToButton("HAIR_", me.equipment.hair_color, me.cms_entry, ui->hairColorButton);
    }
}

void MainWindow::on_eyesColorButton_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("eye_", ui->eyesColorButton->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.eye_color))
    {
        BcsColorToButton("eye_", me.equipment.eye_color, me.cms_entry, ui->eyesColorButton);
    }
}

void MainWindow::on_topColor1Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC00_BUST_", ui->topColor1Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.top_color1))
    {
        BcsColorToButton("CC00_BUST_", me.equipment.top_color1, me.cms_entry, ui->topColor1Button);
    }
}

void MainWindow::on_topColor2Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC01_BUST_", ui->topColor2Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.top_color2))
    {
        BcsColorToButton("CC01_BUST_", me.equipment.top_color2, me.cms_entry, ui->topColor2Button);
    }
}

void MainWindow::on_topColor3Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC02_BUST_", ui->topColor3Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.top_color3))
    {
        BcsColorToButton("CC02_BUST_", me.equipment.top_color3, me.cms_entry, ui->topColor3Button);
    }
}

void MainWindow::on_topColor4Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC03_BUST_", ui->topColor4Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.top_color4))
    {
        BcsColorToButton("CC03_BUST_", me.equipment.top_color4, me.cms_entry, ui->topColor4Button);
    }
}

void MainWindow::on_bottomColor1Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC00_PANTS_", ui->bottomColor1Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.bottom_color1))
    {
        BcsColorToButton("CC00_PANTS_", me.equipment.bottom_color1, me.cms_entry, ui->bottomColor1Button);
    }
}

void MainWindow::on_bottomColor2Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC01_PANTS_", ui->bottomColor2Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.bottom_color2))
    {
        BcsColorToButton("CC01_PANTS_", me.equipment.bottom_color2, me.cms_entry, ui->bottomColor2Button);
    }
}

void MainWindow::on_bottomColor3Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC02_PANTS_", ui->bottomColor3Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.bottom_color3))
    {
        BcsColorToButton("CC02_PANTS_", me.equipment.bottom_color3, me.cms_entry, ui->bottomColor3Button);
    }
}

void MainWindow::on_bottomColor4Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC03_PANTS_", ui->bottomColor4Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.bottom_color4))
    {
        BcsColorToButton("CC03_PANTS_", me.equipment.bottom_color4, me.cms_entry, ui->bottomColor4Button);
    }
}

void MainWindow::on_glovesColor1Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC00_RIST_", ui->glovesColor1Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.gloves_color1))
    {
        BcsColorToButton("CC00_RIST_", me.equipment.gloves_color1, me.cms_entry, ui->glovesColor1Button);
    }
}

void MainWindow::on_glovesColor2Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC01_RIST_", ui->glovesColor2Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.gloves_color2))
    {
        BcsColorToButton("CC01_RIST_", me.equipment.gloves_color2, me.cms_entry, ui->glovesColor2Button);
    }
}

void MainWindow::on_glovesColor3Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC02_RIST_", ui->glovesColor3Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.gloves_color3))
    {
        BcsColorToButton("CC02_RIST_", me.equipment.gloves_color3, me.cms_entry, ui->glovesColor3Button);
    }
}

void MainWindow::on_glovesColor4Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC03_RIST_", ui->glovesColor4Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.gloves_color4))
    {
        BcsColorToButton("CC03_RIST_", me.equipment.gloves_color4, me.cms_entry, ui->glovesColor4Button);
    }
}

void MainWindow::on_shoesColor1Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC00_BOOTS_", ui->shoesColor1Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.shoes_color1))
    {
        BcsColorToButton("CC00_BOOTS_", me.equipment.shoes_color1, me.cms_entry, ui->shoesColor1Button);
    }
}

void MainWindow::on_shoesColor2Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC01_BOOTS_", ui->shoesColor2Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.shoes_color2))
    {
        BcsColorToButton("CC01_BOOTS_", me.equipment.shoes_color2, me.cms_entry, ui->shoesColor2Button);
    }
}

void MainWindow::on_shoesColor3Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC02_BOOTS_", ui->shoesColor3Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.shoes_color3))
    {
        BcsColorToButton("CC02_BOOTS_", me.equipment.shoes_color3, me.cms_entry, ui->shoesColor3Button);
    }
}

void MainWindow::on_shoesColor4Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("CC03_BOOTS_", ui->shoesColor4Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.shoes_color4))
    {
        BcsColorToButton("CC03_BOOTS_", me.equipment.shoes_color4, me.cms_entry, ui->shoesColor4Button);
    }
}

void MainWindow::on_makeupColor1Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("PAINT_A_", ui->makeupColor1Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.makeup_color1))
    {
        BcsColorToButton("PAINT_A_", me.equipment.makeup_color1, me.cms_entry, ui->makeupColor1Button);
    }
}

void MainWindow::on_makeupColor2Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("PAINT_B_", ui->makeupColor2Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.makeup_color2))
    {
        BcsColorToButton("PAINT_B_", me.equipment.makeup_color2, me.cms_entry, ui->makeupColor2Button);
    }
}

void MainWindow::on_makeupColor3Button_clicked()
{
    int idx_me = ui->entriesComboBox->currentIndex();

    if (idx_me < 0 || idx_me >= game_pal->GetNumEntries())
        return;

    PalEntry &me = (*game_pal)[idx_me];

    if (GetBcsColorFromDialog("PAINT_C_", ui->makeupColor3Button->property("bcs_color").toUInt(), me.cms_entry, &me.equipment.makeup_color3))
    {
        BcsColorToButton("PAINT_C_", me.equipment.makeup_color3, me.cms_entry, ui->makeupColor3Button);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    static bool done = false;

    if (index == 1 && !done)
    {
        done = true;

        ui->skinColor1Button->setAutoFillBackground(true);
        ui->skinColor1Button->update();
        ui->skinColor2Button->setAutoFillBackground(true);
        ui->skinColor2Button->update();
        ui->skinColor3Button->setAutoFillBackground(true);
        ui->skinColor3Button->update();
        ui->skinColor4Button->setAutoFillBackground(true);
        ui->skinColor4Button->update();

        ui->hairColorButton->setAutoFillBackground(true);
        ui->hairColorButton->update();
        ui->eyesColorButton->setAutoFillBackground(true);
        ui->eyesColorButton->update();

        ui->topColor1Button->setAutoFillBackground(true);
        ui->topColor1Button->update();
        ui->topColor2Button->setAutoFillBackground(true);
        ui->topColor2Button->update();
        ui->topColor3Button->setAutoFillBackground(true);
        ui->topColor3Button->update();
        ui->topColor4Button->setAutoFillBackground(true);
        ui->topColor4Button->update();

        ui->bottomColor1Button->setAutoFillBackground(true);
        ui->bottomColor1Button->update();
        ui->bottomColor2Button->setAutoFillBackground(true);
        ui->bottomColor2Button->update();
        ui->bottomColor3Button->setAutoFillBackground(true);
        ui->bottomColor3Button->update();
        ui->bottomColor4Button->setAutoFillBackground(true);
        ui->bottomColor4Button->update();

        ui->glovesColor1Button->setAutoFillBackground(true);
        ui->glovesColor1Button->update();
        ui->glovesColor2Button->setAutoFillBackground(true);
        ui->glovesColor2Button->update();
        ui->glovesColor3Button->setAutoFillBackground(true);
        ui->glovesColor3Button->update();
        ui->glovesColor4Button->setAutoFillBackground(true);
        ui->glovesColor4Button->update();

        ui->shoesColor1Button->setAutoFillBackground(true);
        ui->shoesColor1Button->update();
        ui->shoesColor2Button->setAutoFillBackground(true);
        ui->shoesColor2Button->update();
        ui->shoesColor3Button->setAutoFillBackground(true);
        ui->shoesColor3Button->update();
        ui->shoesColor4Button->setAutoFillBackground(true);
        ui->shoesColor4Button->update();

        ui->makeupColor1Button->setAutoFillBackground(true);
        ui->makeupColor1Button->update();
        ui->makeupColor2Button->setAutoFillBackground(true);
        ui->makeupColor2Button->update();
        ui->makeupColor3Button->setAutoFillBackground(true);
        ui->makeupColor3Button->update();
    }
}
