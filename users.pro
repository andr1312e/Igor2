#-------------------------------------------------
#
# Project created by QtCreator 2021-08-10T14:25:15
#
#-------------------------------------------------

QT += core xml network widgets

CONFIG += c++11

TARGET = "Панель управления пользователями РЛС ТИ версия 47"
TEMPLATE = app
CONFIG -= console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050B00    # отключить API, устаревший в 5.11 или более ранних версиях


SOURCES += \
    main.cpp \
    Services/integrityservice.cpp \
    User_GUI/User_GUI.cpp \
    Admin_GUI/Model/usermodel.cpp \
    Admin_GUI/Model/sortmodel.cpp \
    Admin_GUI/Delegates/userdelegate.cpp \
    Admin_GUI/Views/qtoast.cpp \
    lib/qtmaterialstyle.cpp \
    lib/qtmaterialtheme.cpp \
    lib/qtmaterialoverlaywidget.cpp \
    lib/qtmaterialcheckable_internal.cpp \
    lib/qtmaterialcheckable.cpp \
    lib/qtmaterialripple.cpp \
    lib/qtmaterialrippleoverlay.cpp \
    lib/qtmaterialstatetransition.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield.cpp \
    Admin_GUI/Widgets/qtmaterialtoggle_internal.cpp \
    Admin_GUI/Widgets/qtmaterialtoggle.cpp \
    Admin_GUI/Widgets/qtmaterialdialog_internal.cpp \
    Admin_GUI/Widgets/qtmaterialdialog.cpp \
    Admin_GUI/Views/filedialogwidget.cpp \
    Services/terminal.cpp \
    Services/fileexplorer.cpp \
    Services/kioskservice.cpp \
    Services/databaseservice.cpp \
    Services/linuxuserservice.cpp \
    Services/identifyservice.cpp \
    Services/toolbardisableservice.cpp \
    Admin_GUI/Views/desktoppanel.cpp \
    Admin_GUI/Views/roleeditpanel.cpp \
    Admin_GUI/Views/additionalsettingspanel.cpp \
    Admin_GUI/Views/userfileseditpanel.cpp \
    Admin_GUI/Views/topbar.cpp \
    Admin_GUI/Views/startuppanel.cpp \
    Admin_GUI/Delegates/filedelegate.cpp \
    Admin_GUI/Views/admingui.cpp \
    Admin_GUI/Views/linuxuserslistwidget.cpp \
    Admin_GUI/Views/userinfopanel.cpp \
    Admin_GUI/Views/usereditpanel.cpp \
    Admin_GUI/Views/settingspanel.cpp \
    Services/startuprepositoryservice.cpp \
    Services/startuprunnableservice.cpp \
    Server/messagesrepository.cpp \
    Server/dataHandler.cpp \
    program.cpp \
    Admin_GUI/Views/startupdialogwidget.cpp \
    Admin_GUI/Widgets/popupdialog.cpp \
    globalfunctions.cpp \
    Services/appsettingsservice.cpp \
    Styles/darkstyle.cpp \
    Styles/framelesswindow.cpp \
    Styles/windowdragger.cpp

HEADERS += \
    User_GUI/User_GUI.h \
    Services/integrityservice.h \
    Structs/userstruct.h \
    Structs/programstruct.h \
    Admin_GUI/Model/usermodel.h \
    Admin_GUI/Model/sortmodel.h \
    Admin_GUI/Delegates/userdelegate.h \
    Admin_GUI/Views/qtoast.h \ 
    lib/qtmaterialstyle_p.h \
    lib/qtmaterialstyle.h \
    lib/qtmaterialtheme_p.h \
    lib/qtmaterialtheme.h \
    lib/qtmaterialoverlaywidget.h \
    lib/qtmaterialcheckable_internal.h \
    lib/qtmaterialcheckable_p.h \
    lib/qtmaterialripple.h \
    lib/qtmaterialrippleoverlay.h \
    lib/qtmaterialcheckable.h \
    lib/qtmaterialstatetransition.h \
    lib/qtmaterialstatetransitionevent.h \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.h \
    Admin_GUI/Widgets/qtmaterialtextfield_p.h \
    Admin_GUI/Widgets/qtmaterialtextfield.h \
    Admin_GUI/Widgets/qtmaterialtoggle_internal.h \
    Admin_GUI/Widgets/qtmaterialtoggle_p.h \
    Admin_GUI/Widgets/qtmaterialtoggle.h \
    Admin_GUI/Widgets/qtmaterialdialog_internal.h \
    Admin_GUI/Widgets/qtmaterialdialog_p.h \
    Admin_GUI/Widgets/qtmaterialdialog.h \
    Admin_GUI/Views/filedialogwidget.h \
    Services/terminal.h \
    Services/fileexplorer.h \
    Services/kioskservice.h \
    Services/databaseservice.h \
    Services/linuxuserservice.h \
    Services/identifyservice.h \
    Services/toolbardisableservice.h \
    Admin_GUI/Views/desktoppanel.h \
    Admin_GUI/Views/roleeditpanel.h \
    Admin_GUI/Views/additionalsettingspanel.h \
    Admin_GUI/Views/userfileseditpanel.h \
    Admin_GUI/Views/topbar.h \
    Admin_GUI/Views/startuppanel.h \
    Admin_GUI/Delegates/filedelegate.h \
    Admin_GUI/Views/admingui.h \
    Admin_GUI/Views/linuxuserslistwidget.h \
    Admin_GUI/Views/userinfopanel.h \
    Admin_GUI/Views/usereditpanel.h \
    Admin_GUI/Views/settingspanel.h \
    Services/startuprepositoryservice.h \
    Services/startuprunnableservice.h \
    Server/DataMessage.h \
    Server/messagesrepository.h \
    Server/dataHandler.h \
    program.h \
    Admin_GUI/Views/startupdialogwidget.h \
    Admin_GUI/Widgets/popupdialog.h \
    globalfunctions.h \
    Services/appsettingsservice.h \
    Styles/darkstyle.h \
    Styles/framelesswindow.h \
    Styles/windowdragger.h

RESOURCES += \
    resources.qrc

