#ifndef SERVICES_STARTUPRUNNABLESERVICE_H
#define SERVICES_STARTUPRUNNABLESERVICE_H

#include <QProcess>
#include <QPair>
#include <QStringList>
#include <QObject>
#include <QMetaMethod>

#include "Services/Terminals/terminal.h"


class StartupRunnableService: public QObject
{

   Q_OBJECT

public:

   StartupRunnableService(Terminal *terminal, QObject *parent);

   ~StartupRunnableService();

   bool run(const QString &userName);

signals:

   void noExecApplication(const QString &execPath);

   void programFall();

private:

   Terminal *m_terminal;

   QVector<QProcess *> *m_runnableProcess;

   QStringList *m_processParams;

private:

   QStringList readUserExecFile(const QString &userName);

   bool isAllExecsValid(QStringList &execsList);

   void initProcessStruct(QStringList execsList);

private slots:

   void restartProcess();

};

#endif // SERVICES_STARTUPRUNNABLESERVICE_H
