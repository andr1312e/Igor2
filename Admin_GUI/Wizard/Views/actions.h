#ifndef ADMIN_GUI_VIEWS_WIZARD_ACTIONS_H
#define ADMIN_GUI_VIEWS_WIZARD_ACTIONS_H
#include <QStringList>

static const QStringList wizardUsersAllAction = {QStringLiteral("Создать пустую базу данных"), QStringLiteral("Восстановить из файла"), QStringLiteral("Оставить старые данные")};
static const QStringList wizardUsersDeleteAndRestoreActions = {QStringLiteral("Создать пустую базу данных"), QStringLiteral("Восстановить из файла")};
static const QStringList wizardUsersDeleteAndOldActions = {QStringLiteral("Создать пустую базу данных"), QStringLiteral("Оставить старые данные")};
static const QStringList wizardUsersDeleteActions = {QStringLiteral("Создать пустую базу данных")};

static const QStringList wizardRolesAllActions = {QStringLiteral("Убрать все файлы из роли"), QStringLiteral("Восстановить из файла"), QStringLiteral("Оставить старые данные")};
static const QStringList wizardRolesDeleteAndRestoreActions = {QStringLiteral("Убрать все файлы из роли"), QStringLiteral("Восстановить из файла")};
static const QStringList wizardRolesDeleteAndOldActions = {QStringLiteral("Убрать все файлы из роли"), QStringLiteral("Оставить старые данные")};
static const QStringList wizardRolesDeleteActions = {QStringLiteral("Убрать все файлы из роли")};

#endif // ADMIN_GUI_VIEWS_WIZARD_ACTIONS_H
