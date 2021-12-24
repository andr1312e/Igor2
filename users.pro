#-------------------------------------------------
#
# Project created by QtCreator 2021-08-10T14:25:15
#
#-------------------------------------------------

QT += core xml network widgets sql

VERSION = 2.0.5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET      = "53"
TEMPLATE    = app
DESTDIR     = $$PWD/../syncProgram
RESOURCES += \
    resources.qrc
CONFIG += c++latest strict_c strict_c++ precompile_header stl_off utf8_source
CONFIG -= cmdline qml_debug
 no_include_pwd
DEFINES +=                 \
  QT_DISABLE_DEPRECATED_BEFORE=0x050B00 \# отключить API, устаревший в 5.11 или более ранних версиях
  QT_USE_FAST_CONCATENATION\
  QT_DEPRECATED_WARNINGS   \
  QT_USE_FAST_OPERATOR_PLUS\
  DQT_NO_URL_CAST_FROM_STRING\


SOURCES += \
    Admin_GUI/Validator/stringvalidator.cpp \
    Admin_GUI/Views/Wizards/conclusionwizardpage.cpp \
    Admin_GUI/Views/Wizards/intropage.cpp \
    Admin_GUI/Views/Wizards/mywizardpage.cpp \
    Admin_GUI/Views/Wizards/roleappswizardpage.cpp \
    Admin_GUI/Views/Wizards/roleappswizardsubwidget.cpp \
    Admin_GUI/Views/Wizards/userwizardpage.cpp \
    Admin_GUI/Views/Wizards/userwizardwidget.cpp \
    Admin_GUI/Views/topleftbar.cpp \
    Services/Settings/roleappsandstartupsettingsrepository.cpp \
    Services/Settings/usersettingsrepository.cpp \
    Services/Settings/wizardservice.cpp \
    Services/Terminals/linuxsystemeditservice.cpp \
    Services/Terminals/terminal.cpp \
    Services/sqldatabaseserivce.cpp \
    Styles/InteractiveButtons/closewindowbutton.cpp \
    Styles/InteractiveButtons/interactivebuttonbase.cpp \
    Styles/InteractiveButtons/maximizewindowbutton.cpp \
    Styles/InteractiveButtons/minimizewindowbutton.cpp \
    Styles/InteractiveButtons/restorewindowbutton.cpp \
    Styles/Themes/stylechanger.cpp \
    Styles/Themes/theme.cpp \
    functionswithstrings.cpp \
    main.cpp \
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
    Services/linuxuserservice.cpp \
    Services/toolbardisableservice.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/desktoppanel.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/roleeditpanel.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/startuppanel.cpp \
    Admin_GUI/Delegates/filedelegate.cpp \
    Admin_GUI/Views/admingui.cpp \
    Admin_GUI/Views/linuxuserslistwidget.cpp \
    Admin_GUI/Views/SettingsPanel/usereditpanel.cpp \
    Services/startuprepositoryservice.cpp \
    Services/startuprunnableservice.cpp \
    Server/messagesrepository.cpp \
    Server/dataHandler.cpp \
    program.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/startupdialogwidget.cpp \
    Styles/Frameless/framelesswindow.cpp \
    Styles/Frameless/windowdragger.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield.cpp \
    Admin_GUI/Views/Wizards/startupwizard.cpp \
    tray.cpp

HEADERS += \
    Admin_GUI/Validator/stringvalidator.h \
    Admin_GUI/Views/Wizards/actions.h \
    Admin_GUI/Views/Wizards/conclusionwizardpage.h \
    Admin_GUI/Views/Wizards/intropage.h \
    Admin_GUI/Views/Wizards/mywizardpage.h \
    Admin_GUI/Views/Wizards/roleappswizardpage.h \
    Admin_GUI/Views/Wizards/roleappswizardsubwidget.h \
    Admin_GUI/Views/Wizards/userwizardpage.h \
    Admin_GUI/Views/Wizards/userwizardwidget.h \
    Admin_GUI/Views/Wizards/wizardpages.h \
    Admin_GUI/Views/topleftbar.h \
    Services/Settings/programfilesstate.h \
    Services/Settings/roleappsandstartupsettingsrepository.h \
    Services/Settings/usersettingsrepository.h \
    Services/Settings/wizardservice.h \
    Services/Sql/tablecolumnsnames.h \
    Services/Terminals/linuxsystemeditservice.h \
    Services/isqlservice.h \
    Services/sqldatabaseserivce.h \
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
    Styles/Themes/theme.h \
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
    Admin_GUI/Views/AdditionalSettingPanel/startuppanel.h \
    Admin_GUI/Delegates/filedelegate.h \
    Admin_GUI/Views/admingui.h \
    Admin_GUI/Views/linuxuserslistwidget.h \
    Admin_GUI/Views/SettingsPanel/usereditpanel.h \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/startupdialogwidget.h \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.h \
    Admin_GUI/Widgets/qtmaterialtextfield_p.h \
    Admin_GUI/Widgets/qtmaterialtextfield.h \
    Services/fileexplorer.h \
    Services/kioskservice.h \
    Services/linuxuserservice.h \
    Services/Terminals/terminal.h \
    Services/startuprepositoryservice.h \
    Services/startuprunnableservice.h \
    User_GUI/User_GUI.h \
    Server/DataMessage.h \
    Server/messagesrepository.h \
    Server/dataHandler.h \
    functionswithstrings.h \
    program.h \
    Admin_GUI/Views/Wizards/startupwizard.h \
    tray.h

*-g++* {
  QMAKE_CFLAGS = -pedantic -Wall -Wextra -fno-stack-protector -fno-plt

  QMAKE_CXXFLAGS_DEBUG *= -fsanitize=address,undefined
  QMAKE_LFLAGS_DEBUG *= -fsanitize=address,undefined

  QMAKE_CFLAGS_RELEASE *= -Ofast -DNDEBUG NS_BLOCK_ASSERTIONS
  QMAKE_CXXFLAGS_RELEASE *= -Ofast -DNDEBUG -fno-stack-protector -fno-plt

  QMAKE_LFLAGS *= -fno-stack-protector -fuse-ld=gold
}

DISTFILES += \
    astyle.astylerc
