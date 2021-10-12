#include "intropage.h"
#include <QDebug>


IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
    , m_document(new QDomDocument())
    , m_file(new QFile())
{
    initUI();
    setWizardTitle();
    insertWidgetsIntoLayout();
    createConnections();
}

IntroPage::~IntroPage()
{
    delete m_document;
    delete m_file;

    delete m_settingsFileLayout;
    delete m_settingsFileInputLayout;
    delete m_mainLayout;

    delete m_topLabel;
    delete m_settingsFileButton;
    delete m_settingsFileLabel;
    delete m_settingsFilePath;
    delete m_faqTitle;
}

int IntroPage::nextId() const
{
    return Page_FirstRole;
}

void IntroPage::setWizardTitle()
{
    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/1.jpg"));
    m_settingsFilePath->setStyleSheet("border: 1px solid black");
}

void IntroPage::initUI()
{
    m_mainLayout=new QVBoxLayout();


    m_topLabel = new QLabel(("Администратор, добро пожаловать в программу управления пользователями РЛС ТИ\n"
                             "Eсли у вас есть готовый файл настроек, нажмите на иконку папки справа сверху, выберите файл"
                             ",нажмите далее. Значения файла можно будет отредактировать позже.Если файла нет, ознакомтесь с кратким руководством и нажмите далее.."
                             ));
    m_topLabel->setWordWrap(true);

    m_settingsFileLayout=new QVBoxLayout();
    m_settingsFileInputLayout=new QHBoxLayout();
    m_settingsFileButton=new QPushButton();
    m_settingsFileButton->setIcon(QIcon(":/images/folderBlack"));
    m_settingsFileButton->setIconSize(QSize(30, 30));
    m_settingsFileButton->setFlat(true);
    m_settingsFileButton->setStyleSheet("border: 0px;");
    m_settingsFileLabel=new QLabel("Путь к файлам настроек:");
    m_settingsFilePath=new QLineEdit();
    m_settingsFilePath->setText("");
    m_faqTitle=new QLabel("<html>Краткое руководство - процесс настройки состоит из шагов:<ul><li>Внесение в базу ФИО Администратора</li><li>Заполнение ролей</li></ul> Заполнение ролей состоит из:<ul><li></li><li>Заполнение доступных программ для запуска на рабочем столе</li><li>Заполнение списка программ которые будут перезапущены</li></ul></html>");
}

void IntroPage::insertWidgetsIntoLayout()
{
    m_settingsFileInputLayout->addWidget(m_settingsFilePath);
    m_settingsFileInputLayout->addWidget(m_settingsFileButton);

    m_settingsFileLayout->addWidget(m_settingsFileLabel);
    m_settingsFileLayout->addLayout(m_settingsFileInputLayout);
    m_settingsFileLayout->setContentsMargins(30, 0, 0, 0);

    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addLayout(m_settingsFileLayout);
    m_mainLayout->addWidget(m_faqTitle);

    setLayout(m_mainLayout);
}

void IntroPage::createConnections()
{
    connect(m_settingsFileButton, &QPushButton::clicked, this, &IntroPage::addSettingsFile);
}

void IntroPage::addSettingsFile()
{
    QPalette palette;
    palette.setColor(QPalette::Base,Qt::black);
    palette.setColor(QPalette::Text,Qt::white);
    m_settingsFilePath->setPalette(palette);
    QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString loadPath = QFileDialog::getOpenFileName(nullptr, "Выберите исполняемый файл", strDesktop, "Файл настроек (*.kunzevo)");
    checkFile(loadPath);
    m_settingsFilePath->setText(loadPath);

}

bool IntroPage::checkFile(QString &file)
{
    m_file->setFileName(file);
    if (m_file->open(QIODevice::ReadOnly))
    {
        QByteArray ar=m_file->readAll();
        m_file->close();
        QByteArray data=ar;
        m_document->setContent(data);
        QDomElement settings= m_document->firstChildElement();
        if (settings.tagName()=="settings")
        {
            QDomNodeList list=settings.childNodes();
            QStringList tagList;
            QStringList checkedList={"USERS", "firstRole", "secondRole", "thirdRole", "fourthRole"};

            for (int i=0; i<settings.childNodes().count(); i++)
            {
                tagList.append(list.at(i).toElement().tagName());
            }
            if (tagList==checkedList)
            {
                int cccc=3;
            }
        }
        qDebug()<< settings.childNodes().count();
        for (int i=0; i<settings.childNodes().count(); i++)
        {
            qDebug()<< settings.childNodes().at(i).toElement().tagName();
        }
        QDomElement first=settings.childNodes().at(1).childNodes().at(0).toElement();
        qDebug()<< first.attribute("rank");
//        if (firstTagName!="Ku")
        int cccc=3;
        return true;
    }
    return false;
}
