#ifndef X2MCACCOMPONENTSDIALOG_H
#define X2MCACCOMPONENTSDIALOG_H

#include <QDialog>
#include "Xv2SavFile.h"

namespace Ui {
class X2mCacComponentsDialog;
}

class X2mCacComponentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit X2mCacComponentsDialog(uint32_t cac_race, uint32_t x2m_race, QWidget *parent = 0);
    ~X2mCacComponentsDialog();

    bool useBasic() const;
    bool useBodyComponents() const;
    bool keepRace() const;
    bool useCostumes() const;
    bool useSkills() const;
    bool useStats() const;

private:
    Ui::X2mCacComponentsDialog *ui;
};

#endif // X2MCACCOMPONENTSDIALOG_H
