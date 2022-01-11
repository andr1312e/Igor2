#include "program.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setDesktopSettingsAware(false);
    Program a(argc, argv);
    a.CreateAndRunApp();
    return Program::exec();
}
