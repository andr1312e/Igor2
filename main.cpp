#include <QDateTime>

#include "program.h"

#include "Logging/logger.h"
#include "Logging/ttcclayout.h"
#include "Logging/consoleappender.h"
#include "Logging/fileappender.h"
#include "Logging/logmanager.h"
#include <QDebug>

void SetupRootLogger();

int main(int argc, char *argv[])
{
    Program a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setDesktopSettingsAware(false);
    QApplication::setOrganizationName(QStringLiteral("KBK"));
    QApplication::setApplicationName(QStringLiteral("UserControl"));
    SetupRootLogger();
    if(a.HasNoRunningInscance())
    {
        a.CreateAndRunApp();
        return Program::exec();
    }
    return 0;
}

void SetupRootLogger()
{
    Log4Qt::Logger* rootLogger = Log4Qt::Logger::rootLogger();
    rootLogger->setLevel(Log4Qt::Level::INFO_INT);
    // Включить обработку сообщения Qt
    Log4Qt::LogManager::setHandleQtMessages(true);

    Log4Qt::TTCCLayout *layout = new Log4Qt::TTCCLayout();
    layout->setName(QStringLiteral("My Layout"));
    layout->activateOptions();

    // Создать консольный аппендер
    Log4Qt::ConsoleAppender *consoleAppender = new Log4Qt::ConsoleAppender(layout, Log4Qt::ConsoleAppender::STDOUT_TARGET);
    consoleAppender->setName(QStringLiteral("Console logger"));
    consoleAppender->activateOptions();

    // Create a file appender
    // Очищаем файл до
    QFile file(QCoreApplication::applicationDirPath() + "/users.log");
    file.open(QFile::WriteOnly|QFile::Truncate);
    Log4Qt::FileAppender *fileAppender = new Log4Qt::FileAppender(layout, QCoreApplication::applicationDirPath() + "/users.log", true);
    fileAppender->setName(QStringLiteral("File logger"));
    fileAppender->activateOptions();

    // Add appender on root logger
    rootLogger->addAppender(consoleAppender);
    rootLogger->addAppender(fileAppender);

    rootLogger->info((QStringLiteral("Логи программы управления пользователями. Начало записи: ") + QDateTime::currentDateTime().toString(Qt::ISODate)));

    if(Log4Qt::LogManager::handleQtMessages())
    {
        qWarning("%s", QString("Обработка сообщений Qt перехватывается и ведется запись логов: Текстовый файл: " + QCoreApplication::applicationDirPath() + "/users.log , а так же в вывод на консоль").toUtf8().constData());
    }
}

