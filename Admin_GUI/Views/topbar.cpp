#include "topbar.h"
#include <QDebug>

TopBar::TopBar(QWidget *parent)
   : QWidget(parent)
   , m_iconSize(30)
   , m_oldFontSize(0)
   , m_dateTime(new QDateTime())
   , m_timer(new QTimer())
{
   CreateUI();
   ApplyStyle();
   InsertWidgetsIntoLayouts();
   CreateConnections();
   m_timer->start(1000);
}

TopBar::~TopBar()
{
   delete m_iconsSize;

   delete m_titleFont;
   delete m_timer;
   delete m_dateTime;

   delete m_userInfoLayout;

   delete m_currentUserNameLabel;
   delete m_currentTimeLabel;
   delete m_currentUseRoleLabel;
   delete m_settingsPushButtton;

}

void TopBar::SetData(QString &rank, QString &FCS, QString &role)
{
   m_currentUserNameLabel->setText("Добро пожаловать товарищ:  " + rank + " " + FCS);
   m_currentUseRoleLabel->setText("Ваша роль:  " + role);
}

void TopBar::CreateUI()
{
   m_userInfoLayout = new QHBoxLayout();
   m_userInfoLayout->setSpacing(40);
   m_userInfoLayout->setContentsMargins(10, 10, 10, 10);
   m_currentUserNameLabel = new QLabel();
   m_titleFont = new QFont(m_currentUserNameLabel->font());
   m_currentTimeLabel = new QLabel();
   m_currentUseRoleLabel = new QLabel();
   m_currentUserNameLabel->setFont(*m_titleFont);
   m_currentTimeLabel->setFont(*m_titleFont);
   m_currentUseRoleLabel->setFont(*m_titleFont);

   m_iconsSize = new QSize(m_iconSize, m_iconSize);

   m_settingsPushButtton = new QPushButton();
   m_settingsPushButtton->setCheckable(true);
   m_settingsPushButtton->setChecked(false);
   m_settingsPushButtton->setFlat(true);
   m_settingsPushButtton->setDefault(false);
   m_settingsPushButtton->setAutoDefault(false);
   m_settingsPushButtton->setFocusPolicy(Qt::NoFocus);
   m_settingsPushButtton->setIcon(QIcon(":/images/settingsBlack"));
   m_settingsPushButtton->setIconSize(*m_iconsSize);

   m_themePushButton = new QPushButton();
   m_themePushButton->setCheckable(true);
   m_themePushButton->setChecked(false);
   m_themePushButton->setFlat(true);
   m_themePushButton->setDefault(false);
   m_themePushButton->setAutoDefault(false);
   m_themePushButton->setFocusPolicy(Qt::NoFocus);
   m_themePushButton->setIcon(QIcon(":/images/moon"));
   m_themePushButton->setIconSize(*m_iconsSize);
}

void TopBar::ApplyStyle()
{
   m_settingsPushButtton->setStyleSheet("border: 0px;");
   m_settingsPushButtton->setAutoFillBackground(true);
   QPalette pal = m_settingsPushButtton->palette();
   pal.setColor(QPalette::Button, QColor(Qt::transparent));
   m_settingsPushButtton->setPalette(pal);

   m_themePushButton->setStyleSheet("border: 0px;");
   m_themePushButton->setAutoFillBackground(true);
   QPalette Theme = m_themePushButton->palette();
   Theme.setColor(QPalette::Button, QColor(Qt::transparent));
   m_themePushButton->setPalette(pal);
}

void TopBar::InsertWidgetsIntoLayouts()
{
   m_userInfoLayout->setAlignment(Qt::AlignHCenter);
   m_userInfoLayout->addWidget(m_currentUserNameLabel);
   m_userInfoLayout->addWidget(m_currentTimeLabel);
   m_userInfoLayout->addWidget(m_currentUseRoleLabel);
   m_userInfoLayout->addWidget(m_settingsPushButtton);
   m_userInfoLayout->addWidget(m_themePushButton);

   m_userInfoLayout->setAlignment(m_settingsPushButtton, Qt::AlignLeft);
   m_userInfoLayout->setAlignment(m_themePushButton, Qt::AlignLeft);
   setLayout(m_userInfoLayout);
}

void TopBar::CreateConnections()
{
   connect(m_timer, &QTimer::timeout, this, &TopBar::RefreshDateTime);
   connect(m_settingsPushButtton, &QPushButton::clicked, this, &TopBar::OnSettingsButtonClick);
   connect(m_themePushButton, &QPushButton::clicked, this, &TopBar::OnThemeButtonClick);
}

void TopBar::OnSettingsButtonClick(bool state)
{
   m_settingsSetHidden = state;

   if (m_isWhiteTheme) {
      if (m_settingsSetHidden) {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsBlack"));
      } else {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsWhite"));
      }
   } else {
      if (m_settingsSetHidden) {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsWhite"));
      } else {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsBlack"));
      }
   }

   m_settingsPushButtton->setIconSize(*m_iconsSize);
   Q_EMIT HideAdditionalSettings(m_settingsSetHidden);
}

void TopBar::OnThemeButtonClick(bool state)
{
   m_isWhiteTheme = state;

   if (m_isWhiteTheme) {
      m_themePushButton->setIcon(QIcon(":/images/sun"));

      if (m_settingsSetHidden) {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsBlack"));
      } else {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsWhite"));
      }
   } else {
      m_themePushButton->setIcon(QIcon(":/images/moon"));

      if (m_settingsSetHidden) {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsWhite"));
      } else {
         m_settingsPushButtton->setIcon(QIcon(":/images/settingsBlack"));
      }
   }

   m_themePushButton->setIconSize(*m_iconsSize);
   Q_EMIT ChangeTheme(state);
}

void TopBar::RefreshDateTime()
{
   m_currentTimeLabel->setText(m_dateTime->currentDateTime().toString("Дата: dddd dd MMMM yyyy г.  Время: hh:mm:ss"));
}

bool TopBar::IsNeedToUpdateFontSize(int width)
{
   if (width > 1890) {
      if (m_oldFontSize != 20) {
         m_oldFontSize = 20;
         return true;
      }
   } else if (width > 1785) {
      if (m_oldFontSize != 19) {
         m_oldFontSize = 19;
         return true;
      }
   } else if (width > 1730) {
      if (m_oldFontSize != 18) {
         m_oldFontSize = 18;
         return true;
      }
   } else if (width > 1716) {
      if (m_oldFontSize != 17) {
         m_oldFontSize = 17;
         return true;
      }
   } else if (width > 1545) {
      if (m_oldFontSize != 16) {
         m_oldFontSize = 16;
         return true;
      }
   } else if (width > 1470) {
      if (m_oldFontSize != 15) {
         m_oldFontSize = 15;
         return true;
      }
   } else if (width > 1430) {
      if (m_oldFontSize != 14) {
         m_oldFontSize = 14;
         return true;
      }
   } else if (width > 1330) {
      if (m_oldFontSize != 13) {
         m_oldFontSize = 13;
         return true;
      }
   } else if (width > 1235) {
      if (m_oldFontSize != 12) {
         m_oldFontSize = 12;
         return true;
      }
   } else if (width > 1190) {
      if (m_oldFontSize != 11) {
         m_oldFontSize = 11;
         return true;
      }
   } else if (width > 1080) {
      if (m_oldFontSize != 10) {
         m_oldFontSize = 10;
         return true;
      }
   } else if (width > 990) {
      if (m_oldFontSize != 9) {
         m_oldFontSize = 9;
         return true;
      }
   } else {
      if (m_oldFontSize != 8) {
         m_oldFontSize = 8;
         return true;
      }
   }

   return false;
}

void TopBar::UpdateFontSize()
{
   m_titleFont->setPointSize(m_oldFontSize);
   m_currentUserNameLabel->setFont(*m_titleFont);
   m_currentTimeLabel->setFont(*m_titleFont);
   m_currentUseRoleLabel->setFont(*m_titleFont);
}

void TopBar::resizeEvent(QResizeEvent *event)
{
   //   qDebug() << "topBurSize" << event->size().width() << " fontSize " << m_oldFontSize;

   if (IsNeedToUpdateFontSize(event->size().width())) {
      UpdateFontSize();
   }
}
