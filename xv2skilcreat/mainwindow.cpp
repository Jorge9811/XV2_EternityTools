#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QClipboard>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listdialog.h"
#include "cssdialog.h"
#include "embdialog.h"
#include "idbeffectdialog.h"
#include "colordialog.h"

#include "Xenoverse2.h"
#include "xv2ins_common.h"
#include "Config.h"
#include "debug.h"

#define INTERNAL_DATA "Internal package content"
#define GAME_PREFIX "GAME:///"

#define BODY_ID_TEXT    "Id to use in BAC: "

#define BEHAVIOUR_MAX	25

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
    // Cus tab
    ui->cusTypeEdit->setValidator(new QIntValidator(0, 255, this));
    ui->cusU0EEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusPartsetEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusU12Edit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusU30Edit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusU32Edit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusU34Edit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusU36Edit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusPupEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusAuraEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusModelEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusChangeSSEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusNumTransEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->cusCopyButton->addAction(ui->actionFromGameCus);
    ui->cusCopyButton->addAction(ui->actionFromExternalCus);
    ui->cusAcbVoxButton->setStyleSheet("font-weight: bold;");
    ui->cusAcbSeButton->setStyleSheet("font-weight: bold;");
    ui->cusModelButton->setStyleSheet("font-weight: bold;");
    // Idb tab
    ui->idbStarsEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbU0AEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbU0CEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbU0EEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->idbBuyEdit->setValidator(new QIntValidator(this));
    ui->idbSellEdit->setValidator(new QIntValidator(this));
    ui->idbTpEdit->setValidator(new QIntValidator(this));
    ui->idbModelEdit->setValidator(new QIntValidator(this));
    ui->idbU24Edit->setValidator(new QIntValidator(this));
    ui->idbU28Edit->setValidator(new QIntValidator(this));
    ui->idbU2CEdit->setValidator(new QIntValidator(this));
    ui->idbCopyButton->addAction(ui->actionFromGameIdb);
    // Pup tab
    ui->pupU04Edit->setValidator(new QIntValidator(this));
    ui->pupU08Edit->setValidator(new QIntValidator(this));
    ui->pupU0CEdit->setValidator(new QIntValidator(this));
    ui->pupHeaEdit->setValidator(new QDoubleValidator(this));
    ui->pupF14Edit->setValidator(new QDoubleValidator(this));
    ui->pupKiEdit->setValidator(new QDoubleValidator(this));
    ui->pupKiRecEdit->setValidator(new QDoubleValidator(this));
    ui->pupStmEdit->setValidator(new QDoubleValidator(this));
    ui->pupStmRecEdit->setValidator(new QDoubleValidator(this));
    ui->pupEnStaErEdit->setValidator(new QDoubleValidator(this));
    ui->pupStaErEdit->setValidator(new QDoubleValidator(this));
    ui->pupF30Edit->setValidator(new QDoubleValidator(this));
    ui->pupAtkEdit->setValidator(new QDoubleValidator(this));
    ui->pupBasicKiAtkEdit->setValidator(new QDoubleValidator(this));
    ui->pupStrEdit->setValidator(new QDoubleValidator(this));
    ui->pupBlaEdit->setValidator(new QDoubleValidator(this));
    ui->pupAtkDmgEdit->setValidator(new QDoubleValidator(this));
    ui->pupKiDmgEdit->setValidator(new QDoubleValidator(this));
    ui->pupStrDmgEdit->setValidator(new QDoubleValidator(this));
    ui->pupBlaDmgEdit->setValidator(new QDoubleValidator(this));
    ui->pupGroundSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->pupAirSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->pupBoostSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->pupDashSpeedEdit->setValidator(new QDoubleValidator(this));
    ui->pupF64Edit->setValidator(new QDoubleValidator(this));
    ui->pupF68Edit->setValidator(new QDoubleValidator(this));
    ui->pupF6CEdit->setValidator(new QDoubleValidator(this));
    ui->pupF70Edit->setValidator(new QDoubleValidator(this));
    ui->pupF74Edit->setValidator(new QDoubleValidator(this));
    ui->pupF78Edit->setValidator(new QDoubleValidator(this));
    ui->pupF7CEdit->setValidator(new QDoubleValidator(this));
    ui->pupF80Edit->setValidator(new QDoubleValidator(this));
    ui->pupF84Edit->setValidator(new QDoubleValidator(this));
    ui->pupF88Edit->setValidator(new QDoubleValidator(this));
    ui->pupF8CEdit->setValidator(new QDoubleValidator(this));
    ui->pupF90Edit->setValidator(new QDoubleValidator(this));
    ui->pupF94Edit->setValidator(new QDoubleValidator(this));
    ui->pupCopyButton->addAction(ui->actionFromGamePup);
    ui->pupCopyButton->addAction(ui->actionFromExternalPup);
    ui->pupCopyButton->addAction(ui->actionFromPupSkillUsage);
    // Aura tab
    ui->auraIdEdit->setValidator(new QIntValidator(-32768, 32767, this));
    ui->auraEf0Edit->setValidator(new QIntValidator(this));
    ui->auraEf1Edit->setValidator(new QIntValidator(this));
    ui->auraEf2Edit->setValidator(new QIntValidator(this));
    ui->auraEf3Edit->setValidator(new QIntValidator(this));
    ui->auraEf4Edit->setValidator(new QIntValidator(this));
    ui->auraEf5Edit->setValidator(new QIntValidator(this));
    ui->auraEf6Edit->setValidator(new QIntValidator(this));
    ui->auraBH11Edit->setValidator(new QIntValidator(0, BEHAVIOUR_MAX, this));
    ui->auraInt2Edit->setValidator(new QIntValidator(this));
    ui->auraBH10Edit->setValidator(new QIntValidator(0, BEHAVIOUR_MAX, this));
    ui->auraInt3Edit->setValidator(new QIntValidator(this));
    ui->auraBH13Edit->setValidator(new QIntValidator(0, BEHAVIOUR_MAX, this));
    ui->auraBH66Edit->setValidator(new QIntValidator(-1, BEHAVIOUR_MAX, this));
    ui->auraHairColorEdit->setValidator(new QIntValidator(this));
    ui->auraEyesColorEdit->setValidator(new QIntValidator(this));
    ui->auraCopyButton->addAction(ui->actionFromGameAur);
    ui->auraCopyButton->addAction(ui->actionFromExternalAur);
    ui->auraCopyButton->addAction(ui->actionFromAuraSkillUsage);
    ui->auraCopyButton->addAction(ui->actionFromAuraCharUsage);
    ui->auraPatchCopyButton->addAction(ui->actionFromAuraPatchSkillUsage);
    ui->auraGetHairColorButton->addAction(ui->actionFromHumBcs);
    ui->auraGetHairColorButton->addAction(ui->actionFromHufBcs);
    ui->auraGetHairColorButton->addAction(ui->actionFromBcsFile);
    ui->auraGetEyesColorButton->addAction(ui->actionFromHumBcsEyes);
    ui->auraGetEyesColorButton->addAction(ui->actionFromHufBcsEyes);
    ui->auraGetEyesColorButton->addAction(ui->actionFromNmcBcsEyes);
    ui->auraGetEyesColorButton->addAction(ui->actionFromFriBcsEyes);
    ui->auraGetEyesColorButton->addAction(ui->actionFromMamBcsEyes);
    ui->auraGetEyesColorButton->addAction(ui->actionFromMafBcsEyes);
    ui->auraGetEyesColorButton->addAction(ui->actionFromBcsFileEyes);

    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    Bootstrap(false, false);

    x2m = new X2mFile();
    x2m->SetType(X2mType::NEW_SKILL);

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

    if (x2m->GetType() != X2mType::NEW_SKILL)
    {
        DPRINTF("This kind of x2m is not supported by this editor.\n");
        return false;
    }

    ProcessX2m();
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

    if (new_x2m->GetType() != X2mType::NEW_SKILL)
    {
        DPRINTF("That x2m is not of new skill type!\n");
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

    if (!Build())
    {
        DPRINTF("Build failed.\n");
        return;
    }

    if (!x2m->SaveToFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("x2m save failed.\n");
        return;
    }

    UPRINTF("File has been succesfully written.\n");
}

void MainWindow::on_actionExit_triggered()
{
    if (ProcessShutdown())
        qApp->exit();
}

bool MainWindow::ImportSkillDirVisitor(const std::string &path, bool, void *param)
{
    MainWindow *pthis = (MainWindow *)param;
    std::string filename = Utils::GetFileNameString(path);

    if (Utils::BeginsWith(filename, pthis->temp_skill_prefix, false))
    {
        std::string out_path = pthis->temp_skill_dir + X2M_SKILL_PREFIX;
        out_path += filename.substr(pthis->temp_skill_prefix.length());

        if (Utils::EndsWith(path, ".bac", false))
        {
            BacFile bac;

            if (!xv2fs->LoadFile(&bac, path))
            {
                DPRINTF("Failed to load bac \"%s\"\n", path.c_str());
                return false;
            }

            bac.ChangeReferencesToSkill(pthis->temp_skill_id2, X2M_DUMMY_ID16);

            if (!bac.SaveToFile(out_path))
                return false;

            return true;
        }
        else if (Utils::EndsWith(path, ".bsa", false))
        {
            BsaFile bsa;

            if (!xv2fs->LoadFile(&bsa, path))
            {
                DPRINTF("Failed to load bsa \"%s\"\n", path.c_str());
                return false;
            }

            bsa.ChangeReferencesToSkill(pthis->temp_skill_id2, X2M_DUMMY_ID16);

            if (!bsa.SaveToFile(out_path))
                return false;

            return true;
        }
        else if (Utils::EndsWith(path, ".ean", false) || Utils::EndsWith(path, ".bas", false) ||
                 Utils::EndsWith(path, ".eepk", false) || Utils::EndsWith(path, ".bcm", false) ||
                 Utils::EndsWith(path, ".bdm", false))
        {
            size_t size;
            uint8_t *buf = xv2fs->ReadFile(path, &size);

            if (!buf)
            {
                DPRINTF("Failed to read file \"%s\" from game.\n", path.c_str());
                return false;
            }

            bool ret = Utils::WriteFileBool(out_path, buf, size);
            delete[] buf;
            return ret;
        }
    }

    std::string out_path = pthis->temp_skill_dir + filename;
    size_t size;
    uint8_t *buf = xv2fs->ReadFile(path, &size);

    if (!buf)
    {
        DPRINTF("Failed to read file \"%s\" from game.\n", path.c_str());
        return false;
    }

    bool ret = Utils::WriteFileBool(out_path, buf, size);
    delete[] buf;
    return ret;
}

void MainWindow::on_actionImportSkillDir_triggered()
{
    ListDialog dialog(ListMode::SKILL, this, nullptr, SKILL_FLAG_SUPER|SKILL_FLAG_ULTIMATE|SKILL_FLAG_EVASIVE|SKILL_FLAG_AWAKEN);

    if (!dialog.exec())
        return;

    const CusSkill *skill = game_cus->FindSkillAnyByID(dialog.GetResultData());
    if (!skill)
        return;

    std::string skill_dir = Xenoverse2::GetSkillDirectory(*skill, &temp_skill_prefix);
    if (skill_dir.length() == 0)
    {
        DPRINTF("Cannot get skill directory.\n");
        return;
    }

    QString dir = QFileDialog::getExistingDirectory(this, "Select output directory", config.ld_skill_dir);
    if (dir.isEmpty())
        return;

    config.ld_skill_dir = dir;

    temp_skill_dir = Utils::NormalizePath(Utils::QStringToStdString(dir));
    if (temp_skill_dir.back() != '/')
        temp_skill_dir += '/';

    if (!Utils::IsDirectoryEmpty(temp_skill_dir, false))
    {
        if (QMessageBox::question(this, "Proceed?", "That directory is not empty.\nDo you want to proceed?",
                                  QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No), QMessageBox::No)
                != QMessageBox::Yes)
        {
            return;
        }
    }

    temp_skill_id2 = skill->id2;

    if (!xv2fs->VisitDirectory(skill_dir, true, false, false, ImportSkillDirVisitor, this))
    {
       DPRINTF("Error when importing the skill.\n");
       return;
    }

    UPRINTF("Skill directory imported.\n");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox box;

    box.setWindowTitle(PROGRAM_NAME_SKILLCREAT);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Icon::Information);
    box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4<br><br>If you liked the program, you can support its development at<br><a href='https://www.patreon.com/eternity_tools'>https://www.patreon.com/eternity_tools</a>").arg(PROGRAM_NAME_SKILLCREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));
    //box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4").arg(PROGRAM_NAME_SKILLCREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));

    box.exec();
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

void MainWindow::ProcessX2m()
{
    // Info tab
    ui->modNameEdit->setText(Utils::StdStringToQString(x2m->GetModName(), false));
    ui->modAuthorEdit->setText(Utils::StdStringToQString(x2m->GetModAuthor(), false));
    ui->modVersionEdit->setText(QString::number(x2m->GetModVersion()));
    ui->modGuidEdit->setText(Utils::StdStringToQString(x2m->GetModGuid()));
    ui->skillNameEdit->setText(Utils::StdStringToQString(x2m->GetSkillName(ui->skillNameLangComboBox->currentIndex()), false));
    ui->skillDescEdit->setText(Utils::StdStringToQString(x2m->GetSkillDesc(ui->skillDescLangComboBox->currentIndex()), false));

    if (x2m->GetSkillType() == X2mSkillType::SUPER)
    {
        ui->skillTypeComboBox->setCurrentIndex(0);
        on_skillTypeComboBox_currentIndexChanged(0);
    }
    else if (x2m->GetSkillType() == X2mSkillType::ULTIMATE)
    {
        ui->skillTypeComboBox->setCurrentIndex(1);
        on_skillTypeComboBox_currentIndexChanged(1);
    }
    else if (x2m->GetSkillType() == X2mSkillType::EVASIVE)
    {
        ui->skillTypeComboBox->setCurrentIndex(2);
        on_skillTypeComboBox_currentIndexChanged(2);
    }
    else
    {
        ui->skillTypeComboBox->setCurrentIndex(3);
        on_skillTypeComboBox_currentIndexChanged(3);
    }

    // Files tab
    if (x2m->SkillDirectoryExists())
    {
        ui->skillFilesEdit->setText(INTERNAL_DATA);
        ui->skillFilesEdit->setEnabled(false);
    }
    else
    {
        ui->skillFilesEdit->setText("");
        ui->skillFilesEdit->setEnabled(true);
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

    // Cus tab
    if (x2m->HasValidSkillEntry())
    {
        SkillToGui(x2m->GetSkillEntry());
    }

    // Idb tab
    if (x2m->HasSkillIdbEntry())
    {
        IdbEntryToGui(x2m->GetSkillIdbEntry());
        ui->idbEnableCheck->setChecked(true);
    }
    else
    {
        IdbEntry entry;
        IdbEntryToGui(entry);
        ui->idbEnableCheck->setChecked(false);
    }

    on_idbEnableCheck_clicked();

    // Pup tab
    if (x2m->HasSkillPup())
    {
        ui->pupEnableCheck->setChecked(true);
    }
    else
    {
        ui->pupEnableCheck->setChecked(false);
    }

    // Put dummy entry to gui
    PupEntry pup_entry;
    PupEntryToGui(pup_entry);

    on_pupEnableCheck_clicked();

    // Aura tab
    if (x2m->HasSkillAura())
    {
        ui->auraEnableCheck->setChecked(true);
    }
    else
    {
        ui->auraEnableCheck->setChecked(false);
    }

    // Put dummy entry to gui
    X2mSkillAura aura;
    AuraToGui(aura);

    on_auraEnableCheck_clicked();

    // Bcs tab
    if (x2m->HasSkillCostumeDepend())
    {
        QString text;

        if (x2m->SkillCostumeDependHasAttachment())
        {
            text = "[EMBEDDED] ";
        }
        else
        {
            text = "[LINKED] ";
        }

        text += Utils::StdStringToQString(x2m->GetSkillCostumeDepend().name, false);

        ui->bcsAttachmentEdit->setText(text);
        ui->bcsAttachmentRemoveButton->setEnabled(true);

        ui->cusPartsetEdit->setText("-1");
        ui->cusPartsetEdit->setEnabled(false);
    }
    else
    {
        ui->bcsAttachmentEdit->setText(QString());
        ui->bcsAttachmentRemoveButton->setEnabled(false);
        ui->cusPartsetEdit->setEnabled(true);
    }

    if (x2m->HasSkillBodies())
    {
        ui->bcsBodyCheck->setChecked(true);
    }
    else
    {
        ui->bcsBodyCheck->setChecked(false);
    }

    // Put dummy body to gui
    X2mBody body;
    BodyToGui(body, "");

    on_bcsBodyCheck_clicked();

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

    if (Utils::IsEmptyString(x2m->GetSkillName(XV2_LANG_ENGLISH)))
    {
        DPRINTF("[INFO] Skill name cannot be empty for english language.\n");
        return false;
    }

    if (Utils::IsEmptyString(x2m->GetSkillDesc(XV2_LANG_ENGLISH)))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            if (i == XV2_LANG_ENGLISH)
                continue;

            if (!Utils::IsEmptyString(x2m->GetSkillDesc(i)))
            {
                DPRINTF("[INFO] If skill desc is empty in english language, it must be empty in the rest too.\n");
                return false;
            }
        }
    }

    if (ui->skillTypeComboBox->currentIndex() == 3) // Awaken
    {
        // Push current gui entry to the x2m
        int index = ui->skillTransComboBox->currentIndex();
        if (index < 0 || index >= x2m->GetNumSkillTransNames())
        {
            DPRINTF("%s: mult names combobox index out of bounds.\n", FUNCNAME);
            return false;
        }

        X2mSkillTransName &name_entry = x2m->GetSkillTransName(index);
        GuiToTransName(name_entry);

        for (size_t i = 0; i < x2m->GetNumSkillTransNames(); i++)
        {
            const X2mSkillTransName &name_entry = x2m->GetSkillTransName(i);

            if (Utils::IsEmptyString(name_entry.trans_name[XV2_LANG_ENGLISH]))
            {
                DPRINTF("[INFO] Trasnformation name cannot be empty for english language.\n"
                        "In entry %Id.\n", i);
                return false;
            }
        }
    }

    // Files tab
    QString skill_files = ui->skillFilesEdit->text().trimmed();
    if (skill_files.isEmpty())
    {
        DPRINTF("[FILES] Skill files cannot be empty.\n");
        return false;
    }

    if (ui->skillFilesEdit->isEnabled())
    {
        std::string skill_files_std = Utils::NormalizePath(Utils::QStringToStdString(skill_files));

        if (!Utils::DirExists(skill_files_std))
        {
            DPRINTF("[FILES] Directory %s doesn't exist.\n", skill_files_std.c_str());
            return false;
        }
        else if (Utils::IsDirectoryEmpty(skill_files_std, true))
        {
            DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", skill_files_std.c_str());
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

    // Cus tab
    QString cus_name = ui->cusNameEdit->text();

    if (cus_name.isEmpty())
    {
        DPRINTF("[CUS] Name cannot be empty.\n");
        return false;
    }

    if (cus_name.length() != 3 && cus_name.length() != 4)
    {
        DPRINTF("[CUS] Name must be a 3-letter or 4-letter code.\n");
        return false;
    }

    if (ui->cusTypeEdit->text().isEmpty())
    {
        DPRINTF("[CUS] Type cannot be empty.\n");
        return false;
    }

    if (ui->cusU0EEdit->text().isEmpty())
    {
        DPRINTF("[CUS] U_0E cannot be empty.\n");
        return false;
    }

    if (ui->cusPartsetEdit->text().isEmpty())
    {
        DPRINTF("[CUS] Partset cannot be empty.\n");
        return false;
    }

    if (ui->cusU12Edit->text().isEmpty())
    {
        DPRINTF("[CUS] U_12 cannot be empty.\n");
        return false;
    }

    if (ui->cusU30Edit->text().isEmpty())
    {
        DPRINTF("[CUS] U_30 cannot be empty.\n");
        return false;
    }

    if (ui->cusU32Edit->text().isEmpty())
    {
        DPRINTF("[CUS] U_32 cannot be empty.\n");
        return false;
    }

    if (ui->cusU34Edit->text().isEmpty())
    {
        DPRINTF("[CUS] U_34 cannot be empty.\n");
        return false;
    }

    if (ui->cusU36Edit->text().isEmpty())
    {
        DPRINTF("[CUS] U_36 cannot be empty.\n");
        return false;
    }

    if (!ui->pupEnableCheck->isChecked() && ui->cusPupEdit->text().isEmpty())
    {
        DPRINTF("[CUS] PUP cannot be empty.\n");
        return false;
    }

    if (ui->cusAuraEdit->text().isEmpty())
    {
        DPRINTF("[CUS] Aura cannot be empty.\n");
        return false;
    }

    if (ui->cusModelEdit->text().isEmpty())
    {
        DPRINTF("[CUS] Model cannot be empty.\n");
        return false;
    }

    if (ui->cusChangeSSEdit->text().isEmpty())
    {
        DPRINTF("[CUS] Change Skillset cannot be empty.\n");
        return false;
    }

    if (ui->cusNumTransEdit->text().isEmpty())
    {
        DPRINTF("[CUS] Num of transforms cannot be empty.\n");
        return false;
    }

    // IDB tab
    if (ui->idbEnableCheck->isChecked())
    {
        if (ui->idbStarsEdit->text().isEmpty())
        {
            DPRINTF("[IDB] Stars cannot be empty.\n");
            return false;
        }

        if (ui->idbU0AEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_0A cannot be empty.\n");
            return false;
        }

        if (ui->idbU0CEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_0C cannot be empty.\n");
            return false;
        }

        if (ui->idbU0EEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_0E cannot be empty.\n");
            return false;
        }

        if (ui->idbBuyEdit->text().isEmpty())
        {
            DPRINTF("[IDB] Buy cannot be empty.\n");
            return false;
        }

        if (ui->idbSellEdit->text().isEmpty())
        {
            DPRINTF("[IDB] Sell cannot be empty.\n");
            return false;
        }

        if (ui->idbTpEdit->text().isEmpty())
        {
            DPRINTF("[IDB] TP cannot be empty.\n");
            return false;
        }

        if (ui->idbModelEdit->text().isEmpty())
        {
            DPRINTF("[IDB] Model cannot be empty.\n");
            return false;
        }

        if (ui->idbU24Edit->text().isEmpty())
        {
            DPRINTF("[IDB] U_24 cannot be empty.\n");
            return false;
        }

        if (ui->idbU28Edit->text().isEmpty())
        {
            DPRINTF("[IDB] U_28 cannot be empty.\n");
            return false;
        }

        if (ui->idbU2CEdit->text().isEmpty())
        {
            DPRINTF("[IDB] U_2C cannot be empty.\n");
            return false;
        }
    }

    // Pup tab
    if (ui->pupEnableCheck->isChecked())
    {
        QList<QLineEdit *> pup_edits = ui->pupTab->findChildren<QLineEdit *>();

        for (QLineEdit *edit : pup_edits)
        {
            if (edit->text().isEmpty())
            {
                DPRINTF("[PUP] All fields in pup tab must be filled.\n");
                return false;
            }
        }
    }

    // Aur tab
    if (ui->auraEnableCheck->isChecked())
    {
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
            if (ui->auraIdEdit->text().isEmpty())
            {
                DPRINTF("[AURA] Real aura id cannot be empty.\n");
                return false;
            }
        }

        if (ui->auraBH11Edit->text().isEmpty())
        {
            DPRINTF("[AURA] Behaviour 11 cannot be empty.\n");
            return false;
        }

        if (ui->auraBH10Edit->text().isEmpty())
        {
            DPRINTF("[AURA] Behaviour 10 cannot be empty.\n");
            return false;
        }

        if (ui->auraBH13Edit->text().isEmpty())
        {
            DPRINTF("[AURA] Behaviour 13 cannot be empty.\n");
            return false;
        }

        if (ui->auraBH66Edit->text().isEmpty())
        {
            DPRINTF("[AURA] Behaviour 66 cannot be empty.\n");
            return false;
        }

        if (ui->auraHairColorEdit->text().isEmpty())
        {
            DPRINTF("[AURA] Bcs hair color cannot be empty.\n");
            return false;
        }
    }

    // Bcs tab
    if (ui->bcsBodyCheck->isChecked())
    {
        for (size_t i = 0; i < x2m->GetNumSkillBodies(); i++)
        {
            X2mBody body;
            QString xml;
            BcsBody dummy;

            if (i == ui->bcsBodyComboBox->currentIndex())
            {
                GuiToBody(body, xml);
            }
            else
            {
                body = x2m->GetSkillBody(i);
                xml = bcs_bodies[i];
            }

            if (!StringToBody(xml, dummy, "On BCS body " + Utils::ToString(i)))
                return false;
        }
    }

    return true;
}

bool MainWindow::Build()
{
    // Info
    x2m->SetModName(Utils::QStringToStdString(ui->modNameEdit->text(), false));
    x2m->SetModAuthor(Utils::QStringToStdString(ui->modAuthorEdit->text(), false));
    x2m->SetModVersion(ui->modVersionEdit->text().toFloat());

    int type = ui->skillTypeComboBox->currentIndex();
    if (type == 0)
        x2m->SetSkillType(X2mSkillType::SUPER);
    else if (type == 1)
        x2m->SetSkillType(X2mSkillType::ULTIMATE);
    else if (type == 2)
        x2m->SetSkillType(X2mSkillType::EVASIVE);
    else
        x2m->SetSkillType(X2mSkillType::AWAKEN);

    if (x2m->GetSkillType() == X2mSkillType::AWAKEN)
    {
        // Nothing, gui entry was pushed to the file by Validate
    }
    else
    {
        while (x2m->GetNumSkillTransNames() != 0)
            x2m->RemoveSkillTransName(0);
    }

    // Files
    QString skill_files = ui->skillFilesEdit->text().trimmed();

    if (ui->skillFilesEdit->isEnabled())
    {
        const std::string skill_files_std = Utils::NormalizePath(Utils::QStringToStdString(skill_files));
        x2m->DeleteSkillDirectory();

        if (!x2m->AddExternalDirectory(skill_files_std, X2M_SKILL))
        {
            DPRINTF("AddExternalDirectory failed on skill files.\n");
            return false;
        }
    }
    else
    {
        if (skill_files != INTERNAL_DATA)
        {
            DPRINTF("%s: Bug in implementation (1).\n", FUNCNAME);
            return false;
        }

        // Do nothing, keep the data that already exists on the x2m
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

    // CUS
    GuiToSkill(x2m->GetSkillEntry());

    if (ui->pupEnableCheck->isChecked())
        x2m->GetSkillEntry().pup_id = 0xFFFF;

    // IDB
    IdbEntry &idb_entry = x2m->GetSkillIdbEntry();

    if (ui->idbEnableCheck->isChecked())
    {
        GuiToIdbEntry(idb_entry);
        idb_entry.id = idb_entry.name_id = idb_entry.desc_id = X2M_DUMMY_ID16;
    }
    else
    {
        idb_entry.id = X2M_INVALID_ID16;
    }

    // PUP
    if (ui->pupEnableCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int pup_idx = ui->pupComboBox->currentIndex();
        if (pup_idx < 0 || pup_idx >= x2m->GetNumSkillPupEntries())
        {
            DPRINTF("%s: pup entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        PupEntry &entry = x2m->GetSkillPupEntry(pup_idx);
        GuiToPupEntry(entry);
    }
    else
    {
        while (x2m->GetNumSkillPupEntries() != 0)
            x2m->RemoveSkillPupEntry(0);
    }

    // Aura
    if (ui->auraEnableCheck->isChecked())
    {
        // We only need to set up current slot, rest preformed by combobox event
        int aura_idx = ui->auraComboBox->currentIndex();
        if (aura_idx < 0 || aura_idx >= x2m->GetNumSkillAuras())
        {
            DPRINTF("%s: aura entry ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        X2mSkillAura &aura = x2m->GetSkillAura(aura_idx);
        GuiToAura(aura);
    }
    else
    {
        while (x2m->GetNumSkillAuras() != 0)
            x2m->RemoveSkillAura(0);
    }

    // Bcs tab
    if (x2m->HasSkillCostumeDepend())
    {
        x2m->GetSkillEntry().partset = X2M_COSTUME_DEPENDS_ID;
    }

    if (x2m->HasSkillBodies())
    {
        int body_idx = ui->bcsBodyComboBox->currentIndex();
        if (body_idx < 0 || body_idx >= x2m->GetNumSkillBodies())
        {
            DPRINTF("%s: bcs body ComboBox index out of bounds.\n", FUNCNAME);
            return false;
        }

        X2mBody &body = x2m->GetSkillBody(body_idx);
        GuiToBody(body, bcs_bodies[body_idx]);

        // Compile bcs bodies now (Verify already checked that they are correct, so technically a failure should not happen here)
        for (size_t i = 0; i < x2m->GetNumSkillBodies(); i++)
        {
            X2mBody &body = x2m->GetSkillBody(i);

            if (!StringToBody(bcs_bodies[i], body.body, "On BCS body " + Utils::ToString(i)))
                return false;
        }
    }
    else
    {
        while (x2m->GetNumSkillBodies() > 0)
            x2m->RemoveSkillBody(0);

        bcs_bodies.clear();
        ui->bcsBodyId->setText(BODY_ID_TEXT);
    }

    return true;
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

void MainWindow::on_skillTypeComboBox_currentIndexChanged(int index)
{
    bool awaken = (index == 3);

    ui->skillTransComboBox->setEnabled(awaken);
    ui->skillTransCopyButton->setEnabled(awaken);
    ui->skillTransNameEdit->setEnabled(awaken);
    ui->skillTransLangComboBox->setEnabled(awaken);
    ui->skillTransAddButton->setEnabled(awaken);
    ui->skillTransRemoveButton->setEnabled(awaken);

    if (awaken)
    {
        prev_trans_index = -1;
        ui->skillTransComboBox->clear();

        size_t num_names = x2m->GetNumSkillTransNames();

        if (num_names == 0)
        {
            X2mSkillTransName tn;

            GuiToTransName(tn);
            x2m->AddSkillTransName(tn);
            num_names++;
        }

        for (size_t i = 0; i < num_names; i++)
        {
            ui->skillTransComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_names == 1)
        {
            ui->skillTransRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumSkillTransNames() > 0)
            x2m->RemoveSkillTransName(0);
    }
}

void MainWindow::on_skillNameLangComboBox_currentIndexChanged(int index)
{
    ui->skillNameEdit->setText(Utils::StdStringToQString(x2m->GetSkillName(index), false));
}

void MainWindow::on_skillNameEdit_textEdited(const QString &arg1)
{
    x2m->SetSkillName(Utils::QStringToStdString(arg1, false), ui->skillNameLangComboBox->currentIndex());
}

void MainWindow::on_skillNameCopyButton_clicked()
{
    ListDialog dialog(ListMode::SKILL, this, nullptr, SKILL_FLAG_SUPER|SKILL_FLAG_ULTIMATE|SKILL_FLAG_EVASIVE|SKILL_FLAG_AWAKEN);

    if (!dialog.exec())
        return;

    const CusSkill *skill = game_cus->FindSkillAnyByID(dialog.GetResultData());
    if (!skill)
        return;

    std::vector<std::string> name;
    name.resize(XV2_LANG_NUM);

    if (game_cus->FindSuperSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetSuperSkillName(skill->id2, name[i], i);
        }
    }
    else if (game_cus->FindUltimateSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetUltimateSkillName(skill->id2, name[i], i);
        }
    }
    else if (game_cus->FindEvasiveSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetEvasiveSkillName(skill->id2, name[i], i);
        }
    }
    else if (game_cus->FindAwakenSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetAwakenSkillName(skill->id2, name[i], i);
        }
    }

    if (name[0].length() == 0)
        return;

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        x2m->SetSkillName(name[i], i);
    }

    int lang = ui->skillNameLangComboBox->currentIndex();

    if (lang >= 0 && lang < XV2_LANG_NUM)
    {
        ui->skillNameEdit->setText(Utils::StdStringToQString(x2m->GetSkillName(lang), false));
    }
}

void MainWindow::on_skillDescLangComboBox_currentIndexChanged(int index)
{
    ui->skillDescEdit->setText(Utils::StdStringToQString(x2m->GetSkillDesc(index), false));
}

void MainWindow::on_skillDescEdit_textChanged()
{
    QString text = ui->skillDescEdit->toPlainText();
    x2m->SetSkillDesc(Utils::QStringToStdString(text, false), ui->skillDescLangComboBox->currentIndex());
}

void MainWindow::on_skillDescCopyButton_clicked()
{
    ListDialog dialog(ListMode::SKILL, this, nullptr, SKILL_FLAG_SUPER|SKILL_FLAG_ULTIMATE|SKILL_FLAG_EVASIVE|SKILL_FLAG_AWAKEN);

    if (!dialog.exec())
        return;

    const CusSkill *skill = game_cus->FindSkillAnyByID(dialog.GetResultData());
    if (!skill)
        return;

    std::vector<std::string> desc;
    desc.resize(XV2_LANG_NUM);

    if (game_cus->FindSuperSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetSuperSkillDesc(skill->id2, desc[i], i);
        }
    }
    else if (game_cus->FindUltimateSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetUltimateSkillDesc(skill->id2, desc[i], i);
        }
    }
    else if (game_cus->FindEvasiveSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetEvasiveSkillDesc(skill->id2, desc[i], i);
        }
    }
    else if (game_cus->FindAwakenSkillByID(skill->id))
    {
        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            Xenoverse2::GetAwakenSkillDesc(skill->id2, desc[i], i);
        }
    }

    for (int i = 0; i < XV2_LANG_NUM; i++)
    {
        x2m->SetSkillDesc(desc[i], i);
    }

    int lang = ui->skillDescLangComboBox->currentIndex();

    if (lang >= 0 && lang < XV2_LANG_NUM)
    {
        ui->skillDescEdit->setText(Utils::StdStringToQString(x2m->GetSkillDesc(lang), false));
    }
}

void MainWindow::TransNameToGui(const X2mSkillTransName &entry)
{
    int lang = ui->skillTransLangComboBox->currentIndex();
    if (lang >= 0 && lang < XV2_LANG_NUM)
    {
        ui->skillTransNameEdit->setText(Utils::StdStringToQString(entry.trans_name[lang], false));
    }
}

void MainWindow::GuiToTransName(X2mSkillTransName &entry)
{
    // Nothing, processed by edited event
    UNUSED(entry);
}

void MainWindow::on_skillTransNameEdit_textEdited(const QString &arg1)
{
    int entry_index = ui->skillTransComboBox->currentIndex();
    if (entry_index < 0 || entry_index >= x2m->GetNumSkillTransNames())
        return;

    int lang = ui->skillTransLangComboBox->currentIndex();
    if (lang < 0 || lang >= XV2_LANG_NUM)
        return;

    X2mSkillTransName &name = x2m->GetSkillTransName(entry_index);
    name.trans_name[lang] = Utils::QStringToStdString(arg1, false);
}

void MainWindow::on_skillTransLangComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= XV2_LANG_NUM)
        return;

    int entry_index = ui->skillTransComboBox->currentIndex();
    if (entry_index < 0 || entry_index >= x2m->GetNumSkillTransNames())
        return;

    const X2mSkillTransName &name = x2m->GetSkillTransName(entry_index);
    ui->skillTransNameEdit->setText(Utils::StdStringToQString(name.trans_name[index], false));
}

void MainWindow::on_skillTransComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumSkillTransNames())
        return;

    if (prev_trans_index >= 0 && prev_trans_index < x2m->GetNumSkillTransNames())
    {
        X2mSkillTransName &entry = x2m->GetSkillTransName(prev_trans_index);
        GuiToTransName(entry);
    }

    const X2mSkillTransName &entry = x2m->GetSkillTransName(index);
    TransNameToGui(entry);

    prev_trans_index = index;
}

void MainWindow::on_skillTransAddButton_clicked()
{
    X2mSkillTransName entry;

    int idx = ui->skillTransComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSkillTransNames())
    {
        entry = x2m->GetSkillTransName(idx); // copy
        GuiToTransName(entry);
    }

    size_t pos = x2m->AddSkillTransName(entry);

    ui->skillTransComboBox->addItem(QString("Entry %1").arg(pos));
    ui->skillTransComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumSkillTransNames() > 1)
        ui->skillTransRemoveButton->setEnabled(true);
}

void MainWindow::on_skillTransRemoveButton_clicked()
{
    int index = ui->skillTransComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSkillTransNames())
        return;

    x2m->RemoveSkillTransName(index);

    if (prev_trans_index > index)
        prev_trans_index--;
    else
        prev_trans_index = -1;

    ui->skillTransComboBox->removeItem(index);

    for (int i = 0; i < ui->skillTransComboBox->count(); i++)
    {
        ui->skillTransComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumSkillTransNames() == 1)
        ui->skillTransRemoveButton->setDisabled(true);
}

void MainWindow::on_skillTransCopyButton_clicked()
{
    int entry_index = ui->skillTransComboBox->currentIndex();
    if (entry_index < 0 || entry_index >= x2m->GetNumSkillTransNames())
        return;

    X2mSkillTransName &name_entry = x2m->GetSkillTransName(entry_index);

    ListDialog dialog(ListMode::SKILL_TRANS, this);

    if (dialog.exec())
    {
        uint32_t data = dialog.GetResultData();
        uint16_t skill_id2 = data >> 16;
        uint16_t trans = data&0xFFFF;

        for (int i = 0; i < XV2_LANG_NUM; i++)
        {
            std::string name;

            if (Xenoverse2::GetBtlHudAwakenName(skill_id2, trans, name, i))
            {
                name_entry.trans_name[i] = name;
            }
        }

        int lang = ui->skillTransLangComboBox->currentIndex();

        if (lang >= 0 && lang < XV2_LANG_NUM)
        {
            ui->skillTransNameEdit->setText(Utils::StdStringToQString(name_entry.trans_name[lang], false));
        }
    }
}

void MainWindow::on_skillFilesButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select skill directory", config.ld_skill_dir);

    if (dir.isNull())
        return;

    config.ld_skill_dir = dir;
    x2m->DeleteSkillDirectory();

    ui->skillFilesEdit->setText(dir);
    ui->skillFilesEdit->setEnabled(true);
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

void MainWindow::SkillToGui(const CusSkill &skill)
{
    ui->cusNameEdit->setText(Utils::StdStringToQString(skill.name));

    ui->cusHumCheck->setChecked(skill.race_lock & CUS_RACE_HUM);
    ui->cusHufCheck->setChecked(skill.race_lock & CUS_RACE_HUF);
    ui->cusSymCheck->setChecked(skill.race_lock & CUS_RACE_SYM);
    ui->cusSyfCheck->setChecked(skill.race_lock & CUS_RACE_SYF);
    ui->cusNmcCheck->setChecked(skill.race_lock & CUS_RACE_NMC);
    ui->cusFriCheck->setChecked(skill.race_lock & CUS_RACE_FRI);
    ui->cusMamCheck->setChecked(skill.race_lock & CUS_RACE_MAM);
    ui->cusMafCheck->setChecked(skill.race_lock & CUS_RACE_MAF);

    ui->cusTypeEdit->setText(QString("%1").arg(skill.type));
    ui->cusU0EEdit->setText(QString("%1").arg((int16_t)skill.unk_0E));
    ui->cusPartsetEdit->setText(QString("%1").arg((int16_t)skill.partset));
    ui->cusU12Edit->setText(QString("%1").arg((int16_t)skill.unk_12));
    ui->cusU30Edit->setText(QString("%1").arg((int16_t)skill.unk_30));
    ui->cusU32Edit->setText(QString("%1").arg((int16_t)skill.unk_32));
    ui->cusU34Edit->setText(QString("%1").arg((int16_t)skill.unk_34));
    ui->cusU36Edit->setText(QString("%1").arg((int16_t)skill.unk_36));

    if (ui->pupEnableCheck->isChecked())
        ui->cusPupEdit->setText(QString("%1").arg(-1));
    else
        ui->cusPupEdit->setText(QString("%1").arg((int16_t)skill.pup_id));

    ui->cusAuraEdit->setText(QString("%1").arg((int16_t)skill.aura));
    ui->cusModelEdit->setText(QString("%1").arg((int16_t)skill.model));
    ui->cusChangeSSEdit->setText(QString("%1").arg((int16_t)skill.change_skillset));
    ui->cusNumTransEdit->setText(QString("%1").arg((int16_t)skill.num_transforms));

    ui->cusEanEdit->setText(Utils::StdStringToQString(skill.paths[0], false));
    ui->cusCamEanEdit->setText(Utils::StdStringToQString(skill.paths[1], false));
    ui->cusEepkEdit->setText(Utils::StdStringToQString(skill.paths[2], false));
    ui->cusAcbSeEdit->setText(Utils::StdStringToQString(skill.paths[3], false));
    ui->cusAcbVoxEdit->setText(Utils::StdStringToQString(skill.paths[4], false));
    ui->cusBacEdit->setText(Utils::StdStringToQString(skill.paths[5], false));
    ui->cusBcmEdit->setText(Utils::StdStringToQString(skill.paths[6], false));
}

void MainWindow::GuiToSkill(CusSkill &skill)
{
    QString qname = ui->cusNameEdit->text();

    if (qname.length() == 3 || qname.length() == 4)
        skill.name = Utils::QStringToStdString(qname);

    skill.race_lock = 0;

    if (ui->cusHumCheck->isChecked())
        skill.race_lock |= CUS_RACE_HUM;

    if (ui->cusHufCheck->isChecked())
        skill.race_lock |= CUS_RACE_HUF;

    if (ui->cusSymCheck->isChecked())
        skill.race_lock |= CUS_RACE_SYM;

    if (ui->cusSyfCheck->isChecked())
        skill.race_lock |= CUS_RACE_SYF;

    if (ui->cusNmcCheck->isChecked())
        skill.race_lock |= CUS_RACE_NMC;

    if (ui->cusFriCheck->isChecked())
        skill.race_lock |= CUS_RACE_FRI;

    if (ui->cusMamCheck->isChecked())
        skill.race_lock |= CUS_RACE_MAM;

    if (ui->cusMafCheck->isChecked())
        skill.race_lock |= CUS_RACE_MAF;

    skill.type = (uint8_t) ui->cusTypeEdit->text().toUInt();
    skill.unk_0E = (uint16_t) ui->cusU0EEdit->text().toInt();
    skill.partset = (uint16_t) ui->cusPartsetEdit->text().toInt();
    skill.unk_12 = (uint16_t) ui->cusU12Edit->text().toInt();
    skill.unk_30 = (uint16_t) ui->cusU30Edit->text().toInt();
    skill.unk_32 = (uint16_t) ui->cusU32Edit->text().toInt();
    skill.unk_34 = (uint16_t) ui->cusU34Edit->text().toInt();
    skill.unk_36 = (uint16_t) ui->cusU36Edit->text().toInt();
    skill.pup_id = (uint16_t) ui->cusPupEdit->text().toInt();
    skill.aura = (uint16_t) ui->cusAuraEdit->text().toInt();
    skill.model = (uint16_t) ui->cusModelEdit->text().toInt();
    skill.change_skillset = (uint16_t) ui->cusChangeSSEdit->text().toInt();
    skill.num_transforms = (uint16_t) ui->cusNumTransEdit->text().toInt();

    skill.paths[0] = Utils::QStringToStdString(ui->cusEanEdit->text(), false);
    skill.paths[1] = Utils::QStringToStdString(ui->cusCamEanEdit->text(), false);
    skill.paths[2] = Utils::QStringToStdString(ui->cusEepkEdit->text(), false);
    skill.paths[3] = Utils::QStringToStdString(ui->cusAcbSeEdit->text(), false);
    skill.paths[4] = Utils::QStringToStdString(ui->cusAcbVoxEdit->text(), false);
    skill.paths[5] = Utils::QStringToStdString(ui->cusBacEdit->text(), false);
    skill.paths[6] = Utils::QStringToStdString(ui->cusBcmEdit->text(), false);

    skill.id = skill.id2 = X2M_DUMMY_ID16;
}

void MainWindow::on_cusCopyButton_triggered(QAction *arg1)
{
    ListDialog *dialog = nullptr;
    CusFile *cus = nullptr;

    if (arg1 == ui->actionFromGameCus)
    {
        dialog = new ListDialog(ListMode::SKILL, this, nullptr, SKILL_FLAG_ALL);
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

        dialog = new ListDialog(ListMode::CUS, this, cus, SKILL_FLAG_ALL);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            const CusSkill *skill = game_cus->FindSkillAnyByID(dialog->GetResultData());
            if (!skill)
                return;

           CusSkill copy_skill = *skill;
           copy_skill.name = Utils::QStringToStdString(ui->cusNameEdit->text());
           SkillToGui(copy_skill);
        }

        delete dialog;

        if (cus != game_cus)
            delete cus;
    }
}

void MainWindow::on_cusAcbSeButton_clicked()
{
    ListDialog dialog(ListMode::SKILL, this, nullptr, SKILL_FLAG_ALL);

    if (!dialog.exec())
        return ;

    const CusSkill *skill = game_cus->FindSkillAnyByID(dialog.GetResultData());
    if (!skill)
        return;

    if (skill->paths[3].length() != 0)
    {
        ui->cusAcbSeEdit->setText(Utils::StdStringToQString(skill->paths[3], false));
        return;
    }

    const std::string skill_se_path = "data/sound/SE/Battle/Skill/";
    std::string path = skill_se_path + "CAR_BTL_";

    if (skill->id < CUS_ULTIMATE_START)
    {
        path += 'S';
    }
    else if (skill->id >= CUS_ULTIMATE_START && skill->id < CUS_EVASIVE_START)
    {
        path += 'U';
    }
    else if (skill->id >= CUS_EVASIVE_START && skill->id < CUS_UNK_START)
    {
        path += 'E';
    }
    else if (skill->id >= CUS_BLAST_START && skill->id < CUS_AWAKEN_START)
    {
        path += 'B';
    }
    else if (skill->id >= CUS_AWAKEN_START && skill->id < CUS_ID_END)
    {
        path += 'M';
    }
    else
    {
        return;
    }

    path += Utils::ToStringAndPad(skill->id2, 3) + "_";

    path += skill->name;
    path += "_SE";

    if (xv2fs->FileExists(path + ".acb"))
    {
        ui->cusAcbSeEdit->setText(Utils::StdStringToQString(path.substr(skill_se_path.length()), false));
    }
    else
    {
        ui->cusAcbSeEdit->setText(QString());
    }
}

void MainWindow::on_cusAcbVoxButton_clicked()
{
    ListDialog dialog(ListMode::SKILL, this, nullptr, SKILL_FLAG_ALL);

    if (!dialog.exec())
        return ;

    const CusSkill *skill = game_cus->FindSkillAnyByID(dialog.GetResultData());
    if (!skill)
        return;

    if (skill->paths[4].length() != 0)
    {
        ui->cusAcbVoxEdit->setText(Utils::StdStringToQString(skill->paths[4], false));
        return;
    }

    const std::string skill_vox_path = "data/sound/VOX/Battle/Skill/";
    std::string path = skill_vox_path + "CAR_BTL_";

    if (skill->id < CUS_ULTIMATE_START)
    {
        path += 'S';
    }
    else if (skill->id >= CUS_ULTIMATE_START && skill->id < CUS_EVASIVE_START)
    {
        path += 'U';
    }
    else if (skill->id >= CUS_EVASIVE_START && skill->id < CUS_UNK_START)
    {
        path += 'E';
    }
    else if (skill->id >= CUS_BLAST_START && skill->id < CUS_AWAKEN_START)
    {
        path += 'B';
    }
    else if (skill->id >= CUS_AWAKEN_START && skill->id < CUS_ID_END)
    {
        path += 'M';
    }
    else
    {
        return;
    }

    path += Utils::ToStringAndPad(skill->id2, 3) + "_";
    path += skill->name + "_";

    std::string en_path = skill_vox_path + "en/" + Utils::GetFileNameString(path);

    for (CmsEntry *entry : *game_cms)
    {
        if (xv2fs->FileExists(path + entry->name + "_VOX.acb") || xv2fs->FileExists(en_path + entry->name + "_VOX.acb"))
        {
            ui->cusAcbVoxEdit->setText(Utils::StdStringToQString(path.substr(skill_vox_path.length()), false));
            return;
        }
    }

    for (int i = 0; i < 15; i++)
    {
        const std::string voice = std::string("M") + Utils::ToStringAndPad(i, 2);

        if (xv2fs->FileExists(path + voice + "_VOX.acb") || xv2fs->FileExists(en_path + voice + "_VOX.acb"))
        {
            ui->cusAcbVoxEdit->setText(Utils::StdStringToQString(path.substr(skill_vox_path.length()), false));
            return;
        }
    }

    for (int i = 0; i < 15; i++)
    {
        const std::string voice = std::string("F") + Utils::ToStringAndPad(i, 2);

        if (xv2fs->FileExists(path + voice + "_VOX.acb") || xv2fs->FileExists(en_path + voice + "_VOX.acb"))
        {
            ui->cusAcbVoxEdit->setText(Utils::StdStringToQString(path.substr(skill_vox_path.length()), false));
            return;
        }
    }

    ui->cusAcbVoxEdit->setText(QString());
}

void MainWindow::on_cusModelButton_clicked()
{
    ListDialog dialog(ListMode::CMS, this);

    if (!dialog.exec())
        return;

    ui->cusModelEdit->setText(QString("%1").arg(dialog.GetResultData()));
}

void MainWindow::on_cusModelEdit_textChanged(const QString &arg1)
{
    int id = arg1.toInt();

    if (id < 0)
    {
        ui->cusModelEdit->setToolTip(QString());
        return;
    }

    CmsEntry *entry = game_cms->FindEntryByID(id);

    if (entry)
    {
        ui->cusModelEdit->setToolTip(Utils::StdStringToQString(entry->name, false));
    }
    else
    {
        ui->cusModelEdit->setToolTip(QString());
    }
}

void MainWindow::IdbEntryToGui(const IdbEntry &entry)
{
    ui->idbStarsEdit->setText(QString("%1").arg((int16_t)entry.stars));
    ui->idbU0AEdit->setText(QString("%1").arg((int16_t)entry.unk_0A));
    ui->idbU0CEdit->setText(QString("%1").arg((int16_t)entry.unk_0C));
    ui->idbU0EEdit->setText(QString("%1").arg((int16_t)entry.unk_0E));
    ui->idbBuyEdit->setText(QString("%1").arg((int32_t)entry.buy));
    ui->idbSellEdit->setText(QString("%1").arg((int32_t)entry.sell));
    ui->idbTpEdit->setText(QString("%1").arg((int32_t)entry.tp));
    ui->idbModelEdit->setText(QString("%1").arg((int32_t)entry.model));
    ui->idbU24Edit->setText(QString("%1").arg((int32_t)entry.unk_24[0]));
    ui->idbU28Edit->setText(QString("%1").arg((int32_t)entry.unk_24[1]));
    ui->idbU2CEdit->setText(QString("%1").arg((int32_t)entry.unk_24[2]));

    ui->idbHumCheck->setChecked(entry.racelock & IDB_RACE_HUM);
    ui->idbHufCheck->setChecked(entry.racelock & IDB_RACE_HUF);
    ui->idbSymCheck->setChecked(entry.racelock & IDB_RACE_SYM);
    ui->idbSyfCheck->setChecked(entry.racelock & IDB_RACE_SYF);
    ui->idbNmcCheck->setChecked(entry.racelock & IDB_RACE_NMC);
    ui->idbFriCheck->setChecked(entry.racelock & IDB_RACE_FRI);
    ui->idbMamCheck->setChecked(entry.racelock & IDB_RACE_MAM);
    ui->idbMafCheck->setChecked(entry.racelock & IDB_RACE_MAF);
}

void MainWindow::GuiToIdbEntry(IdbEntry &entry)
{
    entry.stars = (uint16_t) ui->idbStarsEdit->text().toInt();
    entry.unk_0A = (uint16_t) ui->idbU0AEdit->text().toInt();
    entry.unk_0C = (uint16_t) ui->idbU0CEdit->text().toInt();
    entry.unk_0E = (uint16_t) ui->idbU0EEdit->text().toInt();
    entry.buy = (uint32_t) ui->idbBuyEdit->text().toInt();
    entry.sell = (uint32_t) ui->idbSellEdit->text().toInt();
    entry.tp = (uint32_t) ui->idbTpEdit->text().toInt();
    entry.model = (uint32_t) ui->idbModelEdit->text().toInt();
    entry.unk_24[0] = (uint32_t) ui->idbU24Edit->text().toInt();
    entry.unk_24[1] = (uint32_t) ui->idbU28Edit->text().toInt();
    entry.unk_24[2] = (uint32_t) ui->idbU2CEdit->text().toInt();

    entry.racelock = 0;

    if (ui->idbHumCheck->isChecked())
        entry.racelock |= IDB_RACE_HUM;

    if (ui->idbHufCheck->isChecked())
        entry.racelock |= IDB_RACE_HUF;

    if (ui->idbSymCheck->isChecked())
        entry.racelock |= IDB_RACE_SYM;

    if (ui->idbSyfCheck->isChecked())
        entry.racelock |= IDB_RACE_SYF;

    if (ui->idbNmcCheck->isChecked())
        entry.racelock |= IDB_RACE_NMC;

    if (ui->idbFriCheck->isChecked())
        entry.racelock |= IDB_RACE_FRI;

    if (ui->idbMamCheck->isChecked())
        entry.racelock |= IDB_RACE_MAM;

    if (ui->idbMafCheck->isChecked())
        entry.racelock |= IDB_RACE_MAF;
}

void MainWindow::EditIdbEffect(IdbEffect &effect)
{
    IdbEffectDialog dialog(this, effect);

    if (!dialog.exec())
        return;

    effect = dialog.GetEffect();
}

void MainWindow::on_idbEnableCheck_clicked()
{
    bool checked = ui->idbEnableCheck->isChecked();

    ui->idbCopyButton->setEnabled(checked);
    ui->idbHumCheck->setEnabled(checked);
    ui->idbHufCheck->setEnabled(checked);
    ui->idbSymCheck->setEnabled(checked);
    ui->idbSyfCheck->setEnabled(checked);
    ui->idbNmcCheck->setEnabled(checked);
    ui->idbFriCheck->setEnabled(checked);
    ui->idbMamCheck->setEnabled(checked);
    ui->idbMafCheck->setEnabled(checked);
    ui->idbStarsEdit->setEnabled(checked);
    ui->idbU0AEdit->setEnabled(checked);
    ui->idbU0CEdit->setEnabled(checked);
    ui->idbU0EEdit->setEnabled(checked);
    ui->idbBuyEdit->setEnabled(checked);
    ui->idbSellEdit->setEnabled(checked);
    ui->idbTpEdit->setEnabled(checked);
    ui->idbModelEdit->setEnabled(checked);
    ui->idbU24Edit->setEnabled(checked);
    ui->idbU28Edit->setEnabled(checked);
    ui->idbU2CEdit->setEnabled(checked);
    ui->idbEff1Button->setEnabled(checked);
    ui->idbEff2Button->setEnabled(checked);
    ui->idbEff3Button->setEnabled(checked);

    IdbEntry &entry = x2m->GetSkillIdbEntry();

    if (checked)
        entry.id = entry.name_id = entry.desc_id = X2M_DUMMY_ID16;
    else
        entry.id = X2M_INVALID_ID16;
}

void MainWindow::on_idbEff1Button_clicked()
{
    EditIdbEffect(x2m->GetSkillIdbEntry().effects[0]);
}

void MainWindow::on_idbEff2Button_clicked()
{
    EditIdbEffect(x2m->GetSkillIdbEntry().effects[1]);
}

void MainWindow::on_idbEff3Button_clicked()
{
    EditIdbEffect(x2m->GetSkillIdbEntry().effects[2]);
}

void MainWindow::on_idbCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromGameIdb)
    {
        ListDialog dialog(ListMode::SKILL_IDB, this);

        if (!dialog.exec())
            return;

        int idx = dialog.GetResultInteger();

        if (idx < 0 || idx >= game_skill_idb->GetNumEntries())
            return;

        IdbEntry &entry = x2m->GetSkillIdbEntry();

        entry = (*game_skill_idb)[idx];
        entry.id = entry.name_id = entry.desc_id = X2M_DUMMY_ID16;

        IdbEntryToGui(entry);
    }
}

void MainWindow::PupEntryToGui(const PupEntry &entry)
{
    ui->pupU04Edit->setText(QString("%1").arg((int32_t)entry.unk_04));
    ui->pupU08Edit->setText(QString("%1").arg((int32_t)entry.unk_08));
    ui->pupU0CEdit->setText(QString("%1").arg((int32_t)entry.unk_0C));
    ui->pupHeaEdit->setText(QString("%1").arg(entry.hea));
    ui->pupF14Edit->setText(QString("%1").arg(entry.unk_14));
    ui->pupKiEdit->setText(QString("%1").arg(entry.ki));
    ui->pupKiRecEdit->setText(QString("%1").arg(entry.ki_recovery));
    ui->pupStmEdit->setText(QString("%1").arg(entry.stm));
    ui->pupStmRecEdit->setText(QString("%1").arg(entry.stamina_recovery));
    ui->pupEnStaErEdit->setText(QString("%1").arg(entry.enemy_stamina_eraser));
    ui->pupStaErEdit->setText(QString("%1").arg(entry.stamina_eraser));
    ui->pupF30Edit->setText(QString("%1").arg(entry.unk_30));
    ui->pupAtkEdit->setText(QString("%1").arg(entry.atk));

    ui->pupBasicKiAtkEdit->setText(QString("%1").arg(entry.basic_ki_attack));
    ui->pupStrEdit->setText(QString("%1").arg(entry.str));
    ui->pupBlaEdit->setText(QString("%1").arg(entry.bla));
    ui->pupAtkDmgEdit->setText(QString("%1").arg(entry.atk_damage));
    ui->pupKiDmgEdit->setText(QString("%1").arg(entry.ki_damage));
    ui->pupStrDmgEdit->setText(QString("%1").arg(entry.str_damage));
    ui->pupBlaDmgEdit->setText(QString("%1").arg(entry.bla_damage));
    ui->pupGroundSpeedEdit->setText(QString("%1").arg(entry.ground_speed));
    ui->pupAirSpeedEdit->setText(QString("%1").arg(entry.air_speed));
    ui->pupBoostSpeedEdit->setText(QString("%1").arg(entry.boosting_speed));
    ui->pupDashSpeedEdit->setText(QString("%1").arg(entry.dash_speed));
    ui->pupF64Edit->setText(QString("%1").arg(entry.unk_64));
    ui->pupF68Edit->setText(QString("%1").arg(entry.unk_68));

    ui->pupF6CEdit->setText(QString("%1").arg(entry.unk_6C));
    ui->pupF70Edit->setText(QString("%1").arg(entry.unk_70));
    ui->pupF74Edit->setText(QString("%1").arg(entry.unk_74));
    ui->pupF78Edit->setText(QString("%1").arg(entry.unk_78));
    ui->pupF7CEdit->setText(QString("%1").arg(entry.unk_7C));
    ui->pupF80Edit->setText(QString("%1").arg(entry.unk_80));
    ui->pupF84Edit->setText(QString("%1").arg(entry.unk_84));
    ui->pupF88Edit->setText(QString("%1").arg(entry.unk_88));
    ui->pupF8CEdit->setText(QString("%1").arg(entry.unk_8C));
    ui->pupF90Edit->setText(QString("%1").arg(entry.unk_90));
    ui->pupF94Edit->setText(QString("%1").arg(entry.unk_94));
}

void MainWindow::GuiToPupEntry(PupEntry &entry)
{
    entry.unk_04 = (uint32_t) ui->pupU04Edit->text().toInt();
    entry.unk_08 = (uint32_t) ui->pupU08Edit->text().toInt();
    entry.unk_0C = (uint32_t) ui->pupU0CEdit->text().toInt();
    entry.hea = ui->pupHeaEdit->text().toFloat();
    entry.unk_14 = ui->pupF14Edit->text().toFloat();
    entry.ki = ui->pupKiEdit->text().toFloat();
    entry.ki_recovery = ui->pupKiRecEdit->text().toFloat();
    entry.stm = ui->pupStmEdit->text().toFloat();
    entry.stamina_recovery = ui->pupStmRecEdit->text().toFloat();
    entry.enemy_stamina_eraser = ui->pupEnStaErEdit->text().toFloat();
    entry.stamina_eraser = ui->pupStaErEdit->text().toFloat();
    entry.unk_30 = ui->pupF30Edit->text().toFloat();
    entry.atk = ui->pupAtkEdit->text().toFloat();

    entry.basic_ki_attack = ui->pupBasicKiAtkEdit->text().toFloat();
    entry.str = ui->pupStrEdit->text().toFloat();
    entry.bla = ui->pupBlaEdit->text().toFloat();
    entry.atk_damage = ui->pupAtkDmgEdit->text().toFloat();
    entry.ki_damage = ui->pupKiDmgEdit->text().toFloat();
    entry.str_damage = ui->pupStrDmgEdit->text().toFloat();
    entry.bla_damage = ui->pupBlaDmgEdit->text().toFloat();
    entry.ground_speed = ui->pupGroundSpeedEdit->text().toFloat();
    entry.air_speed = ui->pupAirSpeedEdit->text().toFloat();
    entry.boosting_speed = ui->pupBoostSpeedEdit->text().toFloat();
    entry.dash_speed = ui->pupDashSpeedEdit->text().toFloat();
    entry.unk_64 = ui->pupF64Edit->text().toFloat();
    entry.unk_68 = ui->pupF68Edit->text().toFloat();

    entry.unk_6C = ui->pupF6CEdit->text().toFloat();
    entry.unk_70 = ui->pupF70Edit->text().toFloat();
    entry.unk_74 = ui->pupF74Edit->text().toFloat();
    entry.unk_78 = ui->pupF78Edit->text().toFloat();
    entry.unk_7C = ui->pupF7CEdit->text().toFloat();
    entry.unk_80 = ui->pupF80Edit->text().toFloat();
    entry.unk_84 = ui->pupF84Edit->text().toFloat();
    entry.unk_88 = ui->pupF88Edit->text().toFloat();
    entry.unk_8C = ui->pupF8CEdit->text().toFloat();
    entry.unk_90 = ui->pupF90Edit->text().toFloat();
    entry.unk_94 = ui->pupF94Edit->text().toFloat();
}

void MainWindow::on_pupEnableCheck_clicked()
{
    bool checked = ui->pupEnableCheck->isChecked();

    ui->pupCopyButton->setEnabled(checked);
    ui->pupU04Edit->setEnabled(checked);
    ui->pupU08Edit->setEnabled(checked);
    ui->pupU0CEdit->setEnabled(checked);
    ui->pupHeaEdit->setEnabled(checked);
    ui->pupF14Edit->setEnabled(checked);
    ui->pupKiEdit->setEnabled(checked);
    ui->pupKiRecEdit->setEnabled(checked);
    ui->pupStmEdit->setEnabled(checked);
    ui->pupStmRecEdit->setEnabled(checked);
    ui->pupEnStaErEdit->setEnabled(checked);
    ui->pupStaErEdit->setEnabled(checked);
    ui->pupF30Edit->setEnabled(checked);
    ui->pupAtkEdit->setEnabled(checked);
    ui->pupBasicKiAtkEdit->setEnabled(checked);
    ui->pupStrEdit->setEnabled(checked);
    ui->pupBlaEdit->setEnabled(checked);
    ui->pupAtkDmgEdit->setEnabled(checked);
    ui->pupKiDmgEdit->setEnabled(checked);
    ui->pupStrDmgEdit->setEnabled(checked);
    ui->pupBlaDmgEdit->setEnabled(checked);
    ui->pupGroundSpeedEdit->setEnabled(checked);
    ui->pupAirSpeedEdit->setEnabled(checked);
    ui->pupBoostSpeedEdit->setEnabled(checked);
    ui->pupDashSpeedEdit->setEnabled(checked);
    ui->pupF64Edit->setEnabled(checked);
    ui->pupF68Edit->setEnabled(checked);
    ui->pupF6CEdit->setEnabled(checked);
    ui->pupF70Edit->setEnabled(checked);
    ui->pupF74Edit->setEnabled(checked);
    ui->pupF78Edit->setEnabled(checked);
    ui->pupF7CEdit->setEnabled(checked);
    ui->pupF80Edit->setEnabled(checked);
    ui->pupF84Edit->setEnabled(checked);
    ui->pupF88Edit->setEnabled(checked);
    ui->pupF8CEdit->setEnabled(checked);
    ui->pupF90Edit->setEnabled(checked);
    ui->pupF94Edit->setEnabled(checked);
    ui->pupAddButton->setEnabled(checked);
    ui->pupRemoveButton->setEnabled(checked);
    ui->pupComboBox->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_pup_index = -1;
        ui->pupComboBox->clear();

        size_t num_pup_entries = x2m->GetNumSkillPupEntries();

        if (num_pup_entries == 0)
        {
            PupEntry entry;

            GuiToPupEntry(entry);
            x2m->AddSkillPupEntry(entry);
            num_pup_entries++;
        }

        for (size_t i = 0; i < num_pup_entries; i++)
        {
            ui->pupComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_pup_entries == 1)
        {
            ui->pupRemoveButton->setDisabled(true);
        }


        ui->cusPupEdit->setText("-1");
        ui->cusPupEdit->setEnabled(false);
    }
    else
    {
        while (x2m->GetNumSkillPupEntries() != 0)
            x2m->RemoveSkillPupEntry(0);

        ui->cusPupEdit->setEnabled(true);
    }
}

void MainWindow::on_pupCopyButton_triggered(QAction *arg1)
{
    if (arg1 == ui->actionFromPupSkillUsage)
    {
        ListDialog dialog(ListMode::PUP_SKILL, this);

        if (dialog.exec())
        {
            const PupEntry *entry = game_pup->FindEntryByID(dialog.GetResultData());

            if (entry)
            {
                PupEntryToGui(*entry);
            }
        }

        return;
    }

    ListDialog *dialog = nullptr;
    PupFile *pup = nullptr;

    if (arg1 == ui->actionFromGamePup)
    {
        dialog = new ListDialog(ListMode::PUP, this);
        pup = game_pup;
    }
    else if (arg1 == ui->actionFromExternalPup)
    {
        QString file = QFileDialog::getOpenFileName(this, "External PUP", config.lf_external_pup, "PUP Files (*.pup *.pup.xml)");

        if (file.isNull())
            return;

        config.lf_external_pup = file;

        pup = new PupFile();
        if (!pup->SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("SmartLoad failed.\n");
            delete pup;
            return;
        }

        dialog = new ListDialog(ListMode::PUP, this, pup);
    }

    if (dialog)
    {
        if (dialog->exec())
        {
            int idx = dialog->GetResultInteger();
            if (idx < 0 || idx >= pup->GetNumEntries())
                return;

            const PupEntry &entry = (*pup)[idx];
            PupEntryToGui(entry);
        }

        delete dialog;

        if (pup != game_pup)
            delete pup;
    }
}

void MainWindow::on_pupComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumSkillPupEntries())
        return;

    if (prev_pup_index >= 0 && prev_pup_index < x2m->GetNumSkillPupEntries())
    {
        PupEntry &entry = x2m->GetSkillPupEntry(prev_pup_index);
        GuiToPupEntry(entry);
    }

    const PupEntry &entry = x2m->GetSkillPupEntry(index);
    PupEntryToGui(entry);

    prev_pup_index = index;
}

void MainWindow::on_pupAddButton_clicked()
{
    PupEntry entry;

    int idx = ui->pupComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSkillPupEntries())
    {
        GuiToPupEntry(entry);
    }

    size_t pos = x2m->AddSkillPupEntry(entry);

    ui->pupComboBox->addItem(QString("Entry %1").arg(pos));
    ui->pupComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumSkillPupEntries() > 1)
        ui->pupRemoveButton->setEnabled(true);
}

void MainWindow::on_pupRemoveButton_clicked()
{
    int index = ui->pupComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSkillPupEntries())
        return;

    x2m->RemoveSkillPupEntry(index);

    if (prev_pup_index > index)
        prev_pup_index--;
    else
        prev_pup_index = -1;

    ui->pupComboBox->removeItem(index);

    for (int i = 0; i < ui->pupComboBox->count(); i++)
    {
        ui->pupComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumSkillPupEntries() == 1)
        ui->pupRemoveButton->setDisabled(true);
}

void MainWindow::AuraToGui(const X2mSkillAura &aura)
{
    if (aura.data.aur_aura_id == X2M_INVALID_ID16)
    {
        ui->auraCustomCheck->setChecked(true);
        ui->auraIdEdit->setText("-1");

        ui->auraIdEdit->setDisabled(true);
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
        ui->auraIdEdit->setText(QString("%1").arg((int16_t)aura.data.aur_aura_id));

        ui->auraIdEdit->setEnabled(true);
        ui->auraEf0Edit->setDisabled(true);
        ui->auraEf1Edit->setDisabled(true);
        ui->auraEf2Edit->setDisabled(true);
        ui->auraEf3Edit->setDisabled(true);
        ui->auraEf4Edit->setDisabled(true);
        ui->auraEf5Edit->setDisabled(true);
        ui->auraEf6Edit->setDisabled(true);
    }

    if (aura.aura.effects.size() >= 1)
    {
        ui->auraEf0Edit->setText(QString("%1").arg((int32_t)aura.aura.effects[0].id));
    }
    else
    {
        ui->auraEf0Edit->setText("-1");
    }

    if (aura.aura.effects.size() >= 2)
    {
        ui->auraEf1Edit->setText(QString("%1").arg((int32_t)aura.aura.effects[1].id));
    }
    else
    {
        ui->auraEf1Edit->setText("-1");
    }

    if (aura.aura.effects.size() >= 3)
    {
        ui->auraEf2Edit->setText(QString("%1").arg((int32_t)aura.aura.effects[2].id));
    }
    else
    {
        ui->auraEf2Edit->setText("-1");
    }

    if (aura.aura.effects.size() >= 4)
    {
        ui->auraEf3Edit->setText(QString("%1").arg((int32_t)aura.aura.effects[3].id));
    }
    else
    {
        ui->auraEf3Edit->setText("-1");
    }

    if (aura.aura.effects.size() >= 5)
    {
        ui->auraEf4Edit->setText(QString("%1").arg((int32_t)aura.aura.effects[4].id));
    }
    else
    {
        ui->auraEf4Edit->setText("-1");
    }

    if (aura.aura.effects.size() >= 6)
    {
        ui->auraEf5Edit->setText(QString("%1").arg((int32_t)aura.aura.effects[5].id));
    }
    else
    {
        ui->auraEf5Edit->setText("-1");
    }

    if (aura.aura.effects.size() >= 7)
    {
        ui->auraEf6Edit->setText(QString("%1").arg((int32_t)aura.aura.effects[6].id));
    }
    else
    {
        ui->auraEf6Edit->setText("-1");
    }

    ui->auraBH11Edit->setText(QString("%1").arg(aura.data.behaviour_11));
    ui->auraInt2Edit->setText(QString("%1").arg((int32_t)aura.data.integer_2));
    ui->auraBH10Edit->setText(QString("%1").arg(aura.data.behaviour_10));
    ui->auraInt3Edit->setText(QString("%1").arg((int32_t)aura.data.integer_3));
    ui->auraTeleportCheck->setChecked(aura.data.force_teleport);
    ui->auraBH13Edit->setText(QString("%1").arg(aura.data.behaviour_13));

    if (!aura.data.force_teleport)
    {
        ui->auraBH66Edit->setText(QString("%1").arg((int8_t)aura.data.behaviour_66));
        ui->auraBH66Edit->setEnabled(true);
    }
    else
    {
        ui->auraBH66Edit->setText("-1");
        ui->auraBH66Edit->setEnabled(false);
    }

    ui->auraRemoveHairAccCombo->setCurrentIndex(((int8_t)aura.data.remove_hair_accessories) + 1);

    ui->auraHairColorEdit->setText(QString("%1").arg((int32_t)aura.data.bcs_hair_color));
    ui->auraEyesColorEdit->setText(QString("%1").arg((int32_t)aura.data.bcs_eyes_color));
}

void MainWindow::GuiToAura(X2mSkillAura &aura)
{
    aura.data.cus_aura_id = X2M_DUMMY_ID16;
    aura.aura.id = X2M_DUMMY_ID;
    aura.aura.unk_04 = 0;

    if (aura.aura.effects.size() != 7)
        aura.aura.effects.resize(7);

    for (uint32_t i = 0; i < 7; i++)
    {
        aura.aura.effects[i].type_index = i;
    }

    if (ui->auraCustomCheck->isChecked())
    {
        aura.data.aur_aura_id = X2M_INVALID_ID16;
    }
    else
    {
        aura.data.aur_aura_id = (uint16_t) ui->auraIdEdit->text().toInt();
    }

    aura.aura.effects[0].id = (uint32_t) ui->auraEf0Edit->text().toInt();
    aura.aura.effects[1].id = (uint32_t) ui->auraEf1Edit->text().toInt();
    aura.aura.effects[2].id = (uint32_t) ui->auraEf2Edit->text().toInt();
    aura.aura.effects[3].id = (uint32_t) ui->auraEf3Edit->text().toInt();
    aura.aura.effects[4].id = (uint32_t) ui->auraEf4Edit->text().toInt();
    aura.aura.effects[5].id = (uint32_t) ui->auraEf5Edit->text().toInt();
    aura.aura.effects[6].id = (uint32_t) ui->auraEf6Edit->text().toInt();

    aura.data.behaviour_11 = (uint8_t) ui->auraBH11Edit->text().toUInt();
    aura.data.integer_2 = (uint32_t) ui->auraInt2Edit->text().toInt();
    aura.data.behaviour_10 = (uint8_t) ui->auraBH10Edit->text().toUInt();
    aura.data.integer_3 = (uint32_t) ui->auraInt3Edit->text().toInt();
    aura.data.force_teleport = ui->auraTeleportCheck->isChecked();
    aura.data.behaviour_13 = (uint8_t) ui->auraBH13Edit->text().toUInt();

    if (!aura.data.force_teleport)
    {
        // This one to Int, not UInt, as we allow -1
        aura.data.behaviour_66 = (uint8_t) ui->auraBH66Edit->text().toInt();
    }
    else
    {
        aura.data.behaviour_66 = 0xFF;
    }

    aura.data.remove_hair_accessories = (uint8_t) (ui->auraRemoveHairAccCombo->currentIndex() - 1);

    aura.data.bcs_hair_color = (uint32_t) ui->auraHairColorEdit->text().toInt();
    aura.data.bcs_eyes_color = (uint32_t) ui->auraEyesColorEdit->text().toInt();
}

void MainWindow::BodyToGui(const X2mBody &body, const QString body_xml)
{
    if (body.race < X2M_CR_NUM)
        ui->bcsBodyRaceComboBox->setCurrentIndex(body.race);
    else
        ui->bcsBodyRaceComboBox->setCurrentIndex(0);

    QString text = BODY_ID_TEXT;

    if (body.id >= 0)
        text += QString("%1").arg(body.id);

    ui->bcsBodyId->setText(text);
    ui->bcsBodyEdit->clear();
    ui->bcsBodyEdit->insertPlainText(body_xml);  // Cannot use setText, because it will interpret <Body> as html!
}

void MainWindow::GuiToBody(X2mBody &body, QString &body_xml)
{
    body.race = ui->bcsBodyRaceComboBox->currentIndex();
    body_xml = ui->bcsBodyEdit->toPlainText();

    //DPRINTF("%s\n", Utils::QStringToStdString(body_xml).c_str());
}

QString MainWindow::BodyToString(const BcsBody &body)
{
    TiXmlElement *bcs_root = body.Decompile(nullptr, -1);

    if (!bcs_root)
    {
        DPRINTF("%s: Internal error, cannot continue.\n", FUNCNAME);
        qApp->exit();
        return QString();
    }

    TiXmlPrinter printer;
    bcs_root->Accept(&printer);

    const char *xml_buf = printer.CStr();
    return QString(xml_buf);
}

bool MainWindow::StringToBody(const QString &str, BcsBody &body, const std::string &error_prefix)
{
    TiXmlDocument doc;

    if (str.trimmed().isEmpty())
    {
        DPRINTF("%s: bcs text is empty!\n", error_prefix.c_str());
        return false;
    }

    doc.Parse(Utils::QStringToStdString(str, false).c_str());

    if (doc.ErrorId() != 0)
    {
        DPRINTF("%s: Xml parse error on line %d, col %d\n"
                "Tinyxml error id/description: %d - %s\n",
                error_prefix.c_str(), doc.ErrorRow(), doc.ErrorCol(),
                 doc.ErrorId(), doc.ErrorDesc());

        return false;
    }

    TiXmlHandle handle(&doc);
    TiXmlElement *root = Utils::FindRoot(&handle, "Body");
    if (!root)
    {
        DPRINTF("%s: cannot find <Body>\n", error_prefix.c_str());
        return false;
    }

    if (!body.Compile(root))
    {
        DPRINTF("%s: body compilation error.\n", error_prefix.c_str());
        return false;
    }

    if (body.IsEmpty())
    {
        DPRINTF("%s: Body cannot be an empty one.\n", error_prefix.c_str());
        return false;
    }

    return true;
}

void MainWindow::on_auraEnableCheck_clicked()
{
    bool checked = ui->auraEnableCheck->isChecked();
    bool checked_custom = ui->auraCustomCheck->isChecked();

    ui->auraComboBox->setEnabled(checked);
    ui->auraAddButton->setEnabled(checked);
    ui->auraRemoveButton->setEnabled(checked);
    ui->auraCopyButton->setEnabled(checked);
    ui->auraPatchCopyButton->setEnabled(checked);
    ui->auraCustomCheck->setEnabled(checked);
    ui->auraIdEdit->setEnabled(checked && !checked_custom);
    ui->auraEf0Edit->setEnabled(checked && checked_custom);
    ui->auraEf1Edit->setEnabled(checked && checked_custom);
    ui->auraEf2Edit->setEnabled(checked && checked_custom);
    ui->auraEf3Edit->setEnabled(checked && checked_custom);
    ui->auraEf4Edit->setEnabled(checked && checked_custom);
    ui->auraEf5Edit->setEnabled(checked && checked_custom);
    ui->auraEf6Edit->setEnabled(checked && checked_custom);
    ui->auraBH11Edit->setEnabled(checked);
    ui->auraInt2Edit->setEnabled(checked);
    ui->auraBH10Edit->setEnabled(checked);
    ui->auraInt3Edit->setEnabled(checked);
    ui->auraTeleportCheck->setEnabled(checked);
    ui->auraBH13Edit->setEnabled(checked);
    ui->auraBH66Edit->setEnabled(checked);
    ui->auraHairColorEdit->setEnabled(checked);
    ui->auraGetHairColorButton->setEnabled(checked);
    ui->auraRemoveHairAccCombo->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_aura_index = -1;
        ui->auraComboBox->clear();

        size_t num_aura_entries = x2m->GetNumSkillAuras();

        if (num_aura_entries == 0)
        {
            X2mSkillAura aura;

            GuiToAura(aura);
            x2m->AddSkillAura(aura);
            num_aura_entries++;
        }

        for (size_t i = 0; i < num_aura_entries; i++)
        {
            ui->auraComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_aura_entries == 1)
        {
            ui->auraRemoveButton->setDisabled(true);
        }


        ui->cusAuraEdit->setText("-1");
        ui->cusAuraEdit->setEnabled(false);
    }
    else
    {
        while (x2m->GetNumSkillAuras() != 0)
            x2m->RemoveSkillAura(0);

        ui->cusAuraEdit->setEnabled(true);
    }
}

void MainWindow::on_auraCustomCheck_clicked()
{
    bool checked = ui->auraCustomCheck->isChecked();

    if (checked)
    {
        ui->auraIdEdit->setDisabled(true);
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
        ui->auraIdEdit->setEnabled(true);
        ui->auraEf0Edit->setDisabled(true);
        ui->auraEf1Edit->setDisabled(true);
        ui->auraEf2Edit->setDisabled(true);
        ui->auraEf3Edit->setDisabled(true);
        ui->auraEf4Edit->setDisabled(true);
        ui->auraEf5Edit->setDisabled(true);
        ui->auraEf6Edit->setDisabled(true);
    }
}

void MainWindow::on_auraTeleportCheck_clicked()
{
    if (!ui->auraTeleportCheck->isChecked())
    {
        ui->auraBH66Edit->setEnabled(true);
    }
    else
    {
        ui->auraBH66Edit->setText("-1");
        ui->auraBH66Edit->setEnabled(false);
    }
}

void MainWindow::on_auraComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumSkillAuras())
        return;

    if (prev_aura_index >= 0 && prev_aura_index < x2m->GetNumSkillAuras())
    {
        X2mSkillAura &aura = x2m->GetSkillAura(prev_aura_index);
        GuiToAura(aura);
    }

    const X2mSkillAura &aura = x2m->GetSkillAura(index);
    AuraToGui(aura);

    prev_aura_index = index;
}

void MainWindow::on_auraAddButton_clicked()
{
    X2mSkillAura aura;

    int idx = ui->auraComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSkillAuras())
    {
        GuiToAura(aura);
    }

    size_t pos = x2m->AddSkillAura(aura);

    ui->auraComboBox->addItem(QString("Entry %1").arg(pos));
    ui->auraComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumSkillAuras() > 1)
        ui->auraRemoveButton->setEnabled(true);
}

void MainWindow::on_auraRemoveButton_clicked()
{
    int index = ui->auraComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSkillAuras())
        return;

    x2m->RemoveSkillAura(index);

    if (prev_aura_index > index)
        prev_aura_index--;
    else
        prev_aura_index = -1;

    ui->auraComboBox->removeItem(index);

    for (int i = 0; i < ui->auraComboBox->count(); i++)
    {
        ui->auraComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumSkillAuras() == 1)
        ui->auraRemoveButton->setDisabled(true);
}

void MainWindow::on_auraCopyButton_triggered(QAction *arg1)
{
    int idx = ui->auraComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumSkillAuras())
        return;

    X2mSkillAura &aura = x2m->GetSkillAura(idx);
    GuiToAura(aura);

    if (arg1 == ui->actionFromAuraSkillUsage)
    {
        ListDialog dialog(ListMode::AURA_SKILL, this);

        if (dialog.exec())
        {
            idx = dialog.GetResultData();
            AurAura *existing_aura = game_aur->FindAuraByID(idx);

            if (existing_aura)
            {
                aura.aura = *existing_aura;

                if (aura.data.aur_aura_id != X2M_INVALID_ID16)
                    aura.data.aur_aura_id = aura.aura.id;

                aura.aura.id = X2M_DUMMY_ID;
                AuraToGui(aura);
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
                aura.aura = *existing_aura;

                if (aura.data.aur_aura_id != X2M_INVALID_ID16)
                    aura.data.aur_aura_id = aura.aura.id;

                aura.aura.id = X2M_DUMMY_ID;
                AuraToGui(aura);
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

            aura.aura = auras[idx];

            if (aura.data.aur_aura_id != X2M_INVALID_ID16)
                aura.data.aur_aura_id = aura.aura.id;

            aura.aura.id = X2M_DUMMY_ID;
            AuraToGui(aura);
        }

        delete dialog;

        if (aur != game_aur)
            delete aur;
    }
}

void MainWindow::on_auraPatchCopyButton_triggered(QAction *arg1)
{
    int idx = ui->auraComboBox->currentIndex();
    if (idx < 0 || idx >= x2m->GetNumSkillAuras())
        return;

    X2mSkillAura &aura = x2m->GetSkillAura(idx);
    GuiToAura(aura);

    if (arg1 == ui->actionFromAuraPatchSkillUsage)
    {
        ListDialog dialog(ListMode::AURA_SKILL, this, nullptr, FLAG_USE_CUS_AURA);

        if (dialog.exec())
        {
            int idx = dialog.GetResultData();
            CusAuraData *data = game_prebaked->FindAuraData(idx);

            if (data)
            {
                aura.data.behaviour_11 = data->behaviour_11;
                aura.data.integer_2 = data->integer_2;
                aura.data.behaviour_10 = data->behaviour_10;
                aura.data.integer_3 = data->integer_3;
                aura.data.force_teleport = data->force_teleport;
                aura.data.behaviour_13 = data->behaviour_13;
                aura.data.behaviour_66 = data->behaviour_66;
                aura.data.remove_hair_accessories = data->remove_hair_accessories;
                aura.data.bcs_hair_color = data->bcs_hair_color;
                aura.data.bcs_eyes_color = data->bcs_eyes_color;
            }
            else
            {
                if (idx <= BEHAVIOUR_MAX)
                {
                    aura.data.behaviour_11 = idx;
                    aura.data.behaviour_10 = idx;
                    aura.data.behaviour_13 = idx;
                }

                if (idx == 1 || idx == 5 || idx == 21 || idx == 23)
                    aura.data.integer_2 = 1;
                else if (idx == 2 || idx == 3 || idx == 6)
                    aura.data.integer_2 = 2;
                else
                    aura.data.integer_2 = 0;

                if (idx == 0 || (idx >= 11 && idx <= 19))
                    aura.data.integer_3 = 1;
                else if (idx == 1 || idx == 7 || idx == 8 || idx == 9)
                    aura.data.integer_3 = 2;
                else if (idx == 2 || idx == 3)
                    aura.data.integer_3 = 3;
                else
                    aura.data.integer_3 = 0;

                aura.data.force_teleport = false;
                aura.data.behaviour_66 = 0xFF;
                aura.data.remove_hair_accessories = 0xFF;

                if (idx == 24 && aura.data.bcs_hair_color == 0xFFFFFFFF) // Case of SSJ blue
                {
                    aura.data.bcs_hair_color = 60;
                }
				else if (idx == 25 && aura.data.bcs_hair_color == 0xFFFFFFFF) // Case of SSJ blue evolution
				{
					aura.data.bcs_hair_color = 67;
				}
                else
                {
                    aura.data.bcs_hair_color = 0xFFFFFFFF;
                }
				
                if (idx == 24 && aura.data.bcs_eyes_color == 0xFFFFFFFF) // Case of SSJ blue
                {
                    aura.data.bcs_eyes_color = 60;
                }
				else if (idx == 25 && aura.data.bcs_eyes_color == 0xFFFFFFFF) // Case of SSJ blue evolution
				{
					aura.data.bcs_eyes_color = 67;
				}
                else
                {
                    aura.data.bcs_eyes_color = 0xFFFFFFFF;
                }
            }
        }

        AuraToGui(aura);
    }
}

bool MainWindow::OpenBcsColorDialog(BcsFile *bcs, const std::string &part_name, uint32_t current_color, uint32_t *out_color)
{
    std::vector<uint32_t> colors;

    if (!bcs->ListPreviewColors(part_name, colors))
    {
        DPRINTF("This bcs doesn't have colors for \"%s\"\n", part_name.c_str());
        return false;
    }

   ColorDialog dialog(colors, current_color, this, true);

    if (dialog.exec())
    {
        uint16_t result = dialog.GetSelection();

        if (result >= colors.size())
        {
            return false; // should happen only if no color selected
        }
        else
        {
            *out_color = result;
            return true;
        }
    }

    return false;
}

void MainWindow::on_auraGetHairColorButton_triggered(QAction *arg1)
{
    uint32_t current_color;
    bool set = false;

    if (ui->auraHairColorEdit->text().isEmpty())
    {
        current_color = 0xFFFFFFFF;
    }
    else
    {
        current_color = (uint32_t) ui->auraHairColorEdit->text().toInt();
    }

    if (arg1 == ui->actionFromHumBcs)
    {
        set = OpenBcsColorDialog(game_hum_bcs, "HAIR_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromHufBcs)
    {
        set = OpenBcsColorDialog(game_huf_bcs, "HAIR_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromBcsFile)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        set = OpenBcsColorDialog(&bcs, "HAIR_", current_color, &current_color);
    }

    if (set)
    {
        ui->auraHairColorEdit->setText(QString("%1").arg((int32_t)current_color));
    }
}

int MainWindow::LinkOrEmbed(X2mFile *cost_x2m)
{
    QMessageBox box(this);
    QPushButton *link_button;
    QPushButton *embed_button;

    QString text = "Do you want to link this costume mod, or embed it?";

    if (cost_x2m)
    {
        text += "\n\nMod name: " + Utils::StdStringToQString(cost_x2m->GetModName(), false);
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

void MainWindow::on_bcsAttachmentSetButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select costume x2m", config.lf_depends_cost, "X2M Files (*.x2m)");

    if (file.isNull())
        return;

    config.lf_depends_cost = file;

    X2mFile cost_x2m;

    if (!cost_x2m.LoadFromFile(Utils::QStringToStdString(file)))
    {
        DPRINTF("Failed to load x2m.\n");
        return;
    }

    if (cost_x2m.GetType() != X2mType::NEW_COSTUME)
    {
        DPRINTF("That x2m is not of new costume type!\n");
        return;
    }

    int ret = LinkOrEmbed(&cost_x2m);
    if (ret == 0)
        return;

    if (x2m->HasSkillCostumeDepend() && x2m->SkillCostumeDependHasAttachment())
    {
        x2m->RemoveSkillCostumeDependAttachment();
    }

    if (!x2m->SetSkillCostumeDepend(&cost_x2m))
    {
        DPRINTF("Failed to set the costume.\n");
        return;
    }

    QString text;

    if (ret > 0)
    {
        if (!x2m->SetSkillCostumeDependAttachment(&cost_x2m))
        {
            DPRINTF("Failed to embed the costume.\n");
            x2m->RemoveSkillCostumeDepend();
            ui->bcsAttachmentEdit->clear();
            ui->bcsAttachmentRemoveButton->setEnabled(false);
            return;
        }

        text = "[EMBEDDED] ";
    }
    else
    {
        text = "[LINKED] ";
    }

    text += Utils::StdStringToQString(cost_x2m.GetModName(), false);

    ui->bcsAttachmentEdit->setText(text);
    ui->bcsAttachmentRemoveButton->setEnabled(true);

    ui->cusPartsetEdit->setText("-1");
    ui->cusPartsetEdit->setEnabled(false);
}

void MainWindow::on_bcsAttachmentRemoveButton_clicked()
{
    if (x2m->HasSkillCostumeDepend())
    {
        if (x2m->SkillCostumeDependHasAttachment())
            x2m->RemoveSkillCostumeDependAttachment();

        x2m->RemoveSkillCostumeDepend();
    }

    ui->bcsAttachmentEdit->clear();
    ui->bcsAttachmentRemoveButton->setEnabled(false);

    ui->cusPartsetEdit->setEnabled(true);
}

void MainWindow::on_bcsBodyCheck_clicked()
{
    bool checked = ui->bcsBodyCheck->isChecked();

    ui->bcsBodyAddButton->setEnabled(checked);
    ui->bcsBodyComboBox->setEnabled(checked);
    ui->bcsBodyEdit->setEnabled(checked);
    ui->bcsBodyRaceComboBox->setEnabled(checked);
    ui->bcsBodyRemoveButton->setEnabled(checked);

    if (checked)
    {
        // Clear combobox first
        prev_body_index = -1;
        ui->bcsBodyComboBox->clear();

        size_t num_body_entries = x2m->GetNumSkillBodies();

        if (num_body_entries == 0)
        {
            X2mBody body(x2m->GetFreeSkillBodyId());
            QString str;

            GuiToBody(body, str);
            x2m->AddSkillBody(body);
            bcs_bodies.push_back(str);
            num_body_entries++;
        }
        else
        {
            bcs_bodies.resize(num_body_entries);

            for (size_t i = 0; i < num_body_entries; i++)
            {
                bcs_bodies[i] = BodyToString(x2m->GetSkillBody(i).body);
            }
        }

        for (size_t i = 0; i < num_body_entries; i++)
        {
            ui->bcsBodyComboBox->addItem(QString("Entry %1").arg(i));
        }

        if (num_body_entries == 1)
        {
            ui->bcsBodyRemoveButton->setDisabled(true);
        }
    }
    else
    {
        while (x2m->GetNumSkillBodies() != 0)
            x2m->RemoveSkillBody(0);

        bcs_bodies.clear();
        ui->bcsBodyId->setText(BODY_ID_TEXT);
    }
}

void MainWindow::on_bcsBodyComboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= x2m->GetNumSkillBodies())
        return;

    if (prev_body_index >= 0 && prev_body_index < x2m->GetNumSkillBodies())
    {
        X2mBody &body = x2m->GetSkillBody(prev_body_index);
        GuiToBody(body, bcs_bodies[prev_body_index]);
    }

    const X2mBody &body = x2m->GetSkillBody(index);
    BodyToGui(body, bcs_bodies[index]);

    prev_body_index = index;
}

void MainWindow::on_bcsBodyAddButton_clicked()
{
    X2mBody body(x2m->GetFreeSkillBodyId());
    QString str;

    int idx = ui->bcsBodyComboBox->currentIndex();
    if (idx >= 0 && idx < x2m->GetNumSkillBodies())
    {
        GuiToBody(body, str);
    }

    size_t pos = x2m->AddSkillBody(body);
    bcs_bodies.push_back(str);

    ui->bcsBodyComboBox->addItem(QString("Entry %1").arg(pos));
    ui->bcsBodyComboBox->setCurrentIndex((int)pos);

    if (x2m->GetNumSkillBodies() > 1)
        ui->bcsBodyRemoveButton->setEnabled(true);
}

void MainWindow::on_bcsBodyRemoveButton_clicked()
{
    int index = ui->bcsBodyComboBox->currentIndex();

    if (index < 0 || index >= x2m->GetNumSkillBodies())
        return;

    x2m->RemoveSkillBody(index);
    bcs_bodies.erase(bcs_bodies.begin()+index);

    if (prev_body_index > index)
        prev_body_index--;
    else
        prev_body_index = -1;

    ui->bcsBodyComboBox->removeItem(index);

    for (int i = 0; i < ui->bcsBodyComboBox->count(); i++)
    {
        ui->bcsBodyComboBox->setItemText(i, QString("Entry %1").arg(i));
    }

    if (x2m->GetNumSkillBodies() == 1)
        ui->bcsBodyRemoveButton->setDisabled(true);
}

void MainWindow::on_bcsBodyEdit_textChanged()
{
    /*int idx = ui->bcsBodyComboBox->currentIndex();
    if (idx < 0 || idx >= bcs_bodies.size())
        return;

    bcs_bodies[idx] = ui->bcsBodyEdit->toPlainText();*/
}

void MainWindow::on_auraGetEyesColorButton_triggered(QAction *arg1)
{
    uint32_t current_color;
    bool set = false;

    if (ui->auraEyesColorEdit->text().isEmpty())
    {
        current_color = 0xFFFFFFFF;
    }
    else
    {
        current_color = (uint32_t) ui->auraEyesColorEdit->text().toInt();
    }

    if (arg1 == ui->actionFromHumBcsEyes)
    {
        set = OpenBcsColorDialog(game_hum_bcs, "eye_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromHufBcsEyes)
    {
        set = OpenBcsColorDialog(game_huf_bcs, "eye_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromNmcBcsEyes)
    {
        set = OpenBcsColorDialog(game_nmc_bcs, "eye_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromFriBcsEyes)
    {
        set = OpenBcsColorDialog(game_fri_bcs, "eye_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromMamBcsEyes)
    {
        set = OpenBcsColorDialog(game_mam_bcs, "eye_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromMafBcsEyes)
    {
        set = OpenBcsColorDialog(game_maf_bcs, "eye_", current_color, &current_color);
    }
    else if (arg1 == ui->actionFromBcsFileEyes)
    {
        QString file = QFileDialog::getOpenFileName(this, "External BCS", config.lf_external_bcs, "BCS Files (*.bcs *.bcs.xml)");

        if (file.isNull())
            return;

        config.lf_external_bcs = file;

        BcsFile bcs;

        if (!bcs.SmartLoad(Utils::QStringToStdString(file)))
        {
            DPRINTF("Failed to load bcs file.\n");
            return;
        }

        set = OpenBcsColorDialog(&bcs, "eye_", current_color, &current_color);
    }

    if (set)
    {
        ui->auraEyesColorEdit->setText(QString("%1").arg((int32_t)current_color));
    }
}

void MainWindow::on_guidCopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->modGuidEdit->text());
}
