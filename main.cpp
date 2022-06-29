#include <QDateTime>
#include <QDateTime>
#include <QDebug>
#include <iostream>
#include "program.h"
#include "Logging/logger.h"
#include "Logging/ttcclayout.h"
#include "Logging/consoleappender.h"
#include "Logging/fileappender.h"
#include "Logging/logmanager.h"

/**
 * Ставим логгер с проекта Log4qt https://github.com/MEONMedical/Log4Qt
 */
void SetupRootLogger(int argc, char *argv[]);
/**
 * Doxygen используется
 */

bool ConsoleInfoFlasgs(int argc, char *argv[]);

bool NeedToPrintVersion(char *argv[]);

void PrintAppVersion();

void PrintHelp();

int main(int argc, char *argv[])
{
    if (ConsoleInfoFlasgs(argc, argv))
    {
        if (NeedToPrintVersion(argv))
        {
            PrintAppVersion();
        }
        else
        {
            PrintHelp();
        }
    }
    else
    {
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QApplication::setDesktopSettingsAware(false);
        Program program(argc, argv);
        if (QLatin1Literal("root") == program.m_currentUserName)
        {
            QMessageBox::critical(Q_NULLPTR, QStringLiteral("Приложение панель управления пользователями"), "Запуск не должен производится от пользователя root");
        }
        else
        {
            program.setOrganizationName(QLatin1Literal("kbk"));
            program.setApplicationName(QLatin1Literal("users"));
            SetupRootLogger(argc, argv);
            if (program.HasNoRunningInstance())
            {
                const DbConnectionState dbConnectionState = program.CreateAndRunApp();
                if (DbConnectionState::Connected == dbConnectionState)
                {
                    return program.exec();
                }
            }
        }
    }
    return 0;
}

bool ConsoleInfoFlasgs(int argc, char *argv[])
{
    return (argc >= 3 || (2 == argc && argv[1] != QLatin1Literal("--restartDriver") && argv[1] != QLatin1Literal("--restartPostgre") && argv[1] != QLatin1Literal("--reset") && argv[1] != QLatin1Literal("--noGui")));
}

bool NeedToPrintVersion(char *argv[])
{
    return (QLatin1Literal("--version") == argv[1] );
}

void PrintAppVersion()
{
    std::cout << APP_VERSION << ' ' << BUILD_DATE <<  std::endl;
}

void PrintHelp()
{
    std::cout << "Программа \"Панель управления пользователями РЛС ТИ\"" << std::endl;
    std::cout << "Версия программы, дата и время сборки: "; PrintAppVersion();
    std::cout << "Список команд    :(только 1 команда передана может быть)" << std::endl;
    std::cout << "--cacheСlear     : \"Отчистка кэша QSettings и логов\"" << std::endl;
    std::cout << "--h или --help   : \"Вызов справки\"" << std::endl;
    std::cout << "--noGui          : \"Отключить графический интерфейс пользователя\"" << std::endl;
    std::cout << "--restartPostgre : \"Запуск еще одного экземляра программы рядом (отладка только)\"" << std::endl;
    std::cout << "--restartDriver  : \"Запуск еще одного экземляра программы рядом (отладка только)\"" << std::endl;
    std::cout << "--reset          : \"Сброс всех данных в бд и начало первичной инициализации\"" << std::endl;
    std::cout << "--version        : \"Версия ПО\"" << std::endl;
    std::cout << "Запись логов (если не отключена) осуществляется в домашнюю директорию текущего пользователя. Имя файла: users.log" << std::endl;
    std::cout << "Настройка подключения к бд (имя бд,порт,логин,пароль) хрaнятся так же в директории домашнего пользователя. Имя файла: sql.ini" << std::endl;
}

void SetupRootLogger(int argc, char *argv[])
{
    Log4Qt::TTCCLayout *const layout = new Log4Qt::TTCCLayout();
    layout->setName(QLatin1Literal("My Layout"));
    layout->activateOptions();

    // Создать консольный аппендер
    Log4Qt::ConsoleAppender *const consoleAppender = new Log4Qt::ConsoleAppender(layout, Log4Qt::ConsoleAppender::STDOUT_TARGET);
    consoleAppender->setName(QLatin1Literal("Console logger"));
    consoleAppender->activateOptions();

    // Create a file appender
    // Очищаем файл до
    const QString currentdateTime=QTime::currentTime().toString(QLatin1Literal("hh::mm::ss"));
    const QString loggerPath = QDir::homePath()+'/' +currentdateTime+ QStringLiteral("users.log");
    QFile logFile(loggerPath);
    if(2==argc && QLatin1Literal("--cacheСlear")==argv[1])
    {
        if(logFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            logFile.close();
        }
    }
    Log4Qt::FileAppender *const fileAppender = new Log4Qt::FileAppender(layout, loggerPath, true);
    fileAppender->setName(QLatin1Literal("File logger"));
    fileAppender->activateOptions();

    // Добавить аппенд
    Log4Qt::Logger  *const rootLogger = Log4Qt::Logger::rootLogger();
    rootLogger->addAppender(consoleAppender);
    rootLogger->addAppender(fileAppender);
    rootLogger->setLevel(Log4Qt::Level::INFO_INT);
    Log4Qt::LogManager::setHandleQtMessages(true);

    Log4QtInfo((QStringLiteral("Логи программы управления пользователями. Начало записи: ") + QDateTime::currentDateTime().toString(Qt::ISODate)));

    if (Log4Qt::LogManager::handleQtMessages())
    {
        qWarning("%s", QString("Обработка сообщений Qt перехватывается и ведется запись логов: Текстовый файл: " + QCoreApplication::applicationDirPath() + "/users.log , а так же в вывод на консоль").toUtf8().constData());
    }
}

