#ifndef TTBCHAREVENTDIALOG_H
#define TTBCHAREVENTDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class TtbCharEventDialog;
}

class TtbCharEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TtbCharEventDialog(MainWindow *parent, std::string *ch_cms, uint32_t *ch_costume, uint32_t *ch_trans, TtbEventResLL *res, std::vector<X2mCustomAudio> *audios);
    ~TtbCharEventDialog();

private slots:


    void on_eventComboBox_currentIndexChanged(const QString &text);

    void on_subLangComboBox_currentIndexChanged(int index);

    void on_subsEdit_textChanged();

    void on_customCheck_clicked();

    void on_buttonBox_accepted();

    void on_listenButton_clicked();

private:
    Ui::TtbCharEventDialog *ui;

    MainWindow *parent;
    std::string *ch_cms;
    uint32_t *ch_costume;
    uint32_t *ch_trans;
    TtbEventResLL *res;

    std::vector<X2mCustomAudio> *audios;

    QPalette original_box_palette;

    void AddEvent(const std::string &name);

    void FillEventsGame();
    void FillEventsCustom();

    bool Validate();
};

#endif // TTBCHAREVENTDIALOG_H
