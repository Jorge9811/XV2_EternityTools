#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QClipboard>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Xenoverse2.h"
#include "xv2ins_common.h"
#include "Config.h"
#include "debug.h"

#include "waitdialog.h"

#define INTERNAL_DATA "Internal package content"
#define GAME_PREFIX "GAME:///"

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
    ui->actionSave_as->setIcon(ui->mainToolBar->style()->standardIcon(QStyle::SP_DialogSaveButton));

    // Main info tab
    ui->modVersionEdit->setValidator(new QDoubleValidator(this));

    set_debug_level(2);
    QDir::setCurrent(qApp->applicationDirPath());

    Bootstrap(false, false);

    x2m = new X2mFile();
    x2m->SetType(X2mType::NEW_QUEST);

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

    if (x2m->GetType() != X2mType::NEW_QUEST)
    {
        DPRINTF("This kind of x2m is not supported by this editor.\n");
        return false;
    }

    ProcessX2m();
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ProcessShutdown())
        event->accept();
    else
        event->ignore();
}

bool MainWindow::ProcessShutdown()
{
    config.Save();
    return true;
}

void MainWindow::ProcessX2m()
{
    // Info tab
    ui->modNameEdit->setText(Utils::StdStringToQString(x2m->GetModName(), false));
    ui->modAuthorEdit->setText(Utils::StdStringToQString(x2m->GetModAuthor(), false));
    ui->modVersionEdit->setText(QString::number(x2m->GetModVersion()));
    ui->modGuidEdit->setText(Utils::StdStringToQString(x2m->GetModGuid()));

    // Files tab
    if (x2m->QuestDirectoryExists())
    {
        ui->questFilesEdit->setText(INTERNAL_DATA);
        ui->questFilesEdit->setEnabled(false);
    }
    else
    {
        ui->questFilesEdit->setText("");
        ui->questFilesEdit->setEnabled(true);
    }

    if (x2m->AudioDirectoryExists())
    {
        ui->audioFilesEdit->setText(INTERNAL_DATA);
        ui->audioFilesEdit->setEnabled(false);
    }
    else
    {
        ui->audioFilesEdit->setText("");
        ui->audioFilesEdit->setEnabled(true);
    }

    if (x2m->QuestAttachmentExists())
    {
        ui->atmodsEdit->setText(INTERNAL_DATA);
        ui->atmodsEdit->setEnabled(false);
    }
    else
    {
        ui->atmodsEdit->setText("");
        ui->atmodsEdit->setEnabled(true);
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

    // Update to new format
    x2m->SetFormatVersion(x2m->X2M_CURRENT_VERSION);
}

static std::string compiler_output;

static void CompilerOutput(const char *dbg)
{
    compiler_output += dbg;
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

    // Files tab

    // audio must be before quest files

    // audio verify
    if (ui->audioFilesEdit->isEnabled())
    {
        QString audio = ui->audioFilesEdit->text().trimmed();

        if (!audio.isEmpty())
        {
            std::string audio_std = Utils::QStringToStdString(audio);

            if (!Utils::IsEmptyString(audio_std))
            {
                if (!Utils::DirExists(audio_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", audio_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(audio_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", audio_std.c_str());
                    return false;
                }
            }
        }
    }

    // audio build
    QString audio = ui->audioFilesEdit->text().trimmed();

    if (ui->audioFilesEdit->isEnabled())
    {
        std::string audio_std = Utils::QStringToStdString(audio);
        x2m->DeleteAudio();

        if (!Utils::IsEmptyString(audio_std) && x2m->SetAudioDir(audio_std) == 0)
        {
            DPRINTF("Warning: the audio directory doesn't contain any valid files (either no hca or not with correct lang suffixes.\n");
        }
    }


    QString quest_files = ui->questFilesEdit->text().trimmed();
    if (quest_files.isEmpty())
    {
        DPRINTF("[FILES] Quest files cannot be empty.\n");
        return false;
    }

    if (ui->questFilesEdit->isEnabled())
    {
        std::string quest_files_std = Utils::NormalizePath(Utils::QStringToStdString(quest_files));

        if (!Utils::DirExists(quest_files_std))
        {
            DPRINTF("[FILES] Directory %s doesn't exist.\n", quest_files_std.c_str());
            return false;
        }
        else if (Utils::IsDirectoryEmpty(quest_files_std, true))
        {
            DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", quest_files_std.c_str());
            return false;
        }

        std::string qs, cs, ds, ps;
        std::vector<std::string> svec, sfvec;

        if (!Utils::ReadTextFile(Utils::MakePathString(quest_files_std, "quest.x2qs"), qs))
            return false;

        if (!Utils::ReadTextFile(Utils::MakePathString(quest_files_std, "chars.x2qs"), cs))
            return false;

        if (!Utils::ReadTextFile(Utils::MakePathString(quest_files_std, "dialogue.x2qs"), ds))
            return false;

        if (!Utils::ReadTextFile(Utils::MakePathString(quest_files_std, "positions.x2qs"), ps))
            return false;

        for (int i = 0; i < 4; i++)
        {
            std::string spath = Utils::MakePathString(quest_files_std, "script");
            std::string script;

            if (i != 0)
                spath += Utils::ToString(i);

            spath += ".x2qs";

            if (!Utils::ReadTextFile(spath, script, (i == 0)))
            {
                if (i == 0)
                    return false;

                break;
            }

            svec.push_back(script);
            sfvec.push_back(Utils::GetFileNameString(spath));
        }

        qc.SetResourceLoad("", x2m);
        qc.SetTestMode(true);

        compiler_output.clear();
        redirect_dprintf(CompilerOutput);
        bool ret = qc.CompileQuest(qs, "quest.x2qs", cs, "chars.x2qs", ds, "dialogue.x2qs", ps, "positions.x2qs", svec, sfvec);
        redirect_dprintf((RedirectFunc)nullptr);

        if (!ret)
        {
            if (compiler_output.length() > 0)
            {
                DPRINTF("%s", compiler_output.c_str());
            }

            DPRINTF("Quest has compilation errors, cannot be saved.\n");
            return false;
        }

        std::string quest_name = qc.GetCompiledQuestName();
        if (!Utils::BeginsWith(quest_name, "TMQ_", false) && !Utils::BeginsWith(quest_name, "HLQ_", false))
        {
            DPRINTF("The quest is not a parallel (TMQ) or expert (HLQ) one. X2m can only support these types.\n");
            return false;
        }

        if (qc.IsVanilla())
        {
            DPRINTF("\"%s\" is the name of a vanilla quest. You must specify a Quest name that doesn't match any vanilla quest.\n", quest_name.c_str());
            return false;
        }

        if (quest_name.length() > MAX_NEW_QUEST_NAME)
        {
            DPRINTF("The name of the quest object is too big (%Id characters). Installer only allows a max of 12 characters for the quest object name.\n", quest_name.length());
            return false;
        }

        x2m->DeleteQuestDirectory();

        if (!x2m->WriteFile(Utils::MakePathString(X2M_QUEST_DIRECTORY, "quest.x2qs"), (const uint8_t *)qs.c_str(), qs.length()))
            return false;

        if (!x2m->WriteFile(Utils::MakePathString(X2M_QUEST_DIRECTORY, "chars.x2qs"), (const uint8_t *)cs.c_str(), cs.length()))
            return false;

        if (!x2m->WriteFile(Utils::MakePathString(X2M_QUEST_DIRECTORY, "dialogue.x2qs"), (const uint8_t *)ds.c_str(), ds.length()))
            return false;

        if (!x2m->WriteFile(Utils::MakePathString(X2M_QUEST_DIRECTORY, "positions.x2qs"), (const uint8_t *)ps.c_str(), ps.length()))
            return false;

        for (size_t i = 0; i < svec.size(); i++)
        {
            if (!x2m->WriteFile(Utils::MakePathString(X2M_QUEST_DIRECTORY, sfvec[i]), (const uint8_t *)svec[i].c_str(), svec[i].length()))
                return false;
        }
    }

    if (ui->audioFilesEdit->isEnabled())
    {
        QString audio = ui->audioFilesEdit->text().trimmed();

        if (!audio.isEmpty())
        {
            std::string audio_std = Utils::QStringToStdString(audio);

            if (!Utils::IsEmptyString(audio_std))
            {
                if (!Utils::DirExists(audio_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", audio_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(audio_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", audio_std.c_str());
                    return false;
                }
            }
        }
    }

    if (ui->atmodsEdit->isEnabled())
    {
        QString atmods = ui->atmodsEdit->text().trimmed();

        if (!atmods.isEmpty())
        {
            std::string atmods_std = Utils::QStringToStdString(atmods);

            if (!Utils::IsEmptyString(atmods_std))
            {
                if (!Utils::DirExists(atmods_std))
                {
                    DPRINTF("[FILES] Directory %s doesn't exist.\n", atmods_std.c_str());
                    return false;
                }
                else if (Utils::IsDirectoryEmpty(atmods_std, true))
                {
                    DPRINTF("[FILES] Directory %s is either empty or it only contains empty directories!\n", atmods_std.c_str());
                    return false;
                }
            }
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

    return true;
}

bool MainWindow::Build()
{
    // Info
    x2m->SetModName(Utils::QStringToStdString(ui->modNameEdit->text(), false));
    x2m->SetModAuthor(Utils::QStringToStdString(ui->modAuthorEdit->text(), false));
    x2m->SetModVersion(ui->modVersionEdit->text().toFloat());

    // Files tab
    // Quest files -> already done on Verify

    // audio files -> done on Verify

    QString atmods = ui->atmodsEdit->text().trimmed();

    if (ui->atmodsEdit->isEnabled())
    {
        std::string atmods_std = Utils::QStringToStdString(atmods);
        x2m->DeleteQuestAttachments();

        if (!Utils::IsEmptyString(atmods_std) && x2m->SetQuestAttachments(atmods_std) == 0)
        {
            DPRINTF("Warning: the attachments directory doesn't contain any x2m files. There won't be any attachment.\n");
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

    if (new_x2m->GetType() != X2mType::NEW_QUEST)
    {
        DPRINTF("That x2m is not of new quest type!\n");
        delete new_x2m;
        return;
    }

    delete x2m;
    x2m = new_x2m;
    ProcessX2m();
}

void MainWindow::on_actionSave_as_triggered()
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
    QMessageBox box;

    box.setWindowTitle(PROGRAM_NAME_QUESTCREAT);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Icon::Information);
    //box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4").arg(PROGRAM_NAME_QUESTCREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));
    box.setText(QString("%1 v%2 by Eternity<br>Built on %3 %4<br><br>If you liked the program, you can support its development at<br><a href='https://www.patreon.com/eternity_tools'>https://www.patreon.com/eternity_tools</a>").arg(PROGRAM_NAME_QUESTCREAT).arg(PROGRAM_VERSION).arg(__DATE__).arg(__TIME__));


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

void MainWindow::on_guidCopyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->modGuidEdit->text());
}

void MainWindow::on_questFilesButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choose directory that contains the x2qs files", config.ld_quest_import);

    if (dir.isNull())
        return;

    config.ld_quest_import = dir;
    x2m->DeleteQuestDirectory();

    ui->questFilesEdit->setText(dir);
    ui->questFilesEdit->setEnabled(true);
}

void MainWindow::on_audioFilesButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choose directory that contains the hca audio", config.ld_css_audio);

    if (dir.isNull())
        return;

    config.ld_css_audio = dir;
    x2m->DeleteAudio();

    ui->audioFilesEdit->setText(dir);
    ui->audioFilesEdit->setEnabled(true);
}

void MainWindow::on_atmodsButton_clicked()
{
     QString dir = QFileDialog::getExistingDirectory(this, "Choose directory that contains x2m attachments", config.ld_quest_attachments);

    if (dir.isNull())
        return;

    config.ld_quest_attachments = dir;
    x2m->DeleteQuestAttachments();

    ui->atmodsEdit->setText(dir);
    ui->atmodsEdit->setEnabled(true);
}

void MainWindow::on_addDataButton_clicked()
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
                                  "what defeats the philosophy of a new stage mod.\n\n"
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
