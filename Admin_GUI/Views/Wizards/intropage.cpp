#include "intropage.h"
#include <QDebug>
#include <QMessageBox>


IntroPage::IntroPage(QDomDocument *backupXMLDocument, QWidget *parent)
    : QWizardPage(parent)
    , m_backupXMLDocument(backupXMLDocument)
    , m_backupFile(new QFile())
    , m_checkedList(new QStringList({ "USERS" , "FIRSTROLE" , "SECONDROLE" , "THIRDROME" , "FOURTHROLE"}))
{
    initUI();
    setWizardTitle();
    insertWidgetsIntoLayout();
    createConnections();
}

IntroPage::~IntroPage()
{
    delete m_backupFile;
    delete m_checkedList;

    delete m_backupFileLayout;
    delete m_backupFileLoadLayout;
    delete m_mainLayout;

    delete m_topLabel;
    delete m_backupLoadButton;
    delete m_backupLabel;
    delete m_backupLineEdit;
    delete m_faqLabel;
}

int IntroPage::nextId() const
{
    return Page_FirstRole;
}

void IntroPage::setWizardTitle()
{
    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/1.jpg"));
    m_backupLineEdit->setStyleSheet("border: 1px solid black");
}

void IntroPage::initUI()
{
    m_mainLayout=new QVBoxLayout();


    m_topLabel = new QLabel(("Администратор, добро пожаловать в программу управления пользователями РЛС ТИ\n"
                             "Eсли у вас есть готовый файл настроек, нажмите на иконку папки справа сверху, выберите файл"
                             ",нажмите далее. Значения файла можно будет отредактировать позже.Если файла нет, ознакомтесь с кратким руководством и нажмите далее.."
                             ));
    m_topLabel->setWordWrap(true);

    m_backupFileLayout=new QVBoxLayout();
    m_backupFileLoadLayout=new QHBoxLayout();
    m_backupLoadButton=new QPushButton();
    m_backupLoadButton->setIcon(QIcon(":/images/folderBlack"));
    m_backupLoadButton->setIconSize(QSize(30, 30));
    m_backupLoadButton->setFlat(true);
    m_backupLoadButton->setStyleSheet("border: 0px;");
    m_backupLabel=new QLabel("Путь к файлам настроек:");
    m_backupLineEdit=new QLineEdit();
    m_backupLineEdit->setText("");
    m_faqLabel=new QLabel("<html>Краткое руководство - процесс настройки состоит из шагов:<ul><li>Внесение в базу ФИО Администратора</li><li>Заполнение ролей</li></ul> Заполнение ролей состоит из:<ul><li></li><li>Заполнение доступных программ для запуска на рабочем столе</li><li>Заполнение списка программ которые будут перезапущены</li></ul></html>");
}

void IntroPage::insertWidgetsIntoLayout()
{
    m_backupFileLoadLayout->addWidget(m_backupLineEdit);
    m_backupFileLoadLayout->addWidget(m_backupLoadButton);

    m_backupFileLayout->addWidget(m_backupLabel);
    m_backupFileLayout->addLayout(m_backupFileLoadLayout);
    m_backupFileLayout->setContentsMargins(30, 0, 0, 0);

    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addLayout(m_backupFileLayout);
    m_mainLayout->addWidget(m_faqLabel);

    setLayout(m_mainLayout);
}

void IntroPage::createConnections()
{
    connect(m_backupLoadButton, &QPushButton::clicked, this, &IntroPage::addSettingsFile);
}

void IntroPage::addSettingsFile()
{
    QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString loadPath = QFileDialog::getOpenFileName(nullptr, "Выберите исполняемый файл", strDesktop, "Файл настроек (*.kunzevo)");
    if(checkBackupFile(loadPath))
    {
        m_backupLineEdit->setText(loadPath);
    }
    else
    {
        m_backupLineEdit->clear();
        QMessageBox::critical(this,"Файл настроек неверный","Файл настроек имеет неверную структуру",QMessageBox::Ok);
    }
}

bool IntroPage::checkBackupFile(QString &backupPath)
{
    m_backupFile->setFileName(backupPath);
    if (m_backupFile->open(QIODevice::ReadOnly))
    {
        QByteArray ar=m_backupFile->readAll();
        m_backupFile->close();
        m_backupXMLDocument->setContent(ar);
        QDomElement settings= m_backupXMLDocument->firstChildElement();
        if (settings.tagName()=="settings")
        {
            QDomNodeList list=settings.childNodes();
            QStringList tagList;
            for (int i=0; i<settings.childNodes().count(); i++)
            {
                tagList.append(list.at(i).toElement().tagName());
            }
            if (tagList==(*m_checkedList))
            {
                return true;
            }
        }
    }
    setToBackupXmlDefaultStruct();
    return false;
}

void IntroPage::setToBackupXmlDefaultStruct()
{
    m_backupXMLDocument->clear();
    QDomElement mainElem=m_backupXMLDocument->createElement("settings");
    m_backupXMLDocument->appendChild(mainElem);
    QDomElement settings=m_backupXMLDocument->firstChildElement();
    QDomElement USERS=m_backupXMLDocument->createElement("USERS");
    QDomElement FIRSTROLE=m_backupXMLDocument->createElement("FIRSTROLE");
    QDomElement SECONDROLE=m_backupXMLDocument->createElement("SECONDROLE");
    QDomElement THIRDROME=m_backupXMLDocument->createElement("THIRDROME");
    QDomElement FOURTHROLE=m_backupXMLDocument->createElement("FOURTHROLE");
    settings.appendChild(USERS);
    settings.appendChild(FIRSTROLE);
    settings.appendChild(SECONDROLE);
    settings.appendChild(THIRDROME);
    settings.appendChild(FOURTHROLE);
}
