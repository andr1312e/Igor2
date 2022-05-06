#include <QDateTime>
#include <QDebug>

#include "program.h"
#include "Logging/logger.h"
#include "Logging/ttcclayout.h"
#include "Logging/consoleappender.h"
#include "Logging/fileappender.h"
#include "Logging/logmanager.h"

void SetupRootLogger();

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    qDebug()<< *argv;
    QApplication::setDesktopSettingsAware(false);
    Program program(argc, argv);
    QApplication::setOrganizationName(QStringLiteral("kbk"));
    QApplication::setApplicationName(QStringLiteral("users"));
    SetupRootLogger();
    if(program.HasNoRunningInstance())
    {
        const bool needRestart=program.CreateAndRunApp();
        if(needRestart)
        {
            return 0;
        }
        else
        {
            return program.exec();
        }
    }
}


void SetupRootLogger()
{
    Log4Qt::Logger*  const rootLogger = Log4Qt::Logger::rootLogger();
    rootLogger->setLevel(Log4Qt::Level::INFO_INT);
    // Включить обработку сообщения Qt
    Log4Qt::LogManager::setHandleQtMessages(true);

    Log4Qt::TTCCLayout * const layout = new Log4Qt::TTCCLayout();
    layout->setName(QStringLiteral("My Layout"));
    layout->activateOptions();

    // Создать консольный аппендер
    Log4Qt::ConsoleAppender * const consoleAppender = new Log4Qt::ConsoleAppender(layout, Log4Qt::ConsoleAppender::STDOUT_TARGET);
    consoleAppender->setName(QStringLiteral("Console logger"));
    consoleAppender->activateOptions();

    // Create a file appender
    // Очищаем файл до
    QFile file(QCoreApplication::applicationDirPath() + QStringLiteral("/users.log"));
    file.open(QFile::WriteOnly|QFile::Truncate);
    Log4Qt::FileAppender * const fileAppender = new Log4Qt::FileAppender(layout, QCoreApplication::applicationDirPath() + "/users.log", true);
    fileAppender->setName(QStringLiteral("File logger"));
    fileAppender->activateOptions();

    // Add appender on root logger
    rootLogger->addAppender(consoleAppender);
    rootLogger->addAppender(fileAppender);

    Log4QtInfo((QStringLiteral("Логи программы управления пользователями. Начало записи: ") + QDateTime::currentDateTime().toString(Qt::ISODate)));

    if(Log4Qt::LogManager::handleQtMessages())
    {
        qWarning("%s", QString("Обработка сообщений Qt перехватывается и ведется запись логов: Текстовый файл: " + QCoreApplication::applicationDirPath() + "/users.log , а так же в вывод на консоль").toUtf8().constData());
    }
}

