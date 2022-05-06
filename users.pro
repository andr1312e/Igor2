#-------------------------------------------------
#
# Project created by QtCreator 2021-08-10T14:25:15
#
#-------------------------------------------------

QT += xml network widgets sql concurrent

VERSION = 22.04.22 #YY.MM.DD

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET      = "users"
TEMPLATE    = app
DESTDIR     = $$PWD/../syncProgram
RESOURCES += \
    resources.qrc

CONFIG += no_lflags_merge rtti_off warn_off c++latest strict_c++ precompile_header utf8_source file_copies
CONFIG -= cmdline qml_debug no_include_pwd stl

COPIES += iniFiles
iniFiles.files =$$files(sql.ini)
iniFiles.path =$$PWD/../syncProgram

DEFINES +=                 \
  QT_DISABLE_DEPRECATED_BEFORE=0x050B00 \# отключить API, устаревший в 5.11 или более ранних версиях
  QT_USE_FAST_CONCATENATION\
  QT_DEPRECATED_WARNINGS   \
  QT_USE_FAST_OPERATOR_PLUS\
  DQT_NO_URL_CAST_FROM_STRING\
  QT_USE_STRING_BUILDER \


SOURCES += \
    Admin_GUI/Validator/stringvalidator.cpp \
    Admin_GUI/Wizard/Views/conclusionwizardpage.cpp \
    Admin_GUI/Wizard/Views/intropage.cpp \
    Admin_GUI/Wizard/Views/mywizardpage.cpp \
    Admin_GUI/Wizard/Views/roleappswizardpage.cpp \
    Admin_GUI/Wizard/Views/roleappswizardsubwidget.cpp \
    Admin_GUI/Wizard/Views/userwizardpage.cpp \
    Admin_GUI/Wizard/Views/userwizardwidget.cpp \
    Admin_GUI/Wizard/Views/wizardnavigtionbar.cpp \
    Admin_GUI/Wizard/Services/dependenciesserivce.cpp \
    Admin_GUI/Wizard/Services/usersprogramiconmakingservice.cpp \
    Admin_GUI/Wizard/Services/wizardservice.cpp \
    Admin_GUI/Wizard/Repositories/roleappsandstartupsettingsrepository.cpp \
    Admin_GUI/Wizard/Repositories/usersettingsrepository.cpp \
    Admin_GUI/Views/topleftbar.cpp \
    Logging/appender.cpp \
    Logging/appenderskeleton.cpp \
    Logging/asyncappender.cpp \
    Logging/basicconfigurator.cpp \
    Logging/binaryfileappender.cpp \
    Logging/binarylayout.cpp \
    Logging/binarylogger.cpp \
    Logging/binaryloggingevent.cpp \
    Logging/binarylogstream.cpp \
    Logging/binarytotextlayout.cpp \
    Logging/binarywriterappender.cpp \
    Logging/colorconsoleappender.cpp \
    Logging/consoleappender.cpp \
    Logging/dailyfileappender.cpp \
    Logging/dailyrollingfileappender.cpp \
    Logging/databaseappender.cpp \
    Logging/databaselayout.cpp \
    Logging/fileappender.cpp \
    Logging/helpers/appenderattachable.cpp \
    Logging/helpers/binaryclasslogger.cpp \
    Logging/helpers/classlogger.cpp \
    Logging/helpers/configuratorhelper.cpp \
    Logging/helpers/datetime.cpp \
    Logging/helpers/dispatcher.cpp \
    Logging/helpers/factory.cpp \
    Logging/helpers/initialisationhelper.cpp \
    Logging/helpers/logerror.cpp \
    Logging/helpers/optionconverter.cpp \
    Logging/helpers/patternformatter.cpp \
    Logging/helpers/properties.cpp \
    Logging/hierarchy.cpp \
    Logging/layout.cpp \
    Logging/level.cpp \
    Logging/logger.cpp \
    Logging/loggerrepository.cpp \
    Logging/loggingevent.cpp \
    Logging/logmanager.cpp \
    Logging/logstream.cpp \
    Logging/mainthreadappender.cpp \
    Logging/mdc.cpp \
    Logging/ndc.cpp \
    Logging/patternlayout.cpp \
    Logging/propertyconfigurator.cpp \
    Logging/qmllogger.cpp \
    Logging/rollingbinaryfileappender.cpp \
    Logging/rollingfileappender.cpp \
    Logging/signalappender.cpp \
    Logging/simplelayout.cpp \
    Logging/simpletimelayout.cpp \
    Logging/spi/filter.cpp \
    Logging/systemlogappender.cpp \
    Logging/telnetappender.cpp \
    Logging/ttcclayout.cpp \
    Logging/varia/binaryeventfilter.cpp \
    Logging/varia/debugappender.cpp \
    Logging/varia/denyallfilter.cpp \
    Logging/varia/levelmatchfilter.cpp \
    Logging/varia/levelrangefilter.cpp \
    Logging/varia/listappender.cpp \
    Logging/varia/nullappender.cpp \
    Logging/varia/stringmatchfilter.cpp \
    Logging/wdcappender.cpp \
    Logging/writerappender.cpp \
    Logging/xmllayout.cpp \
    Server/sockettorarm.cpp \
    Services/Terminals/terminal.cpp \
    Services/Sql/sqldatabaseserivce.cpp \
    Services/idesktopservice.cpp \
    Services/roledesktopservice.cpp \
    Services/startuppanelpresenter.cpp \
    Services/userdesktopservice.cpp \
    Styles/Frameless/windowtitlebar.cpp \
    Styles/InteractiveButtons/closewindowbutton.cpp \
    Styles/InteractiveButtons/interactivebuttonbase.cpp \
    Styles/InteractiveButtons/maximizewindowbutton.cpp \
    Styles/InteractiveButtons/minimizewindowbutton.cpp \
    Styles/InteractiveButtons/restorewindowbutton.cpp \
    Styles/Themes/stylechanger.cpp \
    Tray/traymenu.cpp \
    Tray/traymenuitem.cpp \
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
    Services/startuprunnableservice.cpp \
    Server/messagesrepository.cpp \
    program.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/startupdialogwidget.cpp \
    Styles/Frameless/framelesswindow.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.cpp \
    Admin_GUI/Widgets/qtmaterialtextfield.cpp \
    Admin_GUI/Wizard/Views/startupwizard.cpp \
    Tray/tray.cpp \
    Styles/InteractiveButtons/themebutton.cpp \
    Services/Sql/sqladjuster.cpp \
    Services/singleinstancemaker.cpp \
    Admin_GUI/Views/AdditionalSettingPanel/mylistview.cpp \
    Admin_GUI/Wizard/Services/filesandfoldersserivce.cpp

HEADERS += \
    Admin_GUI/Validator/stringvalidator.h \
    Admin_GUI/Wizard/Views/actions.h \
    Admin_GUI/Wizard/Views/conclusionwizardpage.h \
    Admin_GUI/Wizard/Views/intropage.h \
    Admin_GUI/Wizard/Views/mywizardpage.h \
    Admin_GUI/Wizard/Views/roleappswizardpage.h \
    Admin_GUI/Wizard/Views/roleappswizardsubwidget.h \
    Admin_GUI/Wizard/Views/userwizardpage.h \
    Admin_GUI/Wizard/Views/userwizardwidget.h \
    Admin_GUI/Wizard/Views/wizardnavigtionbar.h \
    Admin_GUI/Wizard/Views/wizardpages.h \
    Admin_GUI/Wizard/Services/dependenciesserivce.h \
    Admin_GUI/Wizard/Structs/programfilesstate.h \
    Admin_GUI/Wizard/Repositories/roleappsandstartupsettingsrepository.h \
    Admin_GUI/Wizard/Repositories/usersettingsrepository.h \
    Admin_GUI/Wizard/Services/usersprogramiconmakingservice.h \
    Admin_GUI/Wizard/Services/wizardservice.h \
    Admin_GUI/Views/topleftbar.h \
    Logging/appender.h \
    Logging/appenderskeleton.h \
    Logging/asyncappender.h \
    Logging/basicconfigurator.h \
    Logging/binaryfileappender.h \
    Logging/binarylayout.h \
    Logging/binarylogger.h \
    Logging/binaryloggingevent.h \
    Logging/binarylogstream.h \
    Logging/binarytotextlayout.h \
    Logging/binarywriterappender.h \
    Logging/colorconsoleappender.h \
    Logging/consoleappender.h \
    Logging/dailyfileappender.h \
    Logging/dailyrollingfileappender.h \
    Logging/databaseappender.h \
    Logging/databaselayout.h \
    Logging/fileappender.h \
    Logging/helpers/appenderattachable.h \
    Logging/helpers/binaryclasslogger.h \
    Logging/helpers/classlogger.h \
    Logging/helpers/configuratorhelper.h \
    Logging/helpers/datetime.h \
    Logging/helpers/dispatcher.h \
    Logging/helpers/factory.h \
    Logging/helpers/initialisationhelper.h \
    Logging/helpers/logerror.h \
    Logging/helpers/optionconverter.h \
    Logging/helpers/patternformatter.h \
    Logging/helpers/properties.h \
    Logging/hierarchy.h \
    Logging/layout.h \
    Logging/level.h \
    Logging/log4qt.h \
    Logging/log4qtshared.h \
    Logging/log4qtsharedptr.h \
    Logging/logger.h \
    Logging/loggerrepository.h \
    Logging/loggingevent.h \
    Logging/logmanager.h \
    Logging/logstream.h \
    Logging/mainthreadappender.h \
    Logging/mdc.h \
    Logging/ndc.h \
    Logging/patternlayout.h \
    Logging/propertyconfigurator.h \
    Logging/qmllogger.h \
    Logging/rollingbinaryfileappender.h \
    Logging/rollingfileappender.h \
    Logging/signalappender.h \
    Logging/simplelayout.h \
    Logging/simpletimelayout.h \
    Logging/spi/filter.h \
    Logging/systemlogappender.h \
    Logging/telnetappender.h \
    Logging/ttcclayout.h \
    Logging/varia/binaryeventfilter.h \
    Logging/varia/debugappender.h \
    Logging/varia/denyallfilter.h \
    Logging/varia/levelmatchfilter.h \
    Logging/varia/levelrangefilter.h \
    Logging/varia/listappender.h \
    Logging/varia/nullappender.h \
    Logging/varia/stringmatchfilter.h \
    Logging/wdcappender.h \
    Logging/writerappender.h \
    Logging/xmllayout.h \
    Server/sockettorarm.h \
    Services/Sql/tablecolumnsnames.h \
    Services/Sql/isqlservice.h \
    Services/Sql/sqldatabaseserivce.h \
    Services/idesktopservice.h \
    Services/roledesktopservice.h \
    Services/startuppanelpresenter.h \
    Services/userdesktopservice.h \
    Structs/userstruct.h \
    Structs/programstruct.h \
    Styles/Frameless/windowtitlebar.h \
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
    Admin_GUI/Views/AdditionalSettingPanel/startuppanel.h \
    Admin_GUI/Delegates/filedelegate.h \
    Admin_GUI/Views/admingui.h \
    Admin_GUI/Views/linuxuserslistwidget.h \
    Admin_GUI/Views/SettingsPanel/usereditpanel.h \
    Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/startupdialogwidget.h \
    Admin_GUI/Widgets/qtmaterialtextfield_internal.h \
    Admin_GUI/Widgets/qtmaterialtextfield_p.h \
    Admin_GUI/Widgets/qtmaterialtextfield.h \
    Services/kioskservice.h \
    Services/linuxuserservice.h \
    Services/Terminals/terminal.h \
    Services/startuprunnableservice.h \
    Tray/traymenu.h \
    Tray/traymenuitem.h \
    User_GUI/User_GUI.h \
    Server/DataMessage.h \
    Server/messagesrepository.h \
    functionswithstrings.h \
    program.h \
    Admin_GUI/Wizard/Views/startupwizard.h \
    Tray/tray.h \
    Styles/InteractiveButtons/themebutton.h \
    Structs/themeitem.h \
    Services/Sql/sqladjuster.h \
    Services/singleinstancemaker.h \
    Admin_GUI/Views/AdditionalSettingPanel/mylistview.h \
    Admin_GUI/Wizard/Services/filesandfoldersserivce.h

*-g++* {
  QMAKE_CFLAGS = -pedantic -Wall -Wextra -fno-stack-protector -fno-plt

  QMAKE_CXXFLAGS_DEBUG *= -fsanitize=address,undefined
  QMAKE_LFLAGS_DEBUG *= -fsanitize=address,undefined

  QMAKE_CFLAGS_RELEASE *= -march=native -Ofast -flto -funroll-loops -ffast-math
#-DNDEBUG NS_BLOCK_ASSERTIONS
  QMAKE_CXXFLAGS_RELEASE *= -Ofast -DNDEBUG -fno-stack-protector -fno-plt

  QMAKE_LFLAGS *= -fno-stack-protector -fuse-ld=gold
}

DISTFILES += \
    astyle.astylerc
