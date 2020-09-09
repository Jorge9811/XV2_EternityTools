#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    if (!w.Initialize())
        return 0;

    w.show();

    return a.exec();
}
