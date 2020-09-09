#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QToolTip>
#include <QClipboard>

#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QAudioOutput>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listdialog.h"
#include "cssdialog.h"
#include "embdialog.h"
#include "waitdialog.h"
#include "ttbchareventdialog.h"
#include "logdialog.h"

#include "Xenoverse2.h"
#include "EmbFile.h"
#include "xv2ins_common.h"
#include "Config.h"
#include "debug.h"

#define INTERNAL_DATA "Internal package content"
#define GAME_PREFIX "GAME:///"
#define AUTO_BTL_PORTRAIT   "Use game battle portrait auto-generation"
#define DDS_PREFIX  "DDS:///"
#define DIR_PREFIX  "DIR:///"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QLocale::setDefault(QLocale::c());
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (x2m)
        delete x2m;

    delete ui;
}

bool MainWindow::Initialize()
{
    ui->actionOpen->setIcon(ui->mainToolBar->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(ui->mainToolBar->style()->standardIcon(QStyle::SP_DialogSaveButton));

    // Main info tab
    ui->modVersionEdit->setValidator(new QDoubleValidator(this));
    ui->multNamesCostEdit->setValidator(new QIntValidator(this));
    ui->bodyShapeEdit->setValidator(new QIntValidator(0, 32767, this));
    ui->charNameClearButon->addAction(ui->actionCharNameClearAll);
    ui->charNameClearButon->addAction(ui->actionCharNameClearThis);
    ui->charNameClearButon->addAction(ui->actionCharNameClearAllButThis);
    // Files tab
    ui->selPortButton->addAction(ui->actionSelectFileSel);
    ui->selPortButton->addAction(ui->actionFromGameSel);
    ui->selPortButton->addAction(ui->actionFromGameBtlSel);
    ui->selPortButton->addAction(ui->actionFromEmbSel);
    ui->btlPortButton->addAction(ui->actionSelectFileBtl);
    ui->btlPortButton->addAction(ui->actionFromGameBtl);
    ui->btlPortButton->addAction(ui->actionUseAutoBtlPort);
    ui->btlPortButton->addAction(ui->actionCreateFromSingleDdsBtl);
    // Slots tab
    ui->costumeIndexEdit->setValidator(new QIntValidator(-32768,32767, this));
    ui->modelPresetEdit->setValidator(new QIntValidator(-32768,32767, this));
    ui->voice1Edit->setValidator(new QIntValidator(this));
    ui->voice2Edit->setValidator(new QIntValidator(this));
    ui->costumeNameClearButton->addAction(ui->actionCostumeNameClearAll);
    ui->costumeNameClearButton->addAction(ui->actionCostumeNameClearThis);
    ui->costumeNameClearButton->addAction(ui->actionCostumeNameClearAllButThis);    
    // Cms tab
    ui->cmsCopyButton->addAction(ui->actionFromGameCms);
    ui->cmsCopyButton->addAction(ui->actionFromExternalCms);
    ui->u10Edit->setValidator(new QIntValidator(this));
    ui->camDistEdit->setValidator(new QIntValidator(-32768,32767, this));
    ui->u16Edit->setValidator(new QIntValidator(-32768,32767, this));
    ui->u18Edit->setValidator(new QIntValidator(-32768,32767, this));
    ui->u1AEdit->setValidator(new QIntValidator(-32768,32767, this));
    // Cus tab
    //ui->cusUnknownEdit->setValidator(new QIntValidator(-32768,32767, this));
    ui->cusCopyButton->addAction(ui->actionFromGameCus);
    ui->cusCopyButton->addAction(ui->actionFromExternalCus);
    // Cso tab
    ui->csoCopyButton->addAction(ui->actionFromGameCso);
    ui->csoCopyButton->addAction(ui->actionFromExternalCso);
    // Psc tab
    ui->pscCamEdit->setValidator(new QIntValidator(this));
    ui->pscU0CEdit->setValidator(new QIntValidator(this));
    ui->pscU10Edit->setValidator(new QIntValidator(this));
    ui->pscHealthEdit->setValidator(new QDoubleValidator(this));
    ui->pscF18Edit->setValidator(new QDoubleValidator(this));
    ui->pscKiEdit->setValidator(new QDoubleValidator(this));
    ui->pscKiRechEdit->setValidator(new QDoubleValidator(this));
    ui->pscU24Edit->setValidator(new QIntValidator(this));
    ui->pscU28Edit->setValidator(new QIntValidator(this));
    ui->pscU2CEdit->setValidator(new QIntValidator(this));
    ui->pscStaminaEdit->setValidator(new QDoubleValidator(this));
    ui->pscStaMovEdit->setValidator(new QDoubleValidator(this));
    ui->pscStaAirEdit->setValidator(new QDoubleValidator(this));
    ui->pscStaGroundEdit->setValidator(new QDoubleValidator(this));
    ui->pscDrain1Edit->setValidator(new QDoubleValidator(this));
    ui->pscDrain2Edit->setValidator(new QDoubleValidator(this));
    ui->pscF48Edit->setValidator(new QDoubleValidator(this));
    ui->pscAtkEdit->setValidator(new QDoubleValidator(this));
    ui->pscKiAtkEdit->setValidator(new QDoubleValidator(this));
    ui->pscStrEdit->setValidator(new QDoubleValidator(this));
    ui->pscBlaEdit->setValidator(new QDoubleValidator(this));
    ui->pscPhyDefEdit->setValidator(new QDoubleValidator(this));
    ui->pscKiDefEdit->setValidator(new QDoubleValidator(this));
    ui->pscStrAtkDefEdit->setValidator(new QDoubleValidator(this));
    ui->pscKiBlastDefEdit->setValidator(new QDoubleValidator(this));
    ui->pscGroundSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->pscAirSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->pscBoostSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->pscDashDistEdit->setValidator(new QDoubleValidator(this));
    ui->pscF7CEdit->setValidator(new QDoubleValidator(this));
    ui->pscReinfEdit->setValidator(new QDoubleValidator(this));
    ui->pscF84Edit->setValidator(new QDoubleValidator(this));
    ui->pscRevivalHPEdit->setValidator(new QDoubleValidator(this));
    ui->pscF8CEdit->setValidator(new QDoubleValidator(this));
    ui->pscRevivingSpeed->setValidator(new QDoubleValidator(this));
    ui->pscU98Edit->setValidator(new QIntValidator(this));
    ui->pscSuperSoulEdit->setValidator(new QIntValidator(this));
    ui->pscSuperSoulButton->setStyleSheet("font-weight: bold;");
    ui->pscUB8Edit->setValidator(new QIntValidator(this));
    ui->pscUBCEdit->setValidator(new QIntValidator(this));
    ui->pscFC0Edit->setValidator(new QDoubleValidator(this));
    ui->pscCopyButton->addAction(ui->actionFromGamePsc);
    ui->pscCopyButton->addAction(ui->actionFromGamePscBuf);
    ui->pscCopyButton->addAction(ui->actionFromExternalPsc);
    // Aur tab
    ui->auraEdit->setValidator(new QIntValidator(this));
    ui->auraEf0Edit->setValidator(new QIntValidator(this));
    ui->auraEf1Edit->setValidator(new QIntValidator(this));
    ui->auraEf2Edit->setValidator(new QIntValidator(this));
    ui->auraEf3Edit->setValidator(new QIntValidator(this));
    ui->auraEf4Edit->setValidator(new QIntValidator(this));
    ui->auraEf5Edit->setValidator(new QIntValidator(this));
    ui->auraEf6Edit->setValidator(new QIntValidator(this));
    ui->aurCopyButton->addAction(ui->actionFromGameAur);
    ui->aurCopyButton->addAction(ui->actionFromExternalAur);
    ui->aurCopyButton->addAction(ui->actionFromAuraCharUsage);
    ui->aurCopyButton->addAction(ui->actionFromAuraSkillUsage);
    // Sev tab
    ui->sevHLCostIndexEdit->setValidator(new QIntValidator(this));
    ui->sevLLCostIndexEdit->setValidator(new QIntValidator(this));
    ui->sevLLOtherCostEdit->setValidator(new QIntValidator(this));    
    ui->sevLLEvEntU00Edit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntCueIdEdit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntFileIdEdit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntU0CEdit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntU10Edit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntNU0CEdit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntRespCueIdEdit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntRespFileIdEdit->setValidator(new QIntValidator(this));
    ui->sevLLEvEntU18Edit->setValidator(new QIntValidator(this));
    ui->sevLLCopyButton->addAction(ui->actionFromGameSev);
    ui->sevLLCopyButton->addAction(ui->actionFromExternalSev);
    ui->sevLLTypeComboBox->addItem("Start", QVariant(0));
    ui->sevLLTypeComboBox->addItem("Victory", QVariant(2));
    ui->sevLLTypeComboBox->addItem("Defeat", QVariant(3));
    ui->sevLLTypeComboBox->addItem("Damage taken", QVariant(5));
    // Cml tab
    ui->cmlU04Edit->setValidator(new QIntValidator(this));
    ui->cmlPosXEdit->setValidator(new QDoubleValidator(this));
    ui->cmlPosYEdit->setValidator(new QDoubleValidator(this));
    ui->cmlPosZEdit->setValidator(new QDoubleValidator(this));
    ui->cmlRotXEdit->setValidator(new QDoubleValidator(this));
    ui->cmlRotYEdit->setValidator(new QDoubleValidator(this));
    ui->cmlF0CEdit->setValidator(new QDoubleValidator(this));
    ui->cmlF10Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF14Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF18Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF1CEdit->setValidator(new QDoubleValidator(this));
    ui->cmlF20Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF24Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF28Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF2CEdit->setValidator(new QDoubleValidator(this));
    ui->cmlF30Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF34Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF38Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF3CEdit->setValidator(new QDoubleValidator(this));
    ui->cmlF40Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF44Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF48Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF5CEdit->setValidator(new QDoubleValidator(this));
    ui->cmlF60Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF64Edit->setValidator(new QDoubleValidator(this));
    ui->cmlF68Edit->setValidator(new QDoubleValidator(this));
    ui->cmlFN10Edit->setValidator(new QDoubleValidator(this));
    ui->cmlCopyButton->addAction(ui->actionFromGameCml);
    ui->cmlCopyButton->addAction(ui->actionFromExternalCml);
    // Hci tab
    ui->hciCostumeIndexEdit->setValidator(new QIntValidator(-32768,32767, this));
    ui->hciState1Edit->setValidator(new QIntValidator(-32768,32767, this));
    ui->hciState2Edit->setValidator(new QIntValidator(-32768,32767, this));
    ui->hciEmbIndexEdit->setValidator(new QIntValidator(this));
    ui->hciCopyButton->addAction(ui->actionFromGameHci);
    ui->hciCopyButton->addAction(ui->actionFromExternalHci);
    // Ttb tab
    ui->ttbHLCostComboBox->setValidator(new QIntValidator(this));
    ui->ttbLLU44Edit->setValidator(new QIntValidator(this));
    ui->ttbLLU48Edit->setValidator(new QIntValidator(this));
    ui->ttbLLCopyButton->addAction(ui->actionFromGameTtb);
    ui->ttbLLCopyButton->addAction(ui->actionFromExternalTtb);
    ui->ttbLLTypeComboBox->addItem("Versus", QVariant("versus"));
    ui->ttbLLTypeComboBox->addItem("Ally talk", QVariant("ally_talk"));
    ui->ttbLLTypeComboBox->addItem("Death", QVariant("death"));
    // Ttc tab
    ui->ttcHLCostComboBox->setValidator(new QIntValidator(this));
    ui->ttcLLTypeComboBox->addItem("Versus", QVariant(TTC_VERSUS_LIST));
    ui->ttcLLTypeComboBox->addItem("Minor damage", QVariant(TTC_MINOR_DAMAGE_LIST));
    ui->ttcLLTypeComboBox->addItem("Major damage", QVariant(TTC_MAJOR_DAMAGE_LIST));
    ui->ttcLLTypeComboBox->addItem("I ko'd the player", QVariant(TTC_PLAYER_KO_ENEMY_LIST));
    ui->ttcLLTypeComboBox->addItem("Strong attack (damaged)", QVariant(TTC_STRONG_ATTACK_DAMAGED_LIST));
    ui->ttcLLTypeComboBox->addItem("Power up", QVariant(TTC_POWER_UP_LIST));
    ui->ttcLLTypeComboBox->addItem("Start talk", QVariant(TTC_START_TALK_LIST));
    ui->ttcLLTypeComboBox->addItem("Player ally damaged by enemy", QVariant(TTC_PLAYER_DAMAGED_LIST));
    ui->ttcLLTypeComboBox->addItem("Little time", QVariant(TTC_LITTLE_TIME_LIST));
    ui->ttcLLTypeComboBox->addItem("Player ally killed an enemy", QVariant(TTC_PLAYER_ALLY_KILLED_ENEMY_LIST));
    ui->ttcLLTypeComboBox->addItem("Challenge", QVariant(TTC_CHALLENGE_LIST));
    ui->ttcLLTypeComboBox->addItem("I was ko'd", QVariant(TTC_KO_LIST));
    ui->ttcLLTypeComboBox->addItem("Entering the quest", QVariant(TTC_ENTERING_LIST));
    ui->ttcLLTypeComboBox->addItem("Master versus", QVariant(TTC_MASTER_VERSUS_LIST));
    ui->ttcLLTypeComboBox->addItem("Player ally has been ko'd by enemy", QVariant(TTC_PLAYER_KO_ALLY_LIST));
    ui->ttcLLTypeComboBox->addItem("Fight seriously", QVariant(TTC_FIGHT_SERIOUSLY_LIST));
    ui->ttcLLCostEdit->setValidator(new QIntValidator(this));
    ui->ttcLLTransEdit->setValidator(new QIntValidator(this));
    ui->ttcLLVerifyButton->addAction(ui->actionTTCVerifySingle);
    ui->ttcLLVerifyButton->addAction(ui->actionTTCVerifyAll);
    ui->ttcLLCopyButton->addAction(ui->actionFromGameTtc);
    ui->ttcLLCopyButton->addAction(ui->actionFromExternalTtc);
    ui->ttcLLCopyFilesButton->addAction(ui->actionTtcFilesAddData);
    ui->ttcLLCopyFilesButton->addAction(ui->actionTtcFilesCustomDir);
    // Cnc tab
    ui->cncCostumeIndexEdit->setValidator(new QIntValidator(0,32767, this));
    ui->cncModelPresetEdit->setValidator(new QIntValidator(0,32767, this));
    ui->cncCopyButton->addAction(ui->actionFromGameCnc);
    ui->cncCopyButton->addAction(ui->actionFromExternalCnc);

    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    Bootstrap(true, false);

    // Cus tab (post)
    FillSuperComboBox(ui->super1ComboBox);
    FillSuperComboBox(ui->super2ComboBox);
    FillSuperComboBox(ui->super3ComboBox);
    FillSuperComboBox(ui->super4ComboBox);
    FillUltimateComboBox(ui->ultimate1ComboBox);
    FillUltimateComboBox(ui->ultimate2ComboBox);
    FillEvasiveComboBox(ui->evasiveComboBox);
    FillBlastComboBox(ui->blastComboBox);
    FillAwakenComboBox(ui->awakenComboBox);
    // Slots tab (post)
    ui->voiceListenLangComboBox->setCurrentIndex(config.listen_css_language_english ? 1 : 0);
    // Sev tab (post)
    FillSevHLComboBox();
    ui->sevListenLangComboBox->setCurrentIndex(config.listen_sev_language_english ? 1 : 0);
    // Ttb tab (post)
    FillTtbHLComboBox();
    // Ttc tab (post)
    FillTtcHLComboBox();
    ui->ttcLLListenCombo->setCurrentIndex(config.listen_sev_language_english ? 1 : 0);
    // Cnc tab (post)
    FillDualSkillComboBox(ui->cncDualSkill1ComboBox);
    FillDualSkillComboBox(ui->cncDualSkill2ComboBox);
    FillDualSkillComboBox(ui->cncDualSkill3ComboBox);

    x2m = new X2mFile();
    x2m->SetType(X2mType::NEW_CHARACTER);

    // Handle arguments
    if (qApp->arguments().size() == 2)
    {
        QString file = qApp->arguments()[1];

        config.lf_editor_open = file;
        X2mFile *new_x2m = new X2mFile();

        if (new_x2m->LoadFromFile(Utils::QStringToStdString(file)))
        {
            delete x2m;
            x2m = new_x2m;
            config.lf_editor_save = file;
        }
        else
        {
            delete new_x2m;
        }
    }

    if (x2m->GetType() != X2mType::NEW_CHARACTER)
    {
        if (x2m->GetType() == X2mType::NEW_SKILL)
        {
            delete x2m; x2m = nullptr;

            QString file = qApp->arguments()[1];
            Utils::RunProgram("xv2skilcreat.exe", { Utils::QStringToStdString(file) }, false);
            return false;
        }
        else if (x2m->GetType() == X2mType::NEW_COSTUME)
        {
            delete x2m; x2m = nullptr;

            QString file = qApp->arguments()[1];
            Utils::RunProgram("xv2costcreat.exe", { Utils::QStringToStdString(file) }, false);
            return false;
        }
        else if (x2m->GetType() == X2mType::NEW_STAGE)
        {
            delete x2m; x2m = nullptr;

            QString file = qApp->arguments()[1];
            Utils::RunProgram("xv2stacreat.exe", { Utils::QStringToStdString(file) }, false);
            return false;
        }
        else if (x2m->GetType() == X2mType::NEW_QUEST)
        {
            delete x2m; x2m = nullptr;

            QString file = qApp->arguments()[1];
            Utils::RunProgram("xv2questcreat.exe", { Utils::QStringToStdString(file) }, false);
            return false;
        }

        DPRINTF("This kind of x2m is not supported.\n");
        return false;
    }

    ProcessX2m();
    return true;
}

bool MainWindow::ProcessShutdown()
{
    config.listen_css_language_english = (ui->voiceListenLangComboBox->currentIndex() == 1);
    config.listen_sev_language_english = (ui->sevListenLangComboBox->currentIndex() == 1);

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

void MainWindow::ProcessX2m()
{
    entry_name = x2m->GetEntryName();

    // Info tab
    ui->modNameEdit->setText(Utils::StdStringToQString(x2m->GetModName(), false));
    ui->modAuthorEdit->setText(Utils::StdStringToQString(x2m->GetModAuthor(), false));
    ui->modVersionEdit->setText(QString::number(x2m->GetModVersion()));
    ui->modGuidEdit->setText(Utils::StdStringToQString(x2m->GetModGuid()));
    ui->charaNameEdit->setText(Utils::StdStringToQString(x2m->GetCharaName(ui->charaNameLangComboBox->currentIndex()), false));
    ui->ozaruCheck->setChecked(x2m->IsOzaru());

    if (x2m->HasMultNames())
    {
        ui->multNamesCheck->setChecked(true);
    }
    else
    {
        ui->multNamesCheck->setChecked(false);
    }

    if (x2m->UsesBodyShape())
    {
        ui->bodyShapeCheck->setChecked(true);
        ui->bodyShapeEdit->setText(QString("%1").arg(x2m->GetBodyShape()));
    }
    else
    {
        ui->bodyShapeCheck->setChecked(false);
        ui->bodyShapeEdit->setText("0");
    }

    on_bodyShapeCheck_clicked();

    // Put dummy entry to gui
    X2mCharaName name_entry;
    NameEntryToGui(name_entry);

    on_multNamesCheck_clicked();

    ui->canUseAnyDualSkillCheck->setChecked(x2m->CanUseAnyDualSkill());

    // Files tab
    if (x2m->CharaDirectoryExists())
    {
        ui->charaFilesEdit->setText(INTERNAL_DATA);
        ui->charaFilesEdit->setEnabled(false);
    }
    else
    {
        ui->charaFilesEdit->setText("");
        ui->charaFilesEdit->setEnabled(true);
    }

    if (x2m->SelPortraitExists())
    {
        ui->selPortEdit->setText(INTERNAL_DATA);
        ui->selPortEdit->setEnabled(false);
    }
    else
    {
        ui->selPortEdit->setText("");
        ui->selPortEdit->setEnabled(true);
    }

    if (x2m->BtlPortraitExists())
    {
        ui->btlPortEdit->setText(INTERNAL_DATA);
        ui->btlPortEdit->setEnabled(false);
    }
    else
    {
        ui->btlPortEdit->setText(AUTO_BTL_PORTRAIT);
        ui->btlPortEdit->setEnabled(false);
    }

    if (x2m->JungleExists())
    {
        ui->addDataEdit->setText(INTERNAL_DATA);
        ui->addDataEdit->setEnabled(false);
    }
    else
    {
        ui->addDataEdit->setText("");
        ui->addDataEdit->setEnabled(true);
    }

    // Clear audio comboboxes first
    // Clear slots one here too
    ui->slotsComboBox->clear();
    ui->voice1ComboBox->clear();
    ui->voice2ComboBox->clear();

    SetCssAudio(INTERNAL_DATA);

    // Slots tab
    // clear combobox first
    prev_slots_index = -1;
    ui->slotsComboBox->clear();

    size_t num_slot_entries = x2m->GetNumSlotEntries();

    if (num_slot_entries == 0)
    {
        X2mSlotEntry entry;

        x2m->AddSlotEntry(entry);
        num_slot_entries++;
    }

    for (size_t i = 0; i < num_slot_entries; i++)
    {
        ui->slotsComboBox->addItem(QString("Slot %1").arg(i));
    }

    if (num_slot_entries == 1)
    {
        ui->slotsRemoveButton->setDisabled(true);
    }

    on_voice1Check_clicked();
    on_voice2Check_clicked();

    // Cms tab
    if (x2m->HasValidCmsEntry())
    {
        CmsEntryToGui(x2m->GetCmsEntry());
    }

    // Cus tab
    // Handle depends first

    ui->cusDependsComboBox->clear();

    EraseCustomSkills(ui->super1ComboBox);
    EraseCustomSkills(ui->super2ComboBox);
    EraseCustomSkills(ui->super3ComboBox);
    EraseCustomSkills(ui->super4ComboBox);
    EraseCustomSkills(ui->ultimate1ComboBox);
    EraseCustomSkills(ui->ultimate2ComboBox);
    EraseCustomSkills(ui->evasiveComboBox);
    EraseCustomSkills(ui->blastComboBox);
    EraseCustomSkills(ui->awakenComboBox);

    if (x2m->HasCharaSkillDepends())
    {
        FillCustomSuper(ui->super1ComboBox);
        FillCustomSuper(ui->super2ComboBox);
        FillCustomSuper(ui->super3ComboBox);
        FillCustomSuper(ui->super4ComboBox);
        FillCustomUltimate(ui->ultimate1ComboBox);
        FillCustomUltimate(ui->ultimate2ComboBox);
        FillCustomEvasive(ui->evasiveComboBox);
        FillCustomBlast(ui->blastComboBox);
        FillCustomAwaken(ui->awakenComboBox);

        for (size_t i = 0; i < x2m->GetNumCharaSkillDepends(); i++)
        {
            QString text;

            if (x2m->CharaSkillDependsHasAttachment(i))
            {
                text = "[EMBEDDED] ";
            }
            else
            {
                text = "[LINKED] ";
            }

            text += Utils::StdStringToQString(x2m->GetCharaSkillDepends(i).name, false);
            ui->cusDependsComboBox->addItem(text);
        }

        ui->cusDependsRemoveButton->setEnabled(true);
        ui->cusDependsUpdateButton->setEnabled(true);
    }
    else
    {
        ui->cusDependsRemoveButton->setEnabled(false);
        ui->cusDependsUpdateButton->setEnabled(false);
    }

    // clear combobox first
    prev_cus_slots_index = -1;
    ui->cusSlotsComboBox->clear();

    size_t num_cus_entries = x2m->GetNumSkillSets();

    if (num_cus_entries == 0)
    {
        CusSkillSet entry;

        x2m->AddSkillSet(entry);
        num_cus_entries++;
    }

    if (num_cus_entries != num_slot_entries)
    {
        DPRINTF("Internal error in ProcessX2m\n");
        exit(-1);
    }

    for (size_t i = 0; i < num_slot_entries; i++)
    {
        ui->cusSlotsComboBox->addItem(QString("Slot %1").arg(i));
    }

    // Cso tab
    if (x2m->HasCso())
    {
        ui->csoCheck->setChecked(true);
    }
    else
    {
        ui->csoCheck->setChecked(false);
    }

    // Put dummy entry to gui
    CsoEntry cso_entry;
    CsoEntryToGui(cso_entry);

    on_csoCheck_clicked();

    // Psc tab
    if (x2m->HasPsc())
    {
        ui->pscCheck->setChecked(true);
    }
    else
    {
        ui->pscCheck->setChecked(false);
    }

    // Put dummy entry to gui
    PscSpecEntry psc_entry;
    PscEntryToGui(psc_entry);

    on_pscCheck_clicked();

    // Aur tab
    if (x2m->HasAur())
    {
        ui->aurCheck->setChecked(true);
    }
    else
    {
        ui->aurCheck->setChecked(false);
    }

    while (x2m->GetNumCharaAuras(false) < x2m->GetNumAurEntries())
    {
        AurAura aura;
        x2m->AddCharaAura(aura);
    }

    // Put dummy entry to gui
    AurCharaLink aur_entry;
    AurAura aura;
    AurEntryToGui(aur_entry, aura);

    on_aurCheck_clicked();

    // Sev tab
    if (x2m->HasSev())
    {
        ui->sevCheck->setChecked(true);

        if (x2m->HasSevHL())
        {
            ui->sevModeComboBox->setCurrentIndex(0);
        }
        else
        {
            ui->sevModeComboBox->setCurrentIndex(1);
        }
    }
    else
    {
        ui->sevCheck->setChecked(false);
    }

    // Put dummy entries to gui
    SevEntryHL sev_hl;
    SevEntryLL sev_ll(true);

    SevEntryHLToGui(sev_hl);
    SevEntryLLToGui(sev_ll);

    on_sevCheck_clicked();

    // Cml tab
    if (x2m->HasCml())
    {
        ui->cmlCheck->setChecked(true);
    }
    else
    {
        ui->cmlCheck->setChecked(false);
    }

    // Put dummy entry to gui
    CmlEntry cml_entry;
    CmlEntryToGui(cml_entry);

    on_cmlCheck_clicked();

    // Hci tab
    if (x2m->HasHci())
    {
        ui->hciCheck->setChecked(true);
    }
    else
    {
        ui->hciCheck->setChecked(false);
    }

    // Put hci entry to gui
    HciEntry hci_entry;
    HciEntryToGui(hci_entry);

    on_hciCheck_clicked();

    // Update old css audio to audio if needed
    if (x2m->GetFormatVersion() < x2m->X2M_MIN_VERSION_CUSTOM_AUDIO)
    {
        x2m->RenamePath(X2M_CSS_AUDIO, X2M_AUDIO);
    }

    // Ttb tab
    if (x2m->HasTtb())
    {
        ui->ttbCheck->setChecked(true);

        if (x2m->HasTtbHL())
        {
            ui->ttbModeComboBox->setCurrentIndex(0);
        }
        else
        {
            ui->ttbModeComboBox->setCurrentIndex(1);
        }
    }
    else
    {
        ui->ttbCheck->setChecked(false);
    }

    // Put dummy entries to gui
    TtbEntryHL ttb_hl;
    TtbEntryLL ttb_ll;

    TtbEntryHLToGui(ttb_hl);
    TtbEntryLLToGui(ttb_ll, true);

    on_ttbCheck_clicked();

    // Ttc tab
    if (x2m->HasTtc())
    {
        ui->ttcCheck->setChecked(true);

        if (x2m->HasTtcHL())
        {
            ui->ttcModeComboBox->setCurrentIndex(0);
        }
        else
        {
            ui->ttcModeComboBox->setCurrentIndex(1);
        }
    }
    else
    {
        ui->ttcCheck->setChecked(false);
    }

    // Put dummy entries to gui
    TtcEntryHL ttc_hl;
    TtcEntryLL ttc_ll;

    TtcEntryHLToGui(ttc_hl);
    TtcEntryLLToGui(ttc_ll);

    on_ttcCheck_clicked();

    // Cnc tab
    if (x2m->HasCnc())
    {
        ui->cncCheck->setChecked(true);
    }
    else
    {
        ui->cncCheck->setChecked(false);
    }

    // Put dummy entry to gui
    CncEntry cnc_entry;
    CncEntryToGui(cnc_entry);

    on_cncCheck_clicked();

    // Vfx tab
    ui->vfxEdit->setText(Utils::StdStringToQString(x2m->GetCharEepk()));

    // Update to new format
    x2m->SetFormatVersion(x2m->X2M_CURRENT_VERSION);
}

bool MainWindow::Validate()
{
    // Info tab
    if (ui->modNameEdit->text().isEmpty())
    {
        DPRINTF("[INFO] Mod name cannot be empty.\n");
        return false;
    }

    if (ui->modAuthorEdit->text().isEmpty())
    {
        DPRINTF("[INFO] Mod author cannot be empty.\n");
        return false;
    }

    if (ui->modVersionEdit->text().isEmpty())
    {
        DPRINTF("[INFO] Mod version cannot be empty.\n");
        return false;
    }

    if (Utils::IsEmptyString(x2m->GetCharaName(XV2_LANG_ENGLISH)))
    {
        DPRINTF("[INFO] Character name cannot be empty for english language.\n");
        return false;
    }

    if (ui->multNamesCheck->isChecked())
    {
        if (ui->multNamesCostEdit->text().isEmpty())
        {
            DPRINTF("[INFO] Costume index cannot be empty.\n");
            return false;
        }

        // Push current gui entry to the x2m
        int index = ui->multNamesComboBox->currentIndex();
        if (index < 0 || index >= (int)x2m->GetNumMultNames())
        {
            DPRINTF("%s: mult names combobox index out of bounds.\n", FUNCNAME);
            return false;
        }

        X2mCharaName &name_entry = x2m->GetMultName((size_t)index);
        GuiToNameEntry(name_entry);

        std::unordered_set<uint32_t> costumes;

        for (size_t i = 0; i < x2m->GetNumMultNames(); i++)
        {
            const X2mCharaName &name_entry = x2m->GetMultName(i);

            if (Utils::IsEmptyString(name_entry.chara_name[XV2_LANG_ENGLISH]))
            {
                DPRINTF("[INFO] Chara name cannot be empty for english language.\n"
                        "In specific names, in entry %Id, with costume index %d.\n", i, name_entry.costume_index);
                return false;
            }

            if (costumes.find(name_entry.costume_index) != costumes.end())
            {
                DPRINTF("[INFO] Costume index %d has been used more than once.\n", name_entry.costume_index);
                return false;
            }

            costumes.insert(name_entry.costume_index);
        }
    }

    if (ui->bodyShapeCheck->isChecked())
    {
        if (ui->bodyShapeEdit->text().isEmpty())
        {
            DPRINTF("[INFO] Body shape value cannot be empty when enabled.\n");
            return false;
        }
    }

    // Files tab
    QString chara_files = ui->charaFilesEdit->text().trimmed();
    if (chara_files.isEmpty())
    {
        DPRINTF("[FILES] Character files cannot be empty.\n");
        return false;
    }

    if (ui->charaFilesEdit->isEnabled())
    {
        std::string chara_files_std = Utils::NormalizePath(Utils::QStringToStdString(chara_files));

        if (!Utils::DirExists(chara_files_std))
        {
            DPRINTF("[FILES] Directory %s doesn't exist.\n", chara_files_std.c_str());
            return false;
        }
        else if (Utils::IsDirectoryEmpty(chara_files_std, true))
        {
            DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", chara_files_std.c_str());
            return false;
        }

        std::string entry_name;
        size_t pos = chara_files_std.rfind('/');
        if (pos == std::string::npos)
            entry_name = chara_files_std;
        else
            entry_name = chara_files_std.substr(pos+1);

        if (entry_name.length() != 3)
        {
            DPRINTF("[FILES] Chara directory must point to a three-letter directory.\n");
            return false;
        }
    }

    QString sel_port = ui->selPortEdit->text().trimmed();
    if (sel_port.isEmpty())
    {
        DPRINTF("[FILES] Select portrait cannot be empty.\n");
        return false;
    }

    if (ui->selPortEdit->isEnabled())
    {
        std::string sel_port_std = Utils::QStringToStdString(sel_port);

        if (!Utils::FileExists(sel_port_std))
        {
            DPRINTF("[FILES] File %s doesn't exist.\n", sel_port_std.c_str());
            return false;
        }
    }

    QString btl_port = ui->btlPortEdit->text().trimmed();
    if (btl_port.isEmpty())
    {
        DPRINTF("[FILES] Battle portrait cannot be empty.\n");
        return false;
    }

    if (ui->btlPortEdit->isEnabled())
    {
        std::string btl_port_std = Utils::QStringToStdString(btl_port);

        if (!Utils::FileExists(btl_port_std))
        {
            DPRINTF("[FILES] File %s doesn't exist.\n", btl_port_std.c_str());
            return false;
        }
    }

    if (ui->addDataEdit->isEnabled())
    {
        QString add_data = ui->addDataEdit->text().trimmed();

        if (!add_data.isEmpty())
        {
            std::string add_data_std = Utils::QStringToStdString(add_data);

            if (!Utils::IsEmptyString(add_data_std))
            {
                if (!Utils::DirExists(add_data_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", add_data_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(add_data_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", add_data_std.c_str());
                    return false;
                }
            }
        }
    }

    // Slots tab
    for (size_t i = 0; i < x2m->GetNumSlotEntries(); i++)
    {
        const X2mSlotEntry &entry = x2m->GetSlotEntry(i);

        if (Utils::IsEmptyString(entry.costume_name[XV2_LANG_ENGLISH]))
        {
            DPRINTF("[SLOT] Costume name cannot be empty for english language (on Slot %Id)\n", i);
            return false;
        }
    }

    if (ui->costumeIndexEdit->text().isEmpty())
    {
        DPRINTF("[SLOT] Costume index cannot be empty.\n");
        return false;
    }

    if (ui->modelPresetEdit->text().isEmpty())
    {
        DPRINTF("[SLOT] Model preset cannot be empty.\n");
        return false;
    }

    if (ui->voice1Edit->text().isEmpty())
    {
        DPRINTF("[SLOT] Voice 1 field cannot be empty.\n");
        return false;
    }

    if (ui->voice2Edit->text().isEmpty())
    {
        DPRINTF("[SLOT] Voice 2 field cannot be empty.\n");
        return false;
    }

    size_t num_slots = x2m->GetNumSlotEntries();
    int slot_idx = ui->slotsComboBox->currentIndex();

    if (slot_idx < 0 || slot_idx >= num_slots)
    {
        DPRINTF("%s: slot ComboBox index out of bounds.\n", FUNCNAME);
        return false;
    }

    GuiToSlotEntry(x2m->GetSlotEntry(slot_idx));

    struct Combination
    {
        int costume_index;
        int model_preset;

        inline bool operator==(const Combination &rhs) const
        {
            return (this->costume_index == rhs.costume_index && this->model_preset == rhs.model_preset);
        }

        inline bool operator!=(const Combination &rhs) const
        {
            return !(*this == rhs);
        }
    };

    if (num_slots > 1)
    {
        std::vector<Combination> combinations;

        for (size_t i = 0; i < num_slots; i++)
        {
            Combination comb;
            const X2mSlotEntry &entry = x2m->GetSlotEntry(i);

            comb.costume_index = entry.costume_index;
            comb.model_preset = entry.model_preset;

            if (std::find(combinations.begin(), combinations.end(), comb) == combinations.end())
            {
                combinations.push_back(comb);
            }
            else
            {
                DPRINTF("Every combination of costume_index+model_preset must be unique.\n");
                return false;
            }
        }
    }

    // CMS
    if (ui->u10Edit->text().isEmpty())
    {
        DPRINTF("[CMS] U_10 cannot be empty.\n");
        return false;
    }

    if (ui->camDistEdit->text().isEmpty())
    {
        DPRINTF("[CMS] Load cam dist cannot be empty.\n");
        return false;
    }

    if (ui->u16Edit->text().isEmpty())
    {
        DPRINTF("[CMS] U_16 cannot be empty.\n");
        return false;
    }

    if (ui->u18Edit->text().isEmpty())
    {
        DPRINTF("[CMS] U_18 cannot be empty.\n");
        return false;
    }

    if (ui->u1AEdit->text().isEmpty())
    {
        DPRINTF("[CMS] U_1A cannot be empty.\n");
        return false;
    }

    // CUS
    /*if (ui->cusUnknownEdit->text().isEmpty())
    {
        DPRINTF("[CUS] Unknown cannot be empty.\n");
        return false;
    }*/

    // Removed in 0.4. xv2patcher 0.6 can support these
    /*QString bac = ui->bacEdit->text();
    QString bcm = ui->bcmEdit->text();

    if (!bac.startsWith("..") || !bcm.startsWith(".."))
    {
        QString text = "I've detected that bac or bcm or both don't point to parent location.\n"
                       "The game may reject it and give a corrupted message.\n\n"
                       "Do you still want to proceed with package creation?\n";

        if (QMessageBox::question(this, "Proceed", text,
                                  QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                  QMessageBox::No) != QMessageBox::Yes)
        {
            return false;
        }
    }*/

    // Get costumes, will be used for cso, psc and aur
    std::unordered_set<int32_t> costumes;

    for (size_t i = 0; i < num_slots; i++)
    {
        costumes.insert(x2m->GetSlotEntry(i).costume_index);
    }

    // CSO
    if (ui->csoCheck->isChecked())
    {
        size_t num_cso = x2m->GetNumCsoEntries();

        if (num_cso != costumes.size())
        {
            DPRINTF("[CSO] Number of cso entries is not equal to number of different costume_index in slot section.\n\n"
                    "Number of cso entries = %Id, number of different costume_index = %Id\n", num_cso, costumes.size());
            return false;
        }
    }

    // PSC
    if (ui->pscCheck->isChecked())
    {
        size_t num_psc = x2m->GetNumPscEntries();

        if (num_psc != costumes.size())
        {
            DPRINTF("[PSC] Number of psc entries is not equal to number of different costume_index in slot section.\n\n"
                    "Number of psc entries = %Id, number of different costume_index = %Id\n", num_psc, costumes.size());
            return false;
        }

        if (ui->pscCamEdit->text().isEmpty() || ui->pscU0CEdit->text().isEmpty() || ui->pscU10Edit->text().isEmpty() ||
            ui->pscHealthEdit->text().isEmpty() || ui->pscF18Edit->text().isEmpty() || ui->pscKiEdit->text().isEmpty() ||
            ui->pscKiRechEdit->text().isEmpty() || ui->pscU24Edit->text().isEmpty() || ui->pscU28Edit->text().isEmpty() ||
            ui->pscU2CEdit->text().isEmpty() || ui->pscStaminaEdit->text().isEmpty() || ui->pscStaMovEdit->text().isEmpty() ||
            ui->pscStaAirEdit->text().isEmpty() || ui->pscStaGroundEdit->text().isEmpty() || ui->pscDrain1Edit->text().isEmpty() ||
            ui->pscDrain2Edit->text().isEmpty() || ui->pscF48Edit->text().isEmpty() || ui->pscAtkEdit->text().isEmpty() ||
            ui->pscKiAtkEdit->text().isEmpty() || ui->pscStrEdit->text().isEmpty() || ui->pscBlaEdit->text().isEmpty() ||
            ui->pscPhyDefEdit->text().isEmpty() || ui->pscKiDefEdit->text().isEmpty() || ui->pscStrAtkDefEdit->text().isEmpty() ||
            ui->pscKiBlastDefEdit->text().isEmpty() || ui->pscGroundSpeedEdit->text().isEmpty() || ui->pscAirSpeedEdit->text().isEmpty() ||
            ui->pscBoostSpeedEdit->text().isEmpty() || ui->pscDashDistEdit->text().isEmpty() || ui->pscF7CEdit->text().isEmpty() ||
            ui->pscReinfEdit->text().isEmpty() || ui->pscF84Edit->text().isEmpty() || ui->pscRevivalHPEdit->text().isEmpty() ||
            ui->pscF8CEdit->text().isEmpty() || ui->pscRevivingSpeed->text().isEmpty() || ui->pscU98Edit->text().isEmpty() ||
            ui->pscSuperSoulEdit->text().isEmpty() || ui->pscUB8Edit->text().isEmpty() || ui->pscUBCEdit->text().isEmpty() ||
            ui->pscFC0Edit->text().isEmpty())
        {
            DPRINTF("[PSC] None of the fields at psc tab can be empty.\n");
            return false;
        }
    }

    // AUR
    if (ui->aurCheck->isChecked())
    {
        size_t num_aur = x2m->GetNumAurEntries();

        if (num_aur != costumes.size())
        {
            DPRINTF("[AUR] Number of aur entries is not equal to number of different costume_index in slot section.\n\n"
                    "Number of aur entries = %Id, number of different costume_index = %Id\n", num_aur, costumes.size());
            return false;
        }

        if (ui->auraCustomCheck->isChecked())
        {
            if (ui->auraEf0Edit->text().isEmpty())
            {
                DPRINTF("[AURA] BoostStart cannot be empty.\n");
                return false;
            }

            if (ui->auraEf1Edit->text().isEmpty())
            {
                DPRINTF("[AURA] BoostLoop cannot be empty.\n");
                return false;
            }

            if (ui->auraEf2Edit->text().isEmpty())
            {
                DPRINTF("[AURA] BoostEnd cannot be empty.\n");
                return false;
            }

            if (ui->auraEf3Edit->text().isEmpty())
            {
                DPRINTF("[AURA] KiaiCharge cannot be empty.\n");
                return false;
            }

            if (ui->auraEf4Edit->text().isEmpty())
            {
                DPRINTF("[AURA] KiryokuMax cannot be empty.\n");
                return false;
            }

            if (ui->auraEf5Edit->text().isEmpty())
            {
                DPRINTF("[AURA] HenshinStart cannot be empty.\n");
                return false;
            }

            if (ui->auraEf6Edit->text().isEmpty())
            {
                DPRINTF("[AURA] HenshinEnd cannot be empty.\n");
                return false;
            }
        }
        else
        {
            if (ui->auraEdit->text().isEmpty())
            {
                DPRINTF("[AURA] Aura id cannot be empty.\n");
                return false;
            }
        }
    }

    // SEV
    if (ui->sevCheck->isChecked())
    {
        std::unordered_set<uint32_t> costumes;

        if (ui->sevModeComboBox->currentIndex() == 0)
        {
            int idx = ui->sevHLCostComboBox->currentIndex();

            if (idx < 0 || idx >= x2m->GetNumSevHLEntries())
            {
                DPRINTF("[SEV HL] Internal error, combobox out of bounds.\n");
                return false;
            }

            GuiToSevEntryHL(x2m->GetSevHLEntry(idx));

            for (size_t i = 0; i < x2m->GetNumSevHLEntries(); i++)
            {
                costumes.insert(x2m->GetSevHLEntry(i).costume_id);
            }

            if (costumes.size() != x2m->GetNumSevHLEntries())
            {
                DPRINTF("[SEV HL] Each \"My costume index\" must be unique.\n");
                return false;
            }
        }
        else
        {
            int idx = ui->sevLLCostComboBox->currentIndex();

            if (idx < 0 || idx >= x2m->GetNumSevLLEntries())
            {
                DPRINTF("[SEV LL] Internal error, combobox out of bounds.\n");
                return false;
            }

            GuiToSevEntryLL(x2m->GetSevLLEntry(idx));

            for (size_t i = 0; i < x2m->GetNumSevLLEntries(); i++)
            {
                const SevEntryLL &sev_entry = x2m->GetSevLLEntry(i);

                for (size_t j = 0; j < sev_entry.sub_entries.size(); j++)
                {
                    const SevEventsLL &sub_entry = sev_entry.sub_entries[j];

                    if (!sub_entry.im_source && sub_entry.char_code.length() != 3)
                    {
                        DPRINTF("[SEV LL] If direction is Other->Myself, char code must be a 3-letter code. In Entry %Id, Character %Id\n", i, j);
                        return false;
                    }
                    else if (sub_entry.char_code.length() != 0 && sub_entry.char_code.length() != 3)
                    {
                        DPRINTF("[SEV LL] char code must be an empty string or a 3-letter code. In Entry %Id, Character %Id\n", i, j);
                        return false;
                    }
                }

                costumes.insert(sev_entry.costume_id);
            }

            if (costumes.size() != x2m->GetNumSevLLEntries())
            {
                DPRINTF("[SEV LL] Each \"My costume index\" must be unique.\n");
                return false;
            }
        }
    }

    // CML
    if (ui->cmlCheck->isChecked())
    {
        size_t num_cml = x2m->GetNumCmlEntries();

        if (num_cml != costumes.size())
        {
            DPRINTF("[CML] Number of cml entries is not equal to number of different costume_index in slot section.\n\n"
                    "Number of cml entries = %Id, number of different costume_index = %Id\n", num_cml, costumes.size());
            return false;
        }

        if (ui->cmlU04Edit->text().isEmpty() || ui->cmlPosXEdit->text().isEmpty() || ui->cmlPosYEdit->text().isEmpty() ||
            ui->cmlPosZEdit->text().isEmpty() || ui->cmlRotXEdit->text().isEmpty() || ui->cmlRotYEdit->text().isEmpty() ||
            ui->cmlF0CEdit->text().isEmpty() || ui->cmlF10Edit->text().isEmpty() || ui->cmlF14Edit->text().isEmpty() ||
            ui->cmlF18Edit->text().isEmpty() || ui->cmlF1CEdit->text().isEmpty() || ui->cmlF20Edit->text().isEmpty() ||
            ui->cmlF24Edit->text().isEmpty() || ui->cmlF28Edit->text().isEmpty() || ui->cmlF2CEdit->text().isEmpty() ||
            ui->cmlF30Edit->text().isEmpty() || ui->cmlF34Edit->text().isEmpty() || ui->cmlF38Edit->text().isEmpty() ||
            ui->cmlF3CEdit->text().isEmpty() || ui->cmlF40Edit->text().isEmpty() || ui->cmlF44Edit->text().isEmpty() ||
            ui->cmlF48Edit->text().isEmpty() || ui->cmlF5CEdit->text().isEmpty() || ui->cmlF60Edit->text().isEmpty() ||
            ui->cmlF64Edit->text().isEmpty() || ui->cmlF68Edit->text().isEmpty())
        {
            DPRINTF("[CML] No fields in cml tab can be empty.\n");
            return false;
        }
    }

    // HCI
    if (ui->hciCheck->isChecked())
    {
        if (ui->hciCostumeIndexEdit->text().isEmpty() || ui->hciState1Edit->text().isEmpty() ||
            ui->hciState2Edit->text().isEmpty() || ui->hciEmbIndexEdit->text().isEmpty())
        {
            DPRINTF("[HCI] No field in hci tab can be empty.\n");
            return false;
        }
    }

    // TTB
    if (ui->ttbCheck->isChecked())
    {
        std::unordered_set<uint32_t> costumes;

        if (ui->ttbModeComboBox->currentIndex() == 0)
        {
            int idx = ui->ttbHLCostComboBox->currentIndex();

            if (idx < 0 || idx >= x2m->GetNumTtbHLEntries())
            {
                DPRINTF("[TTB HL] Internal error, combobox out of bounds.\n");
                return false;
            }

            GuiToTtbEntryHL(x2m->GetTtbHLEntry(idx));

            for (size_t i = 0; i < x2m->GetNumTtbHLEntries(); i++)
            {
                costumes.insert(x2m->GetTtbHLEntry(i).costume_id);
            }

            if (costumes.size() != x2m->GetNumTtbHLEntries())
            {
                DPRINTF("[TTB HL] Each \"My costume index\" must be unique.\n");
                return false;
            }
        }
        else
        {
            int idx = ui->ttbLLCostComboBox->currentIndex();

            if (idx < 0 || idx >= x2m->GetNumTtbLLEntries())
            {
                DPRINTF("[TTB LL] Internal error, combobox out of bounds.\n");
                return false;
            }

            GuiToTtbEntryLL(x2m->GetTtbLLEntry(idx));

            for (size_t i = 0; i < x2m->GetNumTtbLLEntries(); i++)
            {
                const TtbEntryLL &entry = x2m->GetTtbLLEntry(i);

                int num_myself = 0;

                if (entry.cms_code.length() == 0)
                {
                    num_myself++;
                }
                else
                {
                    if (entry.cms_code.length() != 3 || !Utils::IsAlphaNumeric(entry.cms_code))
                    {
                        DPRINTF("[TTB LL] Invalid cms \"%s\", in entry %Id\n", entry.cms_code.c_str(), i);
                        return false;
                    }
                }

                if (entry.event_res1.name.length() == 0)
                {
                    DPRINTF("[TTB LL] An empty ttb event was found, in entry %Id, actor %d\n", i, (entry.reverse_order) ? 2 : 1);
                    return false;
                }

                if (entry.cms2_code.length() == 0)
                {
                    num_myself++;
                }
                else
                {
                    if (entry.cms2_code.length() != 3 || !Utils::IsAlphaNumeric(entry.cms2_code))
                    {
                        DPRINTF("[TTB LL] Invalid cms \"%s\", in entry %Id\n", entry.cms2_code.c_str(), i);
                        return false;
                    }
                }

                if (entry.event_res2.name.length() == 0)
                {
                    DPRINTF("[TTB LL] An empty ttb event was found, in entry %Id, actor %d\n", i, (entry.reverse_order) ? 1 : 2);
                    return false;
                }

                if (entry.num_actors <= 2)
                    goto check_myself;

                if (entry.cms3_code.length() == 0)
                {
                    num_myself++;
                }
                else
                {
                    if (entry.cms3_code.length() != 3 || !Utils::IsAlphaNumeric(entry.cms3_code))
                    {
                        DPRINTF("[TTB LL] Invalid cms \"%s\", in entry %Id\n", entry.cms3_code.c_str(), i);
                        return false;
                    }
                }

                if (entry.event_res3.name.length() == 0)
                {
                    DPRINTF("[TTB LL] An empty ttb event was found, in entry %Id, actor 3\n", i);
                    return false;
                }

                if (entry.num_actors <= 3)
                    goto check_myself;

                if (entry.cms4_code.length() == 0)
                {
                    num_myself++;
                }
                else
                {
                    if (entry.cms4_code.length() != 3 || !Utils::IsAlphaNumeric(entry.cms4_code))
                    {
                        DPRINTF("[TTB LL] Invalid cms \"%s\", in entry %Id\n", entry.cms4_code.c_str(), i);
                        return false;
                    }
                }

                if (entry.event_res4.name.length() == 0)
                {
                    DPRINTF("[TTB LL] An empty ttb event was found, in entry %Id, actor 4\n", i);
                    return false;
                }

                if (entry.num_actors <= 4)
                    goto check_myself;

                if (entry.cms5_code.length() == 0)
                {
                    num_myself++;
                }
                else
                {
                    if (entry.cms5_code.length() != 3 || !Utils::IsAlphaNumeric(entry.cms5_code))
                    {
                        DPRINTF("[TTB LL] Invalid cms \"%s\", in entry %Id\n", entry.cms5_code.c_str(), i);
                        return false;
                    }
                }

                if (entry.event_res5.name.length() == 0)
                {
                    DPRINTF("[TTB LL] An empty ttb event was found, in entry %Id, actor 5\n", i);
                    return false;
                }

check_myself:

                if (num_myself == 0)
                {
                    DPRINTF("[TTB LL] Current char is not referenced in entry %Id.\n", i);
                    return false;
                }
                else if (num_myself != 1)
                {
                    DPRINTF("[TTB LL] Current char is referenced more than once, in entry %Id.\n", i);
                    return false;
                }

            } // end for
        }
    }

    // TTC
    if (ui->ttcCheck->isChecked())
    {
        if (ui->ttcModeComboBox->currentIndex() == 0)
        {
            std::unordered_set<uint32_t> costumes;

            int idx = ui->ttcHLCostComboBox->currentIndex();

            if (idx < 0 || idx >= x2m->GetNumTtcHLEntries())
            {
                DPRINTF("[TTC HL] Internal error, combobox out of bounds.\n");
                return false;
            }

            GuiToTtcEntryHL(x2m->GetTtcHLEntry(idx));

            for (size_t i = 0; i < x2m->GetNumTtcHLEntries(); i++)
            {
                costumes.insert(x2m->GetTtcHLEntry(i).costume_id);
            }

            if (costumes.size() != x2m->GetNumTtcHLEntries())
            {
                DPRINTF("[TTC HL] Each \"My costume index\" must be unique.\n");
                return false;
            }
        }
        else
        {
            int idx = ui->ttcLLCostComboBox->currentIndex();

            if (idx < 0 || idx >= x2m->GetNumTtcLLEntries())
            {
                DPRINTF("[TTC LL] Internal error, combobox out of bounds.\n");
                return false;
            }

            GuiToTtcEntryLL(x2m->GetTtcLLEntry(idx));

            for (size_t i = 0; i < x2m->GetNumTtcLLEntries(); i++)
            {
                const TtcEntryLL &entry = x2m->GetTtcLLEntry(i);

                if (Utils::IsEmptyString(entry.name))
                {
                    DPRINTF("[TTB LL] An empty ttb event name was found, in entry %Id\n", i);
                    return false;
                }
            }
        }
    }

    // CNC
    if (ui->cncCheck->isChecked())
    {
        if (ui->canUseAnyDualSkillCheck->isChecked())
        {
            DPRINTF("CNC cannot be used along with \"This char can respond to any dual request\"\n");
            return false;
        }

        int idx = ui->cncComboBox->currentIndex();

        if (idx < 0 || idx >= x2m->GetNumCncEntries())
        {
            DPRINTF("[CNC] Internal error, combobox out of bounds.\n");
            return false;
        }

        GuiToCncEntry(x2m->GetCncEntry(idx));

        std::vector<Combination> combinations;

        for (size_t i = 0; i < x2m->GetNumCncEntries(); i++)
        {
            Combination comb;
            const CncEntry &entry = x2m->GetCncEntry(i);

            if (!x2m->FindSlotEntry(entry.costume_index, entry.model_preset))
            {
                DPRINTF("[CNC] Slot with costume_index=%d and model_preset=%d doesn't exist (in Entry %Id)\n", entry.costume_index, entry.model_preset, i);
                return false;
            }

            comb.costume_index = entry.costume_index;
            comb.model_preset = entry.model_preset;

            if (std::find(combinations.begin(), combinations.end(), comb) == combinations.end())
            {
                combinations.push_back(comb);
            }
            else
            {
                DPRINTF("[CNC] Every combination of costume_index+model_preset must be unique (entry %Id is a duplicate of previous one)\n", i);
                return false;
            }
        }
    }

    return true;
}

uint8_t *MainWindow::GetGameSelPortrait(const QString &name, size_t *psize)
{
    return Xenoverse2::GetSelPortrait(Utils::QStringToStdString(name), psize);
}

uint8_t *MainWindow::GetGameBtlPortrait(const QString &file, size_t *psize)
{
    EmbFile emb;

    if (!file.endsWith(".emb", Qt::CaseInsensitive))
    {
        DPRINTF("%s: Bug.\n", FUNCNAME);
        return nullptr;
    }

    std::string code = Utils::QStringToStdString(file.left(file.lastIndexOf(".emb", -1, Qt::CaseInsensitive)));

    if (!xv2fs->LoadFile(&emb, "data/ui/texture/" + Utils::QStringToStdString(file)))
        return nullptr;

    if (emb.HasFileNames())
    {
        for (EmbContainedFile &file : emb)
        {
            std::string name = file.GetName();

            //DPRINTF("name = %s, code = %s\n", name.c_str(), code.c_str());

            if (Utils::BeginsWith(name, code, false))
            {
                name = entry_name + name.substr(code.length());
                file.SetName(name);
            }
        }
    }

    return emb.Save(psize);
}

bool MainWindow::Build()
{
    // Info
    x2m->SetModName(Utils::QStringToStdString(ui->modNameEdit->text(), false));
    x2m->SetModAuthor(Utils::QStringToStdString(ui->modAuthorEdit->text(), false));
    x2m->SetModVersion(ui->modVersionEdit->text().toFloat());
    // Guid is uneditable (and set by load or by constructor) - no need to set it here
    // Character name is already handled by on_charaNameEdit_textEdited
    x2m->SetOzaru(ui->ozaruCheck->isChecked());

    if (ui->multNamesCheck->isChecked())
    {
        // Nothing, gui entry was pushed to the file by Validate
    }
    else
    {
        while (x2m->GetNumMultNames() != 0)
            x2m->RemoveMultName(0);
    }

    if (ui->bodyShapeCheck->isChecked())
    {
        x2m->SetBodyShape(ui->bodyShapeEdit->text().toInt());
    }
    else
    {
        x2m->SetBodyShape(-1);
    }

    x2m->SetCanUseAnyDualSkill(ui->canUseAnyDualSkillCheck->isChecked());

    // Files
    QString chara_files = ui->charaFilesEdit->text().trimmed();

    if (ui->charaFilesEdit->isEnabled())
    {
        const std::string chara_files_std = Utils::NormalizePath(Utils::QStringToStdString(chara_files));
        std::string entry_name;

        size_t pos = chara_files_std.rfind('/');
        if (pos == std::string::npos)
            entry_name = chara_files_std;
        else
            entry_name = chara_files_std.substr(pos+1);

        std::string old_entry_name = x2m->GetEntryName();

        if (old_entry_name.length() == 3)
            x2m->DeleteDir(old_entry_name);

        x2m->SetEntryName(entry_name);

        if (entry_name != old_entry_name)
            x2m->DeleteDir(entry_name);

        if (!x2m->AddExternalDirectory(chara_files_std, entry_name))
        {
            DPRINTF("AddExternalDirectory failed on character files.\n");
            return false;
        }
    }
    else
    {
        if (chara_files != INTERNAL_DATA)
        {
            DPRINTF("%s: Bug in implementation (1).\n", FUNCNAME);
            return false;
        }

        // Do nothing, keep the data that already exists on the x2m
    }

    QString sel_port = ui->selPortEdit->text().trimmed();

    if (ui->selPortEdit->isEnabled())
    {
        size_t size;
        uint8_t *buf;

        buf = Utils::ReadFile(Utils::QStringToStdString(sel_port), &size);
        if (!buf)
            return false;

        bool ret = x2m->WriteFile(X2M_SEL_PORTRAIT, buf, size);
        delete[] buf;

        if (!ret)
            return false;
    }
    else
    {
        if (sel_port == INTERNAL_DATA)
        {
            // Nothing to do
        }
        else if (sel_port.startsWith(GAME_PREFIX))
        {
            QString game_dds = sel_port.mid((int)strlen(GAME_PREFIX));

            size_t size;
            uint8_t *buf;

            buf = GetGameSelPortrait(game_dds, &size);
            if (!buf)
            {
                DPRINTF("Failed getting sel portrait from game.\n");
                return false;
            }

            bool ret = x2m->WriteFile(X2M_SEL_PORTRAIT, buf, size);
            delete[] buf;

            if (!ret)
                return false;
        }
        else
        {
            DPRINTF("%s: Bug in implementation (2).\n", FUNCNAME);
            return false;
        }
    }

    QString btl_port = ui->btlPortEdit->text().trimmed();

    if (ui->btlPortEdit->isEnabled())
    {
        std::string btl_port_std = Utils::QStringToStdString(btl_port);
        uint8_t *buf;
        size_t size;

        if (btl_port.endsWith(".xml", Qt::CaseInsensitive))
        {
            EmbFile emb;

            if (!emb.CompileFromFile(btl_port_std))
            {
                DPRINTF("Failed to compile .emb.xml");
                return false;
            }

            buf = emb.Save(&size);
        }
        else
        {
            buf = Utils::ReadFile(btl_port_std, &size);
        }

        if (!buf)
            return false;

        bool ret = x2m->WriteFile(X2M_BTL_PORTRAIT, buf, size);
        delete[] buf;

        if (!ret)
            return false;
    }
    else
    {
        if (btl_port == INTERNAL_DATA)
        {
            // Do nothing
        }
        else if (btl_port.startsWith(GAME_PREFIX))
        {
            QString game_emb = btl_port.mid((int)strlen(GAME_PREFIX));

            size_t size;
            uint8_t *buf;

            buf = GetGameBtlPortrait(game_emb, &size);
            if (!buf)
            {
                DPRINTF("Failed getting btl portrait from game.\n");
                return false;
            }

            bool ret = x2m->WriteFile(X2M_BTL_PORTRAIT, buf, size);
            delete[] buf;

            if (!ret)
                return false;
        }
        else if (btl_port == AUTO_BTL_PORTRAIT)
        {
            x2m->RemoveFile(X2M_BTL_PORTRAIT);
        }
        else if (btl_port.startsWith(DDS_PREFIX))
        {
            QString file = btl_port.mid((int)strlen(DDS_PREFIX));

            size_t dds_size;
            uint8_t *dds_buf = Utils::ReadFile(Utils::QStringToStdString(file), &dds_size);
            if (!dds_buf)
                return false;

            EmbFile emb;
            EmbContainedFile dds;

            dds.SetData(dds_buf, dds_size, true);
            dds.SetName(entry_name + "_000.dds");
            emb.AppendFile(dds);

            size_t emb_size;
            uint8_t *emb_buf = emb.Save(&emb_size);

            if (!emb_buf)
            {
                DPRINTF("Internal error creating emb.\n");
                return false;
            }

            x2m->RemoveFile(X2M_BTL_PORTRAIT);
            bool ret = x2m->WriteFile(X2M_BTL_PORTRAIT, emb_buf, emb_size);
            delete[] emb_buf;

            if (!ret)
                return false;
        }
        else if (btl_port.startsWith(DIR_PREFIX))
        {
            QString dir = btl_port.mid((int)strlen(DDS_PREFIX));

            // TODO
        }
        else
        {
            DPRINTF("%s: Bug in implementation (3).\n", FUNCNAME);
            return false;
        }
    }

    QString add_data = ui->addDataEdit->text().trimmed();

    if (ui->addDataEdit->isEnabled())
    {
        std::string add_data_std = Utils::QStringToStdString(add_data);        

        x2m->DeleteJungle();

        if (!Utils::IsEmptyString(add_data_std) && !x2m->AddExternalDirectory(add_data_std, X2M_JUNGLE))
        {
            DPRINTF("AddExternalDirectory failed on additional data.\n");
            return false;
        }
    }
    else
    {
        if (add_data == INTERNAL_DATA)
        {
            // Do nothing
        }
        else
        {
            DPRINTF("%s: Bug in implementation (4).\n", FUNCNAME);
            return false;
        }
    }

    // Slot
    // We only need to set up current slot, as the rest is performed by the combobox event
    int slot_idx = ui->slotsComboBox->currentIndex();

    if (slot_idx < 0 || slot_idx >= x2m->GetNumSlotEntries())
    {
        DPRINTF("%s: slot ComboBox index out of bounds.\n", FUNCNAME);
        return false;
    }

    X2mSlotEntry &slot_entry = x2m->GetSlotEntry(slot_idx);
    GuiToSlotEntry(slot_entry);

    // Process audio entries
    for (size_t i = 0; i < x2m->GetNumSlotEntries(); i++)
    {
        X2mSlotEntry &slot_entry = x2m->GetSlotEntry(i);

        for (int j = 0; j < 2; j++)
        {
            if (slot_entry.audio_files[j].length() != 0)
            {
                if (x2m->AudioFileExists(slot_entry.audio_files[j]))
                    slot_entry.voices_id_list[j] = X2M_DUMMY_ID;
                else
                {
                    slot_entry.audio_files[j].clear();
                    slot_entry.voices_id_list[j] = -1;
                }
            }
            else
            {
                if (slot_entry.voices_id_list[j] == X2M_DUMMY_ID)
                    slot_entry.voices_id_list[j] = -1;
            }
        }
    }

    // Cms
    GuiToCmsEntry(x2m->GetCmsEntry());

    // CUS
    // We only need to set up current slot, as the rest is performed by the combobox event
    int cus_idx = ui->cusSlotsComboBox->currentIndex();

    if (cus_idx < 0 || cus_idx >= x2m->GetNumSkillSets())
    {
        DPRINTF("%s: cus slot ComboBox index out of bounds.\n", FUNCNAME);
        return false;
    }

    CusSkillSet &skill_entry = x2m->GetSkillSet(cus_idx);
    GuiToSkillSet(skill_entry);

    // CUS - custom skills remove unused
    for (size_t i = 0; i < x2m->GetNumCharaSkillDepends(); i++)
    {
        const X2mDepends &dep = x2m->GetCharaSkillDepends(i);

        if (!x2m->IsCharaSkillDependsReferenced(dep))
        {
            RemoveSkill(ui->super1ComboBox, dep.id);
            RemoveSkill(ui->super2ComboBox, dep.id);
            RemoveSkill(ui->super3ComboBox, dep.id);
            RemoveSkill(ui->super4ComboBox, dep.id);
            RemoveSkill(ui->ultimate1ComboBox, dep.id);
            RemoveSkill(ui->ultimate2ComboBox, dep.id);
            RemoveSkill(ui->evasiveComboBox, dep.id);
            RemoveSkill(ui->blastComboBox, dep.id);
            RemoveSkill(ui->awakenComboBox, dep.id);

            x2m->RemoveCharaSkillDependsAttachment(dep.guid);
            x2m->RemoveCharaSkillDepends(i);
            ui->cusDependsComboBox->removeItem((int)i);
            i--;

            if (ui->cusDependsComboBox->count() == 0)
            {
                ui->cusDependsRemoveButton->setDisabled(true);
                ui->cusDependsUpdateButton->setDisabled(true);
            }
        }
    }

    // CSO
    if (ui->csoCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int cso_idx = ui->csoCostComboBox->currentIndex();
        if (cso_idx < 0 || cso_idx >= x2m->GetNumCsoEntries())
        {
            DPRINTF("%s: cso entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        CsoEntry &entry = x2m->GetCsoEntry(cso_idx);
        GuiToCsoEntry(entry);
    }
    else
    {
        while (x2m->GetNumCsoEntries() != 0)
            x2m->RemoveCsoEntry(0);
    }

    // PSC
    if (ui->pscCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int psc_idx = ui->pscCostComboBox->currentIndex();
        if (psc_idx < 0 || psc_idx >= x2m->GetNumPscEntries())
        {
            DPRINTF("%s: psc entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        PscSpecEntry &entry = x2m->GetPscEntry(psc_idx);
        GuiToPscEntry(entry);
    }
    else
    {
        while (x2m->GetNumPscEntries() != 0)
            x2m->RemovePscEntry(0);
    }

    // AUR
    if (ui->aurCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int aur_idx = ui->aurCostComboBox->currentIndex();
        if (aur_idx < 0 || aur_idx >= x2m->GetNumAurEntries())
        {
            DPRINTF("%s: aur entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        AurCharaLink &entry = x2m->GetAurEntry(aur_idx);
        AurAura &aura = x2m->GetCharaAura(aur_idx);
        GuiToAurEntry(entry, aura);
    }
    else
    {
        while (x2m->GetNumAurEntries() != 0)
            x2m->RemoveAurEntry(0);

        while (x2m->GetNumCharaAuras(false) != 0)
            x2m->RemoveCharaAura(0);
    }

    // SEV
    if (ui->sevCheck->isChecked())
    {
        if (ui->sevModeComboBox->currentIndex() == 0)
        {
            x2m->RemoveAllSevLL();
            // No need to do further processing, Validate already pushed the current entry in the x2m
        }
        else
        {
            x2m->RemoveAllSevHL();
            // No need to do further processing, Validate already pushed the current entry in the x2m
        }
    }
    else
    {
        x2m->RemoveAllSevHL();
        x2m->RemoveAllSevLL();
    }

    // CML
    if (ui->cmlCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int cml_idx = ui->cmlCostComboBox->currentIndex();
        if (cml_idx < 0 || cml_idx >= x2m->GetNumCmlEntries())
        {
            DPRINTF("%s: cml entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        CmlEntry &entry = x2m->GetCmlEntry(cml_idx);
        GuiToCmlEntry(entry);
    }
    else
    {
        while (x2m->GetNumCmlEntries() != 0)
            x2m->RemoveCmlEntry(0);
    }

    // HCI
    if (ui->hciCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int hci_idx = ui->hciEntryComboBox->currentIndex();
        if (hci_idx < 0 || hci_idx >= x2m->GetNumHciEntries())
        {
            DPRINTF("%s: hci entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        HciEntry &entry = x2m->GetHciEntry(hci_idx);
        GuiToHciEntry(entry);
    }
    else
    {
        while (x2m->GetNumHciEntries() != 0)
            x2m->RemoveHciEntry(0);
    }

    // TTB
    if (ui->ttbCheck->isChecked())
    {
        if (ui->ttbModeComboBox->currentIndex() == 0)
        {
            x2m->RemoveAllTtbLL();
            // No need to do further processing, Validate already pushed the current entry in the x2m
        }
        else
        {
            x2m->RemoveAllTtbHL();
            // No need to do further processing, Validate already pushed the current entry in the x2m
        }
    }
    else
    {
        x2m->RemoveAllTtbHL();
        x2m->RemoveAllTtbLL();
    }

    // TTC
    if (ui->ttcCheck->isChecked())
    {
        if (ui->ttcModeComboBox->currentIndex() == 0)
        {
            x2m->RemoveAllTtcLL();
            // No need to do further processing, Validate already pushed the current entry in the x2m
        }
        else
        {
            x2m->RemoveAllTtcHL();
            // No need to do further processing, Validate already pushed the current entry in the x2m
        }
    }
    else
    {
        x2m->RemoveAllTtcHL();
        x2m->RemoveAllTtcLL();
    }

    // CNC
    if (ui->cncCheck->isChecked())
    {
        // Nothing, current entry was pushed from gui by Verify
    }
    else
    {
        while (x2m->GetNumCncEntries() != 0)
            x2m->RemoveCncEntry(0);
    }

    // VFX
    x2m->SetCharEepk(Utils::QStringToStdString(ui->vfxEdit->text().trimmed(), false));

    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open file", config.lf_editor_open, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_editor_open = file;

    X2mFile *new_x2m = new X2mFile();

    if (!new_x2m->LoadFromFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Load file failed.\n");
        delete new_x2m;
        return;
    }

    config.lf_editor_save = file;

    if (new_x2m->GetType() != X2mType::NEW_CHARACTER)
    {
        DPRINTF("That x2m is not of new character type!\n");
        delete new_x2m;
        return;
    }

    delete x2m;
    x2m = new_x2m;
    ProcessX2m();
}

void MainWindow::on_actionSave_triggered()
{
    if (!Validate())
        return;

    QString file = QFileDialog::getSaveFileName(this, "Save file", config.lf_editor_save, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_editor_save = file;    
    WaitDialog wait;

    if (show_wait_onsave)
    {
        wait.show();
        qApp->processEvents();
    }

    if (!Build())
    {
        if (show_wait_onsave)
            wait.close();
            //show_wait_onsave = false;

        DPRINTF("Build failed.\n");
        return;
    }

    bool ret = x2m->SaveToFile(Utils::QStringToStdString(file));

    if (show_wait_onsave)
        wait.close();

    if (!ret)
    {
        DPRINTF("x2m save failed.\n");        
    }
    else
    {
        UPRINTF("File has been succesfully written.\n");
        show_wait_onsave = false;
    }
}

void MainWindow::on_actionExit_triggered()
{
    if (ProcessShutdown())
        qApp->exit();
}

void MainWindow::on_actionAbout_triggered()
{
    //CasiopeaTest(x2m);

    QMessageBox box;

    box.setWindowTitle(PROGRAM_NAME_CHARACREAT);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Icon::Information);
    box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4<br><br>If you liked the program, you can support its development at<br><a href='https://www.patreon.com/eternity_tools'>https://www.patreon.com/eternity_tools</a>").arg(PROGRAM_NAME_CHARACREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));

    box.exec();
}

void MainWindow::on_guidButton_clicked()
{
    uint8_t guid[16];
    std::string guid_str;

    Utils::GetRandomData(guid, sizeof(guid));
    guid_str = Utils::GUID2String(guid);

    x2m->SetModGuid(guid_str);
    ui->modGuidEdit->setText(Utils::StdStringToQString(guid_str));
}

void MainWindow::on_charaNameLangComboBox_currentIndexChanged(int index)
{
    ui->charaNameEdit->setText(Utils::StdStringToQString(x2m->GetCharaName(index), false));
}

void MainWindow::on_charaNameEdit_textEdited(const QString &arg1)
{
    x2m->SetCharaName(Utils::QStringToStdString(arg1, false), ui->charaNameLangComboBox->currentIndex());
}

void MainWindow::on_charNameCopyButton_clicked()
{
    ListDialog dialog(ListMode::CHARASEL_FULL, this);

    if (dialog.exec())
    {
        int index = dialog.GetResultInteger();

        CharaListSlotEntry *entry = chara_list->FindFromAbsolutePos(index);
        if (entry)
        {
            for (int i = 0; i < XV2_LANG_NUM; i++)
            {
                std::string name;

                if (entry->code.length() == 5 && entry->code[0] == '"' && entry->code[4] == '"')
                {
                    const std::string code = entry->code.substr(1, 3);

                    if (!Xenoverse2::GetCharaName(code, name, i))
                        name.clear();
                }

                x2m->SetCharaName(name, i);
            }

            int lang = ui->charaNameLangComboBox->currentIndex();

            if (lang >= 0 && lang < XV2_LANG_NUM)
            {
                ui->charaNameEdit->setText(Utils::StdStringToQString(x2m->GetCharaName(lang), false));
            }
        }
    }
}

void MainWindow::NameEntryToGui(const X2mCharaName &entry)
{
    ui->multNamesCostEdit->setText(QString("%1").arg(entry.costume_index));

    int lang = ui->multNamesLangComboBox->currentIndex();
    if (lang >= 0 && lang < XV2_LANG_NUM)
    {
        ui->multNamesEdit->setText(Utils::StdStringToQString(entry.chara_name[lang], false));
    }
}

void MainWindow::GuiToNameEntry(X2mCharaName &entry)
{
    // Only the costume, the other is processed by the edited event
    entry.costume_index = ui->multNamesCostEdit->text().toInt();
}

void MainWindow::on_multNamesLangComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= XV2_LANG_NUM)
        return;

    int entry_index = ui->multNamesComboBox->currentIndex();
    if (entry_index < 0 || entry_index >= x2m->GetNumMultNames())
        return;

    const X2mCharaName &name = x2m->GetMultName(entry_index);
    ui->multNamesEdit->setText(Utils::StdStringToQString(name.chara_name[index], false));
}

void MainWindow::on_multNamesEdit_textEdited(const QString &arg1)
{
    int entry_index = ui->multNamesComboBox->currentIndex();
    if (entry_index < 0 || entry_index >= x2m->GetNumMultNames())
        return;

    int lang = ui->multNamesLangComboBox->currentIndex();
    if (lang < 0 || lang >= XV2_LANG_NUM)
        return;

    X2mCharaName &name = x2m->GetMultName(entry_index);
    name.chara_name[lang] = Utils::QStringToStdString(arg1, false);
}

void MainWindow::on_multNamesCheck_clicked()
{
    bool checked = ui->multNamesCheck->isChecked();

    ui->multNamesAddButton->setEnabled(checked);
    ui->multNamesComboBox->setEnabled(checked);
    ui->multNamesCopyButton->setEnabled(checked);
    ui->multNamesCostEdit->setEnabled(checked);
    ui->multNamesEdit->setEnabled(checked);
    ui->multNamesLangComboBox->setEnabled(checked);
    ui->multNamesRemoveButton->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_mult_names_index = -1;
        ui->multNamesComboBox->clear();

        size_t num_name_entries = x2m->GetNumMultNames();

        if (num_name_entries == 0)
        {
            X2mCharaName entry;

            GuiToNameEntry(entry);
            x2m->AddMultName(entry);
            num_name_entries++;
        }

        for (size_t i = 0; i < num_name_entries; i++)
        {
            ui->multNamesComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_name_entries == 1)
        {
            ui->multNamesRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumMultNames() != 0)
            x2m->RemoveMultName(0);
    }
}

void MainWindow::on_multNamesComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumMultNames())
        return;

    if (prev_mult_names_index >= 0 && prev_mult_names_index < x2m->GetNumMultNames())
    {
        X2mCharaName &entry = x2m->GetMultName(prev_mult_names_index);
        GuiToNameEntry(entry);
    }

    const X2mCharaName &entry = x2m->GetMultName(index);
    NameEntryToGui(entry);

    prev_mult_names_index = index;
}

void MainWindow::on_multNamesAddButton_clicked()
{
    X2mCharaName entry;

    int idx = ui->multNamesComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumMultNames())
    {
        entry = x2m->GetMultName(idx); // copy
        GuiToNameEntry(entry);
    }

    size_t pos = x2m->AddMultName(entry);

    ui->multNamesComboBox->addItem(QString("Entry %1").arg(pos));

    if (x2m->GetNumMultNames() == XV2_MAX_SUBSLOTS)
        ui->multNamesAddButton->setDisabled(true);

    ui->multNamesComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumMultNames() > 1)
        ui->multNamesRemoveButton->setEnabled(true);
}

void MainWindow::on_multNamesRemoveButton_clicked()
{
    int index = ui->multNamesComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumMultNames())
        return;

    x2m->RemoveMultName(index);

    if (prev_mult_names_index > index)
        prev_mult_names_index--;
    else
        prev_mult_names_index = -1;

    ui->multNamesComboBox->removeItem(index);

    for (int i = 0; i < ui->multNamesComboBox->count(); i++)
    {
        ui->multNamesComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumMultNames() == 1)
        ui->multNamesRemoveButton->setDisabled(true);

    else if (x2m->GetNumMultNames() < XV2_MAX_SUBSLOTS)
        ui->multNamesAddButton->setEnabled(true);
}

void MainWindow::on_multNamesCopyButton_clicked()
{
    int entry_index = ui->multNamesComboBox->currentIndex();
    if (entry_index < 0 || entry_index >= x2m->GetNumMultNames())
        return;

    X2mCharaName &name_entry = x2m->GetMultName(entry_index);

    ListDialog dialog(ListMode::CHARASEL_FULL, this);

    if (dialog.exec())
    {
        int index = dialog.GetResultInteger();

        CharaListSlotEntry *entry = chara_list->FindFromAbsolutePos(index);
        if (entry)
        {
            for (int i = 0; i < XV2_LANG_NUM; i++)
            {
                std::string name;

                if (entry->code.length() == 5 && entry->code[0] == '"' && entry->code[4] == '"')
                {
                    const std::string code = entry->code.substr(1, 3);

                    if (!Xenoverse2::GetCharaName(code, name, i))
                        name.clear();
                }

                name_entry.chara_name[i] = name;
            }

            int lang = ui->multNamesLangComboBox->currentIndex();

            if (lang >= 0 && lang < XV2_LANG_NUM)
            {
                ui->multNamesEdit->setText(Utils::StdStringToQString(name_entry.chara_name[lang], false));
            }
        }
    }
}

void MainWindow::on_bodyShapeCheck_clicked()
{
    ui->bodyShapeEdit->setEnabled(ui->bodyShapeCheck->isChecked());
}

void MainWindow::on_charNameClearButon_triggered(QAction *arg1)
{
    int idx = ui->charaNameLangComboBox->currentIndex();
    if (idx < 0 || idx >= XV2_LANG_NUM)
        return;

    if (arg1 == ui->actionCharNameClearAll)
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            x2m->SetCharaName(std::string(), i);
        }

        ui->charaNameEdit->setText(QString());
    }
    else if (arg1 == ui->actionCharNameClearThis)
    {
        x2m->SetCharaName(std::string(), idx);
        ui->charaNameEdit->setText(QString());
    }
    else if (arg1 == ui->actionCharNameClearAllButThis)
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            if (i == idx)
                continue;

            x2m->SetCharaName(std::string(), i);
        }
    }
}


void MainWindow::on_charaFilesButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choose three-letter char directory", config.ld_chara_dir);

    if (dir.isNull())
        return;

    std::string dir_std = Utils::NormalizePath(Utils::QStringToStdString(dir));
    size_t pos = dir_std.rfind('/');

    if (pos == std::string::npos)
        return;

    std::string dir_name = dir_std.substr(pos+1);
    if (dir_name.length() != 3)
    {
        DPRINTF("The directory name must be a three-letters character code.");
        return;
    }

    if (!Utils::IsAlphaNumeric(dir_name))
    {
        DPRINTF("The directory name must be alpha-numeric.\n");
        return;
    }

    if (Xenoverse2::IsOriginalChara(dir_name))
    {
        DPRINTF("The directory name cannot be that of an original character.");
        return;
    }

    config.ld_chara_dir = dir;
    x2m->DeleteCharaDirectory();

    entry_name = Utils::ToUpperCase(dir_name);
    ui->charaFilesEdit->setText(dir);
    ui->charaFilesEdit->setEnabled(true);
    show_wait_onsave = true;
}

void MainWindow::on_selPortButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameSel)
    {
        /*ListDialog dialog(ListMode::SELECT_PORTRAIT, this);
        if (dialog.exec())
        {
            ui->selPortEdit->setText(GAME_PREFIX + dialog.GetResult());
            ui->selPortEdit->setEnabled(false);
        }*/
        EmbDialog dialog(game_sel_port, this);

        if (dialog.exec())
        {
            ui->selPortEdit->setText(GAME_PREFIX + dialog.GetResult());
            ui->selPortEdit->setEnabled(false);
        }
    }
    else if (arg1 == ui->actionSelectFileSel)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select dds file", config.lf_sel_port, "DDS Files (*.dds)");

        if (file.isNull())
            return;

        config.lf_sel_port = file;

        ui->selPortEdit->setText(file);
        ui->selPortEdit->setEnabled(true);
    }
    else if (arg1 == ui->actionFromGameBtlSel)
    {
        ListDialog dialog(ListMode::BATTLE_PORTRAIT, this);

        if (dialog.exec())
        {
            EmbFile emb;

            if (!xv2fs->LoadFile(&emb, "data/ui/texture/" + Utils::QStringToStdString(dialog.GetResult(), false)))
            {
                DPRINTF("Error loading emb.\n");
                return;
            }

            EmbDialog emb_dialog(&emb, this);

            if (emb_dialog.exec())
            {
                int idx = emb_dialog.GetResultInteger();

                if (idx < 0 || idx >= emb.GetNumFiles())
                    return;

                const EmbContainedFile &file = emb[idx];

                if (!x2m->WriteFile(X2M_SEL_PORTRAIT, file.GetData(), file.GetSize()))
                {
                    DPRINTF("Error writing data.\n");
                    return;
                }

                ui->selPortEdit->setEnabled(false);
                ui->selPortEdit->setText(INTERNAL_DATA);
            }
        }
    }
    else if (arg1 == ui->actionFromEmbSel)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select emb file", config.lf_btl_port, "EMB Files (*.emb *.emb.xml)");

        if (file.isNull())
            return;

        config.lf_btl_port = file;

        EmbFile emb;

        if (!emb.LoadFromFile(Utils::QStringToStdString(file, false)))
        {
            DPRINTF("Error loading emb.\n");
            return;
        }

        EmbDialog emb_dialog(&emb, this);

        if (emb_dialog.exec())
        {
            int idx = emb_dialog.GetResultInteger();

            if (idx < 0 || idx >= emb.GetNumFiles())
                return;

            const EmbContainedFile &file = emb[idx];

            if (!x2m->WriteFile(X2M_SEL_PORTRAIT, file.GetData(), file.GetSize()))
            {
                DPRINTF("Error writing data.\n");
                return;
            }

            ui->selPortEdit->setEnabled(false);
            ui->selPortEdit->setText(INTERNAL_DATA);
        }
    }
}

void MainWindow::on_btlPortButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameBtl)
    {
        ListDialog dialog(ListMode::BATTLE_PORTRAIT, this);

        if (dialog.exec())
        {
            ui->btlPortEdit->setText(GAME_PREFIX + dialog.GetResult());
            ui->btlPortEdit->setEnabled(false);
        }
    }
    else if (arg1 == ui->actionSelectFileBtl)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select emb file", config.lf_btl_port, "EMB Files (*.emb *.emb.xml)");

        if (file.isNull())
            return;

        config.lf_btl_port = file;

        ui->btlPortEdit->setText(file);
        ui->btlPortEdit->setEnabled(true);
    }
    else if (arg1 == ui->actionUseAutoBtlPort)
    {
        ui->btlPortEdit->setText(AUTO_BTL_PORTRAIT);
        ui->btlPortEdit->setEnabled(false);
    }
    else if (arg1 == ui->actionCreateFromSingleDdsBtl)
    {
        QString file = QFileDialog::getOpenFileName(this, "Select dds file", config.lf_sel_port, "DDS Files (*.dds)");

        if (file.isNull())
            return;

        config.lf_sel_port = file;

        ui->btlPortEdit->setText(DDS_PREFIX + file);
        ui->btlPortEdit->setEnabled(false);
    }
}

void MainWindow::on_addEditButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choose directory that conteins data directory", config.ld_add_data);

    if (dir.isNull())
        return;

    std::string dir_std = Utils::NormalizePath(Utils::QStringToStdString(dir));

    if (dir_std.length() != 0 && !Utils::EndsWith(dir_std, "/"))
        dir_std += '/';

    if (!Utils::DirExists(dir_std + "data"))
    {
        if (QMessageBox::question(this, "Use that directory?",
                                  "That directory doesn't contain a directory called \"data\" inside. "
                                  "Are you sure that it is the correct directory?",
                                  QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                  QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
    }
    else if (Utils::DirExists(dir_std + "data/system"))
    {
        if (QMessageBox::question(this, "Use that directory?",
                                  "That directory contains a system directory that will affect the whole game, "
                                  "what defeats the philosophy of a new character mod.\n\n"
                                  "Are you sure you want to use that directory?",
                                  QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),
                                  QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
    }

    config.ld_add_data = dir;
    x2m->DeleteJungle();

    ui->addDataEdit->setText(dir);
    ui->addDataEdit->setEnabled(true);
}

void MainWindow::SetCssAudio(const QString &dir)
{
    // Note: this function now applies to sev audio and not just css, but we kept the old name

    if (dir != INTERNAL_DATA)
    {
        x2m->DeleteAudio();

        /*for (size_t i = 0; i < x2m->GetNumSlotEntries(); i++)
        {
            X2mSlotEntry &slot = x2m->GetSlotEntry(i);

            slot.audio_files[0].clear(); slot.audio_files[1].clear();

            if (slot.voices_id_list[0] == X2M_DUMMY_ID)
                slot.voices_id_list[0] = -1;

            if (slot.voices_id_list[1] == X2M_DUMMY_ID)
                slot.voices_id_list[1] = -1;
        }

        int slot_idx = ui->slotsComboBox->currentIndex();
        if (slot_idx >= 0 && slot_idx < x2m->GetNumSlotEntries())
        {
            SlotEntryToGui(x2m->GetSlotEntry(slot_idx));
        }*/

        size_t num = x2m->SetAudioDir(Utils::QStringToStdString(dir));
        if (num == 0)
        {
            DPRINTF("No audio files were found in that directory!\n"
                    "Remember that the audio files must end with \"_jp.hca\" or \"_en.hca\".");
        }
        else
        {
            UPRINTF("Audio files succesfully added to the package.\n"
                    "If you do any change on them, select the directory again with the change button.\n");
        }
    }

    std::vector<std::string> voices;
    x2m->GetAudioFiles(voices);

    ui->voice1Check->setEnabled(voices.size() != 0);
    ui->voice2Check->setEnabled(voices.size() != 0);

    ui->voice1ComboBox->clear();
    ui->voice2ComboBox->clear();

    for (const std::string &voice : voices)
    {
        QString qvoice = Utils::StdStringToQString(voice);
        ui->voice1ComboBox->addItem(qvoice);
        ui->voice2ComboBox->addItem(qvoice);
    }

    on_voice1Check_clicked();
    on_voice2Check_clicked();

    if (voices.size() == 0)
        ui->cssSoundEdit->setText("");
    else
        ui->cssSoundEdit->setText(dir);    

    // Sev
    bool enable_ll = (ui->sevCheck->isChecked() && ui->sevModeComboBox->currentIndex() == 1);

    if (enable_ll)
    {
        // Check for files that don't longer exist, and remove them from sev, from custom audio and combobox
        std::vector<X2mCustomAudio> audios;
        x2m->GetCustomSevAudio(audios);

        for (const X2mCustomAudio &audio : audios)
        {
            if (std::find(voices.begin(), voices.end(), audio.name) == voices.end())
            {
                x2m->RemoveCustomSevAudio(audio.id);

                int idx = ui->sevLLAudioComboBox->findText(Utils::StdStringToQString(audio.name));
                if (idx != -1)
                    ui->sevLLAudioComboBox->removeItem(idx);

                idx = ui->sevLLAudioRespComboBox->findText(Utils::StdStringToQString(audio.name));
                if (idx != -1)
                    ui->sevLLAudioRespComboBox->removeItem(idx);
            }
        }

        // Check for files that were added new and add them to custom audio and combobox
        for (const std::string &voice : voices)
        {
            if (!x2m->FindCustomAudio(voice))
            {
                x2m->AddCustomAudio(voice);

                ui->sevLLAudioComboBox->addItem(Utils::StdStringToQString(voice));
                ui->sevLLAudioRespComboBox->addItem(Utils::StdStringToQString(voice));
            }
        }
    }

    // Ttb
    enable_ll = (ui->ttbCheck->isChecked() && ui->ttbModeComboBox->currentIndex() == 1);

    if (enable_ll)
    {
        // Check for files that don't longer exist, and remove them from ttb, from custom audio and combobox
        std::vector<X2mCustomAudio> audios;
        x2m->GetCustomTtbAudio(audios);

        TtbEntryLL entry_ll;        
        int idx = ui->ttbLLCostComboBox->currentIndex();

        if (idx >= 0 && idx < x2m->GetNumTtbLLEntries())
        {
            entry_ll = x2m->GetTtbLLEntry(idx);
            GuiToTtbEntryLL(entry_ll);
            x2m->GetTtbLLEntry(idx) = entry_ll;
        }
        else
        {
            GuiToTtbEntryLL(entry_ll);
        }

        bool rebuild_current = false;

        for (const X2mCustomAudio &audio : audios)
        {
            std::string ttb_name = "X2T_" + audio.name;

            if (std::find(voices.begin(), voices.end(), audio.name) == voices.end())
            {
                x2m->RemoveCustomTtbAudio(audio.name);

                if (entry_ll.event_res1.name == ttb_name)
                    rebuild_current = true;

                else if (entry_ll.event_res2.name == ttb_name)
                    rebuild_current = true;

                else
                {
                    if (entry_ll.num_actors >= 3)
                    {
                        if (entry_ll.event_res3.name == ttb_name)
                            rebuild_current = true;

                        if (entry_ll.num_actors >= 4)
                        {
                            if (entry_ll.event_res4.name == ttb_name)
                                rebuild_current = true;

                            if (entry_ll.num_actors >= 5)
                            {
                                if (entry_ll.event_res5.name == ttb_name)
                                    rebuild_current = true;
                            }
                        }
                    }
                }
            }
        }

        // Check for files that were added new and add them to custom audio and combobox
        for (const std::string &voice : voices)
        {
            if (!x2m->FindCustomAudio(voice))
            {
                x2m->AddCustomAudio(voice);
            }
        }

        if (rebuild_current)
        {
            if (idx >= 0 && idx < x2m->GetNumTtbLLEntries())
            {
                TtbEntryLLToGui(x2m->GetTtbLLEntry(idx), false);
            }
        }
    }

    // General cleaning
    std::vector<X2mCustomAudio> &custom_audios = x2m->GetCustomAudios();

    for (size_t i = 0; i < custom_audios.size(); i++)
    {
        if (std::find(voices.begin(), voices.end(), custom_audios[i].name) == voices.end())
        {
            custom_audios.erase(custom_audios.begin()+i);
            i--;
        }
    }
}

void MainWindow::on_cssSoundButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select directory", config.ld_css_audio);

    if (dir.isNull())
        return;

    config.ld_css_audio = dir;
    SetCssAudio(dir);
}

void MainWindow::VoiceToComboBox(QComboBox *comboBox, QCheckBox *check, QLineEdit *edit, const std::string &voice_name)
{
    bool found = false;

    if (voice_name.size() > 0)
    {
        QString qvoice = Utils::StdStringToQString(voice_name);

        for (int i = 0; i < comboBox->count(); i++)
        {
            if (qvoice == comboBox->itemText(i))
            {
                comboBox->setEnabled(true);
                comboBox->setCurrentIndex(i);
                check->setChecked(true);
                edit->setEnabled(false);
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        check->setChecked(false);
        comboBox->setEnabled(false);
        edit->setEnabled(true);

        if (comboBox->count() >= 1)
            comboBox->setCurrentIndex(0);
    }
}

void MainWindow::ComboBoxToVoice(QComboBox *comboBox, QCheckBox *check, std::string &voice_name)
{
    if (!check->isChecked() || comboBox->count() <= 0)
    {
        voice_name.clear();
        return;
    }

    voice_name = Utils::QStringToStdString(comboBox->itemText(comboBox->currentIndex()));
}

void MainWindow::SlotEntryToGui(const X2mSlotEntry &entry)
{
    ui->costumeIndexEdit->setText(QString("%1").arg(entry.costume_index));
    ui->modelPresetEdit->setText(QString("%1").arg(entry.model_preset));
    ui->flagGK2ComboBox->setCurrentIndex(entry.flag_gk2 ? 1 : 0);

    if (entry.voices_id_list[0] == X2M_DUMMY_ID)
        ui->voice1Edit->setText("-1");
    else
        ui->voice1Edit->setText(QString("%1").arg(entry.voices_id_list[0]));

    if (entry.voices_id_list[1] == X2M_DUMMY_ID)
        ui->voice2Edit->setText("-1");
    else
        ui->voice2Edit->setText(QString("%1").arg(entry.voices_id_list[1]));

    VoiceToComboBox(ui->voice1ComboBox, ui->voice1Check, ui->voice1Edit, entry.audio_files[0]);
    VoiceToComboBox(ui->voice2ComboBox, ui->voice2Check, ui->voice2Edit, entry.audio_files[1]);

    ui->costumeNameEdit->setText(Utils::StdStringToQString(entry.costume_name[ui->costumeNameLangComboBox->currentIndex()], false));

    //on_voice1Check_clicked();
    //on_voice2Check_clicked();
}

void MainWindow::GuiToSlotEntry(X2mSlotEntry &entry)
{
    entry.costume_index = ui->costumeIndexEdit->text().toInt();
    entry.model_preset = ui->modelPresetEdit->text().toInt();
    entry.flag_gk2 = (ui->flagGK2ComboBox->currentIndex() == 1);
    entry.voices_id_list[0] = ui->voice1Edit->text().toInt();
    entry.voices_id_list[1] = ui->voice2Edit->text().toInt();
    ComboBoxToVoice(ui->voice1ComboBox, ui->voice1Check, entry.audio_files[0]);
    ComboBoxToVoice(ui->voice2ComboBox, ui->voice2Check, entry.audio_files[1]);
}

void MainWindow::on_slotsComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= (int)x2m->GetNumSlotEntries())
        return;

    if (prev_slots_index >= 0 && prev_slots_index < (int)x2m->GetNumSlotEntries())
    {
        X2mSlotEntry &entry = x2m->GetSlotEntry((size_t)prev_slots_index);
        GuiToSlotEntry(entry);
    }

    const X2mSlotEntry &entry = x2m->GetSlotEntry((size_t)index);
    SlotEntryToGui(entry);

    prev_slots_index = index;
}

void MainWindow::on_slotsAddButton_clicked()
{
    X2mSlotEntry entry;
    CusSkillSet set;

    int idx = ui->slotsComboBox->currentIndex();
    if (idx >= 0 && idx < (int)x2m->GetNumSlotEntries())
    {
        GuiToSlotEntry(entry);
        entry.costume_name = x2m->GetSlotEntry((size_t)idx).costume_name;
    }

    idx = ui->cusSlotsComboBox->currentIndex();
    if (idx >= 0 && idx < (int)x2m->GetNumSkillSets())
    {
        GuiToSkillSet(set);
    }

    size_t pos = x2m->AddSlotEntry(entry);
    size_t pos2 = x2m->AddSkillSet(set);

    if (pos != pos2)
    {
        DPRINTF("Internal error, syncronization between slot and cus was lost somewhere.\n");
        exit(-1);
    }

    ui->slotsComboBox->addItem(QString("Slot %1").arg(pos));
    ui->cusSlotsComboBox->addItem(QString("Slot %1").arg(pos));

    if (x2m->GetNumSlotEntries() == XV2_MAX_SUBSLOTS)
        ui->slotsAddButton->setDisabled(true);

    ui->slotsComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumSlotEntries() > 1)
        ui->slotsRemoveButton->setEnabled(true);
}

void MainWindow::on_slotsRemoveButton_clicked()
{
    int index = ui->slotsComboBox->currentIndex();

    if (index < 0 || index >= (int)x2m->GetNumSlotEntries())
        return;

    x2m->RemoveSlotEntry((size_t)index);
    x2m->RemoveSkillSet((size_t)index);

    if (prev_slots_index > index)
        prev_slots_index--;
    else
        prev_slots_index = -1;

    if (prev_cus_slots_index > index)
        prev_cus_slots_index--;
    else
        prev_cus_slots_index = -1;

    ui->slotsComboBox->removeItem(index);
    ui->cusSlotsComboBox->removeItem(index);

    for (int i = 0; i < ui->slotsComboBox->count(); i++)
    {
        ui->slotsComboBox->setItemText(i, QString("Slot %1").arg(i));
    }

    for (int i = 0; i < ui->cusSlotsComboBox->count(); i++)
    {
        ui->cusSlotsComboBox->setItemText(i, QString("Slot %1").arg(i));
    }

    if (x2m->GetNumSlotEntries() == 1)
        ui->slotsRemoveButton->setDisabled(true);

    else if (x2m->GetNumSlotEntries() < XV2_MAX_SUBSLOTS)
        ui->slotsAddButton->setEnabled(true);
}

void MainWindow::on_costumeNameLangComboBox_currentIndexChanged(int index)
{
    int slot_index = ui->slotsComboBox->currentIndex();
    if (slot_index < 0 || slot_index >= x2m->GetNumSlotEntries())
        return;

    const X2mSlotEntry &entry = x2m->GetSlotEntry(slot_index);
    ui->costumeNameEdit->setText(Utils::StdStringToQString(entry.costume_name[index], false));
}

void MainWindow::on_costumeNameEdit_textEdited(const QString &arg1)
{
    int slot_index = ui->slotsComboBox->currentIndex();
    if (slot_index < 0 || slot_index >= x2m->GetNumSlotEntries())
        return;

    X2mSlotEntry &entry = x2m->GetSlotEntry(slot_index);
    entry.costume_name[ui->costumeNameLangComboBox->currentIndex()] = Utils::QStringToStdString(arg1, false);
}

void MainWindow::on_slotsCopyButton_clicked()
{
    ListDialog dialog(ListMode::CHARASEL_FULL, this);

    if (dialog.exec())
    {
        int ret = dialog.GetResultInteger();

        if (ret >= 0)
        {
            CharaListSlotEntry *ch_entry = chara_list->FindFromAbsolutePos((size_t)ret);

            if (ch_entry)
            {
                int slot_index = ui->slotsComboBox->currentIndex();
                if (slot_index < 0 || slot_index >= x2m->GetNumSlotEntries())
                    return;

                X2mSlotEntry &entry = x2m->GetSlotEntry(slot_index);
                entry.CopyFrom(*ch_entry, true);

                SlotEntryToGui(entry);
            }
        }
    }
}

void MainWindow::on_voice1Check_clicked()
{
    if (!ui->voice1Check->isEnabled())
        ui->voice1Check->setChecked(false);

    bool checked = ui->voice1Check->isChecked();

    ui->voice1ComboBox->setEnabled(checked);
    ui->voice1Edit->setEnabled(!checked);    

    int idx = ui->slotsComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSlotEntries())
    {
        GuiToSlotEntry(x2m->GetSlotEntry(idx));
    }
}

void MainWindow::on_voice2Check_clicked()
{
    if (!ui->voice2Check->isEnabled())
        ui->voice2Check->setChecked(false);

    bool checked = ui->voice2Check->isChecked();

    ui->voice2ComboBox->setEnabled(checked);
    ui->voice2Edit->setEnabled(!checked);    

    int idx = ui->slotsComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSlotEntries())
    {
        GuiToSlotEntry(x2m->GetSlotEntry(idx));
    }
}

void MainWindow::on_costumeNameClearButton_triggered(QAction *arg1)
{
    int idx = ui->slotsComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumSlotEntries())
        return;

    X2mSlotEntry &slot = x2m->GetSlotEntry(idx);

    int idx_lang = ui->costumeNameLangComboBox->currentIndex();
    if (idx_lang < 0 || idx_lang >= XV2_LANG_NUM)
        return;

    if (arg1 == ui->actionCostumeNameClearAll)
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            slot.costume_name[i].clear();
        }

        ui->costumeNameEdit->clear();
    }
    else if (arg1 == ui->actionCostumeNameClearThis)
    {
        slot.costume_name[idx_lang].clear();
        ui->costumeNameEdit->clear();
    }
    else if (arg1 == ui->actionCostumeNameClearAllButThis)
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            if (i == idx_lang)
                continue;

            slot.costume_name[i].clear();
        }
    }
}

void MainWindow::CmsEntryToGui(const CmsEntryXV2 &entry)
{
    ui->u10Edit->setText(QString("%1").arg((int32_t)entry.unk_10));
    ui->camDistEdit->setText(QString("%1").arg((int16_t)entry.load_cam_dist));
    ui->u16Edit->setText(QString("%1").arg((int16_t)entry.unk_16));
    ui->u18Edit->setText(QString("%1").arg((int16_t)entry.unk_18));
    ui->u1AEdit->setText(QString("%1").arg((int16_t)entry.unk_1A));

    ui->characterEdit->setText(Utils::StdStringToQString(entry.character));
    ui->eanEdit->setText(Utils::StdStringToQString(entry.ean));
    ui->fceEanEdit->setText(Utils::StdStringToQString(entry.fce_ean));
    ui->fceEdit->setText(Utils::StdStringToQString(entry.fce));
    ui->camEanEdit->setText(Utils::StdStringToQString(entry.cam_ean));
    ui->bacEdit->setText(Utils::StdStringToQString(entry.bac));
    ui->bcmEdit->setText(Utils::StdStringToQString(entry.bcm));
    ui->aiEdit->setText(Utils::StdStringToQString(entry.ai));
    ui->bdmEdit->setText(Utils::StdStringToQString(entry.bdm));
}

void MainWindow::GuiToCmsEntry(CmsEntryXV2 &entry)
{
    entry.name = "123";
    entry.id = X2M_DUMMY_ID;

    entry.unk_10 = (uint32_t)ui->u10Edit->text().toInt();
    entry.load_cam_dist = (uint16_t)(uint32_t)ui->camDistEdit->text().toInt();
    entry.unk_16 = (uint16_t)(uint32_t)ui->u16Edit->text().toInt();
    entry.unk_18 = (uint16_t)(uint32_t)ui->u18Edit->text().toInt();
    entry.unk_1A = (uint16_t)(uint32_t)ui->u1AEdit->text().toInt();

    entry.character = Utils::QStringToStdString(ui->characterEdit->text());
    entry.ean = Utils::QStringToStdString(ui->eanEdit->text());
    entry.fce_ean = Utils::QStringToStdString(ui->fceEanEdit->text());
    entry.fce = Utils::QStringToStdString(ui->fceEdit->text());
    entry.cam_ean = Utils::QStringToStdString(ui->camEanEdit->text());
    entry.bac = Utils::QStringToStdString(ui->bacEdit->text());
    entry.bcm = Utils::QStringToStdString(ui->bcmEdit->text());
    entry.ai = Utils::QStringToStdString(ui->aiEdit->text());
    entry.bdm = Utils::QStringToStdString(ui->bdmEdit->text());
}

void MainWindow::on_cmsCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    CmsFile *cms = nullptr;

    if (arg1 == ui->actionFromGameCms)
    {
        dialog = new ListDialog(ListMode::CMS, this);
        cms = game_cms;
    }
    else if (arg1 == ui->actionFromExternalCms)
    {
        QString file = QFileDialog::getOpenFileName(this, "External CMS", config.lf_external_cms, "CMS Files (*.cms *.cms.xml)");

        if (file.isNull())
            return;

        config.lf_external_cms = file;

        cms = new CmsFile();
        if (!cms->SmartLoad(Utils::QStringToStdString(file)) || !cms->IsXV2())
        {
            DPRINTF("SmartLoad failed or this is not a XV2 cms.\n\nIf this is a XML file, you may be using an old version of genser.");
            delete cms;
            return;
        }

        dialog = new ListDialog(ListMode::CMS, this, cms);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            CmsEntryXV2 *entry = dynamic_cast<CmsEntryXV2 *>(cms->FindEntryByName(Utils::QStringToStdString(dialog->GetResult())));

            if (entry)
            {
                CmsEntryToGui(*entry);
            }
            else
            {
                DPRINTF("Internal error when importing entry.\n");
            }
        }

        delete dialog;

        if (cms != game_cms)
            delete cms;
    }
}

void MainWindow::FillSuperComboBox(QComboBox *comboBox)
{
    const std::vector<CusSkill> &skills = game_cus->GetSuperSkills();

    comboBox->addItem("[NONE]", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetSuperSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)skill.id));
        }
    }
}

void MainWindow::FillUltimateComboBox(QComboBox *comboBox)
{
    const std::vector<CusSkill> &skills = game_cus->GetUltimateSkills();

    comboBox->addItem("[NONE]", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetUltimateSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)skill.id));
        }
    }
}

void MainWindow::FillEvasiveComboBox(QComboBox *comboBox)
{
    const std::vector<CusSkill> &skills = game_cus->GetEvasiveSkills();

    comboBox->addItem("[NONE]", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetEvasiveSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false), QVariant((int16_t)skill.id));
        }
    }
}

void MainWindow::FillBlastComboBox(QComboBox *comboBox)
{
    const std::vector<CusSkill> &skills = game_cus->GetBlastSkills();

    comboBox->addItem("[NONE]", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id));
    }
}

void MainWindow::FillAwakenComboBox(QComboBox *comboBox)
{
    const std::vector<CusSkill> &skills = game_cus->GetAwakenSkills();

    comboBox->addItem("[NONE]", QVariant(-1));

    for (const CusSkill &skill : skills)
    {
        std::string name;

        if (!Xenoverse2::GetAwakenSkillName(skill.id2, name))
        {
            comboBox->addItem(Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id));
        }
        else
        {
            comboBox->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(skill.name) + " - " + QString("%1").arg((int16_t)skill.id), QVariant((int16_t)skill.id));
        }
    }
}

void MainWindow::AddCustomSkill(QComboBox *comboBox, uint16_t id, const QString &name)
{
    int index = comboBox->findData(QVariant((int16_t)id));

    if (index == -1)
    {
        comboBox->addItem("[X2M] " + name, QVariant((int16_t)id));
    }
    else
    {
        comboBox->setItemText(index, "[X2M] " + name);
    }
}

void MainWindow::RemoveSkill(QComboBox *comboBox, uint16_t id)
{
    int index = comboBox->findData(QVariant((int16_t)id));

    if (index >= 0)
    {
        if (index == comboBox->currentIndex())
            comboBox->setCurrentIndex(0);

        comboBox->removeItem(index);
    }
}

bool MainWindow::FindCustomSkillType(uint16_t id, X2mSkillType *type)
{
    int index;

    index = ui->super1ComboBox->findData(QVariant((int16_t)id));
    if (index != -1)
    {
        *type = X2mSkillType::SUPER;
        return true;
    }

    index = ui->ultimate1ComboBox->findData(QVariant((int16_t)id));
    if (index != -1)
    {
        *type = X2mSkillType::ULTIMATE;
        return true;
    }

    index = ui->evasiveComboBox->findData(QVariant((int16_t)id));
    if (index != -1)
    {
        *type = X2mSkillType::EVASIVE;
        return true;
    }

    index = ui->blastComboBox->findData(QVariant((int16_t)id));
    if (index != -1)
    {
        *type = X2mSkillType::BLAST;
        return true;
    }

    index = ui->awakenComboBox->findData(QVariant((int16_t)id));
    if (index != -1)
    {
        *type = X2mSkillType::AWAKEN;
        return true;
    }

    return false;
}

void MainWindow::FillCustomSuper(QComboBox *comboBox)
{
    for (size_t i = 0; i < x2m->GetNumSkillSets(); i++)
    {
        const CusSkillSet &skill_set = x2m->GetSkillSet(i);

        for (int j = 0; j < 4; j++)
        {
            if (skill_set.char_skills[j] >= X2M_SKILL_DEPENDS_BEGIN && skill_set.char_skills[j] < X2M_SKILL_DEPENDS_END)
            {
                X2mDepends *dep = x2m->FindCharaSkillDepends(skill_set.char_skills[j]);

                if (dep)
                {
                    AddCustomSkill(comboBox, skill_set.char_skills[j], Utils::StdStringToQString(dep->name, false));
                }
            }
        }
    }
}

void MainWindow::FillCustomUltimate(QComboBox *comboBox)
{
    for (size_t i = 0; i < x2m->GetNumSkillSets(); i++)
    {
        const CusSkillSet &skill_set = x2m->GetSkillSet(i);

        for (int j = 4; j < 6; j++)
        {
            if (skill_set.char_skills[j] >= X2M_SKILL_DEPENDS_BEGIN && skill_set.char_skills[j] < X2M_SKILL_DEPENDS_END)
            {
                X2mDepends *dep = x2m->FindCharaSkillDepends(skill_set.char_skills[j]);

                if (dep)
                {
                    AddCustomSkill(comboBox, skill_set.char_skills[j], Utils::StdStringToQString(dep->name, false));
                }
            }
        }
    }
}

void MainWindow::FillCustomEvasive(QComboBox *comboBox)
{
    for (size_t i = 0; i < x2m->GetNumSkillSets(); i++)
    {
        const CusSkillSet &skill_set = x2m->GetSkillSet(i);

        if (skill_set.char_skills[6] >= X2M_SKILL_DEPENDS_BEGIN && skill_set.char_skills[6] < X2M_SKILL_DEPENDS_END)
        {
            X2mDepends *dep = x2m->FindCharaSkillDepends(skill_set.char_skills[6]);

            if (dep)
            {
                AddCustomSkill(comboBox, skill_set.char_skills[6], Utils::StdStringToQString(dep->name, false));
            }
        }
    }
}

void MainWindow::FillCustomBlast(QComboBox *comboBox)
{
    for (size_t i = 0; i < x2m->GetNumSkillSets(); i++)
    {
        const CusSkillSet &skill_set = x2m->GetSkillSet(i);

        if (skill_set.char_skills[7] >= X2M_SKILL_DEPENDS_BEGIN && skill_set.char_skills[7] < X2M_SKILL_DEPENDS_END)
        {
            X2mDepends *dep = x2m->FindCharaSkillDepends(skill_set.char_skills[7]);

            if (dep)
            {
                AddCustomSkill(comboBox, skill_set.char_skills[7], Utils::StdStringToQString(dep->name, false));
            }
        }
    }
}

void MainWindow::FillCustomAwaken(QComboBox *comboBox)
{
    for (size_t i = 0; i < x2m->GetNumSkillSets(); i++)
    {
        const CusSkillSet &skill_set = x2m->GetSkillSet(i);

        if (skill_set.char_skills[8] >= X2M_SKILL_DEPENDS_BEGIN && skill_set.char_skills[8] < X2M_SKILL_DEPENDS_END)
        {
            X2mDepends *dep = x2m->FindCharaSkillDepends(skill_set.char_skills[8]);

            if (dep)
            {
                AddCustomSkill(comboBox, skill_set.char_skills[8], Utils::StdStringToQString(dep->name, false));
            }
        }
    }
}

void MainWindow::EraseCustomSkills(QComboBox *comboBox)
{
    for (int i = 0; i < comboBox->count(); i++)
    {
        uint16_t id = (uint16_t) (int16_t) comboBox->itemData(i).toInt();

        if (id >= X2M_SKILL_DEPENDS_BEGIN && id < X2M_SKILL_DEPENDS_END)
        {
            comboBox->removeItem(i);;
            i--;
        }
    }
}

void MainWindow::SkillToComboBox(uint16_t id, QComboBox *comboBox, const QString &name)
{
    for (int i = 0; i < comboBox->count(); i++)
    {
        QVariant var = comboBox->itemData(i);

        if (var.toInt() == (int16_t)id)
        {
            comboBox->setCurrentIndex(i);
            return;
        }
    }

    DPRINTF("%s was not found. Automatically set to NONE.\n", Utils::QStringToStdString(name).c_str());
    comboBox->setCurrentIndex(0);
}

void MainWindow::SkillSetToGui(const CusSkillSet &entry)
{
    SkillToComboBox(entry.char_skills[0], ui->super1ComboBox, "Super 1");
    SkillToComboBox(entry.char_skills[1], ui->super2ComboBox, "Super 2");
    SkillToComboBox(entry.char_skills[2], ui->super3ComboBox, "Super 3");
    SkillToComboBox(entry.char_skills[3], ui->super4ComboBox, "Super 4");

    SkillToComboBox(entry.char_skills[4], ui->ultimate1ComboBox, "Ultimate 1");
    SkillToComboBox(entry.char_skills[5], ui->ultimate2ComboBox, "Ultimate 2");

    SkillToComboBox(entry.char_skills[6], ui->evasiveComboBox, "Evasive");
    SkillToComboBox(entry.char_skills[7], ui->blastComboBox, "Blast");
    SkillToComboBox(entry.char_skills[8], ui->awakenComboBox, "Awaken");
}

void MainWindow::ComboBoxToSkill(QComboBox *comboBox, uint16_t *skill)
{
    *skill = (uint16_t)(uint32_t) comboBox->currentData().toInt();
}

void MainWindow::GuiToSkillSet(CusSkillSet &entry)
{
    ComboBoxToSkill(ui->super1ComboBox, &entry.char_skills[0]);
    ComboBoxToSkill(ui->super2ComboBox, &entry.char_skills[1]);
    ComboBoxToSkill(ui->super3ComboBox, &entry.char_skills[2]);
    ComboBoxToSkill(ui->super4ComboBox, &entry.char_skills[3]);

    ComboBoxToSkill(ui->ultimate1ComboBox, &entry.char_skills[4]);
    ComboBoxToSkill(ui->ultimate2ComboBox, &entry.char_skills[5]);

    ComboBoxToSkill(ui->evasiveComboBox, &entry.char_skills[6]);
    ComboBoxToSkill(ui->blastComboBox, &entry.char_skills[7]);
    ComboBoxToSkill(ui->awakenComboBox, &entry.char_skills[8]);

    entry.model_preset = X2M_DUMMY_ID16;
}

void MainWindow::on_cusSlotsComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumSkillSets())
        return;

    if (prev_cus_slots_index >= 0 && prev_cus_slots_index < x2m->GetNumSlotEntries())
    {
        CusSkillSet &entry = x2m->GetSkillSet(prev_cus_slots_index);
        GuiToSkillSet(entry);
    }

    const CusSkillSet &entry = x2m->GetSkillSet(index);
    SkillSetToGui(entry);

    prev_cus_slots_index = index;
}

void MainWindow::on_cusCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    CusFile *cus = nullptr;

    if (arg1 == ui->actionFromGameCus)
    {
        dialog = new ListDialog(ListMode::CUS, this);
        cus = game_cus;
    }
    else if (arg1 == ui->actionFromExternalCus)
    {
        QString file = QFileDialog::getOpenFileName(this, "External CUS", config.lf_external_cus, "CUS Files (*.cus *.cus.xml)");

        if (file.isNull())
            return;

        config.lf_external_cus = file;

        cus = new CusFile();
        if (!cus->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("SmartLoad failed.\n\nIf this is a XML file, you may be using an old version of genser.");
            delete cus;
            return;
        }

        dialog = new ListDialog(ListMode::CUS, this, cus);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < cus->GetNumeSkillSets())
            {
                const CusSkillSet &entry = cus->GetSkillSet(idx);
                SkillSetToGui(entry);
            }
        }

        delete dialog;

        if (cus != game_cus)
            delete cus;
    }
}

void MainWindow::on_cusDependsComboBox_currentIndexChanged(int index)
{
    UNUSED(index);
    // We don't really need this
}

int MainWindow::LinkOrEmbed(X2mFile *skill_x2m)
{
    QMessageBox box(this);
    QPushButton *link_button;
    QPushButton *embed_button;

    QString text = "Do you want to link this skill mod, or embed it?";

    if (skill_x2m)
    {
        text += "\n\nMod name: " + Utils::StdStringToQString(skill_x2m->GetModName(), false);
        text += "\nType: ";

        if (skill_x2m->GetSkillType() == X2mSkillType::SUPER)
        {
            text += "SUPER";
        }
        else if (skill_x2m->GetSkillType() == X2mSkillType::ULTIMATE)
        {
            text += "ULTIMATE";
        }
        else if (skill_x2m->GetSkillType() == X2mSkillType::EVASIVE)
        {
            text += "EVASIVE";
        }
        else if (skill_x2m->GetSkillType() == X2mSkillType::BLAST)
        {
            text += "BLAST";
        }
        else if (skill_x2m->GetSkillType() == X2mSkillType::AWAKEN)
        {
            text += "AWAKEN";
        }
    }

    box.setWindowTitle("Link or embed?");
    box.setText(text);

    link_button = box.addButton("Link", QMessageBox::YesRole);
    embed_button = box.addButton("Embed", QMessageBox::YesRole);
    box.addButton(QMessageBox::Cancel);

    box.setDefaultButton(embed_button);

    box.exec();
    QAbstractButton *clicked = box.clickedButton();

    if (clicked == link_button)
    {
        return -1;
    }
    else if (clicked == embed_button)
    {
        return 1;
    }

    return 0;
}

void MainWindow::on_cusDependsAddButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select skill x2m", config.lf_depends_skill, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_depends_skill = file;

    X2mFile skill_x2m;

    if (!skill_x2m.LoadFromFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Failed to load x2m.\n");
        return;
    }

    if (skill_x2m.GetType() != X2mType::NEW_SKILL)
    {
        DPRINTF("That x2m is not of new skill type!\n");
        return;
    }

    if (x2m->CharaSkillDependsExist(skill_x2m.GetModGuid()))
    {
        DPRINTF("That skill is already linked or embedded. Use the \"update\" button if you want to update it.");
        return;
    }

    int ret = LinkOrEmbed(&skill_x2m);
    if (ret == 0)
        return;

    if (!x2m->AddCharaSkillDepends(&skill_x2m, false))
    {
        DPRINTF("Internal error when adding data.\n");
        return;
    }

    QString text;

    if (ret > 0)
    {
        if (!x2m->SetCharaSkillDependsAttachment(&skill_x2m))
        {
            DPRINTF("Failed to attach the skill.\n");
            x2m->RemoveCharaSkillDepends(skill_x2m.GetModGuid());
            return;
        }

        text += "[EMBEDDED] ";
    }
    else
    {
        text += "[LINKED] ";
    }

    QString mod_name = Utils::StdStringToQString(skill_x2m.GetModName(), false);

    text += mod_name;
    ui->cusDependsComboBox->addItem(text);
    ui->cusDependsComboBox->setCurrentIndex(ui->cusDependsComboBox->count()-1);

    ui->cusDependsRemoveButton->setEnabled(true);
    ui->cusDependsUpdateButton->setEnabled(true);

    const X2mDepends &dep = x2m->GetCharaSkillDepends(x2m->GetNumCharaSkillDepends()-1);

    if (skill_x2m.GetSkillType() == X2mSkillType::SUPER)
    {
        AddCustomSkill(ui->super1ComboBox, dep.id, mod_name);
        AddCustomSkill(ui->super2ComboBox, dep.id, mod_name);
        AddCustomSkill(ui->super3ComboBox, dep.id, mod_name);
        AddCustomSkill(ui->super4ComboBox, dep.id, mod_name);
    }
    else if (skill_x2m.GetSkillType() == X2mSkillType::ULTIMATE)
    {
        AddCustomSkill(ui->ultimate1ComboBox, dep.id, mod_name);
        AddCustomSkill(ui->ultimate2ComboBox, dep.id, mod_name);
    }
    else if (skill_x2m.GetSkillType() == X2mSkillType::EVASIVE)
    {
        AddCustomSkill(ui->evasiveComboBox, dep.id, mod_name);
    }
    else if (skill_x2m.GetSkillType() == X2mSkillType::BLAST)
    {
        AddCustomSkill(ui->blastComboBox, dep.id, mod_name);
    }
    else if (skill_x2m.GetSkillType() == X2mSkillType::AWAKEN)
    {
        AddCustomSkill(ui->awakenComboBox, dep.id, mod_name);
    }
}

void MainWindow::on_cusDependsRemoveButton_clicked()
{
    int idx = ui->cusDependsComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCharaSkillDepends())
        return;

    const X2mDepends &dep = x2m->GetCharaSkillDepends(idx);

    RemoveSkill(ui->super1ComboBox, dep.id);
    RemoveSkill(ui->super2ComboBox, dep.id);
    RemoveSkill(ui->super3ComboBox, dep.id);
    RemoveSkill(ui->super4ComboBox, dep.id);
    RemoveSkill(ui->ultimate1ComboBox, dep.id);
    RemoveSkill(ui->ultimate2ComboBox, dep.id);
    RemoveSkill(ui->evasiveComboBox, dep.id);
    RemoveSkill(ui->blastComboBox, dep.id);
    RemoveSkill(ui->awakenComboBox, dep.id);

    x2m->RemoveCharaSkillDependsAttachment(dep.guid);
    x2m->RemoveCharaSkillDepends(idx);
    ui->cusDependsComboBox->removeItem(idx);

    if (x2m->GetNumCharaSkillDepends() == 0)
    {
        ui->cusDependsRemoveButton->setDisabled(true);
        ui->cusDependsUpdateButton->setDisabled(true);
    }
}

void MainWindow::on_cusDependsUpdateButton_clicked()
{
    int idx = ui->cusDependsComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumCharaSkillDepends())
        return;

    const X2mDepends &dep = x2m->GetCharaSkillDepends(idx);

    QString file = QFileDialog::getOpenFileName(this, "Select skill x2m", config.lf_depends_skill, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_depends_skill = file;

    X2mFile skill_x2m;

    if (!skill_x2m.LoadFromFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Failed to load x2m.\n");
        return;
    }

    if (skill_x2m.GetType() != X2mType::NEW_SKILL)
    {
        DPRINTF("That x2m is not of new skill type!\n");
        return;
    }

    if (!x2m->CharaSkillDependsExist(skill_x2m.GetModGuid()))
    {
        DPRINTF("The selected x2m is not an update of this skill.\nTo add a new skill, use the Add button instead.\n");
        return;
    }

    X2mSkillType type;
    if (FindCustomSkillType(dep.id, &type) && type != skill_x2m.GetSkillType())
    {
        DPRINTF("The selected x2m is an update of this mod, but the type of skill is different.\n"
                "To fix this: remove it, and then use the Add button.\n");
        return;
    }

    int ret = LinkOrEmbed(nullptr);
    if (ret == 0)
        return;

    if (!x2m->AddCharaSkillDepends(&skill_x2m, true))
    {
        DPRINTF("Internal error when setting data.\n");
        return;
    }

    QString text;

    if (ret > 0)
    {
        if (!x2m->SetCharaSkillDependsAttachment(&skill_x2m))
        {
            DPRINTF("Failed to attach the skill.\n");
            return;
        }

        text += "[EMBEDDED] ";
    }
    else
    {
        x2m->RemoveCharaSkillDependsAttachment(skill_x2m.GetModGuid());
        text += "[LINKED] ";
    }

    QString mod_name = Utils::StdStringToQString(skill_x2m.GetModName(), false);

    text += mod_name;
    ui->cusDependsComboBox->setItemText(idx, text);

    {
        const X2mDepends &dep = x2m->GetCharaSkillDepends(idx);

        if (skill_x2m.GetSkillType() == X2mSkillType::SUPER)
        {
            AddCustomSkill(ui->super1ComboBox, dep.id, mod_name);
            AddCustomSkill(ui->super2ComboBox, dep.id, mod_name);
            AddCustomSkill(ui->super3ComboBox, dep.id, mod_name);
            AddCustomSkill(ui->super4ComboBox, dep.id, mod_name);
        }
        else if (skill_x2m.GetSkillType() == X2mSkillType::ULTIMATE)
        {
            AddCustomSkill(ui->ultimate1ComboBox, dep.id, mod_name);
            AddCustomSkill(ui->ultimate2ComboBox, dep.id, mod_name);
        }
        else if (skill_x2m.GetSkillType() == X2mSkillType::EVASIVE)
        {
            AddCustomSkill(ui->evasiveComboBox, dep.id, mod_name);
        }
        else if (skill_x2m.GetSkillType() == X2mSkillType::BLAST)
        {
            AddCustomSkill(ui->blastComboBox, dep.id, mod_name);
        }
        else if (skill_x2m.GetSkillType() == X2mSkillType::AWAKEN)
        {
            AddCustomSkill(ui->awakenComboBox, dep.id, mod_name);
        }
    }
}

void MainWindow::CsoEntryToGui(const CsoEntry &entry)
{
    ui->csoSeEdit->setText(Utils::StdStringToQString(entry.se, false));
    ui->csoVoxEdit->setText(Utils::StdStringToQString(entry.vox, false));
    ui->csoAmkEdit->setText(Utils::StdStringToQString(entry.amk, false));
    ui->csoSkillsEdit->setText(Utils::StdStringToQString(entry.skills, false));
}

void MainWindow::GuiToCsoEntry(CsoEntry &entry)
{
    entry.se = Utils::QStringToStdString(ui->csoSeEdit->text(), false);
    entry.vox = Utils::QStringToStdString(ui->csoVoxEdit->text(), false);
    entry.amk = Utils::QStringToStdString(ui->csoAmkEdit->text(), false);
    entry.skills = Utils::QStringToStdString(ui->csoSkillsEdit->text(), false);
}

void MainWindow::on_csoCheck_clicked()
{
    if (ui->csoCheck->isChecked())
    {
        ui->csoCostComboBox->setEnabled(true);
        ui->csoAddButton->setEnabled(true);
        ui->csoRemoveButton->setEnabled(true);
        ui->csoCopyButton->setEnabled(true);
        ui->csoSeEdit->setEnabled(true);
        ui->csoVoxEdit->setEnabled(true);
        ui->csoAmkEdit->setEnabled(true);
        ui->csoSkillsEdit->setEnabled(true);

        // Clear combobox first
        prev_cso_cost_index = -1;
        ui->csoCostComboBox->clear();

        size_t num_cso_entries = x2m->GetNumCsoEntries();

        if (num_cso_entries == 0)
        {
            CsoEntry entry;

            GuiToCsoEntry(entry);
            x2m->AddCsoEntry(entry);
            num_cso_entries++;
        }

        for (size_t i = 0; i < num_cso_entries; i++)
        {
            ui->csoCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_cso_entries == 1)
        {
            ui->csoRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumCsoEntries() != 0)
            x2m->RemoveCsoEntry(0);

        ui->csoCostComboBox->setDisabled(true);
        ui->csoAddButton->setDisabled(true);
        ui->csoRemoveButton->setDisabled(true);
        ui->csoCopyButton->setDisabled(true);
        ui->csoSeEdit->setDisabled(true);
        ui->csoVoxEdit->setDisabled(true);
        ui->csoAmkEdit->setDisabled(true);
        ui->csoSkillsEdit->setDisabled(true);
    }
}

void MainWindow::on_csoCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumCsoEntries())
        return;    

    if (prev_cso_cost_index >= 0 && prev_cso_cost_index < x2m->GetNumCsoEntries())
    {
        CsoEntry &entry = x2m->GetCsoEntry(prev_cso_cost_index);
        GuiToCsoEntry(entry);
    }

    const CsoEntry &entry = x2m->GetCsoEntry(index);
    CsoEntryToGui(entry);

    prev_cso_cost_index = index;
}

void MainWindow::on_csoAddButton_clicked()
{
    CsoEntry entry;

    int idx = ui->csoCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumCsoEntries())
    {
        GuiToCsoEntry(entry);
    }

    size_t pos = x2m->AddCsoEntry(entry);

    ui->csoCostComboBox->addItem(QString("Entry %1").arg(pos));

    if (x2m->GetNumCsoEntries() == XV2_MAX_SUBSLOTS)
        ui->csoAddButton->setDisabled(true);

    ui->csoCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumCsoEntries() > 1)
        ui->csoRemoveButton->setEnabled(true);
}

void MainWindow::on_csoRemoveButton_clicked()
{
    int index = ui->csoCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumCsoEntries())
        return;

    x2m->RemoveCsoEntry(index);

    if (prev_cso_cost_index > index)
        prev_cso_cost_index--;
    else
        prev_cso_cost_index = -1;

    ui->csoCostComboBox->removeItem(index);

    for (int i = 0; i < ui->csoCostComboBox->count(); i++)
    {
        ui->csoCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumCsoEntries() == 1)
        ui->csoRemoveButton->setDisabled(true);

    else if (x2m->GetNumCsoEntries() < XV2_MAX_SUBSLOTS)
        ui->csoAddButton->setEnabled(true);
}

void MainWindow::on_csoCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    CsoFile *cso = nullptr;

    if (arg1 == ui->actionFromGameCso)
    {
        dialog = new ListDialog(ListMode::CSO, this);
        cso = game_cso;
    }
    else if (arg1 == ui->actionFromExternalCso)
    {
        QString file = QFileDialog::getOpenFileName(this, "External CSO", config.lf_external_cso, "CSO Files (*.cso *.cso.xml)");

        if (file.isNull())
            return;

        config.lf_external_cso = file;

        cso = new CsoFile();
        if (!cso->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete cso;
            return;
        }

        dialog = new ListDialog(ListMode::CSO, this, cso);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < cso->GetNumEntries())
            {
                const CsoEntry &entry = (*cso)[idx];
                CsoEntryToGui(entry);
            }
        }

        delete dialog;

        if (cso != game_cso)
            delete cso;
    }
}

void MainWindow::PscEntryToGui(const PscSpecEntry &entry)
{
    ui->pscCamEdit->setText(QString("%1").arg((int32_t)entry.camera_position));
    ui->pscU0CEdit->setText(QString("%1").arg((int32_t)entry.unk_0C));
    ui->pscU10Edit->setText(QString("%1").arg((int32_t)entry.unk_10));
    ui->pscHealthEdit->setText(QString("%1").arg(entry.health));
    ui->pscF18Edit->setText(QString("%1").arg(entry.unk_18));
    ui->pscKiEdit->setText(QString("%1").arg(entry.ki));
    ui->pscKiRechEdit->setText(QString("%1").arg(entry.ki_recharge));
    ui->pscU24Edit->setText(QString("%1").arg((int32_t)entry.unk_24));
    ui->pscU28Edit->setText(QString("%1").arg((int32_t)entry.unk_28));
    ui->pscU2CEdit->setText(QString("%1").arg((int32_t)entry.unk_2C));
    ui->pscStaminaEdit->setText(QString("%1").arg(entry.stamina));
    ui->pscStaMovEdit->setText(QString("%1").arg(entry.stamina_recharge_move));
    ui->pscStaAirEdit->setText(QString("%1").arg(entry.stamina_recharge_air));
    ui->pscStaGroundEdit->setText(QString("%1").arg(entry.stamina_recharge_ground));

    ui->pscDrain1Edit->setText(QString("%1").arg(entry.stamina_drain_rate1));
    ui->pscDrain2Edit->setText(QString("%1").arg(entry.stamina_drain_rate2));
    ui->pscF48Edit->setText(QString("%1").arg(entry.unk_48));
    ui->pscAtkEdit->setText(QString("%1").arg(entry.basic_attack));
    ui->pscKiAtkEdit->setText(QString("%1").arg(entry.basic_ki_attack));
    ui->pscStrEdit->setText(QString("%1").arg(entry.strike_attack));
    ui->pscBlaEdit->setText(QString("%1").arg(entry.ki_blast_super));
    ui->pscPhyDefEdit->setText(QString("%1").arg(entry.basic_phys_defense));
    ui->pscKiDefEdit->setText(QString("%1").arg(entry.basic_ki_defense));
    ui->pscStrAtkDefEdit->setText(QString("%1").arg(entry.strike_atk_defense));
    ui->pscKiBlastDefEdit->setText(QString("%1").arg(entry.super_ki_blast_defense));
    ui->pscGroundSpeedEdit->setText(QString("%1").arg(entry.ground_speed));
    ui->pscAirSpeedEdit->setText(QString("%1").arg(entry.air_speed));
    ui->pscBoostSpeedEdit->setText(QString("%1").arg(entry.boosting_speed));

    ui->pscDashDistEdit->setText(QString("%1").arg(entry.dash_distance));
    ui->pscF7CEdit->setText(QString("%1").arg(entry.unk_7C));
    ui->pscReinfEdit->setText(QString("%1").arg(entry.reinf_skill_duration));
    ui->pscF84Edit->setText(QString("%1").arg(entry.unk_84));
    ui->pscRevivalHPEdit->setText(QString("%1").arg(entry.revival_hp_amount));
    ui->pscF8CEdit->setText(QString("%1").arg(entry.unk_8C));
    ui->pscRevivingSpeed->setText(QString("%1").arg(entry.reviving_speed));
    ui->pscU98Edit->setText(QString("%1").arg((int32_t)entry.unk_98));
    ui->pscSuperSoulEdit->setText(QString("%1").arg((int32_t)entry.talisman));
    ui->pscUB8Edit->setText(QString("%1").arg((int32_t)entry.unk_B8));
    ui->pscUBCEdit->setText(QString("%1").arg((int32_t)entry.unk_BC));
    ui->pscFC0Edit->setText(QString("%1").arg(entry.unk_C0));
}

void MainWindow::GuiToPscEntry(PscSpecEntry &entry)
{
    entry.camera_position = (uint32_t) ui->pscCamEdit->text().toInt();
    entry.unk_0C = (uint32_t) ui->pscU0CEdit->text().toInt();
    entry.unk_10 = (uint32_t) ui->pscU10Edit->text().toInt();
    entry.health = ui->pscHealthEdit->text().toFloat();
    entry.unk_18 = ui->pscF18Edit->text().toFloat();
    entry.ki = ui->pscKiEdit->text().toFloat();
    entry.ki_recharge = ui->pscKiRechEdit->text().toFloat();
    entry.unk_24 = (uint32_t) ui->pscU24Edit->text().toInt();
    entry.unk_28 = (uint32_t) ui->pscU28Edit->text().toInt();
    entry.unk_2C = (uint32_t) ui->pscU2CEdit->text().toInt();
    entry.stamina = ui->pscStaminaEdit->text().toFloat();
    entry.stamina_recharge_move = ui->pscStaMovEdit->text().toFloat();
    entry.stamina_recharge_air = ui->pscStaAirEdit->text().toFloat();
    entry.stamina_recharge_ground = ui->pscStaGroundEdit->text().toFloat();

    entry.stamina_drain_rate1 = ui->pscDrain1Edit->text().toFloat();
    entry.stamina_drain_rate2 = ui->pscDrain2Edit->text().toFloat();
    entry.unk_48 = ui->pscF48Edit->text().toFloat();
    entry.basic_attack = ui->pscAtkEdit->text().toFloat();
    entry.basic_ki_attack = ui->pscKiAtkEdit->text().toFloat();
    entry.strike_attack = ui->pscStrEdit->text().toFloat();
    entry.ki_blast_super = ui->pscBlaEdit->text().toFloat();
    entry.basic_phys_defense = ui->pscPhyDefEdit->text().toFloat();
    entry.basic_ki_defense = ui->pscKiDefEdit->text().toFloat();
    entry.strike_atk_defense = ui->pscStrAtkDefEdit->text().toFloat();
    entry.super_ki_blast_defense = ui->pscKiBlastDefEdit->text().toFloat();
    entry.ground_speed = ui->pscGroundSpeedEdit->text().toFloat();
    entry.air_speed = ui->pscAirSpeedEdit->text().toFloat();
    entry.boosting_speed = ui->pscBoostSpeedEdit->text().toFloat();

    entry.dash_distance = ui->pscDashDistEdit->text().toFloat();
    entry.unk_7C = ui->pscF7CEdit->text().toFloat();
    entry.reinf_skill_duration = ui->pscReinfEdit->text().toFloat();
    entry.unk_84 = ui->pscF84Edit->text().toFloat();
    entry.revival_hp_amount = ui->pscRevivalHPEdit->text().toFloat();
    entry.unk_8C = ui->pscF8CEdit->text().toFloat();
    entry.reviving_speed = ui->pscRevivingSpeed->text().toFloat();
    entry.unk_98 = (uint32_t) ui->pscU98Edit->text().toInt();
    entry.talisman = (uint32_t) ui->pscSuperSoulEdit->text().toInt();
    entry.unk_B8 = (uint32_t) ui->pscUB8Edit->text().toInt();
    entry.unk_BC = (uint32_t) ui->pscUBCEdit->text().toInt();
    entry.unk_C0 = ui->pscFC0Edit->text().toFloat();
}

void MainWindow::on_pscCheck_clicked()
{
    bool checked = ui->pscCheck->isChecked();

    ui->pscCamEdit->setEnabled(checked);
    ui->pscU0CEdit->setEnabled(checked);
    ui->pscU10Edit->setEnabled(checked);
    ui->pscHealthEdit->setEnabled(checked);
    ui->pscF18Edit->setEnabled(checked);
    ui->pscKiEdit->setEnabled(checked);
    ui->pscKiRechEdit->setEnabled(checked);
    ui->pscU24Edit->setEnabled(checked);
    ui->pscU28Edit->setEnabled(checked);
    ui->pscU2CEdit->setEnabled(checked);
    ui->pscStaminaEdit->setEnabled(checked);
    ui->pscStaMovEdit->setEnabled(checked);
    ui->pscStaAirEdit->setEnabled(checked);
    ui->pscStaGroundEdit->setEnabled(checked);

    ui->pscDrain1Edit->setEnabled(checked);
    ui->pscDrain2Edit->setEnabled(checked);
    ui->pscF48Edit->setEnabled(checked);
    ui->pscAtkEdit->setEnabled(checked);
    ui->pscKiAtkEdit->setEnabled(checked);
    ui->pscStrEdit->setEnabled(checked);
    ui->pscBlaEdit->setEnabled(checked);
    ui->pscPhyDefEdit->setEnabled(checked);
    ui->pscKiDefEdit->setEnabled(checked);
    ui->pscStrAtkDefEdit->setEnabled(checked);
    ui->pscKiBlastDefEdit->setEnabled(checked);
    ui->pscGroundSpeedEdit->setEnabled(checked);
    ui->pscAirSpeedEdit->setEnabled(checked);
    ui->pscBoostSpeedEdit->setEnabled(checked);

    ui->pscDashDistEdit->setEnabled(checked);
    ui->pscF7CEdit->setEnabled(checked);
    ui->pscReinfEdit->setEnabled(checked);
    ui->pscF84Edit->setEnabled(checked);
    ui->pscRevivalHPEdit->setEnabled(checked);
    ui->pscF8CEdit->setEnabled(checked);
    ui->pscRevivingSpeed->setEnabled(checked);
    ui->pscU98Edit->setEnabled(checked);
    ui->pscSuperSoulEdit->setEnabled(checked);
    ui->pscSuperSoulButton->setEnabled(checked);
    ui->pscUB8Edit->setEnabled(checked);
    ui->pscUBCEdit->setEnabled(checked);
    ui->pscFC0Edit->setEnabled(checked);

    ui->pscCostComboBox->setEnabled(checked);
    ui->pscAddButton->setEnabled(checked);
    ui->pscRemoveButton->setEnabled(checked);
    ui->pscCopyButton->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_psc_cost_index = -1;
        ui->pscCostComboBox->clear();

        size_t num_psc_entries = x2m->GetNumPscEntries();

        if (num_psc_entries == 0)
        {
            PscSpecEntry entry;

            GuiToPscEntry(entry);
            x2m->AddPscEntry(entry);
            num_psc_entries++;
        }

        for (size_t i = 0; i < num_psc_entries; i++)
        {
            ui->pscCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_psc_entries == 1)
        {
            ui->pscRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumPscEntries() != 0)
            x2m->RemovePscEntry(0);
    }
}

void MainWindow::on_pscCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumPscEntries())
        return;

    if (prev_psc_cost_index >= 0 && prev_psc_cost_index < x2m->GetNumPscEntries())
    {
        PscSpecEntry &entry = x2m->GetPscEntry(prev_psc_cost_index);
        GuiToPscEntry(entry);
    }

    const PscSpecEntry &entry = x2m->GetPscEntry(index);
    PscEntryToGui(entry);

    prev_psc_cost_index = index;
}

void MainWindow::on_pscAddButton_clicked()
{
    PscSpecEntry entry;

    int idx = ui->pscCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumPscEntries())
    {
        GuiToPscEntry(entry);
    }

    size_t pos = x2m->AddPscEntry(entry);

    ui->pscCostComboBox->addItem(QString("Entry %1").arg(pos));

    if (x2m->GetNumPscEntries() == XV2_MAX_SUBSLOTS)
        ui->pscAddButton->setDisabled(true);

    ui->pscCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumPscEntries() > 1)
        ui->pscRemoveButton->setEnabled(true);
}

void MainWindow::on_pscRemoveButton_clicked()
{
    int index = ui->pscCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumPscEntries())
        return;

    x2m->RemovePscEntry(index);

    if (prev_psc_cost_index > index)
        prev_psc_cost_index--;
    else
        prev_psc_cost_index = -1;

    ui->pscCostComboBox->removeItem(index);

    for (int i = 0; i < ui->pscCostComboBox->count(); i++)
    {
        ui->pscCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumPscEntries() == 1)
        ui->pscRemoveButton->setDisabled(true);

    else if (x2m->GetNumPscEntries() < XV2_MAX_SUBSLOTS)
        ui->pscAddButton->setEnabled(true);
}

void MainWindow::on_pscCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    PscFile *psc = nullptr;
    size_t cfg = 0;

    if (arg1 == ui->actionFromGamePsc)
    {
        dialog = new ListDialog(ListMode::PSC, this);
        psc = game_psc;
    }
    else if (arg1 == ui->actionFromGamePscBuf)
    {
        cfg = 1;
        dialog = new ListDialog(ListMode::PSC, this, nullptr, 1);
        psc = game_psc;
    }
    else if (arg1 == ui->actionFromExternalPsc)
    {
        QString file = QFileDialog::getOpenFileName(this, "External PSC", config.lf_external_psc, "PSC Files (*.psc *.psc.xml)");

        if (file.isNull())
            return;

        config.lf_external_psc = file;

        psc = new PscFile();
        if (!psc->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete psc;
            return;
        }

        dialog = new ListDialog(ListMode::PSC, this, psc);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0)
            {
                PscSpecEntry *entry = psc->FindSpecFromAbsolutePos((size_t)idx, cfg);

                if (entry)
                {
                    PscEntryToGui(*entry);
                }
            }
        }

        delete dialog;

        if (psc != game_psc)
            delete psc;
    }
}

void MainWindow::on_pscSuperSoulButton_clicked()
{
    ListDialog dialog(ListMode::TALISMAN, this);

    if (!dialog.exec())
        return;

    int idx = dialog.GetResultInteger();

    if (idx < 0 || idx >= game_talisman_idb->GetNumEntries())
        return;

    ui->pscSuperSoulEdit->setText(QString("%1").arg((*game_talisman_idb)[idx].id));
}

void MainWindow::on_pscSuperSoulEdit_textChanged(const QString &arg1)
{
    int id = arg1.toInt();

    if (id < 0)
    {
        ui->pscSuperSoulEdit->setToolTip(QString());
        return;
    }

    std::string name = Xenoverse2::GetTalismanNameEx(id);

    if (name.length() != 0)
    {
        ui->pscSuperSoulEdit->setToolTip(Utils::StdStringToQString(name, false));
    }
    else
    {
        ui->pscSuperSoulEdit->setToolTip(QString());
    }
}

void MainWindow::AurEntryToGui(const AurCharaLink &entry, const AurAura &aura)
{
    //ui->auraEdit->setText(QString("%1").arg((int32_t)entry.aura_id));
    ui->glareCheck->setChecked(entry.glare);    

    if (entry.aura_id == X2M_INVALID_ID)
    {
        ui->auraCustomCheck->setChecked(true);
        ui->auraEdit->setText("-1");

        ui->auraEdit->setDisabled(true);
        ui->auraEf0Edit->setEnabled(true);
        ui->auraEf1Edit->setEnabled(true);
        ui->auraEf2Edit->setEnabled(true);
        ui->auraEf3Edit->setEnabled(true);
        ui->auraEf4Edit->setEnabled(true);
        ui->auraEf5Edit->setEnabled(true);
        ui->auraEf6Edit->setEnabled(true);
    }
    else
    {
        ui->auraCustomCheck->setChecked(false);
        ui->auraEdit->setText(QString("%1").arg((int32_t)entry.aura_id));

        ui->auraEdit->setEnabled(true);
        ui->auraEf0Edit->setDisabled(true);
        ui->auraEf1Edit->setDisabled(true);
        ui->auraEf2Edit->setDisabled(true);
        ui->auraEf3Edit->setDisabled(true);
        ui->auraEf4Edit->setDisabled(true);
        ui->auraEf5Edit->setDisabled(true);
        ui->auraEf6Edit->setDisabled(true);
    }

    if (aura.effects.size() >= 1)
    {
        ui->auraEf0Edit->setText(QString("%1").arg((int32_t)aura.effects[0].id));
    }
    else
    {
        ui->auraEf0Edit->setText("-1");
    }

    if (aura.effects.size() >= 2)
    {
        ui->auraEf1Edit->setText(QString("%1").arg((int32_t)aura.effects[1].id));
    }
    else
    {
        ui->auraEf1Edit->setText("-1");
    }

    if (aura.effects.size() >= 3)
    {
        ui->auraEf2Edit->setText(QString("%1").arg((int32_t)aura.effects[2].id));
    }
    else
    {
        ui->auraEf2Edit->setText("-1");
    }

    if (aura.effects.size() >= 4)
    {
        ui->auraEf3Edit->setText(QString("%1").arg((int32_t)aura.effects[3].id));
    }
    else
    {
        ui->auraEf3Edit->setText("-1");
    }

    if (aura.effects.size() >= 5)
    {
        ui->auraEf4Edit->setText(QString("%1").arg((int32_t)aura.effects[4].id));
    }
    else
    {
        ui->auraEf4Edit->setText("-1");
    }

    if (aura.effects.size() >= 6)
    {
        ui->auraEf5Edit->setText(QString("%1").arg((int32_t)aura.effects[5].id));
    }
    else
    {
        ui->auraEf5Edit->setText("-1");
    }

    if (aura.effects.size() >= 7)
    {
        ui->auraEf6Edit->setText(QString("%1").arg((int32_t)aura.effects[6].id));
    }
    else
    {
        ui->auraEf6Edit->setText("-1");
    }
}

void MainWindow::GuiToAurEntry(AurCharaLink &entry, AurAura &aura)
{
    entry.aura_id = (uint32_t) ui->auraEdit->text().toInt();
    entry.glare = ui->glareCheck->isChecked();

    if (aura.effects.size() != 7)
        aura.effects.resize(7);

    for (uint32_t i = 0; i < 7; i++)
    {
        aura.effects[i].type_index = i;
    }

    if (ui->auraCustomCheck->isChecked())
    {
        entry.aura_id = X2M_INVALID_ID;
    }
    else
    {
        entry.aura_id = (uint32_t) ui->auraEdit->text().toInt();
    }

    aura.effects[0].id = (uint32_t) ui->auraEf0Edit->text().toInt();
    aura.effects[1].id = (uint32_t) ui->auraEf1Edit->text().toInt();
    aura.effects[2].id = (uint32_t) ui->auraEf2Edit->text().toInt();
    aura.effects[3].id = (uint32_t) ui->auraEf3Edit->text().toInt();
    aura.effects[4].id = (uint32_t) ui->auraEf4Edit->text().toInt();
    aura.effects[5].id = (uint32_t) ui->auraEf5Edit->text().toInt();
    aura.effects[6].id = (uint32_t) ui->auraEf6Edit->text().toInt();
}

void MainWindow::on_aurCheck_clicked()
{
    bool checked = ui->aurCheck->isChecked();
    bool checked_custom = ui->auraCustomCheck->isChecked();

    ui->auraEdit->setEnabled(checked && !checked_custom);
    ui->glareCheck->setEnabled(checked);
    ui->aurCostComboBox->setEnabled(checked);
    ui->aurAddButton->setEnabled(checked);
    ui->aurRemoveButton->setEnabled(checked);
    ui->aurCopyButton->setEnabled(checked);
    ui->auraCustomCheck->setEnabled(checked);
    ui->auraEf0Edit->setEnabled(checked && checked_custom);
    ui->auraEf1Edit->setEnabled(checked && checked_custom);
    ui->auraEf2Edit->setEnabled(checked && checked_custom);
    ui->auraEf3Edit->setEnabled(checked && checked_custom);
    ui->auraEf4Edit->setEnabled(checked && checked_custom);
    ui->auraEf5Edit->setEnabled(checked && checked_custom);
    ui->auraEf6Edit->setEnabled(checked && checked_custom);

    if (checked)
    {
        // Clear combobox first
        prev_aur_cost_index = -1;
        ui->aurCostComboBox->clear();

        size_t num_aur_entries = x2m->GetNumAurEntries();

        if (num_aur_entries == 0)
        {
            AurCharaLink entry;
            AurAura aura;

            GuiToAurEntry(entry, aura);
            x2m->AddAurEntry(entry);
            x2m->AddCharaAura(aura);
            num_aur_entries++;
        }

        for (size_t i = 0; i < num_aur_entries; i++)
        {
            ui->aurCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_aur_entries == 1)
        {
            ui->aurRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumAurEntries() != 0)
            x2m->RemoveAurEntry(0);

        while (x2m->GetNumCharaAuras(false) != 0)
            x2m->RemoveCharaAura(0);
    }
}

void MainWindow::on_aurCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumAurEntries())
        return;

    if (prev_aur_cost_index >= 0 && prev_aur_cost_index < x2m->GetNumAurEntries())
    {
        AurCharaLink &entry = x2m->GetAurEntry(prev_aur_cost_index);
        AurAura &aura = x2m->GetCharaAura(prev_aur_cost_index);
        GuiToAurEntry(entry, aura);
    }

    const AurCharaLink &entry = x2m->GetAurEntry(index);
    const AurAura &aura = x2m->GetCharaAura(index);
    AurEntryToGui(entry, aura);

    prev_aur_cost_index = index;
}

void MainWindow::on_aurAddButton_clicked()
{
    AurCharaLink entry;
    AurAura aura;

    int idx = ui->aurCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumAurEntries())
    {
        GuiToAurEntry(entry, aura);
    }

    size_t pos = x2m->AddAurEntry(entry);
    if (x2m->AddCharaAura(aura) != pos)
    {
        DPRINTF("%s: Internal syncronization bug.\n", FUNCNAME);
        exit(-1);
    }

    ui->aurCostComboBox->addItem(QString("Entry %1").arg(pos));

    if (x2m->GetNumAurEntries() == XV2_MAX_SUBSLOTS)
        ui->aurAddButton->setDisabled(true);

    ui->aurCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumAurEntries() > 1)
        ui->aurRemoveButton->setEnabled(true);
}

void MainWindow::on_aurRemoveButton_clicked()
{
    int index = ui->aurCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumAurEntries())
        return;

    x2m->RemoveAurEntry(index);
    x2m->RemoveCharaAura(index);

    if (prev_aur_cost_index > index)
        prev_aur_cost_index--;
    else
        prev_aur_cost_index = -1;

    ui->aurCostComboBox->removeItem(index);

    for (int i = 0; i < ui->aurCostComboBox->count(); i++)
    {
        ui->aurCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumAurEntries() == 1)
        ui->aurRemoveButton->setDisabled(true);

    else if (x2m->GetNumAurEntries() < XV2_MAX_SUBSLOTS)
        ui->aurAddButton->setEnabled(true);
}

void MainWindow::on_aurCopyButton_triggered(QAction *arg1)
{
    int idx = ui->aurCostComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumAurEntries())
        return;

    AurCharaLink &entry = x2m->GetAurEntry(idx);
    AurAura &aura = x2m->GetCharaAura(idx);
    GuiToAurEntry(entry, aura);

    if (arg1 == ui->actionFromAuraSkillUsage)
    {
        ListDialog dialog(ListMode::AURA_SKILL, this);

        if (dialog.exec())
        {
            idx = dialog.GetResultData();
            AurAura *existing_aura = game_aur->FindAuraByID(idx);

            if (existing_aura)
            {
                aura = *existing_aura;

                if (entry.aura_id != X2M_INVALID_ID)
                    entry.aura_id = aura.id;

                aura.id = X2M_DUMMY_ID;
                AurEntryToGui(entry, aura);
            }
        }

        return;
    }

    if (arg1 == ui->actionFromAuraCharUsage)
    {
        ListDialog dialog(ListMode::AUR, this);

        if (dialog.exec())
        {
            const std::vector<AurCharaLink> &chara_links = game_aur->GetCharaLinks();

            idx = dialog.GetResultInteger();
            if (idx < 0 || idx >= chara_links.size())
                return;

            AurAura *existing_aura = game_aur->FindAuraByID(chara_links[idx].aura_id);

            if (existing_aura)
            {
                aura = *existing_aura;

                if (entry.aura_id != X2M_INVALID_ID)
                    entry.aura_id = aura.id;

                entry.glare = chara_links[idx].glare;
                aura.id = X2M_DUMMY_ID;

                AurEntryToGui(entry, aura);
            }
        }

        return;
    }

    ListDialog *dialog = nullptr;
    AurFile *aur = nullptr;

    if (arg1 == ui->actionFromGameAur)
    {
        dialog = new ListDialog(ListMode::AURA, this);
        aur = game_aur;
    }
    else if (arg1 == ui->actionFromExternalAur)
    {
        QString file = QFileDialog::getOpenFileName(this, "External AUR", config.lf_external_aur, "AUR Files (*.aur *.aur.xml)");

        if (file.isNull())
            return;

        config.lf_external_aur = file;

        aur = new AurFile();
        if (!aur->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("SmartLoad failed.\n");
            delete aur;
            return;
        }

        dialog = new ListDialog(ListMode::AURA, this, aur);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            const std::vector<AurAura> &auras = aur->GetAuras();

            idx = dialog->GetResultInteger();
            if (idx < 0 || idx >= auras.size())
                return;

            aura = auras[idx];

            if (entry.aura_id != X2M_INVALID_ID)
                entry.aura_id = aura.id;

            aura.id = X2M_DUMMY_ID;
            AurEntryToGui(entry, aura);
        }

        delete dialog;

        if (aur != game_aur)
            delete aur;
    }
}

void MainWindow::on_auraCustomCheck_clicked()
{
    bool checked = ui->auraCustomCheck->isChecked();

    if (checked)
    {
        ui->auraEdit->setDisabled(true);
        ui->auraEf0Edit->setEnabled(true);
        ui->auraEf1Edit->setEnabled(true);
        ui->auraEf2Edit->setEnabled(true);
        ui->auraEf3Edit->setEnabled(true);
        ui->auraEf4Edit->setEnabled(true);
        ui->auraEf5Edit->setEnabled(true);
        ui->auraEf6Edit->setEnabled(true);
    }
    else
    {
        ui->auraEdit->setEnabled(true);
        ui->auraEf0Edit->setDisabled(true);
        ui->auraEf1Edit->setDisabled(true);
        ui->auraEf2Edit->setDisabled(true);
        ui->auraEf3Edit->setDisabled(true);
        ui->auraEf4Edit->setDisabled(true);
        ui->auraEf5Edit->setDisabled(true);
        ui->auraEf6Edit->setDisabled(true);
    }
}

void MainWindow::FillSevHLComboBox()
{
    QComboBox *cb = ui->sevHLCharComboBox;

    for (const SevEntry &entry : *game_sev)
    {
        if (entry.char_id >= XV2_FREE_ID_SEARCH_START)
            continue;

        CmsEntry *cms_entry = game_cms->FindEntryByID(entry.char_id);

        if (!cms_entry || cms_entry->name.length() != 3)
            continue;

        std::string name, costume_name;

        if (!Xenoverse2::GetCharaName(cms_entry->name, name, XV2_LANG_ENGLISH, (entry.costume_id == 0xFFFFFFFF) ? 0 : entry.costume_id))
            name = cms_entry->name;
        else
            name = cms_entry->name + " - " + name;

        if (entry.costume_id == 0xFFFFFFFF)
        {
            costume_name = "Any";
        }
        else
        {
           if (!Xenoverse2::GetCharaCostumeName(cms_entry->name, entry.costume_id, 0, costume_name))
               costume_name = Utils::ToString(entry.costume_id);
        }

        cb->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(costume_name, false),
                    QVariant(Utils::StdStringToQString(cms_entry->name) + QString("%1").arg(entry.costume_id)));
    }
}

void MainWindow::SevEntryHLToGui(const SevEntryHL &entry)
{
    ui->sevHLCostIndexEdit->setText(QString("%1").arg((int32_t)entry.costume_id));

    QString search_term = Utils::StdStringToQString(entry.copy_char) + QString("%1").arg(entry.copy_costume);
    int index = ui->sevHLCharComboBox->findData(QVariant(search_term));

    if (index < 0)
    {
        index = 0;
    }

    ui->sevHLCharComboBox->setCurrentIndex(index);
}

void MainWindow::GuiToSevEntryHL(SevEntryHL &entry)
{
    entry.costume_id = (uint32_t)ui->sevHLCostIndexEdit->text().toInt();

    QString str = ui->sevHLCharComboBox->itemData(ui->sevHLCharComboBox->currentIndex()).toString();

    entry.copy_char = Utils::QStringToStdString(str.left(3));
    entry.copy_costume = str.mid(3).toUInt();
}

void MainWindow::SevEventEntryToGui(const SevEventEntry &entry)
{
    bool custom = (entry.cue_id >= X2M_CUSTOM_AUDIO_BEGIN && entry.cue_id < X2M_CUSTOM_AUDIO_END);
    bool custom_resp = (entry.response_cue_id >= X2M_CUSTOM_AUDIO_BEGIN && entry.response_cue_id < X2M_CUSTOM_AUDIO_END);

    // Set defaults
    ui->sevLLAudioComboBox->setCurrentIndex(0);
    ui->sevLLAudioRespComboBox->setCurrentIndex(0);

    if (custom)
    {
        X2mCustomAudio *audio = x2m->FindCustomAudio(entry.cue_id);

        if (audio)
        {
            int idx = ui->sevLLAudioComboBox->findText(Utils::StdStringToQString(audio->name));
            if (idx != -1)
            {
                ui->sevLLAudioComboBox->setCurrentIndex(idx);
            }
        }

        ui->sevLLEvEntCueIdEdit->setText("-1");
        ui->sevLLEvEntFileIdEdit->setText("-1");

        ui->sevLLEvEntCueIdEdit->setEnabled(false);
        ui->sevLLEvEntFileIdEdit->setEnabled(false);
    }
    else
    {
        ui->sevLLEvEntCueIdEdit->setText(QString("%1").arg((int32_t)entry.cue_id));
        ui->sevLLEvEntFileIdEdit->setText(QString("%1").arg((int32_t)entry.file_id));

        ui->sevLLEvEntCueIdEdit->setEnabled(true);
        ui->sevLLEvEntFileIdEdit->setEnabled(true);
    }

    if (custom_resp)
    {
        X2mCustomAudio *audio = x2m->FindCustomAudio(entry.response_cue_id);

        if (audio)
        {
            int idx = ui->sevLLAudioRespComboBox->findText(Utils::StdStringToQString(audio->name));
            if (idx != -1)
                ui->sevLLAudioRespComboBox->setCurrentIndex(idx);
        }

        ui->sevLLEvEntRespCueIdEdit->setText("-1");
        ui->sevLLEvEntRespFileIdEdit->setText("-1");

        ui->sevLLEvEntRespCueIdEdit->setEnabled(false);
        ui->sevLLEvEntRespFileIdEdit->setEnabled(false);
    }
    else
    {
        ui->sevLLEvEntRespCueIdEdit->setText(QString("%1").arg((int32_t)entry.response_cue_id));
        ui->sevLLEvEntRespFileIdEdit->setText(QString("%1").arg((int32_t)entry.response_file_id));

        ui->sevLLEvEntRespCueIdEdit->setEnabled(true);
        ui->sevLLEvEntRespFileIdEdit->setEnabled(true);
    }

    ui->sevLLEvEntU00Edit->setText(QString("%1").arg((int32_t)entry.unk_00));
    ui->sevLLEvEntU0CEdit->setText(QString("%1").arg((int32_t)entry.old_unk_0C));
    ui->sevLLEvEntU10Edit->setText(QString("%1").arg((int32_t)entry.old_unk_10));
    ui->sevLLEvEntNU0CEdit->setText(QString("%1").arg((int32_t)entry.new_unk_0C));
    ui->sevLLEvEntU18Edit->setText(QString("%1").arg((int32_t)entry.unk_18));
}

void MainWindow::GuiToSevEventEntry(SevEventEntry &entry)
{
    bool custom = (ui->sevLLAudioComboBox->isEnabled() && ui->sevLLAudioComboBox->currentIndex() != 0);
    bool custom_resp = (ui->sevLLAudioRespComboBox->isEnabled() && ui->sevLLAudioRespComboBox->currentIndex() != 0);

    if (custom)
    {
        entry.cue_id = x2m->AddCustomAudio(Utils::QStringToStdString(ui->sevLLAudioComboBox->currentText()));
        entry.file_id = 2;
    }
    else
    {
        entry.cue_id = (uint32_t) ui->sevLLEvEntCueIdEdit->text().toInt();
        entry.file_id = (uint32_t) ui->sevLLEvEntFileIdEdit->text().toInt();
    }

    if (custom_resp)
    {
        entry.response_cue_id = x2m->AddCustomAudio(Utils::QStringToStdString(ui->sevLLAudioRespComboBox->currentText()));
        entry.response_file_id = 2;
    }
    else
    {
        entry.response_cue_id = (uint32_t) ui->sevLLEvEntRespCueIdEdit->text().toInt();
        entry.response_file_id = (uint32_t) ui->sevLLEvEntRespFileIdEdit->text().toInt();
    }

    entry.unk_00 = (uint32_t) ui->sevLLEvEntU00Edit->text().toInt();
    entry.old_unk_0C = (uint32_t) ui->sevLLEvEntU0CEdit->text().toInt();
    entry.old_unk_10 = (uint32_t) ui->sevLLEvEntU10Edit->text().toInt();
    entry.new_unk_0C = (uint32_t) ui->sevLLEvEntNU0CEdit->text().toInt();
    entry.unk_18 = (uint32_t) ui->sevLLEvEntU18Edit->text().toInt();
}

void MainWindow::SevEventToGui(const SevEvent &entry)
{
    int idx = ui->sevLLTypeComboBox->findData(QVariant(entry.type));

    if (idx == -1)
    {
        idx = ui->sevLLTypeComboBox->count();
        ui->sevLLTypeComboBox->addItem(QString("Unknown %1").arg(entry.type), QVariant(entry.type));
    }

    ui->sevLLTypeComboBox->setCurrentIndex(idx);

    int index = ui->sevLLEvEntriesComboBox->currentIndex();
    int count = ui->sevLLEvEntriesComboBox->count();

    if (count != (int)entry.entries.size())
    {
        // Clear combobox
        prev_sevll_ev_ent_index = -1;
        ui->sevLLEvEntriesComboBox->clear();

        if (entry.entries.size() == 0)
        {
            index = -1;
        }
        else
        {
            index = 0;

            for (size_t i = 0; i < entry.entries.size(); i++)
            {
                ui->sevLLEvEntriesComboBox->addItem(QString("Event entry %1").arg(i));
            }
        }
    }

    count = ui->sevLLEvEntriesComboBox->count();
    ui->sevLLEvEntriesRemoveButton->setEnabled(count > 1);

    if (entry.entries.size() > 0 && (index < 0 || index >= (int)entry.entries.size()))
    {
        index = 0;
    }

    if (index >= 0 && index < (int)entry.entries.size())
    {
        SevEventEntryToGui(entry.entries[index]);
    }
}

void MainWindow::GuiToSevEvent(SevEvent &entry)
{
    entry.type = (uint32_t) ui->sevLLTypeComboBox->currentData().toInt();
    entry.entries.resize(ui->sevLLEvEntriesComboBox->count());

    int index = ui->sevLLEvEntriesComboBox->currentIndex();

    if (index >= 0 && index < (int)entry.entries.size())
    {
        GuiToSevEventEntry(entry.entries[index]);
    }
}

void MainWindow::SevEventsLLToGui(const SevEventsLL &entry)
{
    ui->sevLLCodeEdit->setText(Utils::StdStringToQString(entry.char_code));
    ui->sevLLOtherCostEdit->setText(QString("%1").arg((int32_t)entry.costume_id));
    ui->sevLLDirectionComboBox->setCurrentIndex((entry.im_source) ? 0 : 1);

    int index = ui->sevLLEventsComboBox->currentIndex();
    int count = ui->sevLLEventsComboBox->count();

    if (count != (int)entry.events.size())
    {
        // Clear combobox
        prev_sevll_ev_index = -1;
        ui->sevLLEventsComboBox->clear();

        if (entry.events.size() == 0)
        {
            index = -1;
        }
        else
        {
            index = 0;

            for (size_t i = 0; i < entry.events.size(); i++)
            {
                ui->sevLLEventsComboBox->addItem(QString("Event %1").arg(i));
            }
        }
    }

    count = ui->sevLLEventsComboBox->count();
    ui->sevLLEventsRemoveButton->setEnabled(count > 1);

    if (entry.events.size() > 0 && (index < 0 || index >= (int)entry.events.size()))
    {
        index = 0;
    }

    if (index >= 0 && index < (int)entry.events.size())
    {
        SevEventToGui(entry.events[index]);
    }
}

void MainWindow::GuiToSevEventsLL(SevEventsLL &entry)
{
    SevEvent def;
    def.entries.resize(1);

    entry.char_code = Utils::QStringToStdString(ui->sevLLCodeEdit->text());
    entry.costume_id = (uint32_t) ui->sevLLOtherCostEdit->text().toInt();
    entry.im_source = (ui->sevLLDirectionComboBox->currentIndex() == 0);
    entry.events.resize(ui->sevLLEventsComboBox->count(), def);

    int index = ui->sevLLEventsComboBox->currentIndex();

    if (index >= 0 && index < (int)entry.events.size())
    {
        GuiToSevEvent(entry.events[index]);
    }
}

void MainWindow::SevEntryLLToGui(const SevEntryLL &entry)
{
    ui->sevLLCostIndexEdit->setText(QString("%1").arg((int32_t)entry.costume_id));

    int index = ui->sevLLCharComboBox->currentIndex();
    int count = ui->sevLLCharComboBox->count();

    if (count != (int)entry.sub_entries.size())
    {
        // Clear combobox
        prev_sevll_char_index = -1;
        ui->sevLLCharComboBox->clear();

        if (entry.sub_entries.size() == 0)
        {
            index = -1;
        }
        else
        {
            index = 0;

            for (size_t i = 0; i < entry.sub_entries.size(); i++)
            {
                ui->sevLLCharComboBox->addItem(QString("Character %1").arg(i));
            }
        }
    }

    count = ui->sevLLCharComboBox->count();
    ui->sevLLCharRemoveButton->setEnabled(count > 1);

    if (entry.sub_entries.size() > 0 && (index < 0 || index >= (int)entry.sub_entries.size()))
    {
        index = 0;
    }

    if (index >= 0 && index < (int)entry.sub_entries.size())
    {
        SevEventsLLToGui(entry.sub_entries[index]);
    }
}

void MainWindow::GuiToSevEntryLL(SevEntryLL &entry)
{
    SevEventsLL def;
    def.events.resize(1);
    def.events[0].entries.resize(1);

    entry.costume_id = (uint32_t) ui->sevLLCostIndexEdit->text().toInt();
    entry.sub_entries.resize(ui->sevLLCharComboBox->count(), def);

    int index = ui->sevLLCharComboBox->currentIndex();

    if (index >= 0 && index < (int)entry.sub_entries.size())
    {
        GuiToSevEventsLL(entry.sub_entries[index]);
    }
}

void MainWindow::RebuildSevEnable(bool enable_hl, bool enable_ll)
{
    ui->sevModeComboBox->setEnabled(enable_hl || enable_ll);

    ui->sevHLAddButton->setEnabled(enable_hl);
    ui->sevHLCharComboBox->setEnabled(enable_hl);
    ui->sevHLCostComboBox->setEnabled(enable_hl);
    ui->sevHLCostIndexEdit->setEnabled(enable_hl);
    ui->sevHLRemoveButton->setEnabled(enable_hl && ui->sevHLCostComboBox->count() > 1);

    ui->sevLLAddButton->setEnabled(enable_ll);
    ui->sevLLCharAddButton->setEnabled(enable_ll);
    ui->sevLLCharComboBox->setEnabled(enable_ll);
    ui->sevLLCharRemoveButton->setEnabled(enable_ll && ui->sevLLCharComboBox->count() > 1);
    ui->sevLLCodeEdit->setEnabled(enable_ll);
    ui->sevLLCopyButton->setEnabled(enable_ll);
    ui->sevLLCostComboBox->setEnabled(enable_ll);
    ui->sevLLCostIndexEdit->setEnabled(enable_ll);
    ui->sevLLDirectionComboBox->setEnabled(enable_ll);
    ui->sevLLOtherCostEdit->setEnabled(enable_ll);
    ui->sevLLRemoveButton->setEnabled(enable_ll && ui->sevLLCostComboBox->count() > 1);
    ui->sevLLEventsAddButton->setEnabled(enable_ll);
    ui->sevLLEventsComboBox->setEnabled(enable_ll);
    ui->sevLLEventsRemoveButton->setEnabled(enable_ll && ui->sevLLEventsComboBox->count() > 1);
    ui->sevLLTypeComboBox->setEnabled(enable_ll);
    ui->sevLLEvEntriesAddButton->setEnabled(enable_ll);
    ui->sevLLEvEntriesComboBox->setEnabled(enable_ll);
    ui->sevLLEvEntriesRemoveButton->setEnabled(enable_ll && ui->sevLLEvEntriesComboBox->count() > 1);
    ui->sevLLEvEntU00Edit->setEnabled(enable_ll);
    ui->sevLLEvEntCueIdEdit->setEnabled(enable_ll && ui->sevLLAudioComboBox->currentIndex() <= 0);
    ui->sevLLEvEntFileIdEdit->setEnabled(enable_ll && ui->sevLLAudioComboBox->currentIndex() <= 0);
    ui->sevLLEvEntU0CEdit->setEnabled(enable_ll);
    ui->sevLLEvEntU10Edit->setEnabled(enable_ll);
    ui->sevLLEvEntNU0CEdit->setEnabled(enable_ll);
    ui->sevLLEvEntRespCueIdEdit->setEnabled(enable_ll && ui->sevLLAudioRespComboBox->currentIndex() <= 0);
    ui->sevLLEvEntRespFileIdEdit->setEnabled(enable_ll && ui->sevLLAudioRespComboBox->currentIndex() <= 0);
    ui->sevLLEvEntU18Edit->setEnabled(enable_ll);
    ui->sevListenButton->setEnabled(enable_ll);
    ui->sevListenResponseButton->setEnabled(enable_ll);
    ui->sevListenLangComboBox->setEnabled(enable_ll);

    if (enable_ll)
    {
        std::vector<std::string> audio_files;
        x2m->GetAudioFiles(audio_files);

        bool enable = (audio_files.size() != 0);

        ui->sevLLAudioComboBox->setEnabled(enable);
        ui->sevLLAudioRespComboBox->setEnabled(enable);
    }
    else
    {
        ui->sevLLAudioComboBox->setEnabled(false);
        ui->sevLLAudioRespComboBox->setEnabled(false);
    }
}

void MainWindow::on_sevCheck_clicked()
{
    bool checked = ui->sevCheck->isChecked();
    bool enable_hl = (checked && ui->sevModeComboBox->currentIndex() == 0);
    bool enable_ll = (checked && !enable_hl);

    RebuildSevEnable(enable_hl, enable_ll);

    if (checked)
    {
        // Update audio files
        std::vector<std::string> audio_files;
        x2m->GetAudioFiles(audio_files);

        ui->sevLLAudioComboBox->clear();
        ui->sevLLAudioRespComboBox->clear();

        ui->sevLLAudioComboBox->addItem("[NONE]");
        ui->sevLLAudioRespComboBox->addItem("[NONE]");

        for (const std::string &file : audio_files)
        {
            ui->sevLLAudioComboBox->addItem(Utils::StdStringToQString(file));
            ui->sevLLAudioRespComboBox->addItem(Utils::StdStringToQString(file));
        }

        size_t num_hl_entries = x2m->GetNumSevHLEntries();
        size_t num_ll_entries = x2m->GetNumSevLLEntries();

        if (num_hl_entries == 0)
        {
            SevEntryHL entry;

            GuiToSevEntryHL(entry);
            x2m->AddSevHLEntry(entry);
            num_hl_entries++;
        }

        if (num_ll_entries == 0)
        {
            SevEntryLL entry(true);

            GuiToSevEntryLL(entry);
            x2m->AddSevLLEntry(entry);
            num_ll_entries++;
        }

        // Clear combobox now
        prev_sevhl_cost_index = prev_sevll_cost_index = -1;
        prev_sevll_char_index = prev_sevll_ev_index = prev_sevll_ev_ent_index = -1;
        ui->sevHLCostComboBox->clear();
        ui->sevLLCostComboBox->clear();
        ui->sevLLCharComboBox->clear();
        ui->sevLLEventsComboBox->clear();
        ui->sevLLEvEntriesComboBox->clear();

        for (size_t i = 0; i < num_hl_entries; i++)
        {
            ui->sevHLCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        for (size_t i = 0; i < num_ll_entries; i++)
        {
            ui->sevLLCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_hl_entries == 1)
        {
            ui->sevHLRemoveButton->setDisabled(true);
        }

        if (num_ll_entries == 1)
        {
            ui->sevLLRemoveButton->setDisabled(true);
        }

        RebuildSevEnable(enable_hl, enable_ll); // again
    }
    else
    {
        x2m->RemoveAllSevHL();
        x2m->RemoveAllCustomSevAudio(false);
        x2m->RemoveAllSevLL();

        // Clear comboboxes too
        prev_sevhl_cost_index = prev_sevll_cost_index = -1;
        prev_sevll_char_index = prev_sevll_ev_index = prev_sevll_ev_ent_index = -1;
        ui->sevHLCostComboBox->clear();
        ui->sevLLCostComboBox->clear();
        ui->sevLLCharComboBox->clear();
        ui->sevLLEventsComboBox->clear();
        ui->sevLLEvEntriesComboBox->clear();

        // Put dummy entries to gui
        SevEntryHL sev_hl;
        SevEntryLL sev_ll(true);

        SevEntryHLToGui(sev_hl);
        SevEntryLLToGui(sev_ll);
    }
}

void MainWindow::on_sevModeComboBox_currentIndexChanged(int index)
{
    if (index == 0)
        RebuildSevEnable(true, false);
    else
        RebuildSevEnable(false, true);
}

void MainWindow::on_sevHLCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumSevHLEntries())
        return;

    if (prev_sevhl_cost_index >= 0 && prev_sevhl_cost_index < x2m->GetNumSevHLEntries())
    {
        SevEntryHL &entry = x2m->GetSevHLEntry(prev_sevhl_cost_index);
        GuiToSevEntryHL(entry);
    }

    const SevEntryHL &entry = x2m->GetSevHLEntry(index);
    SevEntryHLToGui(entry);

    prev_sevhl_cost_index = index;
}

void MainWindow::on_sevHLAddButton_clicked()
{
    SevEntryHL entry;

    int idx = ui->sevHLCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSevHLEntries())
    {
        GuiToSevEntryHL(entry);
    }

    size_t pos = x2m->AddSevHLEntry(entry);

    ui->sevHLCostComboBox->addItem(QString("Entry %1").arg(pos));
    ui->sevHLCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumSevHLEntries() > 1)
        ui->sevHLRemoveButton->setEnabled(true);
}

void MainWindow::on_sevHLRemoveButton_clicked()
{
    int index = ui->sevHLCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSevHLEntries())
        return;

    x2m->RemoveSevHLEntry(index);

    if (prev_sevhl_cost_index > index)
        prev_sevhl_cost_index--;
    else
        prev_sevhl_cost_index = -1;

    ui->sevHLCostComboBox->removeItem(index);

    for (int i = 0; i < ui->sevHLCostComboBox->count(); i++)
    {
        ui->sevHLCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumSevHLEntries() == 1)
        ui->sevHLRemoveButton->setDisabled(true);
}

void MainWindow::on_sevLLCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumSevLLEntries())
        return;

    if (prev_sevll_cost_index >= 0 && prev_sevll_cost_index < x2m->GetNumSevLLEntries())
    {
        SevEntryLL &entry = x2m->GetSevLLEntry(prev_sevll_cost_index);
        GuiToSevEntryLL(entry);
    }

    const SevEntryLL &entry = x2m->GetSevLLEntry(index);
    SevEntryLLToGui(entry);

    prev_sevll_cost_index = index;
}

void MainWindow::on_sevLLAddButton_clicked()
{
    SevEntryLL entry(true);

    int idx = ui->sevLLCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSevLLEntries())
    {
        entry.sub_entries = x2m->GetSevLLEntry(idx).sub_entries;
        GuiToSevEntryLL(entry);
    }

    size_t pos = x2m->AddSevLLEntry(entry);

    ui->sevLLCostComboBox->addItem(QString("Entry %1").arg(pos));
    ui->sevLLCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumSevLLEntries() > 1)
        ui->sevLLRemoveButton->setEnabled(true);
}

void MainWindow::on_sevLLRemoveButton_clicked()
{
    int index = ui->sevLLCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSevLLEntries())
        return;

    x2m->RemoveSevLLEntry(index);

    if (prev_sevll_cost_index > index)
        prev_sevll_cost_index--;
    else
        prev_sevll_cost_index = -1;

    ui->sevLLCostComboBox->removeItem(index);

    for (int i = 0; i < ui->sevLLCostComboBox->count(); i++)
    {
        ui->sevLLCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumSevLLEntries() == 1)
        ui->sevLLRemoveButton->setDisabled(true);
}

void MainWindow::on_sevLLCharComboBox_currentIndexChanged(int index)
{
    int sevll_index = ui->sevLLCostComboBox->currentIndex();

    if (sevll_index < 0 || sevll_index >= x2m->GetNumSevLLEntries())
        return;

    SevEntryLL &entry = x2m->GetSevLLEntry(sevll_index);

    if (index < 0 || index >= entry.sub_entries.size())
        return;

    if (prev_sevll_char_index >= 0 && prev_sevll_char_index < entry.sub_entries.size())
    {
        SevEventsLL &sub_entry = entry.sub_entries[prev_sevll_char_index];
        GuiToSevEventsLL(sub_entry);
    }

    const SevEventsLL &sub_entry = entry.sub_entries[index];
    SevEventsLLToGui(sub_entry);

    prev_sevll_char_index = index;
}

void MainWindow::on_sevLLCharAddButton_clicked()
{
    SevEventsLL sub_entry;
    sub_entry.events.resize(1);
    sub_entry.events[0].entries.resize(1);

    int idx = ui->sevLLCostComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumSevLLEntries())
    {
        return;
    }

    SevEntryLL &entry = x2m->GetSevLLEntry(idx);

    idx = ui->sevLLCharComboBox->currentIndex();
    if (idx >= 0 && idx < entry.sub_entries.size())
    {
        sub_entry.events = entry.sub_entries[idx].events;
        GuiToSevEventsLL(sub_entry);
    }

    size_t pos = entry.sub_entries.size();
    entry.sub_entries.push_back(sub_entry);

    ui->sevLLCharComboBox->addItem(QString("Character %1").arg(pos));
    ui->sevLLCharComboBox->setCurrentIndex((int)pos);

    if (entry.sub_entries.size() > 1)
        ui->sevLLCharRemoveButton->setEnabled(true);
}

void MainWindow::on_sevLLCharRemoveButton_clicked()
{
    int index = ui->sevLLCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSevLLEntries())
        return;

    SevEntryLL &entry = x2m->GetSevLLEntry(index);

    index = ui->sevLLCharComboBox->currentIndex();

    if (index < 0 || index >= entry.sub_entries.size())
        return;

    entry.sub_entries.erase(entry.sub_entries.begin()+index);

    if (prev_sevll_char_index > index)
        prev_sevll_char_index--;
    else
        prev_sevll_char_index = -1;

    ui->sevLLCharComboBox->removeItem(index);

    for (int i = 0; i < ui->sevLLCharComboBox->count(); i++)
    {
        ui->sevLLCharComboBox->setItemText(i, QString("Character %1").arg(i));
    }

    if (entry.sub_entries.size() == 1)
        ui->sevLLCharRemoveButton->setDisabled(true);
}

void MainWindow::on_sevLLEventsComboBox_currentIndexChanged(int index)
{
    int sevll_index = ui->sevLLCostComboBox->currentIndex();

    if (sevll_index < 0 || sevll_index >= x2m->GetNumSevLLEntries())
        return;

    SevEntryLL &entry = x2m->GetSevLLEntry(sevll_index);

    int sevll_char_index = ui->sevLLCharComboBox->currentIndex();

    if (sevll_char_index < 0 || sevll_char_index >= entry.sub_entries.size())
        return;

    SevEventsLL &sub_entry = entry.sub_entries[sevll_char_index];

    if (index < 0 || index >= sub_entry.events.size())
        return;

    if (prev_sevll_ev_index >= 0 && prev_sevll_ev_index < sub_entry.events.size())
    {
        SevEvent &event = sub_entry.events[prev_sevll_ev_index];
        GuiToSevEvent(event);
    }

    const SevEvent &event = sub_entry.events[index];
    SevEventToGui(event);

    prev_sevll_ev_index = index;
}

void MainWindow::on_sevLLEventsAddButton_clicked()
{
    SevEvent event;
    event.entries.resize(1);

    int idx = ui->sevLLCostComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumSevLLEntries())
    {
        return;
    }

    SevEntryLL &entry = x2m->GetSevLLEntry(idx);

    idx = ui->sevLLCharComboBox->currentIndex();
    if (idx < 0 || idx >= entry.sub_entries.size())
    {
        return;
    }

    SevEventsLL &sub_entry = entry.sub_entries[idx];

    idx = ui->sevLLEventsComboBox->currentIndex();
    if (idx >= 0 && idx < sub_entry.events.size())
    {
        event.entries = sub_entry.events[idx].entries;
        GuiToSevEvent(event);
    }

    size_t pos = sub_entry.events.size();
    sub_entry.events.push_back(event);

    ui->sevLLEventsComboBox->addItem(QString("Event %1").arg(pos));
    ui->sevLLEventsComboBox->setCurrentIndex((int)pos);

    if (sub_entry.events.size() > 1)
        ui->sevLLEventsRemoveButton->setEnabled(true);
}

void MainWindow::on_sevLLEventsRemoveButton_clicked()
{
    int index = ui->sevLLCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSevLLEntries())
        return;

    SevEntryLL &entry = x2m->GetSevLLEntry(index);

    index = ui->sevLLCharComboBox->currentIndex();

    if (index < 0 || index >= entry.sub_entries.size())
        return;

    SevEventsLL &sub_entry = entry.sub_entries[index];

    index = ui->sevLLEventsComboBox->currentIndex();

    if (index < 0 || index >= sub_entry.events.size())
        return;

    sub_entry.events.erase(sub_entry.events.begin()+index);

    if (prev_sevll_ev_index > index)
        prev_sevll_ev_index--;
    else
        prev_sevll_ev_index = -1;

    ui->sevLLEventsComboBox->removeItem(index);

    for (int i = 0; i < ui->sevLLEventsComboBox->count(); i++)
    {
        ui->sevLLEventsComboBox->setItemText(i, QString("Event %1").arg(i));
    }

    if (sub_entry.events.size() == 1)
        ui->sevLLEventsRemoveButton->setDisabled(true);
}

void MainWindow::on_sevLLEvEntriesComboBox_currentIndexChanged(int index)
{
    int sevll_index = ui->sevLLCostComboBox->currentIndex();

    if (sevll_index < 0 || sevll_index >= x2m->GetNumSevLLEntries())
        return;

    SevEntryLL &entry = x2m->GetSevLLEntry(sevll_index);

    int sevll_char_index = ui->sevLLCharComboBox->currentIndex();

    if (sevll_char_index < 0 || sevll_char_index >= entry.sub_entries.size())
        return;

    SevEventsLL &sub_entry = entry.sub_entries[sevll_char_index];

    int sevll_ev_index = ui->sevLLEventsComboBox->currentIndex();

    if (sevll_ev_index < 0 || sevll_ev_index >= sub_entry.events.size())
        return;

    SevEvent &event = sub_entry.events[sevll_ev_index];

    if (index < 0 || index >= event.entries.size())
        return;

    if (prev_sevll_ev_ent_index >= 0 && prev_sevll_ev_ent_index < event.entries.size())
    {
        SevEventEntry &ev_entry = event.entries[prev_sevll_ev_ent_index];
        GuiToSevEventEntry(ev_entry);
    }

    const SevEventEntry &ev_entry = event.entries[index];
    SevEventEntryToGui(ev_entry);

    prev_sevll_ev_ent_index = index;
}

void MainWindow::on_sevLLEvEntriesAddButton_clicked()
{
    SevEventEntry ev_entry;

    int idx = ui->sevLLCostComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumSevLLEntries())
    {
        return;
    }

    SevEntryLL &entry = x2m->GetSevLLEntry(idx);

    idx = ui->sevLLCharComboBox->currentIndex();
    if (idx < 0 || idx >= entry.sub_entries.size())
    {
        return;
    }

    SevEventsLL &sub_entry = entry.sub_entries[idx];

    idx = ui->sevLLEventsComboBox->currentIndex();
    if (idx < 0 || idx >= sub_entry.events.size())
    {
        return;
    }

    SevEvent &event = sub_entry.events[idx];

    idx = ui->sevLLEvEntriesComboBox->currentIndex();
    if (idx >= 0 && idx < event.entries.size())
    {
        GuiToSevEventEntry(ev_entry);
    }

    size_t pos = event.entries.size();
    event.entries.push_back(ev_entry);

    ui->sevLLEvEntriesComboBox->addItem(QString("Event entry %1").arg(pos));
    ui->sevLLEvEntriesComboBox->setCurrentIndex((int)pos);

    if (event.entries.size() > 1)
        ui->sevLLEvEntriesRemoveButton->setEnabled(true);
}

void MainWindow::on_sevLLEvEntriesRemoveButton_clicked()
{
    int index = ui->sevLLCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSevLLEntries())
        return;

    SevEntryLL &entry = x2m->GetSevLLEntry(index);

    index = ui->sevLLCharComboBox->currentIndex();

    if (index < 0 || index >= entry.sub_entries.size())
        return;

    SevEventsLL &sub_entry = entry.sub_entries[index];

    index = ui->sevLLEventsComboBox->currentIndex();

    if (index < 0 || index >= sub_entry.events.size())
        return;

    SevEvent &event = sub_entry.events[index];

    index = ui->sevLLEvEntriesComboBox->currentIndex();

    if (index < 0 || index >= event.entries.size())
        return;

    event.entries.erase(event.entries.begin()+index);

    if (prev_sevll_ev_ent_index > index)
        prev_sevll_ev_ent_index--;
    else
        prev_sevll_ev_ent_index = -1;

    ui->sevLLEvEntriesComboBox->removeItem(index);

    for (int i = 0; i < ui->sevLLEvEntriesComboBox->count(); i++)
    {
        ui->sevLLEvEntriesComboBox->setItemText(i, QString("Event entry %1").arg(i));
    }

    if (event.entries.size() == 1)
        ui->sevLLEvEntriesRemoveButton->setDisabled(true);
}

void MainWindow::on_sevLLCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    SevFile *sev = nullptr;
    bool ignore_mods = true;

    if (arg1 == ui->actionFromGameSev)
    {
        dialog = new ListDialog(ListMode::SEV, this);
        sev = game_sev;
    }
    else if (arg1 == ui->actionFromExternalSev)
    {
        QString file = QFileDialog::getOpenFileName(this, "External SEV", config.lf_external_sev, "SEV Files (*.sev *.sev.xml)");

        if (file.isNull())
            return;

        config.lf_external_sev = file;

        sev = new SevFile();
        if (!sev->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete sev;
            return;
        }

        dialog = new ListDialog(ListMode::SEV, this, sev);
        ignore_mods = false;
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < sev->GetNumEntries())
            {
                const SevEntry &entry_native = (*sev)[idx];
                CmsEntry *cms_entry = game_cms->FindEntryByID(entry_native.char_id);

                idx = ui->sevLLCostComboBox->currentIndex();

                if (idx >= 0 && idx < x2m->GetNumSevLLEntries() && entry_native.chars_events.size() != 0 && cms_entry)
                {
                    SevEntryLL &entry = x2m->GetSevLLEntry(idx);

                    entry.sub_entries.clear();
                    // Clear comboboxes too
                    prev_sevll_char_index = prev_sevll_ev_index = prev_sevll_ev_ent_index = -1;
                    ui->sevLLCharComboBox->clear();
                    ui->sevLLEventsComboBox->clear();
                    ui->sevLLEvEntriesComboBox->clear();

                    for (const SevCharEvents &cevents : entry_native.chars_events)
                    {
                        SevEventsLL cev;
                        CmsEntry *cms_entry_this = game_cms->FindEntryByID(cevents.char_id);

                        if (cms_entry_this)
                        {
                            if (ignore_mods && cevents.char_id >= XV2_FREE_ID_SEARCH_START)
                                continue;

                            cev.char_code = cms_entry_this->name;
                        }
                        else
                        {
                            if (cevents.char_id != 0xFFFFFFFF)
                                continue;

                            cev.char_code.clear();
                        }

                        cev.costume_id = cevents.costume_id;
                        cev.events = cevents.events;
                        cev.im_source = true;
                        entry.sub_entries.push_back(cev);
                    }

                    for (const SevEntry &o_entry : *sev)
                    {
                        CmsEntry *cms_entry_this = game_cms->FindEntryByID(o_entry.char_id);
                        if (!cms_entry_this)
                            continue;

                        if (ignore_mods && o_entry.char_id >= XV2_FREE_ID_SEARCH_START)
                            continue;

                        for (const SevCharEvents &cevents : o_entry.chars_events)
                        {
                            if (cevents.char_id == entry_native.char_id && cevents.costume_id == entry_native.costume_id)
                            {
                                SevEventsLL cev;

                                cev.char_code = cms_entry_this->name;
                                cev.costume_id = o_entry.costume_id;
                                cev.events = cevents.events;
                                cev.im_source = false;
                                entry.sub_entries.push_back(cev);
                            }
                        }
                    }

                    SevEntryLLToGui(entry);
                    RebuildSevEnable(false, true);
                }
            }
        }

        delete dialog;

        if (sev != game_sev)
            delete sev;
    }
}

void MainWindow::CmlEntryToGui(const CmlEntry &entry)
{
    ui->cmlU04Edit->setText(QString("%1").arg((int32_t)entry.unk_04));
    ui->cmlPosXEdit->setText(QString("%1").arg(entry.css_pos[0]));
    ui->cmlPosYEdit->setText(QString("%1").arg(entry.css_pos[1]));
    ui->cmlPosZEdit->setText(QString("%1").arg(entry.css_pos[2]));
    ui->cmlRotXEdit->setText(QString("%1").arg(entry.css_rot[0]));
    ui->cmlRotYEdit->setText(QString("%1").arg(entry.css_rot[1]));
    ui->cmlF0CEdit->setText(QString("%1").arg(entry.unk_0C));
    ui->cmlF10Edit->setText(QString("%1").arg(entry.unk_10));
    ui->cmlF14Edit->setText(QString("%1").arg(entry.unk_14));
    ui->cmlF18Edit->setText(QString("%1").arg(entry.unk_18));
    ui->cmlF1CEdit->setText(QString("%1").arg(entry.unk_1C));
    ui->cmlF20Edit->setText(QString("%1").arg(entry.unk_20));
    ui->cmlF24Edit->setText(QString("%1").arg(entry.unk_24));
    ui->cmlF28Edit->setText(QString("%1").arg(entry.unk_28));
    ui->cmlF2CEdit->setText(QString("%1").arg(entry.unk_2C));
    ui->cmlF30Edit->setText(QString("%1").arg(entry.unk_30));
    ui->cmlF34Edit->setText(QString("%1").arg(entry.unk_34));
    ui->cmlF38Edit->setText(QString("%1").arg(entry.unk_38));
    ui->cmlF3CEdit->setText(QString("%1").arg(entry.unk_3C));
    ui->cmlF40Edit->setText(QString("%1").arg(entry.unk_40));
    ui->cmlF44Edit->setText(QString("%1").arg(entry.unk_44));
    ui->cmlF48Edit->setText(QString("%1").arg(entry.unk_48));
    ui->cmlF5CEdit->setText(QString("%1").arg(entry.unk_5C));
    ui->cmlF60Edit->setText(QString("%1").arg(entry.unk_60));
    ui->cmlF64Edit->setText(QString("%1").arg(entry.unk_64));
    ui->cmlF68Edit->setText(QString("%1").arg(entry.unk_68));
    ui->cmlFN10Edit->setText(QString("%1").arg(entry.neo_unk_10));
}

void MainWindow::GuiToCmlEntry(CmlEntry &entry)
{
    entry.unk_04 = (uint32_t) ui->cmlU04Edit->text().toInt();
    entry.css_pos[0] = ui->cmlPosXEdit->text().toFloat();
    entry.css_pos[1] = ui->cmlPosYEdit->text().toFloat();
    entry.css_pos[2] = ui->cmlPosZEdit->text().toFloat();
    entry.css_rot[0] = ui->cmlRotXEdit->text().toFloat();
    entry.css_rot[1] = ui->cmlRotYEdit->text().toFloat();
    entry.unk_0C = ui->cmlF0CEdit->text().toFloat();
    entry.unk_10 = ui->cmlF10Edit->text().toFloat();
    entry.unk_14 = ui->cmlF14Edit->text().toFloat();
    entry.unk_18 = ui->cmlF18Edit->text().toFloat();
    entry.unk_1C = ui->cmlF1CEdit->text().toFloat();
    entry.unk_20 = ui->cmlF20Edit->text().toFloat();
    entry.unk_24 = ui->cmlF24Edit->text().toFloat();
    entry.unk_28 = ui->cmlF28Edit->text().toFloat();
    entry.unk_2C = ui->cmlF2CEdit->text().toFloat();
    entry.unk_30 = ui->cmlF30Edit->text().toFloat();
    entry.unk_34 = ui->cmlF34Edit->text().toFloat();
    entry.unk_38 = ui->cmlF38Edit->text().toFloat();
    entry.unk_3C = ui->cmlF3CEdit->text().toFloat();
    entry.unk_40 = ui->cmlF40Edit->text().toFloat();
    entry.unk_44 = ui->cmlF44Edit->text().toFloat();
    entry.unk_48 = ui->cmlF48Edit->text().toFloat();
    entry.unk_5C = ui->cmlF5CEdit->text().toFloat();
    entry.unk_60 = ui->cmlF60Edit->text().toFloat();
    entry.unk_64 = ui->cmlF64Edit->text().toFloat();
    entry.unk_68 = ui->cmlF68Edit->text().toFloat();
    entry.neo_unk_10 = ui->cmlFN10Edit->text().toFloat();
}

void MainWindow::on_cmlCheck_clicked()
{
    bool checked = ui->cmlCheck->isChecked();

    ui->cmlU04Edit->setEnabled(checked);
    ui->cmlPosXEdit->setEnabled(checked);
    ui->cmlPosYEdit->setEnabled(checked);
    ui->cmlPosZEdit->setEnabled(checked);
    ui->cmlRotXEdit->setEnabled(checked);
    ui->cmlRotYEdit->setEnabled(checked);
    ui->cmlF0CEdit->setEnabled(checked);
    ui->cmlF10Edit->setEnabled(checked);
    ui->cmlF14Edit->setEnabled(checked);
    ui->cmlF18Edit->setEnabled(checked);
    ui->cmlF1CEdit->setEnabled(checked);
    ui->cmlF20Edit->setEnabled(checked);
    ui->cmlF24Edit->setEnabled(checked);
    ui->cmlF28Edit->setEnabled(checked);
    ui->cmlF2CEdit->setEnabled(checked);
    ui->cmlF30Edit->setEnabled(checked);
    ui->cmlF34Edit->setEnabled(checked);
    ui->cmlF38Edit->setEnabled(checked);
    ui->cmlF3CEdit->setEnabled(checked);
    ui->cmlF40Edit->setEnabled(checked);
    ui->cmlF44Edit->setEnabled(checked);
    ui->cmlF48Edit->setEnabled(checked);
    ui->cmlF5CEdit->setEnabled(checked);
    ui->cmlF60Edit->setEnabled(checked);
    ui->cmlF64Edit->setEnabled(checked);
    ui->cmlF68Edit->setEnabled(checked);
    ui->cmlFN10Edit->setEnabled(checked);
    ui->cmlAddButton->setEnabled(checked);
    ui->cmlRemoveButton->setEnabled(checked);
    ui->cmlCopyButton->setEnabled(checked);
    ui->cmlCostComboBox->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_cml_cost_index = -1;
        ui->cmlCostComboBox->clear();

        size_t num_cml_entries = x2m->GetNumCmlEntries();

        if (num_cml_entries == 0)
        {
            CmlEntry entry;

            GuiToCmlEntry(entry);
            x2m->AddCmlEntry(entry);
            num_cml_entries++;
        }

        for (size_t i = 0; i < num_cml_entries; i++)
        {
            ui->cmlCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_cml_entries == 1)
        {
            ui->cmlRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumCmlEntries() != 0)
            x2m->RemoveCmlEntry(0);
    }
}

void MainWindow::on_cmlCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumCmlEntries())
        return;

    if (prev_cml_cost_index >= 0 && prev_cml_cost_index < x2m->GetNumCmlEntries())
    {
        CmlEntry &entry = x2m->GetCmlEntry(prev_cml_cost_index);
        GuiToCmlEntry(entry);
    }

    const CmlEntry &entry = x2m->GetCmlEntry(index);
    CmlEntryToGui(entry);

    prev_cml_cost_index = index;
}

void MainWindow::on_cmlAddButton_clicked()
{
    CmlEntry entry;

    int idx = ui->cmlCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumCmlEntries())
    {
        GuiToCmlEntry(entry);
    }

    size_t pos = x2m->AddCmlEntry(entry);

    ui->cmlCostComboBox->addItem(QString("Entry %1").arg(pos));

    if (x2m->GetNumCmlEntries() == XV2_MAX_SUBSLOTS)
        ui->cmlAddButton->setDisabled(true);

    ui->cmlCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumCmlEntries() > 1)
        ui->cmlRemoveButton->setEnabled(true);
}

void MainWindow::on_cmlRemoveButton_clicked()
{
    int index = ui->cmlCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumCmlEntries())
        return;

    x2m->RemoveCmlEntry(index);

    if (prev_cml_cost_index > index)
        prev_cml_cost_index--;
    else
        prev_cml_cost_index = -1;

    ui->cmlCostComboBox->removeItem(index);

    for (int i = 0; i < ui->cmlCostComboBox->count(); i++)
    {
        ui->cmlCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumCmlEntries() == 1)
        ui->cmlRemoveButton->setDisabled(true);

    else if (x2m->GetNumCmlEntries() < XV2_MAX_SUBSLOTS)
        ui->cmlAddButton->setEnabled(true);
}

void MainWindow::on_cmlCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    CmlFile *cml = nullptr;

    if (arg1 == ui->actionFromGameCml)
    {
        dialog = new ListDialog(ListMode::CML, this);
        cml = game_cml;
    }
    else if (arg1 == ui->actionFromExternalCml)
    {
        QString file = QFileDialog::getOpenFileName(this, "External CML", config.lf_external_cml, "CML Files (*.cml *.cml.xml)");

        if (file.isNull())
            return;

        config.lf_external_cml = file;

        cml = new CmlFile();
        if (!cml->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete cml;
            return;
        }

        dialog = new ListDialog(ListMode::CML, this, cml);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < cml->GetNumEntries())
            {
                const CmlEntry &entry = (*cml)[idx];
                CmlEntryToGui(entry);
            }
        }

        delete dialog;

        if (cml != game_cml)
            delete cml;
    }
}

void MainWindow::HciEntryToGui(const HciEntry &entry)
{
    ui->hciCostumeIndexEdit->setText(QString("%1").arg((int16_t)entry.costume_id));
    ui->hciState1Edit->setText(QString("%1").arg((int16_t)entry.state1));
    ui->hciState2Edit->setText(QString("%1").arg((int16_t)entry.state2));
    ui->hciEmbIndexEdit->setText(QString("%1").arg((int32_t)entry.emb_index));
}

void MainWindow::GuiToHciEntry(HciEntry &entry)
{
    entry.costume_id = (uint16_t) ui->hciCostumeIndexEdit->text().toInt();
    entry.state1 = (uint16_t) ui->hciState1Edit->text().toInt();
    entry.state2 = (uint16_t) ui->hciState2Edit->text().toInt();
    entry.emb_index = (uint32_t) ui->hciEmbIndexEdit->text().toInt();
}

void MainWindow::on_hciCheck_clicked()
{
    bool checked = ui->hciCheck->isChecked();

    ui->hciAddButton->setEnabled(checked);
    ui->hciCopyButton->setEnabled(checked);
    ui->hciCostumeIndexEdit->setEnabled(checked);
    ui->hciEmbIndexEdit->setEnabled(checked);
    ui->hciEntryComboBox->setEnabled(checked);
    ui->hciGetEmbIndexButton->setEnabled(checked);
    ui->hciRemoveButton->setEnabled(checked);
    ui->hciState1Edit->setEnabled(checked);
    ui->hciState2Edit->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_hci_ent_index = -1;
        ui->hciEntryComboBox->clear();

        size_t num_hci_entries = x2m->GetNumHciEntries();

        if (num_hci_entries == 0)
        {
            HciEntry entry;

            GuiToHciEntry(entry);
            x2m->AddHciEntry(entry);
            num_hci_entries++;
        }

        for (size_t i = 0; i < num_hci_entries; i++)
        {
            ui->hciEntryComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_hci_entries == 1)
        {
            ui->hciRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumHciEntries() != 0)
            x2m->RemoveHciEntry(0);
    }
}

void MainWindow::on_hciEntryComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumHciEntries())
        return;

    if (prev_hci_ent_index >= 0 && prev_hci_ent_index < x2m->GetNumHciEntries())
    {
        HciEntry &entry = x2m->GetHciEntry(prev_hci_ent_index);
        GuiToHciEntry(entry);
    }

    const HciEntry &entry = x2m->GetHciEntry(index);
    HciEntryToGui(entry);

    prev_hci_ent_index = index;
}

void MainWindow::on_hciAddButton_clicked()
{
    HciEntry entry;

    int idx = ui->hciEntryComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumHciEntries())
    {
        GuiToHciEntry(entry);
    }

    size_t pos = x2m->AddHciEntry(entry);

    ui->hciEntryComboBox->addItem(QString("Entry %1").arg(pos));
    ui->hciEntryComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumHciEntries() > 1)
        ui->hciRemoveButton->setEnabled(true);
}

void MainWindow::on_hciRemoveButton_clicked()
{
    int index = ui->hciEntryComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumHciEntries())
        return;

    x2m->RemoveHciEntry(index);

    if (prev_hci_ent_index > index)
        prev_hci_ent_index--;
    else
        prev_hci_ent_index = -1;

    ui->hciEntryComboBox->removeItem(index);

    for (int i = 0; i < ui->hciEntryComboBox->count(); i++)
    {
        ui->hciEntryComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumHciEntries() == 1)
        ui->hciRemoveButton->setDisabled(true);

    else if (x2m->GetNumHciEntries() < XV2_MAX_SUBSLOTS)
        ui->hciAddButton->setEnabled(true);
}

void MainWindow::on_hciCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    HciFile *hci = nullptr;

    if (arg1 == ui->actionFromGameHci)
    {
        dialog = new ListDialog(ListMode::HCI, this);
        hci = game_hci;
    }
    else if (arg1 == ui->actionFromExternalHci)
    {
        QString file = QFileDialog::getOpenFileName(this, "External HCI", config.lf_external_hci, "HCI Files (*.hci *.hci.xml)");

        if (file.isNull())
            return;

        config.lf_external_hci = file;

        hci = new HciFile();
        if (!hci->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete hci;
            return;
        }

        dialog = new ListDialog(ListMode::HCI, this, hci);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            QString str = dialog->GetResult();

            if (str.length() > 3)
            {
                std::string code = Utils::QStringToStdString(str.left(3));
                CmsEntry *cms_entry = game_cms->FindEntryByName(code);

                if (cms_entry)
                {
                    std::vector<HciEntry *> hci_entries_;
                    std::vector<HciEntry> hci_entries;

                    hci->FindEntriesByCharID(cms_entry->id, hci_entries_);

                    for (size_t i = 0; i < hci_entries_.size(); i++)
                    {
                        if (std::find(hci_entries.begin(), hci_entries.end(), *(hci_entries_[i])) == hci_entries.end())
                        {
                            hci_entries.push_back(*(hci_entries_[i]));
                        }
                    }

                    if (hci_entries.size() > 0)
                    {
                        while (x2m->GetNumHciEntries() != 0)
                            x2m->RemoveHciEntry(0);

                        prev_hci_ent_index = -1;
                        ui->hciEntryComboBox->clear();

                        for (size_t i = 0; i < hci_entries.size(); i++)
                        {
                            x2m->AddHciEntry(hci_entries[i]);
                            ui->hciEntryComboBox->addItem(QString("Entry %1").arg(i));
                        }

                        ui->hciRemoveButton->setEnabled(x2m->GetNumHciEntries() > 1);
                    }
                }
            }
        }

        delete dialog;

        if (hci != game_hci)
            delete hci;
    }
}

void MainWindow::on_hciGetEmbIndexButton_clicked()
{
    EmbFile emb;

    QString btl_portrait = ui->btlPortEdit->text().trimmed();

    if (btl_portrait.isEmpty())
    {
        DPRINTF("You haven't set yet a battle portrait.\n");
        return;
    }

    if (btl_portrait == INTERNAL_DATA)
    {
        size_t size;
        uint8_t *buf = x2m->ReadFile(X2M_BTL_PORTRAIT, &size);

        if (!buf)
            return;

        bool ret = emb.Load(buf, size);
        delete[] buf;

        if (!ret)
            return;
    }
    else if (btl_portrait.startsWith(GAME_PREFIX))
    {
        QString file = btl_portrait.mid((int)strlen(GAME_PREFIX));

        if (!file.endsWith(".emb", Qt::CaseInsensitive))
        {
            DPRINTF("%s: Bug.\n", FUNCNAME);
            return;
        }

        if (!xv2fs->LoadFile(&emb, "data/ui/texture/" + Utils::QStringToStdString(file)))
            return;
    }
    else
    {
        if (!emb.SmartLoad(Utils::QStringToStdString(btl_portrait)))
            return;
    }

    EmbDialog dialog(&emb, this);

    if (dialog.exec())
    {
        ui->hciEmbIndexEdit->setText(QString("%1").arg(dialog.GetResultInteger()));
    }
}

void MainWindow::PlayAudio(HcaFile *hca)
{
    static QBuffer *audio_buffer = nullptr;
    static QAudioOutput *audio_output = nullptr;

    if (audio_playing)
    {
        audio_output->reset();
        audio_playing = false;
    }

    int bits;
    size_t size;

    uint8_t *buf = hca->Decode(&bits, &size);
    if (!buf)
    {
        DPRINTF("%s: hca decode failed.\n", FUNCNAME);
        return;
    }

    QAudioFormat format;

    format.setSampleRate(hca->GetSampleRate());
    format.setChannelCount(hca->GetNumChannels());
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);

    if (bits == AUDIO_FORMAT_FLOAT)
    {
        format.setSampleSize(32);
        format.setSampleType(QAudioFormat::Float);
    }
    else
    {
        format.setSampleSize(bits);
        format.setSampleType(QAudioFormat::SignedInt);
    }

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
    {
        DPRINTF("Cannot play that format.\n");
        delete[] buf;
        return;
    }

    if (!audio_buffer)
    {
        audio_buffer = new QBuffer();
    }
    else
    {
        delete audio_buffer;
        audio_buffer = new QBuffer();
    }

    audio_buffer->setData((const char *)buf, (int)size);
    delete[] buf;

    audio_buffer->open(QIODevice::ReadOnly);

    if (!audio_output)
    {
        audio_output = new QAudioOutput(format, this);
    }
    else
    {
        delete audio_output;
        audio_output = new QAudioOutput(format, this);
    }

    audio_playing = true;
    audio_output->start(audio_buffer);
}

void MainWindow::PlayAudio(AcbFile *acb, AwbFile *awb, uint32_t cue_id)
{
    uint32_t cue_idx = acb->CueIdToCueIndex(cue_id);
    if (cue_idx == (uint32_t)-1)
    {
        DPRINTF("%s: CueIdToCueIndex failed.\n", FUNCNAME);
        return;
    }

    bool external;
    uint32_t awb_idx = acb->CueIndexToAwbIndex(cue_idx, &external);

    if (awb_idx == (uint32_t)-1)
    {
        DPRINTF("%s: CueIndexToAwbIndex failed.\n", FUNCNAME);
        return;
    }

    Afs2File internal_awb;

    if (!external)
    {
        uint32_t awb_size;
        uint8_t *awb_buf = acb->GetAwb(&awb_size);

        if (!awb_buf)
        {
            DPRINTF("%s internal awb couldn't be extracted.\n", FUNCNAME);
            return;
        }

        if (!internal_awb.Load(awb_buf, awb_size))
        {
            DPRINTF("%s: Failed to load internal awb.\n", FUNCNAME);
            return;
        }

        awb = &internal_awb;
    }

    uint64_t size;
    uint8_t *hca_buf = awb->ExtractFile(awb_idx, &size);

    if (!hca_buf)
    {
        DPRINTF("%s: AwbFile::ExtractFile failed.\n", FUNCNAME);
        return;
    }

    HcaFile hca;

    if (!hca.Load(hca_buf, size))
    {
        DPRINTF("%s: HcaFile::Load failed.\n", FUNCNAME);
        return;
    }

    delete[] hca_buf;
    PlayAudio(&hca);
}

void MainWindow::PlayCssAudio(QLineEdit *cue_id_edit, QCheckBox *custom_check, QComboBox *custom_combobox)
{
    bool japanese = (ui->voiceListenLangComboBox->currentIndex() == 0);

    if (custom_check->isChecked())
    {
        HcaFile *hca = x2m->LoadAudioFile(Utils::QStringToStdString(custom_combobox->currentText()), !japanese, true);

        if (!hca)
        {
            DPRINTF("%s: Failed to load hca from inside x2m.\n", FUNCNAME);
            return;
        }

        PlayAudio(hca);
        delete hca;
    }
    else
    {
        bool ok;
        int cue_id = cue_id_edit->text().toInt(&ok);

        if (!ok)
        {
            DPRINTF("Id not specified.\n");
            return;
        }

        if (cue_id < 0)
        {
            DPRINTF("Id less than 0, cannot play.\n");
            return;
        }

        if (!game_css_acb_jp)
        {
            if (!Xenoverse2::InitSound(true))
            {
                DPRINTF("Failed to InitSound\n");
                return;
            }
        }

        PlayAudio((japanese) ? game_css_acb_jp : game_css_acb_en, (japanese) ? game_css_awb_jp : game_css_awb_en, cue_id);
    }
}

void MainWindow::on_voice1ListenButton_clicked()
{
    PlayCssAudio(ui->voice1Edit, ui->voice1Check, ui->voice1ComboBox);
}

void MainWindow::on_voice2ListenButton_clicked()
{
    PlayCssAudio(ui->voice2Edit, ui->voice2Check, ui->voice2ComboBox);
}

void MainWindow::PlaySevAudio(QLineEdit *cue_id_edit, QLineEdit *file_id_edit, QComboBox *custom_cb)
{
    bool japanese = (ui->sevListenLangComboBox->currentIndex() == 0);

    if (custom_cb->currentIndex() != 0)
    {
        HcaFile *hca = x2m->LoadAudioFile(Utils::QStringToStdString(custom_cb->currentText()), !japanese, true);
        if (!hca)
        {
            DPRINTF("Internal error.\n");
            return;
        }

        PlayAudio(hca);
        delete hca;
        return;
    }

    bool ok;
    int file_id = file_id_edit->text().toInt(&ok);

    if (!ok)
    {
        DPRINTF("File Id not specified.\n");
        return;
    }

    if (file_id != 0 && file_id != 2)
    {
        DPRINTF("Unknown file id.\n");
        return;
    }

    int cue_id = cue_id_edit->text().toInt(&ok);

    if (!ok)
    {
        DPRINTF("Cue Id not specified.\n");
        return;
    }

    if (cue_id < 0)
    {
        DPRINTF("Cue Id < 0, nothing to listen.\n");
        return;
    }

    AcbFile *acb;
    Afs2File *awb;

    if (file_id == 0)
    {
        std::string path = "data/sound/VOX/Battle/Chara/";

        if (!japanese)
            path += "en/";

        if (ui->csoVoxEdit->text().isEmpty())
        {
            DPRINTF("If File Id is 0, VOX field in CSO tab must be filled for listen to work.\n");
            return;
        }

        path += Utils::QStringToStdString(ui->csoVoxEdit->text());

        acb = new AcbFile();
        if (!xv2fs->LoadFile(dynamic_cast<CriwareAudioContainer *>(acb), path + ".acb"))
        {
            DPRINTF("Failed to load acb file: %s.acb\n", path.c_str());
            delete acb;
            return;
        }

        awb = new Afs2File();
        if (!xv2fs->LoadFile(awb, path + ".awb"))
        {
            DPRINTF("Failed to load awb file: %s.awb\n", path.c_str());
            delete acb; delete awb;
            return;
        }
    }
    else // 2
    {
        if (!game_sev_cmn_acb_jp)
        {
            if (!Xenoverse2::InitSound(true))
            {
                DPRINTF("Failed to InitSound\n");
                return;
            }
        }

        uint32_t highest = game_sev_cmn_acb_en->GetHighestCueId();
        if (highest == (uint32_t)-1)
        {
            DPRINTF("Error, invalid highest cue id value.\n");
            return;
        }

        if (cue_id <= (int)highest)
        {
            if (japanese)
            {
                acb = game_sev_cmn_acb_jp;
                awb = game_sev_cmn_awb_jp;
            }
            else
            {
                acb = game_sev_cmn_acb_en;
                awb = game_sev_cmn_awb_en;
            }
        }
        else
        {
            if (japanese)
            {
                acb = game_sev_cmn2_acb_jp;
                awb = game_sev_cmn2_awb_jp;
            }
            else
            {
                acb = game_sev_cmn2_acb_en;
                awb = game_sev_cmn2_awb_en;
            }

            cue_id = cue_id - highest - 1;
        }
    }

    PlayAudio(acb, awb, cue_id);

    if (file_id != 2)
    {
        delete acb;
        delete awb;
    }
}

void MainWindow::PlayTtbAudio(const std::string &event_name, bool japanese)
{
    if (Utils::BeginsWith(event_name, "X2T_"))
    {
        std::string audio_name = event_name.substr(4);
        HcaFile *hca = x2m->LoadAudioFile(audio_name, !japanese, true);
        if (!hca)
        {
            DPRINTF("Internal error.\n");
            return;
        }

        PlayAudio(hca);
        delete hca;
        return;
    }

    std::string cue_name;

    if (!Xenoverse2::GetTtbVoice(event_name, cue_name))
    {
        DPRINTF("Couldn't get voice.\n");
        return;
    }

    if (cue_name.length() == 0)
        cue_name = event_name;

    bool is_sev2;
    uint32_t cue_id = Xenoverse2::GetSevCmnCueId(cue_name, !japanese, &is_sev2, false);

    if (cue_id == (uint32_t)-1)
    {
        DPRINTF("Couldn't get cue id.\n");
        return;
    }

    AcbFile *acb;
    AwbFile *awb;

    if (is_sev2)
    {
        acb = (japanese) ? game_sev_cmn2_acb_jp : game_sev_cmn2_acb_en;
        awb = (japanese) ? game_sev_cmn2_awb_jp : game_sev_cmn2_awb_en;
    }
    else
    {
        acb = (japanese) ? game_sev_cmn_acb_jp : game_sev_cmn_acb_en;
        awb = (japanese) ? game_sev_cmn_awb_jp : game_sev_cmn_awb_en;
    }

    PlayAudio(acb, awb, cue_id);
}

void MainWindow::PlayTtcAudio()
{
    if (entry_name.length() != 3)
    {
        DPRINTF("The character 3-letter code of this char must be known for this functionality to work. (you must set the files directory)\n");
        return;
    }

    QString additional_dir = ui->addDataEdit->text().trimmed();
    if (additional_dir.isEmpty())
    {
        DPRINTF("You must set an additional directory with the required sound and msg files for this functionality to work.\n");
        return;
    }

    QString event_name = ui->ttcLLEventNameEdit->text();
    if (event_name.isEmpty())
    {
        DPRINTF("Event name is empty, nothing to listen.\n");
        return;
    }

    std::string msg_path = "data/msg/qc_" + entry_name + "_voice.msg";

    int lang = ui->ttcLLListenCombo->currentIndex();
    std::string audio_path = "data/sound/VOX/Quest/Common/";

    if (lang == 1)
        audio_path += "en/";

    audio_path += "CAQ_" + entry_name + "_VOX";

    AcbFile acb;
    Afs2File awb;
    MsgFile msg;

    if (additional_dir == INTERNAL_DATA)
    {
        msg_path = Utils::MakePathString(X2M_JUNGLE, msg_path);

        size_t msg_size;
        uint8_t *msg_buf = x2m->ReadFile(msg_path, &msg_size);

        if (!msg_buf)
        {
            DPRINTF("Cannot find file \"%s\" inside x2m.\n", msg_path.c_str());
            return;
        }

        bool ret = msg.Load(msg_buf, msg_size);
        delete[] msg_buf;

        if (!ret)
        {
            DPRINTF("Failed to load msg file \"%s\" inside x2m (file exists but load failed)\n", msg_path.c_str());
            return;
        }

        audio_path = Utils::MakePathString(X2M_JUNGLE, audio_path);

        const std::string acb_path = audio_path + ".acb";
        const std::string awb_path = audio_path + ".awb";

        size_t acb_size;
        uint8_t *acb_buf = x2m->ReadFile(acb_path, &acb_size);

        if (!acb_buf)
        {
            DPRINTF("Cannot find file \"%s\" inside x2m.\n", acb_path.c_str());
            return;
        }

        ret = acb.Load(acb_buf, acb_size);
        delete[] acb_buf;

        if (!ret)
        {
            DPRINTF("Failed to load acb file \"%s\" inside x2m (file exists but load failed)\n", acb_path.c_str());
            return;
        }

        size_t awb_size;
        uint8_t *awb_buf = x2m->ReadFile(awb_path, &awb_size);

        if (!awb_buf)
        {
            DPRINTF("Cannot find file \"%s\" inside x2m.\n", awb_path.c_str());
            return;
        }

        ret = awb.Load(awb_buf, awb_size);
        delete[] awb_buf;

        if (!ret)
        {
            DPRINTF("Failed to load awb file \"%s\" inside x2m (file exists but load failed)\n", awb_path.c_str());
            return;
        }
    }
    else
    {
        msg_path = Utils::MakePathString(Utils::QStringToStdString(additional_dir), msg_path);
        if (!msg.LoadFromFile(msg_path, false))
        {
            DPRINTF("Failed to load msg file \"%s\"\n", msg_path.c_str());
            return;
        }

        audio_path = Utils::MakePathString(Utils::QStringToStdString(additional_dir), audio_path);

        const std::string acb_path = audio_path + ".acb";
        const std::string awb_path = audio_path + ".awb";

        if (!acb.LoadFromFile(acb_path, false))
        {
            DPRINTF("Failed to load acb file \"%s\"\n", acb_path.c_str());
            return;
        }

        if (!awb.LoadFromFile(awb_path, false))
        {
            DPRINTF("Failed to load awb file \"%s\"\n", awb_path.c_str());
            return;
        }
    }

    std::string cue_name;
    const std::string event_name_std = Utils::QStringToStdString(event_name, false);

    MsgEntry *msg_entry = msg.FindEntryByName(event_name_std);
    if (!msg_entry)
    {
        DPRINTF("Cannot find \"%s\" inside the _voice.msg file.\n", event_name_std.c_str());
        return;
    }

    cue_name = msg_entry->lines[0];

    if (Utils::IsEmptyString(cue_name))
        cue_name = event_name_std;

    uint32_t cue_id = acb.FindCueId(cue_name);
    if (cue_id == (uint32_t)-1)
    {
        DPRINTF("Cannot find cue id for cue name \"%s\"\n", cue_name.c_str());
        return;
    }

    PlayAudio(&acb, &awb, cue_id);
}

void MainWindow::FillTtbHLComboBox()
{
    QComboBox *cb = ui->ttbHLCharComboBox;

    struct CharCostumeDef
    {
        std::string code;
        int costume;

        inline bool operator < (const CharCostumeDef &other) const
        {
            if (code == other.code)
                return (costume < other.costume);

            return (code.compare(other.code) < 0);
        }
    };

    std::set<CharCostumeDef> char_set;

    if (!game_ttb)
    {
        if (!Xenoverse2::InitCommonDialogue())
        {
            DPRINTF("Failed to init common dialogue.\n");
            exit(-1);
        }
    }

    for (const TtbEntry &entry : *game_ttb)
    {
        CharCostumeDef cd;

        if (entry.cms_id >= XV2_FREE_ID_SEARCH_START)
            continue;

        CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(entry.cms_id));

        if (cms)
        {
            cd.code = cms->name;

            for (const TtbEvent &event : entry.events)
            {
                cd.costume = (int)event.costume;
                char_set.insert(cd);
            }
        }

        for (const TtbEvent &event : entry.events)
        {
            if (event.cms2_id < XV2_FREE_ID_SEARCH_START)
            {
                cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms2_id));

                if (cms)
                {
                    cd.code = cms->name;
                    cd.costume = (int)event.costume2;
                    char_set.insert(cd);
                }
            }

            if (event.cms3_id < XV2_FREE_ID_SEARCH_START)
            {
                cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms3_id));

                if (cms)
                {
                    cd.code = cms->name;
                    cd.costume = (int)event.costume3;
                    char_set.insert(cd);
                }
            }

            if (event.cms4_id < XV2_FREE_ID_SEARCH_START)
            {
                cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms4_id));

                if (cms)
                {
                    cd.code = cms->name;
                    cd.costume = (int)event.costume4;
                    char_set.insert(cd);
                }
            }

            if (event.cms5_id < XV2_FREE_ID_SEARCH_START)
            {
                cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms5_id));

                if (cms)
                {
                    cd.code = cms->name;
                    cd.costume = (int)event.costume5;
                    char_set.insert(cd);
                }
            }
        }
    }

    for (const CharCostumeDef &def : char_set)
    {
        std::string name, costume_name;

        if (!Xenoverse2::GetCharaName(def.code, name, XV2_LANG_ENGLISH, (def.costume == -1) ? 0 : def.costume))
            name = def.code;
        else
            name = def.code + " - " + name;

        if (def.costume == -1)
        {
            costume_name = "Any";
        }
        else
        {
           if (!Xenoverse2::GetCharaCostumeName(def.code, def.costume, 0, costume_name))
               costume_name = Utils::ToString(def.costume);
        }

        cb->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(costume_name, false),
                    QVariant(Utils::StdStringToQString(def.code) + QString("%1").arg((uint32_t)def.costume)));
    }
}

void MainWindow::TtbEntryHLToGui(const TtbEntryHL &entry)
{
    ui->ttbHLCostIndexEdit->setText(QString("%1").arg((int32_t)entry.costume_id));

    QString search_term = Utils::StdStringToQString(entry.copy_char) + QString("%1").arg(entry.copy_costume);
    int index = ui->ttbHLCharComboBox->findData(QVariant(search_term));

    if (index < 0)
    {
        index = 0;
    }

    ui->ttbHLCharComboBox->setCurrentIndex(index);
}

void MainWindow::GuiToTtbEntryHL(TtbEntryHL &entry)
{
    entry.costume_id = (uint32_t)ui->ttbHLCostIndexEdit->text().toInt();

    QString str = ui->ttbHLCharComboBox->itemData(ui->ttbHLCharComboBox->currentIndex()).toString();

    entry.copy_char = Utils::QStringToStdString(str.left(3));
    entry.copy_costume = str.mid(3).toUInt();
}

void MainWindow::TtbEntryLLToGui(const TtbEntryLL &entry, bool dummy)
{
    ui->ttbLLNumActorsComboBox->setCurrentIndex(entry.num_actors-2);

    if (entry.num_actors == 2)
    {
        ui->ttbLLReverseCheck->setEnabled(true);
        ui->ttbLLReverseCheck->setChecked(entry.reverse_order);
    }
    else
    {
        ui->ttbLLReverseCheck->setEnabled(false);
        ui->ttbLLReverseCheck->setChecked(false);
    }

    ui->ttbLLDummy1Check->setChecked(entry.dummy_order1);
    ui->ttbLLDummy2Check->setChecked(entry.dummy_order2);

    ui->ttbLLTypeComboBox->setCurrentIndex(entry.type);
    ui->ttbLLU44Edit->setText(QString("%1").arg((int)entry.unk_44));
    ui->ttbLLU48Edit->setText(QString("%1").arg((int)entry.unk_48));

    ui->ttbLLActor3Button->setEnabled(entry.num_actors >= 3);
    ui->ttbLLActor4Button->setEnabled(entry.num_actors >= 4);
    ui->ttbLLActor5Button->setEnabled(entry.num_actors >= 5);

    if (dummy)
        return;

    QString text1, text2, text3, text4, text5;

    text1 = Utils::StdStringToQString(entry.cms_code, false);
    if (text1.isEmpty())
        text1 = "Myself";

    text1 += ", " + QString("%1").arg((int)entry.costume) + ", " + QString("%1").arg((int)entry.transformation);
    text1 += ", ";

    if (entry.num_actors == 2 && entry.reverse_order)
        text1 += Utils::StdStringToQString(entry.event_res2.name, false);
    else
        text1 += Utils::StdStringToQString(entry.event_res1.name, false);

    text2 = Utils::StdStringToQString(entry.cms2_code, false);
    if (text2.isEmpty())
        text2 = "Myself";

    text2 += ", " + QString("%1").arg((int)entry.costume2) + ", " + QString("%1").arg((int)entry.transformation2);
    text2 += ", ";

    if (entry.num_actors == 2 && entry.reverse_order)
         text2 += Utils::StdStringToQString(entry.event_res1.name, false);
    else
         text2 += Utils::StdStringToQString(entry.event_res2.name, false);

    if (entry.num_actors >= 3)
    {
        text3 = Utils::StdStringToQString(entry.cms3_code, false);
        if (text3.isEmpty())
            text3 = "Myself";

        text3 += ", " + QString("%1").arg((int)entry.costume3) + ", " + QString("%1").arg((int)entry.transformation3);
        text3 += ", " + Utils::StdStringToQString(entry.event_res3.name, false);
    }

    if (entry.num_actors >= 4)
    {
        text4 = Utils::StdStringToQString(entry.cms4_code, false);
        if (text4.isEmpty())
            text4 = "Myself";

        text4 += ", " + QString("%1").arg((int)entry.costume4) + ", " + QString("%1").arg((int)entry.transformation4);
        text4 += ", " + Utils::StdStringToQString(entry.event_res4.name, false);
    }

    if (entry.num_actors >= 5)
    {
        text5 = Utils::StdStringToQString(entry.cms5_code, false);
        if (text5.isEmpty())
            text5 = "Myself";

        text5 += ", " + QString("%1").arg((int)entry.costume5) + ", " + QString("%1").arg((int)entry.transformation5);
        text5 += ", " + Utils::StdStringToQString(entry.event_res5.name, false);
    }

    ui->ttbLLActor1Label->setText(text1);
    ui->ttbLLActor2Label->setText(text2);
    ui->ttbLLActor3Label->setText(text3);
    ui->ttbLLActor4Label->setText(text4);
    ui->ttbLLActor5Label->setText(text5);
}

void MainWindow::GuiToTtbEntryLL(TtbEntryLL &entry)
{
    entry.num_actors = ui->ttbLLNumActorsComboBox->currentIndex() + 2;

    if (entry.num_actors == 2)
        entry.reverse_order = ui->ttbLLReverseCheck->isChecked();
    else
        entry.reverse_order = false;

    entry.dummy_order1 = ui->ttbLLDummy1Check->isChecked();
    entry.dummy_order2 = ui->ttbLLDummy2Check->isChecked();

    entry.type = (uint32_t)ui->ttbLLTypeComboBox->currentIndex();
    entry.unk_44 = (uint32_t) ui->ttbLLU44Edit->text().toInt();
    entry.unk_48 = (uint32_t) ui->ttbLLU48Edit->text().toInt();
}

void MainWindow::RebuildTtbEnable(bool enable_hl, bool enable_ll)
{
    ui->ttbModeComboBox->setEnabled(enable_hl || enable_ll);

    ui->ttbHLAddButton->setEnabled(enable_hl);
    ui->ttbHLCharComboBox->setEnabled(enable_hl);
    ui->ttbHLCostComboBox->setEnabled(enable_hl);
    ui->ttbHLCostIndexEdit->setEnabled(enable_hl);
    ui->ttbHLRemoveButton->setEnabled(enable_hl && ui->ttbHLCostComboBox->count() > 1);

    ui->ttbLLAddButton->setEnabled(enable_ll);
    ui->ttbLLRemoveButton->setEnabled(enable_ll && ui->ttbLLCostComboBox->count() > 1);
    ui->ttbLLCostComboBox->setEnabled(enable_ll);
    ui->ttbLLCopyButton->setEnabled(enable_ll);
    ui->ttbLLNumActorsComboBox->setEnabled(enable_ll);
    ui->ttbLLReverseCheck->setEnabled(enable_ll && ui->ttbLLNumActorsComboBox->currentIndex() == 0);
    ui->ttbLLDummy1Check->setEnabled(enable_ll);
    ui->ttbLLDummy2Check->setEnabled(enable_ll);
    ui->ttbLLTypeComboBox->setEnabled(enable_ll);
    ui->ttbLLU44Edit->setEnabled(enable_ll);
    ui->ttbLLU48Edit->setEnabled(enable_ll);
    ui->ttbLLActor1Button->setEnabled(enable_ll);
    ui->ttbLLActor2Button->setEnabled(enable_ll);
    ui->ttbLLActor3Button->setEnabled(enable_ll && ui->ttbLLNumActorsComboBox->currentIndex() >= 1);
    ui->ttbLLActor4Button->setEnabled(enable_ll && ui->ttbLLNumActorsComboBox->currentIndex() >= 2);
    ui->ttbLLActor5Button->setEnabled(enable_ll && ui->ttbLLNumActorsComboBox->currentIndex() >= 3);

    if (!enable_ll)
    {
        ui->ttbLLActor1Label->setText("");
        ui->ttbLLActor2Label->setText("");
    }

    if (!enable_ll || ui->ttbLLNumActorsComboBox->currentIndex() == 0)
    {
        ui->ttbLLActor3Label->setText("");
    }

    if (!enable_ll || ui->ttbLLNumActorsComboBox->currentIndex() <= 1)
    {
        ui->ttbLLActor4Label->setText("");
    }

    if (!enable_ll || ui->ttbLLNumActorsComboBox->currentIndex() <= 2)
    {
        ui->ttbLLActor5Label->setText("");
    }
}

void MainWindow::EditTtbChar(int index)
{
    std::string *cms;
    uint32_t *costume, *trans;
    TtbEventResLL *res;    

    int idx = ui->ttbLLCostComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumTtbLLEntries())
        return;

    TtbEntryLL entry = x2m->GetTtbLLEntry(idx);
    GuiToTtbEntryLL(entry);

    if (index == 0)
    {
        cms = &entry.cms_code;
        costume = &entry.costume;
        trans = &entry.transformation;

        if (entry.num_actors == 2 && entry.reverse_order)
        {
            res = &entry.event_res2;
        }
        else
        {
            res = &entry.event_res1;
        }
    }
    else if (index == 1)
    {
        cms = &entry.cms2_code;
        costume = &entry.costume2;
        trans = &entry.transformation2;

        if (entry.num_actors == 2 && entry.reverse_order)
        {
            res = &entry.event_res1;
        }
        else
        {
            res = &entry.event_res2;
        }
    }
    else if (index == 2)
    {
        cms = &entry.cms3_code;
        costume = &entry.costume3;
        trans = &entry.transformation3;
        res = &entry.event_res3;
    }
    else if (index == 3)
    {
        cms = &entry.cms4_code;
        costume = &entry.costume4;
        trans = &entry.transformation4;
        res = &entry.event_res4;
    }
    else
    {
        cms = &entry.cms5_code;
        costume = &entry.costume5;
        trans = &entry.transformation5;
        res = &entry.event_res5;
    }

    TtbCharEventDialog dialog(this, cms, costume, trans, res, &x2m->GetCustomAudios());

    if (!dialog.exec())
        return;

    if (entry_name.length() == 3 && entry_name == *cms)
        cms->clear();

    x2m->GetTtbLLEntry(idx) = entry;
    TtbEntryLLToGui(entry, false);
}

void MainWindow::FillTtcHLComboBox()
{
    QComboBox *cb = ui->ttcHLCharComboBox;

    struct CharCostumeDef
    {
        std::string code;
        int costume;

        inline bool operator < (const CharCostumeDef &other) const
        {
            if (code == other.code)
                return (costume < other.costume);

            return (code.compare(other.code) < 0);
        }
    };

    std::set<CharCostumeDef> char_set;

    if (!game_ttc)
    {
        if (!Xenoverse2::InitCommonDialogue())
        {
            DPRINTF("Failed to init common dialogue.\n");
            exit(-1);
        }
    }

    for (const TtcEntry &entry : *game_ttc)
    {
        CharCostumeDef cd;
        CmsEntry *cms = game_cms->FindEntryByID(entry.cms_id);

        if (!cms)
            continue;

        if (cms->id >= XV2_FREE_ID_SEARCH_START)
            continue;

        cd.code = cms->name;

        for (const TtcEventList &list : entry.lists)
        {
            for (const TtcEvent &event : list.events)
            {
                cd.costume = event.costume;
                char_set.insert(cd);
            }
        }
    }

    for (const CharCostumeDef &def : char_set)
    {
        std::string name, costume_name;

        if (!Xenoverse2::GetCharaName(def.code, name, XV2_LANG_ENGLISH, (def.costume == -1) ? 0 : def.costume))
            name = def.code;
        else
            name = def.code + " - " + name;

        if (def.costume == -1)
        {
            costume_name = "Any";
        }
        else
        {
           if (!Xenoverse2::GetCharaCostumeName(def.code, def.costume, 0, costume_name))
               costume_name = Utils::ToString(def.costume);
        }

        cb->addItem(Utils::StdStringToQString(name, false) + " - " + Utils::StdStringToQString(costume_name, false),
                    QVariant(Utils::StdStringToQString(def.code) + QString("%1").arg((uint32_t)def.costume)));
    }
}

void MainWindow::TtcEntryHLToGui(const TtcEntryHL &entry)
{
    ui->ttcHLCostIndexEdit->setText(QString("%1").arg((int32_t)entry.costume_id));

    QString search_term = Utils::StdStringToQString(entry.copy_char) + QString("%1").arg(entry.copy_costume);
    int index = ui->ttcHLCharComboBox->findData(QVariant(search_term));

    if (index < 0)
    {
        index = 0;
    }

    ui->ttcHLCharComboBox->setCurrentIndex(index);
}

void MainWindow::GuiToTtcEntryHL(TtcEntryHL &entry)
{
    entry.costume_id = (uint32_t)ui->ttcHLCostIndexEdit->text().toInt();

    QString str = ui->ttcHLCharComboBox->itemData(ui->ttcHLCharComboBox->currentIndex()).toString();

    entry.copy_char = Utils::QStringToStdString(str.left(3));
    entry.copy_costume = str.mid(3).toUInt();
}

void MainWindow::TtcEntryLLToGui(const TtcEntryLL &entry)
{
    int type_idx = ui->ttcLLTypeComboBox->findData(QVariant(entry.type));
    if (type_idx < 0)
        type_idx = 0;

    ui->ttcLLTypeComboBox->setCurrentIndex(type_idx);
    ui->ttcLLCondCombo->setCurrentIndex(entry.condition);
    ui->ttcLLCostEdit->setText(QString("%1").arg((int32_t)entry.costume));
    ui->ttcLLTransEdit->setText(QString("%1").arg((int32_t)entry.transformation));
    ui->ttcLLEventNameEdit->setText(Utils::StdStringToQString(entry.name, false));
}

void MainWindow::GuiToTtcEntryLL(TtcEntryLL &entry)
{
    entry.type = ui->ttcLLTypeComboBox->currentData().toUInt();
    entry.condition = ui->ttcLLCondCombo->currentIndex();
    entry.costume = (uint32_t) ui->ttcLLCostEdit->text().toInt();
    entry.transformation = (uint32_t) ui->ttcLLTransEdit->text().toInt();
    entry.name = Utils::QStringToStdString(ui->ttcLLEventNameEdit->text(), false);
}

void MainWindow::RebuildTtcEnable(bool enable_hl, bool enable_ll)
{
    ui->ttcModeComboBox->setEnabled(enable_hl || enable_ll);

    ui->ttcHLAddButton->setEnabled(enable_hl);
    ui->ttcHLCharComboBox->setEnabled(enable_hl);
    ui->ttcHLCostComboBox->setEnabled(enable_hl);
    ui->ttcHLCostIndexEdit->setEnabled(enable_hl);
    ui->ttcHLRemoveButton->setEnabled(enable_hl && ui->ttbHLCostComboBox->count() > 1);

    ui->ttcLLCostComboBox->setEnabled(enable_ll);
    ui->ttcLLAddButton->setEnabled(enable_ll);
    ui->ttcLLRemoveButton->setEnabled(enable_ll);
    ui->ttcLLCopyButton->setEnabled(enable_ll);
    ui->ttcLLTypeComboBox->setEnabled(enable_ll);
    ui->ttcLLCondCombo->setEnabled(enable_ll);
    ui->ttcLLCostEdit->setEnabled(enable_ll);
    ui->ttcLLTransEdit->setEnabled(enable_ll);
    ui->ttcLLEventNameEdit->setEnabled(enable_ll);
    ui->ttcLLListenCombo->setEnabled(enable_ll);
    ui->ttcLLListenButton->setEnabled(enable_ll);
    ui->ttcLLSubCombo->setEnabled(enable_ll);
    ui->ttcLLSubButton->setEnabled(enable_ll);
    ui->ttcLLVerifyButton->setEnabled(enable_ll);
    ui->ttcLLCopyFilesButton->setEnabled(enable_ll);
}

void MainWindow::on_sevListenButton_clicked()
{
    PlaySevAudio(ui->sevLLEvEntCueIdEdit, ui->sevLLEvEntFileIdEdit, ui->sevLLAudioComboBox);
}

void MainWindow::on_sevListenResponseButton_clicked()
{
    PlaySevAudio(ui->sevLLEvEntRespCueIdEdit, ui->sevLLEvEntRespFileIdEdit, ui->sevLLAudioRespComboBox);
}

void MainWindow::on_sevLLAudioComboBox_currentIndexChanged(int index)
{
    bool enable = (index == 0);

    ui->sevLLEvEntCueIdEdit->setEnabled(enable);
    ui->sevLLEvEntFileIdEdit->setEnabled(enable);
}

void MainWindow::on_sevLLAudioRespComboBox_currentIndexChanged(int index)
{
    bool enable = (index == 0);

    ui->sevLLEvEntRespCueIdEdit->setEnabled(enable);
    ui->sevLLEvEntRespFileIdEdit->setEnabled(enable);
}

void MainWindow::on_ttbCheck_clicked()
{
    bool checked = ui->ttbCheck->isChecked();
    bool enable_hl = (checked && ui->ttbModeComboBox->currentIndex() == 0);
    bool enable_ll = (checked && !enable_hl);

    RebuildTtbEnable(enable_hl, enable_ll);

    if (checked)
    {
        size_t num_hl_entries = x2m->GetNumTtbHLEntries();
        size_t num_ll_entries = x2m->GetNumTtbLLEntries();

        if (num_hl_entries == 0)
        {
            TtbEntryHL entry;

            GuiToTtbEntryHL(entry);
            x2m->AddTtbHLEntry(entry);
            num_hl_entries++;
        }

        if (num_ll_entries == 0)
        {
            TtbEntryLL entry;

            GuiToTtbEntryLL(entry);
            x2m->AddTtbLLEntry(entry);
            num_ll_entries++;
        }

        // Clear combobox now
        prev_ttbhl_cost_index = prev_ttbll_cost_index = -1;
        ui->ttbHLCostComboBox->clear();
        ui->ttbLLCostComboBox->clear();

        for (size_t i = 0; i < num_hl_entries; i++)
        {
            ui->ttbHLCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        for (size_t i = 0; i < num_ll_entries; i++)
        {
            ui->ttbLLCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_hl_entries == 1)
        {
            ui->ttbHLRemoveButton->setDisabled(true);
        }

        if (num_ll_entries == 1)
        {
            ui->ttbLLRemoveButton->setDisabled(true);
        }

        RebuildTtbEnable(enable_hl, enable_ll); // again
    }
    else
    {
        x2m->RemoveAllTtbHL();
        x2m->RemoveAllCustomTtbAudio(false);
        x2m->RemoveAllTtbLL();

        // Clear comboboxes too
        prev_ttbhl_cost_index = prev_ttbll_cost_index = -1;
        ui->ttbHLCostComboBox->clear();
        ui->ttbLLCostComboBox->clear();

        // Put dummy entries to gui
        TtbEntryHL ttb_hl;
        TtbEntryLL ttb_ll;

        TtbEntryHLToGui(ttb_hl);
        TtbEntryLLToGui(ttb_ll, true);
    }
}

void MainWindow::on_ttbModeComboBox_currentIndexChanged(int index)
{
    if (index == 0)
        RebuildTtbEnable(true, false);
    else
    {
        TtbEntryLL entry_ll;
        int idx = ui->ttbLLCostComboBox->currentIndex();

        if (idx >= 0 && idx < x2m->GetNumTtbLLEntries())
        {
            entry_ll = x2m->GetTtbLLEntry(idx);
            GuiToTtbEntryLL(entry_ll);
        }
        else
        {
            GuiToTtbEntryLL(entry_ll);
        }

        RebuildTtbEnable(false, true);
        TtbEntryLLToGui(entry_ll, false);
    }
}

void MainWindow::on_ttbLLNumActorsComboBox_currentIndexChanged(int)
{
    if (ui->ttbCheck->isChecked())
    {
        TtbEntryLL entry;
        int idx = ui->ttbLLCostComboBox->currentIndex();

        if (idx >= 0 && idx < x2m->GetNumTtbLLEntries())
        {
            entry = x2m->GetTtbLLEntry(idx);
            GuiToTtbEntryLL(entry);
        }
        else
        {
            GuiToTtbEntryLL(entry);
        }

        TtbEntryLLToGui(entry, false);
    }
}

void MainWindow::on_ttbHLCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumTtbHLEntries())
        return;

    if (prev_ttbhl_cost_index >= 0 && prev_ttbhl_cost_index < x2m->GetNumTtbHLEntries())
    {
        TtbEntryHL &entry = x2m->GetTtbHLEntry(prev_ttbhl_cost_index);
        GuiToTtbEntryHL(entry);
    }

    const TtbEntryHL &entry = x2m->GetTtbHLEntry(index);
    TtbEntryHLToGui(entry);

    prev_ttbhl_cost_index = index;
}

void MainWindow::on_ttbLLCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumTtbLLEntries())
        return;

    if (prev_ttbll_cost_index >= 0 && prev_ttbll_cost_index < x2m->GetNumTtbLLEntries())
    {
        TtbEntryLL &entry = x2m->GetTtbLLEntry(prev_ttbll_cost_index);
        GuiToTtbEntryLL(entry);
    }

    const TtbEntryLL &entry = x2m->GetTtbLLEntry(index);
    TtbEntryLLToGui(entry, false);

    prev_ttbll_cost_index = index;
}

void MainWindow::on_ttbHLAddButton_clicked()
{
    TtbEntryHL entry;

    int idx = ui->ttbHLCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumTtbHLEntries())
    {
        GuiToTtbEntryHL(entry);
    }

    size_t pos = x2m->AddTtbHLEntry(entry);

    ui->ttbHLCostComboBox->addItem(QString("Entry %1").arg(pos));
    ui->ttbHLCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumTtbHLEntries() > 1)
        ui->ttbHLRemoveButton->setEnabled(true);
}

void MainWindow::on_ttbLLAddButton_clicked()
{
    TtbEntryLL entry;

    int idx = ui->ttbLLCostComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumTtbLLEntries())
    {
        entry = x2m->GetTtbLLEntry(idx);
        GuiToTtbEntryLL(entry);
    }

    size_t pos = x2m->AddTtbLLEntry(entry);

    ui->ttbLLCostComboBox->addItem(QString("Entry %1").arg(pos));
    ui->ttbLLCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumTtbLLEntries() > 1)
        ui->ttbLLRemoveButton->setEnabled(true);
}

void MainWindow::on_ttbHLRemoveButton_clicked()
{
    int index = ui->ttbHLCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumTtbHLEntries())
        return;

    x2m->RemoveTtbHLEntry(index);

    if (prev_ttbhl_cost_index > index)
        prev_ttbhl_cost_index--;
    else
        prev_ttbhl_cost_index = -1;

    ui->ttbHLCostComboBox->removeItem(index);

    for (int i = 0; i < ui->ttbHLCostComboBox->count(); i++)
    {
        ui->ttbHLCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumTtbHLEntries() == 1)
        ui->ttbHLRemoveButton->setDisabled(true);
}

void MainWindow::on_ttbLLRemoveButton_clicked()
{
    int index = ui->ttbLLCostComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumTtbLLEntries())
        return;

    x2m->RemoveTtbLLEntry(index);

    if (prev_ttbll_cost_index > index)
        prev_ttbll_cost_index--;
    else
        prev_ttbll_cost_index = -1;

    ui->ttbLLCostComboBox->removeItem(index);

    for (int i = 0; i < ui->ttbLLCostComboBox->count(); i++)
    {
        ui->ttbLLCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumTtbLLEntries() == 1)
        ui->ttbLLRemoveButton->setDisabled(true);
}

void MainWindow::on_ttbLLActor1Button_clicked()
{
    EditTtbChar(0);
}

void MainWindow::on_ttbLLActor2Button_clicked()
{
    EditTtbChar(1);
}

void MainWindow::on_ttbLLActor3Button_clicked()
{
    EditTtbChar(2);
}

void MainWindow::on_ttbLLActor4Button_clicked()
{
    EditTtbChar(3);
}

void MainWindow::on_ttbLLActor5Button_clicked()
{
    EditTtbChar(4);
}

void MainWindow::on_ttbLLCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    TtbFile *ttb = nullptr;
    bool ignore_mods = true;

    if (arg1 == ui->actionFromGameTtb)
    {
        dialog = new ListDialog(ListMode::TTB, this);
        ttb = game_ttb;
    }
    else if (arg1 == ui->actionFromExternalTtb)
    {
        QString file = QFileDialog::getOpenFileName(this, "External TTB", config.lf_external_ttb, "TTB Files (*.ttb *.ttb.xml)");

        if (file.isNull())
            return;

        config.lf_external_ttb = file;

        ttb = new TtbFile();
        if (!ttb->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete ttb;
            return;
        }

        dialog = new ListDialog(ListMode::TTB, this, ttb);
        ignore_mods = false;
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            std::vector<TtbEntryLL> entries_ll;

            QString data = dialog->GetResultDataString();
            std::string char_code;
            uint32_t cms_id, costume;

            char_code = Utils::QStringToStdString(data.left(3));
            costume = data.mid(3).toUInt();
            CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByName(char_code));
            if (cms)
            {
                cms_id = cms->id;

                // First include the entries where we are source
                for (const TtbEntry &entry : *ttb)
                {
                    if (entry.cms_id != cms_id)
                        continue;

                    for (const TtbEvent &event : entry.events)
                    {
                        TtbEntryLL entry_ll;

                        if (event.costume != costume)
                            continue;

                        // Nothing, we must be empty for "Myself" case
                        //entry_ll.cms_code = char_code; //
                        entry_ll.costume = costume;
                        entry_ll.transformation = event.transformation;
                        entry_ll.event_res1.name = event.voice_name;
                        entry_ll.reverse_order = event.reverse_order;
                        entry_ll.dummy_order1 = event.dummy_order1;
                        entry_ll.dummy_order2 = event.dummy_order2;
                        entry_ll.type = event.type;
                        entry_ll.unk_44 = event.unk_44;
                        entry_ll.unk_48 = event.unk_48;

                        cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms2_id));
                        if (!cms)
                            continue;

                        entry_ll.cms2_code = cms->name;
                        entry_ll.costume2 = event.costume2;
                        entry_ll.transformation2 = event.transformation2;
                        entry_ll.event_res2.name = event.voice2_name;

                        if (event.cms3_id != 0xFFFFFFFF)
                        {
                            entry_ll.num_actors = 3;

                            cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms3_id));
                            if (!cms)
                                continue;

                            entry_ll.cms3_code = cms->name;
                            entry_ll.costume3 = event.costume3;
                            entry_ll.transformation3 = event.transformation3;
                            entry_ll.event_res3.name = event.voice3_name;

                            if (event.cms4_id != 0xFFFFFFFF)
                            {
                                entry_ll.num_actors = 4;

                                cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms4_id));
                                if (!cms)
                                    continue;

                                entry_ll.cms4_code = cms->name;
                                entry_ll.costume4 = event.costume4;
                                entry_ll.transformation4 = event.transformation4;
                                entry_ll.event_res4.name = event.voice4_name;

                                if (event.cms5_id != 0xFFFFFFFF)
                                {
                                    entry_ll.num_actors = 5;

                                    cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms5_id));
                                    if (!cms)
                                        continue;

                                    entry_ll.cms5_code = cms->name;
                                    entry_ll.costume5 = event.costume5;
                                    entry_ll.transformation5 = event.transformation5;
                                    entry_ll.event_res5.name = event.voice5_name;
                                }
                            }
                        }

                        entries_ll.push_back(entry_ll);
                    }
                }

                // Now the entries where we aren't the source

                for (const TtbEntry &entry : *game_ttb)
                {
                    for (const TtbEvent &event : entry.events)
                    {
                        if (entry.cms_id == cms_id && event.costume == costume)
                            continue;

                        bool include_it = false;
                        TtbEntryLL entry_ll;

                        cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(entry.cms_id));
                        if (!cms)
                            continue;

                        entry_ll.cms_code = cms->name;
                        entry_ll.costume = event.costume;
                        entry_ll.transformation = event.transformation;
                        entry_ll.event_res1.name = event.voice_name;
                        entry_ll.reverse_order = event.reverse_order;
                        entry_ll.dummy_order1 = event.dummy_order1;
                        entry_ll.dummy_order2 = event.dummy_order2;
                        entry_ll.type = event.type;
                        entry_ll.unk_44 = event.unk_44;
                        entry_ll.unk_48 = event.unk_48;

                        cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms2_id));
                        if (!cms)
                            continue;

                        if (event.cms2_id == cms_id && event.costume2 == costume)
                        {
                            include_it = true;
                        }
                        else
                        {
                            entry_ll.cms2_code = cms->name;
                        }

                        entry_ll.costume2 = event.costume2;
                        entry_ll.transformation2 = event.transformation2;
                        entry_ll.event_res2.name = event.voice2_name;

                        if (event.cms3_id != 0xFFFFFFFF)
                        {
                            entry_ll.num_actors = 3;

                            cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms3_id));
                            if (!cms)
                                continue;

                            if (event.cms3_id == cms_id && event.costume3 == costume)
                            {
                                include_it = true;
                            }
                            else
                            {
                                entry_ll.cms3_code = cms->name;
                            }

                            entry_ll.costume3 = event.costume3;
                            entry_ll.transformation3 = event.transformation3;
                            entry_ll.event_res3.name = event.voice3_name;

                            if (event.cms4_id != 0xFFFFFFFF)
                            {
                                entry_ll.num_actors = 4;

                                cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms4_id));
                                if (!cms)
                                    continue;

                                if (event.cms4_id == cms_id && event.costume4 == costume)
                                {
                                    include_it = true;
                                }
                                else
                                {
                                    entry_ll.cms4_code = cms->name;
                                }

                                entry_ll.costume4 = event.costume4;
                                entry_ll.transformation4 = event.transformation4;
                                entry_ll.event_res4.name = event.voice4_name;

                                if (event.cms5_id != 0xFFFFFFFF)
                                {
                                    entry_ll.num_actors = 5;

                                    cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByID(event.cms5_id));
                                    if (!cms)
                                        continue;

                                    if (event.cms5_id == cms_id && event.costume5 == costume)
                                    {
                                        include_it = true;
                                    }
                                    else
                                    {
                                        entry_ll.cms5_code = cms->name;
                                    }

                                    entry_ll.costume5 = event.costume5;
                                    entry_ll.transformation5 = event.transformation5;
                                    entry_ll.event_res5.name = event.voice5_name;
                                }
                            }
                        }

                        if (include_it)
                            entries_ll.push_back(entry_ll);
                    }
                }

                if (entries_ll.size() > 0)
                {
                    ui->ttbLLCostComboBox->clear();
                    x2m->RemoveAllTtbLL();

                    for (const TtbEntryLL &entry_ll : entries_ll)
                        x2m->AddTtbLLEntry(entry_ll);

                    on_ttbCheck_clicked();
                }
                else
                {
                    DPRINTF("No entries to copy.");
                }
            } // if cms
        }

        delete dialog;

        if (ttb != game_ttb)
            delete ttb;
    }
}

void MainWindow::on_ttcCheck_clicked()
{
    bool checked = ui->ttcCheck->isChecked();
    bool enable_hl = (checked && ui->ttcModeComboBox->currentIndex() == 0);
    bool enable_ll = (checked && !enable_hl);

    RebuildTtcEnable(enable_hl, enable_ll);

    if (checked)
    {
        size_t num_hl_entries = x2m->GetNumTtcHLEntries();
        size_t num_ll_entries = x2m->GetNumTtcLLEntries();

        if (num_hl_entries == 0)
        {
            TtcEntryHL entry;

            GuiToTtcEntryHL(entry);
            x2m->AddTtcHLEntry(entry);
            num_hl_entries++;
        }

        if (num_ll_entries == 0)
        {
            TtcEntryLL entry;

            GuiToTtcEntryLL(entry);
            x2m->AddTtcLLEntry(entry);
            num_ll_entries++;
        }

        // Clear combobox now
        prev_ttchl_cost_index = prev_ttcll_cost_index = -1;
        ui->ttcHLCostComboBox->clear();
        ui->ttcLLCostComboBox->clear();

        for (size_t i = 0; i < num_hl_entries; i++)
        {
            ui->ttcHLCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        for (size_t i = 0; i < num_ll_entries; i++)
        {
            ui->ttcLLCostComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_hl_entries == 1)
        {
            ui->ttcHLRemoveButton->setDisabled(true);
        }

        if (num_ll_entries == 1)
        {
            ui->ttcLLRemoveButton->setDisabled(true);
        }

        RebuildTtcEnable(enable_hl, enable_ll); // again
    }
    else
    {
        x2m->RemoveAllTtcHL();
        x2m->RemoveAllTtcLL();

        // Clear comboboxes too
        prev_ttchl_cost_index = prev_ttcll_cost_index = -1;
        ui->ttcHLCostComboBox->clear();
        ui->ttcLLCostComboBox->clear();

        // Put dummy entries to gui
        TtcEntryHL ttc_hl;
        TtcEntryLL ttc_ll;

        TtcEntryHLToGui(ttc_hl);
        TtcEntryLLToGui(ttc_ll);
    }
}

void MainWindow::on_ttcModeComboBox_currentIndexChanged(int index)
{
    if (index == 0)
        RebuildTtcEnable(true, false);
    else
        RebuildTtcEnable(false, true);
}

void MainWindow::on_ttcHLCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= (int)x2m->GetNumTtcHLEntries())
        return;

    if (prev_ttchl_cost_index >= 0 && prev_ttchl_cost_index < x2m->GetNumTtcHLEntries())
    {
        TtcEntryHL &entry = x2m->GetTtcHLEntry((size_t)prev_ttchl_cost_index);
        GuiToTtcEntryHL(entry);
    }

    const TtcEntryHL &entry = x2m->GetTtcHLEntry(index);
    TtcEntryHLToGui(entry);

    prev_ttchl_cost_index = (size_t)index;
}

void MainWindow::on_ttcHLAddButton_clicked()
{
    TtcEntryHL entry;

    int idx = ui->ttcHLCostComboBox->currentIndex();
    if (idx >= 0 && idx < (int)x2m->GetNumTtcHLEntries())
    {
        GuiToTtcEntryHL(entry);
    }

    size_t pos = x2m->AddTtcHLEntry(entry);

    ui->ttcHLCostComboBox->addItem(QString("Entry %1").arg(pos));
    ui->ttcHLCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumTtcHLEntries() > 1)
        ui->ttcHLRemoveButton->setEnabled(true);
}

void MainWindow::on_ttcHLRemoveButton_clicked()
{
    int index = ui->ttcHLCostComboBox->currentIndex();

    if (index < 0 || index >= (int)x2m->GetNumTtcHLEntries())
        return;

    x2m->RemoveTtcHLEntry((size_t)index);

    if (prev_ttchl_cost_index > index)
        prev_ttchl_cost_index--;
    else
        prev_ttchl_cost_index = -1;

    ui->ttcHLCostComboBox->removeItem(index);

    for (int i = 0; i < ui->ttcHLCostComboBox->count(); i++)
    {
        ui->ttcHLCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumTtcHLEntries() == 1)
        ui->ttcHLRemoveButton->setDisabled(true);
}

void MainWindow::on_ttcLLCostComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= (int)x2m->GetNumTtcLLEntries())
        return;

    if (prev_ttcll_cost_index >= 0 && prev_ttcll_cost_index < x2m->GetNumTtcLLEntries())
    {
        TtcEntryLL &entry = x2m->GetTtcLLEntry((size_t)prev_ttcll_cost_index);
        GuiToTtcEntryLL(entry);
    }

    const TtcEntryLL &entry = x2m->GetTtcLLEntry((size_t)index);
    TtcEntryLLToGui(entry);

    prev_ttcll_cost_index = index;
}

void MainWindow::on_ttcLLAddButton_clicked()
{
    TtcEntryLL entry;

    int idx = ui->ttcLLCostComboBox->currentIndex();
    if (idx >= 0 && idx < (int)x2m->GetNumTtcLLEntries())
    {
        GuiToTtcEntryLL(entry);
    }

    size_t pos = x2m->AddTtcLLEntry(entry);

    ui->ttcLLCostComboBox->addItem(QString("Entry %1").arg(pos));
    ui->ttcLLCostComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumTtcLLEntries() > 1)
        ui->ttcLLRemoveButton->setEnabled(true);
}

void MainWindow::on_ttcLLRemoveButton_clicked()
{
    int index = ui->ttcLLCostComboBox->currentIndex();

    if (index < 0 || index >= (int)x2m->GetNumTtcLLEntries())
        return;

    x2m->RemoveTtcLLEntry((size_t)index);

    if (prev_ttcll_cost_index > index)
        prev_ttcll_cost_index--;
    else
        prev_ttcll_cost_index = -1;

    ui->ttcLLCostComboBox->removeItem(index);

    for (int i = 0; i < ui->ttcLLCostComboBox->count(); i++)
    {
        ui->ttcLLCostComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumTtcLLEntries() == 1)
        ui->ttcLLRemoveButton->setDisabled(true);
}

void MainWindow::on_ttcLLListenButton_clicked()
{
    PlayTtcAudio();
}

void MainWindow::on_ttcLLSubButton_clicked()
{
    if (entry_name.length() != 3)
    {
        DPRINTF("The character 3-letter code of this char must be known for this functionality to work. (you must set the files directory)\n");
        return;
    }

    QString additional_dir = ui->addDataEdit->text().trimmed();
    if (additional_dir.isEmpty())
    {
        DPRINTF("You must set an additional directory with the required sound and msg files for this functionality to work.\n");
        return;
    }

    QString event_name = ui->ttcLLEventNameEdit->text();
    if (event_name.isEmpty())
    {
        DPRINTF("Event name is empty, nothing to listen.\n");
        return;
    }

    std::string event_name_std = Utils::QStringToStdString(event_name, false);

    size_t lang = (size_t)ui->ttcLLSubCombo->currentIndex();
    std::string msg_path = "data/msg/qc_" + entry_name + "_" + xv2_lang_codes[lang] + ".msg";

    MsgFile msg;

    if (additional_dir == INTERNAL_DATA)
    {
        msg_path = Utils::MakePathString(X2M_JUNGLE, msg_path);

        size_t msg_size;
        uint8_t *msg_buf = x2m->ReadFile(msg_path, &msg_size);

        if (!msg_buf)
        {
            DPRINTF("Cannot find file \"%s\" inside x2m.\n", msg_path.c_str());
            return;
        }

        bool ret = msg.Load(msg_buf, msg_size);
        delete[] msg_buf;

        if (!ret)
        {
            DPRINTF("Failed to load file \"%s\" inside x2m (file exists but load failed)\n", msg_path.c_str());
            return;
        }
    }
    else
    {
        msg_path = Utils::MakePathString(Utils::QStringToStdString(additional_dir), msg_path);

        if (!msg.LoadFromFile(msg_path, false))
        {
            DPRINTF("Failed to load file \"%s\"\n", msg_path.c_str());
            return;
        }
    }

    MsgEntry *entry = msg.FindEntryByName(event_name_std);
    if (!entry)
    {
        DPRINTF("Cannot find entry \"%s\" in \"%s\"\n", event_name_std.c_str(), msg_path.c_str());
        return;
    }

    LogDialog dialog(this);

    dialog.setWindowTitle("Subtitle preview");
    dialog.Resize(130, -50);
    dialog.Append(Utils::StdStringToQString(Xenoverse2::UnescapeHtml(entry->lines[0]), false));
    dialog.exec();

}

bool MainWindow::VerifyTtcLLCommon(AcbFile *acbs, Afs2File *awbs, MsgFile &voice, MsgFile *subs)
{
    if (entry_name.length() != 3)
    {
        DPRINTF("The character 3-letter code of this char must be known for this functionality to work. (you must set the files directory)\n");
        return false;
    }

    QString additional_dir = ui->addDataEdit->text().trimmed();
    if (additional_dir.isEmpty())
    {
        DPRINTF("You must set an additional directory with the required sound and msg files for this functionality to work.\n");
        return false;
    }

    for (int i = 0; i < 2; i++)
    {
        std::string audio_path = "data/sound/VOX/Quest/Common/";

        if (i == 1)
            audio_path += "en/";

        audio_path += "CAQ_" + entry_name + "_VOX";

        std::string acb_path = audio_path + ".acb";
        std::string awb_path = audio_path + ".awb";

        if (additional_dir == INTERNAL_DATA)
        {
            acb_path = Utils::MakePathString(X2M_JUNGLE, acb_path);
            awb_path = Utils::MakePathString(X2M_JUNGLE, awb_path);

            size_t acb_size;
            uint8_t *acb_buf = x2m->ReadFile(acb_path, &acb_size);

            if (!acb_buf)
            {
                DPRINTF("Cannot find file \"%s\" inside x2m.\n", acb_path.c_str());
                return false;
            }

            bool ret = acbs[i].Load(acb_buf, acb_size);
            delete[] acb_buf;

            if (!ret)
            {
                DPRINTF("Failed to load acb file \"%s\" inside x2m (file exists but load failed)\n", acb_path.c_str());
                return false;
            }

            size_t awb_size;
            uint8_t *awb_buf = x2m->ReadFile(awb_path, &awb_size);

            if (!awb_buf)
            {
                DPRINTF("Cannot find file \"%s\" inside x2m.\n", awb_path.c_str());
                return false;
            }

            ret = awbs[i].Load(awb_buf, awb_size);
            delete[] awb_buf;

            if (!ret)
            {
                DPRINTF("Failed to load awb file \"%s\" inside x2m (file exists but load failed)\n", awb_path.c_str());
                return false;
            }
        }
        else
        {
            acb_path = Utils::MakePathString(Utils::QStringToStdString(additional_dir), acb_path);
            awb_path = Utils::MakePathString(Utils::QStringToStdString(additional_dir), awb_path);

            if (!acbs[i].LoadFromFile(acb_path, false))
            {
                DPRINTF("Failed to load acb file \"%s\"\n", acb_path.c_str());
                return false;
            }

            if (!awbs[i].LoadFromFile(awb_path, false))
            {
                DPRINTF("Failed to load awb file \"%s\"\n", awb_path.c_str());
                return false;
            }
        }
    }

    std::string voice_path = "data/msg/qc_" + entry_name + "_voice.msg";

    if (additional_dir == INTERNAL_DATA)
    {
        voice_path = Utils::MakePathString(X2M_JUNGLE, voice_path);

        size_t voice_size;
        uint8_t *voice_buf = x2m->ReadFile(voice_path, &voice_size);

        if (!voice_buf)
        {
            DPRINTF("Cannot find file \"%s\" inside x2m.\n", voice_path.c_str());
            return false;
        }

        bool ret = voice.Load(voice_buf, voice_size);
        delete[] voice_buf;

        if (!ret)
        {
            DPRINTF("Failed to load msg file \"%s\" inside x2m (file exists but load failed)\n", voice_path.c_str());
            return false;
        }
    }
    else
    {
        voice_path = Utils::MakePathString(Utils::QStringToStdString(additional_dir), voice_path);
        if (!voice.LoadFromFile(voice_path, false))
        {
            DPRINTF("Failed to load msg file \"%s\"\n", voice_path.c_str());
            return false;
        }
    }

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        std::string sub_path = "data/msg/qc_" + entry_name + "_" + xv2_lang_codes[i] + ".msg";

        if (additional_dir == INTERNAL_DATA)
        {
            sub_path = Utils::MakePathString(X2M_JUNGLE, sub_path);

            size_t sub_size;
            uint8_t *sub_buf = x2m->ReadFile(sub_path, &sub_size);

            if (!sub_buf)
            {
                DPRINTF("Cannot find file \"%s\" inside x2m.\n", sub_path.c_str());
                return false;
            }

            bool ret = subs[i].Load(sub_buf, sub_size);
            delete[] sub_buf;

            if (!ret)
            {
                DPRINTF("Failed to load msg file \"%s\" inside x2m (file exists but load failed)\n", sub_path.c_str());
                return false;
            }
        }
        else
        {
            sub_path = Utils::MakePathString(Utils::QStringToStdString(additional_dir), sub_path);

            if (!subs[i].LoadFromFile(sub_path, false))
            {
                DPRINTF("Failed to load msg file \"%s\"\n", sub_path.c_str());
                return false;
            }
        }
    }

    return true;
}

bool MainWindow::VerifyTtcLLEntry(const TtcEntryLL &entry, AcbFile *acbs, MsgFile &voice, MsgFile *subs, const char *err_message)
{
    if (Utils::IsEmptyString(entry.name))
    {
        DPRINTF("Event name is empty. %s\n", err_message);
        return false;
    }

    std::string cue_name;
    MsgEntry *voice_entry = voice.FindEntryByName(entry.name);
    if (!voice_entry)
    {
        DPRINTF("Cannot find \"%s\" inside the _voice.msg file. %s\n", entry.name.c_str(), err_message);
        return false;
    }

    cue_name = voice_entry->lines[0];

    if (Utils::IsEmptyString(cue_name))
        cue_name = entry.name;

    for (int i = 0; i < 2; i++)
    {
        const char *err_lang = (i == 0) ? "japanese acb" : "english acb";

        uint32_t cue_id = acbs[i].FindCueId(cue_name);
        if (cue_id == (uint32_t)-1)
        {
            DPRINTF("Cannot find cue id for cue name \"%s\" in %s. %s\n", cue_name.c_str(), err_lang, err_message);
            return false;
        }
    }

    for (size_t i = 0; i < XV2_LANG_NUM; i++)
    {
        MsgEntry *sub_entry = subs[i].FindEntryByName(entry.name);
        if (!sub_entry)
        {
            DPRINTF("Cannot find entry \"%s\" (in subtitle lang %s). %s\n", entry.name.c_str(), xv2_lang_codes[i].c_str(), err_message);
            return false;
        }
    }

    return true;
}

bool MainWindow::CopyTtcLLFiles(const std::string &src_char, const std::string &out_path, bool internal)
{
    const std::string &dst_char = entry_name;

    if (dst_char.length() != 3)
    {
        DPRINTF("This functionality requires the cms code of this char to be known (you must set the char files)");
        return false;
    }

    for (int i = 0; i < 2; i++)
    {
        std::string audio_path_in = "data/sound/VOX/Quest/Common/";

        if (i == 1)
            audio_path_in += "en/";

        audio_path_in += "CAQ_" + src_char + "_VOX";

        const std::string acb_path_in = audio_path_in + ".acb";
        const std::string awb_path_in = audio_path_in + ".awb";

        size_t acb_size;
        uint8_t *acb_buf = xv2fs->ReadFile(acb_path_in, &acb_size);

        if (!acb_buf)
        {
            DPRINTF("Failed to read input file \"%s\" from game.\n", acb_path_in.c_str());
            return false;
        }

        size_t awb_size;
        uint8_t *awb_buf = xv2fs->ReadFile(awb_path_in, &awb_size);

        if (!awb_buf)
        {
            delete[] acb_buf;
            DPRINTF("Failed to read input file \"%s\" from game.\n", awb_path_in.c_str());
            return false;
        }

        if (internal)
        {
            std::string audio_path_out = "data/sound/VOX/Quest/Common/";

            if (i == 1)
                audio_path_out += "en/";

            audio_path_out += "CAQ_" + dst_char+ "_VOX";
            audio_path_out = Utils::MakePathString(X2M_JUNGLE, audio_path_out);

            const std::string acb_path_out = audio_path_out + ".acb";
            const std::string awb_path_out = audio_path_out + ".awb";

            bool ret_acb = x2m->WriteFile(acb_path_out, acb_buf, acb_size);
            bool ret_awb = x2m->WriteFile(awb_path_out, awb_buf, awb_size);

            delete[] acb_buf;
            delete[] awb_buf;

            if (!ret_acb || !ret_awb)
            {
                DPRINTF("Failed to copy audio files (fail in internal data)\n");
                return false;
            }
        }
        else
        {
            std::string audio_path_out = "data/sound/VOX/Quest/Common/";

            if (i == 1)
                audio_path_out += "en/";

            audio_path_out += "CAQ_" + dst_char+ "_VOX";
            audio_path_out = Utils::MakePathString(out_path, audio_path_out);

            const std::string acb_path_out = audio_path_out + ".acb";
            const std::string awb_path_out = audio_path_out + ".awb";

            bool ret_acb = Utils::WriteFileBool(acb_path_out, acb_buf, acb_size, true, true);
            bool ret_awb = Utils::WriteFileBool(awb_path_out, awb_buf, awb_size, true, true);

            delete[] acb_buf;
            delete[] awb_buf;

            if (!ret_acb || !ret_awb)
            {
                DPRINTF("Failed to copy audio files (fail in copying data to destination)\n");
                return false;
            }
        }
    }

    {
        std::string voice_path_in = "data/msg/qc_" + src_char + "_voice.msg";

        size_t size;
        uint8_t *buf = xv2fs->ReadFile(voice_path_in, &size);

        if (!buf)
        {
            DPRINTF("Failed to read input file \"%s\" from game.\n", voice_path_in.c_str());
            return false;
        }

        if (internal)
        {
            std::string voice_path_out = "data/msg/qc_" + dst_char + "_voice.msg";
            voice_path_out = Utils::MakePathString(X2M_JUNGLE, voice_path_out);

            bool ret = x2m->WriteFile(voice_path_out, buf, size);
            delete[] buf;

            if (!ret)
            {
                DPRINTF("Failed to copy voice file (fail in internal data)\n");
                return false;
            }
        }
        else
        {
            std::string voice_path_out = "data/msg/qc_" + dst_char + "_voice.msg";
            voice_path_out = Utils::MakePathString(out_path, voice_path_out);

            bool ret = Utils::WriteFileBool(voice_path_out, buf, size, true, true);
            delete[] buf;

            if (!ret)
            {
                DPRINTF("Failed to copy voice file (fail in copying data to destination)\n");
                return false;
            }
        }
    }

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        std::string sub_path_in = "data/msg/qc_" + src_char + "_" + xv2_lang_codes[i] + ".msg";

        size_t size;
        uint8_t *buf = xv2fs->ReadFile(sub_path_in, &size);

        if (!buf)
        {
            DPRINTF("Failed to read input file \"%s\" from game.\n", sub_path_in.c_str());
            return false;
        }

        if (internal)
        {
            std::string sub_path_out = "data/msg/qc_" + dst_char + "_" + xv2_lang_codes[i] + ".msg";
            sub_path_out = Utils::MakePathString(X2M_JUNGLE, sub_path_out);

            bool ret = x2m->WriteFile(sub_path_out, buf, size);
            delete[] buf;

            if (!ret)
            {
                DPRINTF("Failed to copy subs files (fail in internal data)\n");
                return false;
            }
        }
        else
        {
            std::string sub_path_out = "data/msg/qc_" + dst_char + "_" + xv2_lang_codes[i] + ".msg";
            sub_path_out = Utils::MakePathString(out_path, sub_path_out);

            bool ret = Utils::WriteFileBool(sub_path_out, buf, size, true, true);
            delete[] buf;

            if (!ret)
            {
                DPRINTF("Failed to copy subs files (fail in copying data to destination)\n");
                return false;
            }
        }
    }

    return true;
}

void MainWindow::on_ttcLLVerifyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionTTCVerifySingle)
    {
        AcbFile acbs[2];
        Afs2File awbs[2];
        MsgFile voice;
        MsgFile subs[XV2_LANG_NUM];

        if (!VerifyTtcLLCommon(acbs, awbs, voice, subs))
            return;

        TtcEntryLL entry;
        GuiToTtcEntryLL(entry);

        if (!VerifyTtcLLEntry(entry, acbs, voice, subs, ""))
            return;

        UPRINTF("Verification success.\n");
    }
    else if (arg1 == ui->actionTTCVerifyAll)
    {
        AcbFile acbs[2];
        Afs2File awbs[2];
        MsgFile voice;
        MsgFile subs[XV2_LANG_NUM];

        if (!VerifyTtcLLCommon(acbs, awbs, voice, subs))
            return;

        GuiToTtcEntryLL(x2m->GetTtcLLEntry(ui->ttcLLCostComboBox->currentIndex()));

        for (size_t i = 0; i < x2m->GetNumTtcLLEntries(); i++)
        {
            const TtcEntryLL &entry = x2m->GetTtcLLEntry(i);
            char msg[48];

            snprintf(msg, sizeof(msg), "(In entry %Id)", i);

            if (!VerifyTtcLLEntry(entry, acbs, voice, subs, msg))
                return;
        }

        UPRINTF("Verification success.\n");
    }
}

void MainWindow::on_ttcLLCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    TtcFile *ttc = nullptr;
    bool ignore_mods = true;

    if (arg1 == ui->actionFromGameTtc)
    {
        dialog = new ListDialog(ListMode::TTC, this);
        ttc = game_ttc;
    }
    else if (arg1 == ui->actionFromExternalTtc)
    {
        QString file = QFileDialog::getOpenFileName(this, "External TTC", config.lf_external_ttc, "TTC Files (*.ttc *.ttc.xml)");

        if (file.isNull())
            return;

        config.lf_external_ttc = file;

        ttc = new TtcFile();
        if (!ttc->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete ttc;
            return;
        }

        dialog = new ListDialog(ListMode::TTC, this, ttc);
        ignore_mods = false;
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            std::vector<TtcEntryLL> entries_ll;

            QString data = dialog->GetResultDataString();
            std::string char_code;
            uint32_t cms_id, costume;

            char_code = Utils::QStringToStdString(data.left(3));
            costume = data.mid(3).toUInt();
            CmsEntryXV2 *cms = dynamic_cast<CmsEntryXV2 *>(game_cms->FindEntryByName(char_code));
            if (cms)
            {
                cms_id = cms->id;

                for (const TtcEntry &entry : *ttc)
                {
                    if (entry.cms_id != cms_id)
                        continue;

                    for (const TtcEventList &list : entry.lists)
                    {
                        for (const TtcEvent &event : list.events)
                        {
                            if (event.costume == costume)
                            {
                                TtcEntryLL entry_ll;

                                entry_ll.type = list.type;
                                entry_ll.condition = event.condition;
                                entry_ll.costume = event.costume;
                                entry_ll.transformation = event.transformation;
                                entry_ll.name = event.name;

                                entries_ll.push_back(entry_ll);
                            }
                        }
                    }
                }

                if (entries_ll.size() > 0)
                {
                    ui->ttcLLCostComboBox->clear();
                    x2m->RemoveAllTtcLL();

                    for (const TtcEntryLL &entry_ll : entries_ll)
                        x2m->AddTtcLLEntry(entry_ll);

                    on_ttcCheck_clicked();
                }
                else
                {
                    DPRINTF("No entries to copy.");
                }
            } // if cms
        }

        delete dialog;

        if (ttc != game_ttc)
            delete ttc;
    }
}

void MainWindow::on_ttcLLCopyFilesButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionTtcFilesAddData)
    {
        QString add_data = ui->addDataEdit->text().trimmed();

        if (add_data.isEmpty())
        {
            DPRINTF("Additional data directory must be set for this functionality to work.\n");
            return;
        }

        ListDialog dialog(ListMode::TTC_FILES, this);
        if (!dialog.exec())
            return;

        QString src_char = dialog.GetResult();
        if (src_char.length() != 3)
            return;

        if (CopyTtcLLFiles(Utils::QStringToStdString(src_char), Utils::QStringToStdString(add_data), (add_data == INTERNAL_DATA)))
        {
            UPRINTF("Copy succesfull.\n");
        }
    }
    else if (arg1 == ui->actionTtcFilesCustomDir)
    {
        QString dir = QFileDialog::getExistingDirectory(this, "Choose directory", config.ld_add_data);

        if (dir.isNull())
            return;

        ListDialog dialog(ListMode::TTC_FILES, this);
        if (!dialog.exec())
            return;

        QString src_char = dialog.GetResult();
        if (src_char.length() != 3)
            return;

        if (CopyTtcLLFiles(Utils::QStringToStdString(src_char), Utils::QStringToStdString(dir), false))
        {
            UPRINTF("Copy succesfull.\n");
        }
    }
}

void MainWindow::FillDualSkillComboBox(QComboBox *cb)
{
    cb->clear();

    if (!game_cns)
    {
        if (!Xenoverse2::InitDualSkill(true, true))
        {
            DPRINTF("Failed to init dual skill data.\n");
            exit(-1);
        }
    }

    cb->addItem("NONE", QVariant((uint16_t)0xFFFF));

    for (const CnsEntry &cns_entry : *game_cns)
    {
        if (!cns_entry.IsValid())
            continue;

        std::vector<CusSkill *> skills;

        if (game_cus->FindUltimateSkillByName(cns_entry.code, skills) == 0)
        {
            // RWKV and DMKV don't exist in cus
            //DPRINTF("Cannot find skill \"%s\n", cns_entry.code);
            continue;
        }

        CusSkill *dual_skill = skills[0]; // We'll assume that if there is more than once, it is the first one...
        std::string name;
        if (!Xenoverse2::GetUltimateSkillName(dual_skill->id2, name))
        {
            name = Utils::ToString(dual_skill->id2);
        }

        cb->addItem(Utils::StdStringToQString(name, false), QVariant(cns_entry.id));
    }
}

void MainWindow::DualSkillToComboBox(uint16_t dual_skill_id, QComboBox *cb)
{
    for (int i = 0; i < cb->count(); i++)
    {
        QVariant var = cb->itemData(i);

        if (var.toUInt() == (uint16_t)dual_skill_id)
        {
            cb->setCurrentIndex(i);
            return;
        }
    }

    cb->setCurrentIndex(0);
}

uint16_t MainWindow::ComboBoxToDualSkill(QComboBox *cb)
{
    return (uint16_t) cb->currentData().toUInt();
}

void MainWindow::CncEntryToGui(const CncEntry &entry)
{
    ui->cncCostumeIndexEdit->setText(QString("%1").arg(entry.costume_index));
    ui->cncModelPresetEdit->setText(QString("%1").arg(entry.model_preset));

    DualSkillToComboBox(entry.cns_skills_ids[0], ui->cncDualSkill1ComboBox);
    DualSkillToComboBox(entry.cns_skills_ids[1], ui->cncDualSkill2ComboBox);
    DualSkillToComboBox(entry.cns_skills_ids[2], ui->cncDualSkill3ComboBox);
}

void MainWindow::GuiToCncEntry(CncEntry &entry)
{
    entry.cms_id = X2M_DUMMY_ID16;
    entry.costume_index = ui->cncCostumeIndexEdit->text().toUInt();
    entry.model_preset = ui->cncModelPresetEdit->text().toUInt();

    entry.cns_skills_ids[0] = ComboBoxToDualSkill(ui->cncDualSkill1ComboBox);
    entry.cns_skills_ids[1] = ComboBoxToDualSkill(ui->cncDualSkill2ComboBox);
    entry.cns_skills_ids[2] = ComboBoxToDualSkill(ui->cncDualSkill3ComboBox);
}

void MainWindow::on_cncCheck_clicked()
{
    if (ui->cncCheck->isChecked())
    {
        ui->cncComboBox->setEnabled(true);
        ui->cncAddButton->setEnabled(true);
        ui->cncRemoveButton->setEnabled(true);
        ui->cncCopyButton->setEnabled(true);
        ui->cncCostumeIndexEdit->setEnabled(true);
        ui->cncModelPresetEdit->setEnabled(true);
        ui->cncDualSkill1ComboBox->setEnabled(true);
        ui->cncDualSkill2ComboBox->setEnabled(true);
        ui->cncDualSkill3ComboBox->setEnabled(true);

        // Clear combobox first
        prev_cnc_index = -1;
        ui->cncComboBox->clear();

        size_t num_cnc_entries = x2m->GetNumCncEntries();

        if (num_cnc_entries == 0)
        {
            CncEntry entry;

            GuiToCncEntry(entry);
            x2m->AddCncEntry(entry);
            num_cnc_entries++;
        }

        for (size_t i = 0; i < num_cnc_entries; i++)
        {
            ui->cncComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_cnc_entries == 1)
        {
            ui->cncRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumCncEntries() != 0)
            x2m->RemoveCncEntry(0);

        ui->cncComboBox->setDisabled(true);
        ui->cncAddButton->setDisabled(true);
        ui->cncRemoveButton->setDisabled(true);
        ui->cncCopyButton->setDisabled(true);
        ui->cncCostumeIndexEdit->setDisabled(true);
        ui->cncModelPresetEdit->setDisabled(true);
        ui->cncDualSkill1ComboBox->setDisabled(true);
        ui->cncDualSkill2ComboBox->setDisabled(true);
        ui->cncDualSkill3ComboBox->setDisabled(true);
    }
}

void MainWindow::on_cncComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumCncEntries())
        return;

    if (prev_cnc_index >= 0 && prev_cnc_index < x2m->GetNumCncEntries())
    {
        CncEntry &entry = x2m->GetCncEntry(prev_cnc_index);
        GuiToCncEntry(entry);
    }

    const CncEntry &entry = x2m->GetCncEntry(index);
    CncEntryToGui(entry);

    prev_cnc_index = index;
}

void MainWindow::on_cncAddButton_clicked()
{
    CncEntry entry;

    int idx = ui->cncComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumCncEntries())
    {
        GuiToCncEntry(entry);
    }

    size_t pos = x2m->AddCncEntry(entry);

    ui->cncComboBox->addItem(QString("Entry %1").arg(pos));
    ui->cncComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumCncEntries() > 1)
        ui->cncRemoveButton->setEnabled(true);
}

void MainWindow::on_cncRemoveButton_clicked()
{
    int index = ui->cncComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumCncEntries())
        return;

    x2m->RemoveCncEntry(index);

    if (prev_cnc_index > index)
        prev_cnc_index--;
    else
        prev_cnc_index = -1;

    ui->cncComboBox->removeItem(index);

    for (int i = 0; i < ui->cncComboBox->count(); i++)
    {
        ui->cncComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumCncEntries() == 1)
        ui->cncRemoveButton->setDisabled(true);
}

void MainWindow::on_cncCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    CncFile *cnc = nullptr;

    if (arg1 == ui->actionFromGameCnc)
    {
        dialog = new ListDialog(ListMode::CNC, this);
        cnc = game_cnc;
    }
    else if (arg1 == ui->actionFromExternalCnc)
    {
        QString file = QFileDialog::getOpenFileName(this, "External CNC", config.lf_external_cnc, "CNC Files (*.cnc *.cnc.xml)");

        if (file.isNull())
            return;

        config.lf_external_cnc = file;

        cnc = new CncFile();
        if (!cnc->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed loading file.");
            delete cnc;
            return;
        }

        dialog = new ListDialog(ListMode::CNC, this, cnc);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();

            if (idx >= 0 && idx < cnc->GetNumEntries())
            {
                const CncEntry &entry = (*cnc)[idx];
                CncEntryToGui(entry);
            }
        }

        delete dialog;

        if (cnc != game_cnc)
            delete cnc;
    }
}

void MainWindow::on_guidCopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->modGuidEdit->text());
}

void MainWindow::on_vfxCopyButton_clicked()
{
    ListDialog dialog(ListMode::CMS);

    if (dialog.exec())
    {
        CmsEntry *cms_entry = game_cms->FindEntryByName(Utils::QStringToStdString(dialog.GetResult()));
        if (!cms_entry)
            return;

        if (!game_ers)
        {
            if (!Xenoverse2::InitVfx())
            {
                DPRINTF("InitVfx failed.\n");
                return;
            }
        }

        ui->vfxEdit->setText(Utils::StdStringToQString(game_ers->GetCharEepk(cms_entry->id)));
    }
}
