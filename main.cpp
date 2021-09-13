#include "program.h"

int main(int argc, char *argv[])
{
    Program a(argc, argv);
//    if (a.hasNoRunningInscance())
//    {
        a.createApp();
        return a.exec();
//    }
//    else
//    {
//        return 0;
//    }
}
