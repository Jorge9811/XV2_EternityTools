#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QCheckBox>

#include "Xenoverse2.h"
#include "Xv2SavFile.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    bool Initialize();

protected:

    void closeEvent(QCloseEvent *event);

private slots:
    void on_codeButton_clicked();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;

    Xv2SavFile sav;
    XV2Cac *cac;

    QVector<QCheckBox *> costume_checks;

    void GameRequirements();
    bool ProcessShutdown();

    bool ConvertToX2m(const QString &file, const QString &code, const uint8_t *guid, const std::vector<XV2CacSet *> &sets, bool css_voice);
};

#endif // DIALOG_H
