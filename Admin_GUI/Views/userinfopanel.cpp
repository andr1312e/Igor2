#include "userinfopanel.h"

UserInfoPanel::UserInfoPanel(Terminal *terminal, QWidget *parent)
    : QWidget(parent)
{
    initServices(terminal);
    initUI();
    insertWidgetsIntoLayout();
}

UserInfoPanel::~UserInfoPanel()
{
    delete m_integrityService;

    delete m_roleLayout;
    delete m_rankLayout;
    delete m_FCSLayout;
    delete m_userIntegrityLevelLayout;
    delete m_fileIntegrityLevelLayout;
    delete m_userNameLayout;
    delete m_userIdLayout;
    delete m_mainLayout;

    delete m_userNameLabel;
    delete m_userNameValue;
    delete m_userIdLabel;
    delete m_userIdValue;
    delete m_isInDBExistLabel;
    delete m_FCSLabel;
    delete m_FCSValue;
    delete m_rankLabel;
    delete m_rankValue;
    delete m_roleLabel;
    delete m_roleValue;

    delete m_fileIntegrityLevelLabel;
    delete m_fileIntegrityLevelValue;
    delete m_userIntegrityLevelLabel;
    delete m_userIntegrityLevelValue;
    delete m_canUserEditFile;

}

void UserInfoPanel::setUser(User &user)
{
    setBasicInfo(user.userId, user.name);
    if (user.hasData)
    {
        fillUserAdditionalInfo(user.FCS, user.rank, user.role);
    }
    else
    {
        clearAdditionalInfo();
    }
}

void UserInfoPanel::initServices(Terminal *terminal)
{
    m_integrityService=new IntegrityService(terminal);
}


void UserInfoPanel::initUI()
{
    m_mainLayout=new QVBoxLayout();

    m_userIdLayout=new QHBoxLayout();
    m_userIdLabel=new QLabel("Идентификатор выбранного пользователя: ");
    m_userIdLabel->setStyleSheet("font-weight: bold;");
    m_userIdValue=new QLabel("");

    m_userNameLayout=new QHBoxLayout();
    m_userNameLabel=new QLabel("Имя выбранного пользователя в системе: ");
    m_userNameValue=new QLabel("");

    m_fileIntegrityLevelLayout=new QHBoxLayout();
    m_fileIntegrityLevelLabel=new QLabel("Уровень целостности файла:");
    m_fileIntegrityLevelLabel->setToolTip("Атрибут уровень целостности отвечает за то, чтобы информацию не могли изменять те, кому не положено её изменять. Нажмите для настройки или более подробного пояснения...");
    m_fileIntegrityLevelValue=new QLabel("");

    m_userIntegrityLevelLayout=new QHBoxLayout();
    m_userIntegrityLevelLabel=new QLabel("Уровень целостности пользователя:");
    m_userIntegrityLevelLabel->setToolTip("Атрибут уровень целостности отвечает за то, чтобы информацию не могли изменять те, кому не положено её изменять. Нажмите для настройки или более подробного пояснения...");
    m_userIntegrityLevelValue=new QLabel("");

    m_canUserEditFile=new QLabel("");

    m_isInDBExistLabel=new QLabel("");

    m_FCSLayout=new QHBoxLayout();
    m_FCSLabel=new QLabel("ФИО: ");
    m_FCSValue=new QLabel("");

    m_rankLayout=new QHBoxLayout();
    m_rankLabel=new QLabel("Звание выбранного пользователя: ");
    m_rankValue=new QLabel();

    m_roleLayout=new QHBoxLayout();
    m_roleLabel=new QLabel("Роль выбранного пользователя: ");
    m_roleValue=new QLabel("");
}

void UserInfoPanel::insertWidgetsIntoLayout()
{
    m_userIdLayout->addWidget(m_userIdLabel);
    m_userIdLayout->addStretch(3);
    m_userIdLayout->addWidget(m_userIdValue);
    m_mainLayout->addLayout(m_userIdLayout);

    m_userNameLayout->addWidget(m_userNameLabel);
    m_userNameLayout->addStretch(3);
    m_userNameLayout->addWidget(m_userNameValue);
    m_mainLayout->addLayout(m_userNameLayout);

    m_fileIntegrityLevelLayout->addWidget(m_fileIntegrityLevelLabel);
    m_fileIntegrityLevelLayout->addStretch(3);
    m_fileIntegrityLevelLayout->addWidget(m_fileIntegrityLevelValue);
    m_mainLayout->addLayout(m_fileIntegrityLevelLayout);

    m_userIntegrityLevelLayout->addWidget(m_userIntegrityLevelLabel);
    m_userIntegrityLevelLayout->addStretch(3);
    m_userIntegrityLevelLayout->addWidget(m_userIntegrityLevelValue);
    m_mainLayout->addLayout(m_userIntegrityLevelLayout);

    m_mainLayout->addWidget(m_canUserEditFile);

    m_mainLayout->addWidget(m_isInDBExistLabel);

    m_FCSLayout->addWidget(m_FCSLabel);
    m_FCSLayout->addStretch(3);
    m_FCSLayout->addWidget(m_FCSValue);
    m_mainLayout->addLayout(m_FCSLayout);

    m_rankLayout->addWidget(m_rankLabel);
    m_rankLayout->addStretch(3);
    m_rankLayout->addWidget(m_rankValue);
    m_mainLayout->addLayout(m_rankLayout);

    m_roleLayout->addWidget(m_roleLabel);
    m_roleLayout->addStretch(3);
    m_roleLayout->addWidget(m_roleValue);

    m_mainLayout->addLayout(m_roleLayout);

    setLayout(m_mainLayout);
}

void UserInfoPanel::setBasicInfo(QString &userId, QString userName)
{
    m_userNameValue->setText(userName);
    m_userIdValue->setText(userId);

    QString userIngerityLevel=m_integrityService->getUserIngerityLevel(userName);
    QString fileIngerityLevel=m_integrityService->getFileIntegrityLevel(m_repositoryPath);

    m_userIntegrityLevelValue->setText(userIngerityLevel);
    m_fileIntegrityLevelValue->setText(fileIngerityLevel);
    bool canUserEditFile=userIngerityLevel.toInt() >= fileIngerityLevel.toInt();
    if (canUserEditFile)
    {

        m_canUserEditFile->setText("Данный пользователь <b>МОЖЕТ</b> вносить изменения в файл базы");
    }
    else
    {
        m_canUserEditFile->setText("Данный пользователь <b>НЕ может</b> вносить изменения в файл базы!");
    }
}

void UserInfoPanel::fillUserAdditionalInfo(const QString &FCs, const QString &rank, const QString &role)
{
    m_isInDBExistLabel->setText("Пользователь с ид: " + m_userIdValue->text() + " был успешно загружен");
    m_FCSValue->setText(FCs);
    m_rankValue->setText(rank);
    m_roleValue->setText(role);
}

void UserInfoPanel::clearAdditionalInfo()
{
    m_isInDBExistLabel->setText("Пользователь с ид: " + m_userIdValue->text() + " в базе не хранится");
    m_FCSValue->setText("Данные отсутствуют");
    m_rankValue->setText("Данные отсутствуют");
    m_roleValue->setText("Данные отсутствуют");
}
