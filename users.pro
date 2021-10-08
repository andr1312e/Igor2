#-------------------------------------------------
#
# Project created by QtCreator 2021-08-10T14:25:15
#
#-------------------------------------------------

QT += core xml network widgets

CONFIG += c++11
#Панель управления пользователями РЛС ТИ версия
TARGET = "49"
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
    Services/Terminals/terminal.cpp \
    Styles/InteractiveButtons/closewindowbutton.cpp \
    Styles/InteractiveButtons/interactivebuttonbase.cpp \
    Styles/InteractiveButtons/maximizewindowbutton.cpp \
    Styles/InteractiveButtons/minimizewindowbutton.cpp \
    Styles/InteractiveButtons/restorewindowbutton.cpp \
    Styles/Themes/stylechanger.cpp \
    main.cpp \
    Services/integrityservice.cpp \
    User_GUI/User_GUI.cpp \
    Admin_GUI/Model/usermodel.cpp \
    Admin_GUI/Model/sortmodel.cpp \
    Admin_GUI/Delegates/userdelegate.cpp \
    Admin_GUI/Views/qtoast.cpp \
    Styles/MaterialWidgets/qtmaterialstyle.cpp \
    Styles/MaterialWidgets/qtmaterialtheme.cpp \
    Styles/MaterialWidgets/qtmaterialoverlaywidget.cpp \
    Styles/MaterialWidgets/qtmaterialcheckable_internal.cpp \
    Styles/MaterialWidgets/qtmaterialcheckable.cpp \
    Styles/MaterialWidgets/qtmaterialripple.cpp \
    Styles/MaterialWidgets/qtmaterialrippleoverlay.cpp \
    Styles/MaterialWidgets/qtmaterialstatetransition.cpp \
    Admin_GUI/Widgets/qtmaterialtoggle_internal.cpp \
    Admin_GUI/Widgets/qtmaterialtoggle.cpp \
    Admin_GUI/Widgets/qtmaterialdialog_internal.cpp \
    Admin_GUI/Widgets/qtmaterialdialog.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/filedialogwidget.cpp \
    Services/fileexplorer.cpp \
    Services/kioskservice.cpp \
    Services/databaseservice.cpp \
    Services/linuxuserservice.cpp \
    Services/identifyservice.cpp \
    Services/toolbardisableservice.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/desktoppanel.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/roleeditpanel.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/additionalsettingspanel.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/userfileseditpanel.cpp \
    Admin_GUI/Views/topbar.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/startuppanel.cpp \
    Admin_GUI/Delegates/filedelegate.cpp \
    Admin_GUI/Views/admingui.cpp \
    Admin_GUI/Views/linuxuserslistwidget.cpp \
    Admin_GUI/Views/SettingsPanel/userinfopanel.cpp \
    Admin_GUI/Views/SettingsPanel/usereditpanel.cpp \
    Admin_GUI/Views/SettingsPanel/settingspanel.cpp \
    Services/startuprepositoryservice.cpp \
    Services/startuprunnableservice.cpp \
    Server/messagesrepository.cpp \
    Server/dataHandler.cpp \
    program.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/startupdialogwidget.cpp \
    globalfunctions.cpp \
    Services/appsettingsservice.cpp \
    Styles/Frameless/framelesswindow.cpp \
    Styles/Frameless/windowdragger.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield.cpp

HEADERS += \
    Structs/userstruct.h \
    Structs/programstruct.h \
    Styles/InteractiveButtons/closewindowbutton.h \
    Styles/InteractiveButtons/interactivebuttonbase.h \
    Styles/InteractiveButtons/maximizewindowbutton.h \
    Styles/InteractiveButtons/minimizewindowbutton.h \
    Styles/InteractiveButtons/restorewindowbutton.h \
    Styles/MaterialWidgets/qtmaterialstyle_p.h \
    Styles/MaterialWidgets/qtmaterialstyle.h \
    Styles/MaterialWidgets/qtmaterialtheme_p.h \
    Styles/MaterialWidgets/qtmaterialtheme.h \
    Styles/MaterialWidgets/qtmaterialoverlaywidget.h \
    Styles/MaterialWidgets/qtmaterialcheckable_internal.h \
    Styles/MaterialWidgets/qtmaterialcheckable_p.h \
    Styles/MaterialWidgets/qtmaterialripple.h \
    Styles/MaterialWidgets/qtmaterialrippleoverlay.h \
    Styles/MaterialWidgets/qtmaterialcheckable.h \
    Styles/MaterialWidgets/qtmaterialstatetransition.h \
    Styles/MaterialWidgets/qtmaterialstatetransitionevent.h \
    Styles/Frameless/framelesswindow.h \
    Styles/Frameless/windowdragger.h \
    Admin_GUI/Model/usermodel.h \
    Admin_GUI/Model/sortmodel.h \
    Admin_GUI/Delegates/userdelegate.h \
    Admin_GUI/Views/qtoast.h \  \
    Styles/Themes/stylechanger.h \
    Styles/Themes/themecolors.h \
    Admin_GUI/Widgets/qtmaterialtoggle_internal.h \
    Admin_GUI/Widgets/qtmaterialtoggle_p.h \
    Admin_GUI/Widgets/qtmaterialtoggle.h \
    Admin_GUI/Widgets/qtmaterialdialog_internal.h \
    Admin_GUI/Widgets/qtmaterialdialog_p.h \
    Admin_GUI/Widgets/qtmaterialdialog.h \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/filedialogwidget.h \
    Admin_GUI/Views/AdditionalSettingPanel/desktoppanel.h \
    Admin_GUI/Views/AdditionalSettingPanel/roleeditpanel.h \
    Admin_GUI/Views/AdditionalSettingPanel/additionalsettingspanel.h \
    Admin_GUI/Views/AdditionalSettingPanel/userfileseditpanel.h \
    Admin_GUI/Views/topbar.h \
    Admin_GUI/Views/AdditionalSettingPanel/startuppanel.h \
    Admin_GUI/Delegates/filedelegate.h \
    Admin_GUI/Views/admingui.h \
    Admin_GUI/Views/linuxuserslistwidget.h \
    Admin_GUI/Views/SettingsPanel/userinfopanel.h \
    Admin_GUI/Views/SettingsPanel/usereditpanel.h \
    Admin_GUI/Views/SettingsPanel/settingspanel.h \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/startupdialogwidget.h \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.h \
    Admin_GUI/Widgets/qtmaterialtextfield_p.h \
    Admin_GUI/Widgets/qtmaterialtextfield.h \
    Services/fileexplorer.h \
    Services/kioskservice.h \
    Services/databaseservice.h \
    Services/linuxuserservice.h \
    Services/identifyservice.h \
    Services/Terminals/terminal.h \
    Services/integrityservice.h \
    Services/startuprepositoryservice.h \
    Services/startuprunnableservice.h \
    Services/appsettingsservice.h \
    User_GUI/User_GUI.h \
    Server/DataMessage.h \
    Server/messagesrepository.h \
    Server/dataHandler.h \
    globalfunctions.h \
    program.h

RESOURCES += \
    resources.qrc

