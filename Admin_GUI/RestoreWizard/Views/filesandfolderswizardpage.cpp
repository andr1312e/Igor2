#include "filesandfolderswizardpage.h"

#include <QDir>

FilesAndFoldersWizardPage::FilesAndFoldersWizardPage(const QString currentUserName, FilesAndFoldersSerivce *service, QWidget *parent)
    : MyWizardPage(parent)
    , m_service(service)
    , m_currentUserName(currentUserName)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

FilesAndFoldersWizardPage::~FilesAndFoldersWizardPage()
{
    delete m_topLabel;

    delete m_filesLabel;
    delete m_filesListWidget;
    delete m_addFileButton;

    delete m_foldersLabel;
    delete m_foldersTableWidget;
    delete m_addFolderButton;
}

void FilesAndFoldersWizardPage::CreateUI()
{
    m_topLabel=new QLabel();

    m_filesLabel=new QLabel();
    m_filesListWidget=new QListWidget();
    m_addFileButton=new QPushButton();

    m_foldersLabel=new QLabel();
    m_foldersTableWidget=new QTableWidget();
    m_addFolderButton=new QPushButton();
}

void FilesAndFoldersWizardPage::InsertWidgetsIntoLayout()
{
    MainLayout()->addWidget(m_topLabel, Qt::AlignHCenter);
    MainLayout()->addWidget(m_filesLabel);
    MainLayout()->addWidget(m_filesListWidget);
    MainLayout()->addWidget(m_addFileButton);

    MainLayout()->addWidget(m_foldersLabel);
    MainLayout()->addWidget(m_foldersTableWidget);
    MainLayout()->addWidget(m_addFolderButton);
}

void FilesAndFoldersWizardPage::FillUI()
{
    m_topLabel->setText(QStringLiteral("Папки и файлы из файла восстановления"));
    QFont font=m_topLabel->font();
    font.setPointSize(font.pointSize() +2);
    m_topLabel->setFont(font);


    m_filesLabel->setText(QStringLiteral("Дополнительные файлы необходимые для копирования:"));
    m_filesListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_addFileButton->setText(QStringLiteral("Добавить файл"));

    m_foldersLabel->setText(QStringLiteral("Дополнительные папки необходимые для копирования:"));
    m_foldersTableWidget->setColumnCount(2);
    m_foldersTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_foldersTableWidget->setHorizontalHeaderLabels(QStringList()<< "Имя папки" << "Колличество файлов");
    m_addFolderButton->setText(QStringLiteral("Добавить папку"));
}

void FilesAndFoldersWizardPage::ConnectObjects()
{
    connect(m_addFileButton, &QPushButton::clicked, this, &FilesAndFoldersWizardPage::OnAddNewFile);
    connect(m_addFolderButton, &QPushButton::clicked, this, &FilesAndFoldersWizardPage::OnAddNewFolder);
}

void FilesAndFoldersWizardPage::OnAddNewFile()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Добавляем новый файл"));
    const QString newFileName=QFileDialog::getOpenFileName(this, QStringLiteral("Добавить новый файл"), QDir::homePath());
    if(m_service->AddFileToList(newFileName))
    {
        m_filesListWidget->addItem(newFileName);
    }
}

void FilesAndFoldersWizardPage::OnAddNewFolder()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Добавляем новую папку"));
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    const QString directory=dialog.getExistingDirectory(this, QStringLiteral("Выберите папку"), QDir::homePath());
    const AddFolderError error=IsCopiedDirPathCorrect(directory);
    if (AddFolderError::NoError==error)
    {
        if(m_service->AddFolderToList(directory))
        {
            const quint32 filesCount=GetFilesCount(directory);
            m_foldersTableWidget->insertRow(m_foldersTableWidget->rowCount());
            m_foldersTableWidget->setItem(m_foldersTableWidget->rowCount()-1, 0, new QTableWidgetItem(directory));
            m_foldersTableWidget->setItem(m_foldersTableWidget->rowCount()-1, 1, new QTableWidgetItem(QString::number(filesCount)));
        }
    }
    else
    {
        ParceAddFolderError(error);
    }
}

int FilesAndFoldersWizardPage::nextId() const
{
    return static_cast<int>(WizardPage::Page_Conclusion);
}

void FilesAndFoldersWizardPage::initializePage()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Перешли на страницу файлов и папок"));
    const QStringList filesList=m_service->GetAllFilesList();
    m_filesListWidget->clear();
    m_filesListWidget->addItems(filesList);

    const QStringList foldersPaths=m_service->GetAllFoldersList();
    while(m_foldersTableWidget->rowCount()!=0)
    {
        m_foldersTableWidget->removeRow(0);
    }
    m_foldersTableWidget->setRowCount(foldersPaths.count());
    QDir dir;
    int index=0;
    for (const QString &folderPath: foldersPaths)
    {
        dir.setPath(folderPath);
        if(dir.exists())
        {
            const int count = GetFilesCount(folderPath);
            m_foldersTableWidget->setItem(index, 0, new QTableWidgetItem(folderPath));
            m_foldersTableWidget->setItem(index, 1, new QTableWidgetItem(QString::number(count)));
            index++;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Папка есть в файле восстановления. Но в файловой системе ее нет: ") + folderPath);
        }
    }
}

void FilesAndFoldersWizardPage::resizeEvent(QResizeEvent *event)
{
    const int halfWidth=(event->size().width()-45)/m_foldersTableWidget->columnCount();
    for (int i=0; i<m_foldersTableWidget->columnCount(); ++i)
    {
        m_foldersTableWidget->setColumnWidth(i, halfWidth);
    }
}

quint32 FilesAndFoldersWizardPage::GetFilesCount(const QString &path, bool needToCountDirs) const noexcept
{
    QDir directory(path);
    if(directory.exists())
    {
        quint32 filesCount = 0;
        const QStringList subDirsList = directory.entryList(QDir::AllEntries|QDir::NoSymLinks|QDir::NoDotAndDotDot);
        for(const QString &subDir: subDirsList)
        {
            const QString fullPath=path + QDir::separator()+ subDir;
            const QFileInfo childDirInfo(fullPath);
            if(childDirInfo.isDir())
            {
                if(needToCountDirs)
                {
                    filesCount++;
                }
                filesCount+=GetFilesCount(fullPath, needToCountDirs);
            }
            else
            {
                filesCount++;
            }
        }
        return filesCount;
    }
    else
    {
        return 0;
    }
}

AddFolderError FilesAndFoldersWizardPage::IsCopiedDirPathCorrect(const QString &path) const noexcept
{
    if(path.startsWith("/home"))
    {
        if(QLatin1Literal("/home/")+m_currentUserName==path)
        {
            return AddFolderError::TryToAddHomeFolder;
        }
        else
        {
            return AddFolderError::NoError;
        }
    }
    else
    {
        return AddFolderError::AddSystemFolder;
    }
}

void FilesAndFoldersWizardPage::ParceAddFolderError(AddFolderError error)
{
    switch (error)
    {
    case AddFolderError::AddSystemFolder:
    {
        QMessageBox::critical(this, QStringLiteral("Ошибка в выборе папки"), QStringLiteral("Папку следует добавлять из домашней директории текущего пользователя"));
        break;
    }
    case AddFolderError::TryToAddHomeFolder:
    {
        QMessageBox::critical(this, QStringLiteral("Ошибка в выборе папки"), QStringLiteral("Папка не может быть всей домашней директорией"));
        break;
    }
    default:
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно обработать состояние так как обработчик не написан")).toUtf8().constData());
    }
    }
}
