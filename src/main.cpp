#include <QApplication>

#include "app.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("mkeeter");
    QCoreApplication::setOrganizationDomain("https://github.com/mkeeter/fstl");
    QCoreApplication::setApplicationName("fdrc");
    QCoreApplication::setApplicationVersion(FDRC_VERSION);
    App a(argc, argv);
    return a.exec();
}
