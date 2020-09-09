#include "colordialog.h"
#include "ui_colordialog.h"

#include <QGraphicsDropShadowEffect>
#include "debug.h"

#define ROW_NUM 7

#define MARGIN_LEFT 40
#define MARGIN_TOP  20

#define BUTTON_WIDTH   60
#define BUTTON_HEIGHT  25

#define BUTTON_X_SPACING    10
#define BUTTON_Y_SPACING    10

#define BUTTON_X_STEP   (BUTTON_WIDTH + BUTTON_X_SPACING)
#define BUTTON_Y_STEP   (BUTTON_HEIGHT + BUTTON_Y_SPACING)

#define BLUR_RADIUS 30.0

ColorDialog::ColorDialog(const std::vector<uint32_t> &colors_list, uint16_t selected_color, QWidget *parent, bool disable_no_color) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    previous_selection = 0xFFFF;
    selection = selected_color;

    if (selection >= colors_list.size())
        selection = 0xFFFF;

    int margin_top = MARGIN_TOP;

    if (colors_list.size() <= 98)
        margin_top += BUTTON_Y_STEP;

    for (int i = 0; i < (int)colors_list.size(); i++)
    {
        int x = i % ROW_NUM;
        int y = i / ROW_NUM;

        ColorButton *button = new ColorButton(colors_list[i], this);
        button->setGeometry(MARGIN_LEFT + x*BUTTON_X_STEP, margin_top + y*BUTTON_Y_STEP, BUTTON_WIDTH, BUTTON_HEIGHT);
        button->setProperty("index", QVariant(i));

        QString tooltip = QString("%1").arg(i) + ": #" + QString("%1").arg((uint)colors_list[i], (int)6, (int)16, QChar('0'));
        button->setToolTip(tooltip);

        if (i == selection)
            button->setFocus();

        buttons.push_back(button);
    }

    if (selection == 0xFFFF && !disable_no_color)
    {
        ui->noColorCheck->setChecked(true);
        on_noColorCheck_clicked();
    }

    if (disable_no_color)
    {
        ui->noColorCheck->setDisabled(true);
        ui->noColorCheck->setVisible(false);
    }
}

ColorDialog::~ColorDialog()
{
    delete ui;
}

void ColorDialog::on_noColorCheck_clicked()
{
    bool enable = !ui->noColorCheck->isChecked();

    for (int i = 0; i < buttons.size(); i++)
    {
        ColorButton *button = buttons[i];

        button->setEnabled(enable);

        if (enable)
        {
            if (i == previous_selection)
            {
                button->setFocus();
                previous_selection = 0xFFFF;
            }
        }
    }

    if (!enable)
    {

        previous_selection = selection;
        selection = 0xFFFF;
    }
}

//static std::vector<uint32_t> gradient_animation = { 0xFFFFFF, 0xFFFBE5, 0xFFF7CC, 0xFFF3B2, 0xFFEF99, 0xFFEB7F, 0xFFE766, 0xFFE34C, 0xFFDF33, 0xFFDB19, 0xFFD800};
static std::vector<uint32_t> gradient_animation = { 0x000000, 0x191500, 0x332B00, 0x4C4000, 0x665600, 0x7F6C00, 0x998100, 0xB29700, 0xCCAC00,  0xE5C200, 0xFFD800 };

ColorButton::ColorButton(uint32_t color, QWidget *parent) : QPushButton(parent)
{
    QString style = "QPushButton {background-color: " + Utils::StdStringToQString(Utils::ColorToString(color)) + ";";
    style += "border-color: black; border-width: 1px; border-style: solid; }";

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setOffset(.0);
    effect->setBlurRadius(BLUR_RADIUS);
    effect->setColor(Qt::black);
    setGraphicsEffect(effect);

    setStyleSheet(style);
    update();

    timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer, SIGNAL(timeout()), this, SLOT(onFrameUpdate()));    
    installEventFilter(this);
}

bool ColorButton::eventFilter(QObject *object, QEvent *event)
{
    ColorButton *button = dynamic_cast<ColorButton *>(object);

    if (button != this)
        return false;

    if (event->type() == QEvent::FocusIn)
    {        
        animation_index = 0;
        direction_up = true;
        timer->start();

        ColorDialog *parent = dynamic_cast<ColorDialog *>(button->parent());
        if (parent)
        {
            parent->selection = button->property("index").toUInt();
        }
    }
    else if (event->type() == QEvent::FocusOut)
    {
       timer->stop();

       QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
       effect->setOffset(.0);
       effect->setBlurRadius(BLUR_RADIUS);
       effect->setColor(Qt::black);
       setGraphicsEffect(effect);
    }
    else if (event->type() == QEvent::EnabledChange)
    {
        if (!isEnabled())
        {
            QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
            effect->setColor(Qt::gray);
            setGraphicsEffect(effect);
        }
        else
        {
            if (!hasFocus())
            {
                QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
                effect->setOffset(.0);
                effect->setBlurRadius(BLUR_RADIUS);
                effect->setColor(Qt::black);
                setGraphicsEffect(effect);
            }
        }
    }

    return false;
}

void ColorButton::onFrameUpdate()
{
    QRgb color = gradient_animation[animation_index];

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setOffset(.0);
    effect->setBlurRadius(BLUR_RADIUS);
    effect->setColor(color);
    setGraphicsEffect(effect);

    if (direction_up)
    {
        if (animation_index == gradient_animation.size()-1)
        {
            direction_up = false;
            animation_index--;
        }
        else
        {
            animation_index++;
        }
    }
    else
    {
        if (animation_index == 0)
        {
            direction_up = true;
            animation_index++;
        }
        else
        {
            animation_index--;
        }
    }
}


