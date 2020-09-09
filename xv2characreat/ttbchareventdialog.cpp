#include "ttbchareventdialog.h"
#include "ui_ttbchareventdialog.h"
#include "Config.h"

TtbCharEventDialog::TtbCharEventDialog(MainWindow *parent, std::string *ch_cms, uint32_t *ch_costume, uint32_t *ch_trans, TtbEventResLL *res, std::vector<X2mCustomAudio> *audios) :
    QDialog(parent),
    ui(new Ui::TtbCharEventDialog),
    parent(parent),
    ch_cms(ch_cms),
    ch_costume(ch_costume),
    ch_trans(ch_trans),
    res(res),
    audios(audios)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->costumeEdit->setValidator(new QIntValidator(this));
    ui->transEdit->setValidator(new QIntValidator(this));
    ui->listenLangComboBox->setCurrentIndex(config.listen_sev_language_english ? 1 : 0);

    if (audios->size() == 0)
    {
        ui->customCheck->setEnabled(false);
    }

    ui->charEdit->setText(Utils::StdStringToQString(*ch_cms, false));
    ui->costumeEdit->setText(QString("%1").arg((int32_t)*ch_costume));
    ui->transEdit->setText(QString("%1").arg((int32_t)*ch_trans));

    bool custom = false;
    int index = 0;

    if (Utils::BeginsWith(res->name, "X2T_"))
    {
        for (int i = 0; i < (int)audios->size(); i++)
        {
            const X2mCustomAudio &audio = (*audios)[i];
            std::string audio_name = res->name.substr(4);

            if (audio_name == audio.name)
            {
                index = i;
                custom = true;
                break;
            }
        }
    }

    if (custom)
    {
        FillEventsCustom();
        ui->eventLabel->setText("Audio:");
        ui->customCheck->setChecked(true);
    }
    else
    {
        FillEventsGame();

        for (int i = 0; i < ui->eventComboBox->count(); i++)
        {
            std::string text = Utils::QStringToStdString(ui->eventComboBox->itemText(i));

            if (text == res->name)
            {
                index = i;
                break;
            }
        }
    }

    ui->eventComboBox->setCurrentIndex(index);
    original_box_palette = ui->subsEdit->palette();

    if (custom)
    {
        ui->subsEdit->setText(Utils::StdStringToQString(res->subtitle[XV2_LANG_ENGLISH], false));
    }
    else
    {
        ui->subsEdit->setReadOnly(true);

        QPalette palette = original_box_palette;
        palette.setColor(QPalette::Base, palette.color(QPalette::Button));
        ui->subsEdit->setPalette(palette);

        std::string event_name = Utils::QStringToStdString(ui->eventComboBox->currentText(), false);
        std::string subtitle;

        if (Xenoverse2::GetTtbSubtitle(event_name, subtitle))
        {
            ui->subsEdit->setText(Utils::StdStringToQString(subtitle, false));
        }
    }
}

TtbCharEventDialog::~TtbCharEventDialog()
{
    delete ui;
}

void TtbCharEventDialog::AddEvent(const std::string &name)
{
    QString qname = Utils::StdStringToQString(name, false);

    if (ui->eventComboBox->findText(qname) == -1)
        ui->eventComboBox->addItem(qname);
}


void TtbCharEventDialog::FillEventsGame()
{
    ui->eventComboBox->clear();

    for (const TtbEntry &entry : *game_ttb)
    {
        if (entry.cms_id >= XV2_FREE_ID_SEARCH_START)
            continue;

        for (const TtbEvent &event : entry.events)
        {
            AddEvent(event.voice_name);

            if (event.cms2_id < XV2_FREE_ID_SEARCH_START)
                AddEvent(event.voice2_name);

            if (event.cms3_id < XV2_FREE_ID_SEARCH_START)
                AddEvent(event.voice3_name);

            if (event.cms4_id < XV2_FREE_ID_SEARCH_START)
                AddEvent(event.voice4_name);

            if (event.cms5_id < XV2_FREE_ID_SEARCH_START)
                AddEvent(event.voice5_name);
        }
    }
}

void TtbCharEventDialog::FillEventsCustom()
{
    ui->eventComboBox->clear();

    for (const X2mCustomAudio &audio : *audios)
    {
        AddEvent(audio.name);
    }
}

bool TtbCharEventDialog::Validate()
{
    std::string cms = Utils::QStringToStdString(ui->charEdit->text().trimmed(), false);

    if (cms.length() != 0 && cms.length() != 3)
    {
        DPRINTF("Char must be a 3-letter code, or empty to refer to this custom char.\n");
        return false;
    }

    if (cms.length() > 0 && !Utils::IsAlphaNumeric(cms))
    {
        DPRINTF("Char can only have alphanumeric characters.\n");
        return false;
    }

    if (ui->costumeEdit->text().trimmed().isEmpty())
    {
        DPRINTF("Costume cannot be empty.\n");
        return false;
    }

    if (ui->transEdit->text().trimmed().isEmpty())
    {
        DPRINTF("Trans cannot be empty.\n");
        return false;
    }

    if (ui->customCheck->isChecked())
    {
        if (Utils::IsEmptyString(res->subtitle[XV2_LANG_ENGLISH]))
        {
            DPRINTF("English subtitle cannot be empty.\n");
            return false;
        }
    }

    return true;
}

void TtbCharEventDialog::on_eventComboBox_currentIndexChanged(const QString &text)
{
    if (ui->customCheck->isChecked())
        return;

    int lang = ui->subLangComboBox->currentIndex();
    std::string subtitle;

    Xenoverse2::GetTtbSubtitle(Utils::QStringToStdString(text, false), subtitle, lang);
    ui->subsEdit->setText(Utils::StdStringToQString(subtitle, false));
}

void TtbCharEventDialog::on_subLangComboBox_currentIndexChanged(int index)
{
    if (ui->customCheck->isChecked())
    {
        ui->subsEdit->setText(Utils::StdStringToQString(res->subtitle[index], false));
    }
    else
    {
        on_eventComboBox_currentIndexChanged(ui->eventComboBox->currentText());
    }
}

void TtbCharEventDialog::on_subsEdit_textChanged()
{
    if (!ui->customCheck->isChecked())
        return;

    int lang = ui->subLangComboBox->currentIndex();
    res->subtitle[lang] = Utils::QStringToStdString(ui->subsEdit->toPlainText(), false);
}

void TtbCharEventDialog::on_customCheck_clicked()
{
    if (ui->customCheck->isChecked())
    {
        ui->subsEdit->setReadOnly(false);
        ui->subsEdit->setPalette(original_box_palette);

        FillEventsCustom();
        ui->eventLabel->setText("Audio:");
    }
    else
    {
        ui->subsEdit->setReadOnly(true);

        QPalette palette = original_box_palette;
        palette.setColor(QPalette::Base, palette.color(QPalette::Button));
        ui->subsEdit->setPalette(palette);

        FillEventsGame();
        ui->eventLabel->setText("Event:");
    }

    on_subLangComboBox_currentIndexChanged(ui->subLangComboBox->currentIndex());
}

void TtbCharEventDialog::on_buttonBox_accepted()
{
    if (!Validate())
        return;

    *ch_cms = Utils::QStringToStdString(ui->charEdit->text().trimmed(), false);
    *ch_costume = (uint32_t) ui->costumeEdit->text().toInt();
    *ch_trans = (uint32_t) ui->transEdit->text().toInt();

    if (ui->customCheck->isChecked())
    {
        res->name = "X2T_";
    }
    else
    {
        res->Clear();
    }

    res->name += Utils::QStringToStdString(ui->eventComboBox->currentText(), false);
    config.listen_sev_language_english = ui->listenLangComboBox->currentIndex();
    accept();
}


void TtbCharEventDialog::on_listenButton_clicked()
{
    std::string event;

    if (ui->customCheck->isChecked())
        event = "X2T_";

    event += Utils::QStringToStdString(ui->eventComboBox->currentText(), false);
    parent->PlayTtbAudio(event, ui->listenLangComboBox->currentIndex() == 0);
    config.listen_sev_language_english = ui->listenLangComboBox->currentIndex();
}
