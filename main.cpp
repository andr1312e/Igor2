#include "program.h"

int main(int argc, char *argv[])
{
    Program a(argc, argv);
    a.CreateApp();
    return Program::exec();
}
