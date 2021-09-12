#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_aActions = { ui->actionRecentFile0, ui->actionRecentFile1, ui->actionRecentFile2, ui->actionRecentFile3, ui->actionRecentFile4 };
    for (size_t i = 0; i < 5; ++i)
        connect(m_aActions.at(i), &QAction::triggered, this, &MainWindow::sFileOpenRecent);
    updateRecentFileList();

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::sFileNew);
    connect(ui->actionOpen_file, &QAction::triggered, this, &MainWindow::sFileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::sFileSave);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::sFileSaveAs);
    connect(ui->actionClose_all_windows, &QAction::triggered, this, &MainWindow::sCloseAllSubWindows);
    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::sSetActiveWindow);
    connect(this, &MainWindow::hasActiveWindow, this, &MainWindow::sMenuBarUnlocker);
    connect(this, &MainWindow::noActiveWindow, this, &MainWindow::sMenuBarLocker);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_addRow_clicked()
{
    if (m_activeWindow)
        m_activeWindow->getModel()->insertRows(m_activeWindow->getModel()->rowCount());
}

void MainWindow::on_button_deleteRow_clicked()
{
    if (m_activeWindow)
        m_activeWindow->getModel()->removeRows(m_activeWindow->getModel()->rowCount() - 1);
}

void MainWindow::sFileNew()
{
    m_vpModels.push_back(new TableModel);
    m_vpWindows.push_back(new SubWindowView(m_vpModels[m_vpModels.size() - 1]));

    ui->mdiArea->addSubWindow(m_vpWindows[m_vpWindows.size() - 1]);
    m_vpWindows[m_vpWindows.size() - 1]->show();
}

void MainWindow::sFileOpen()
{
    QString t_fileName = QFileDialog::getOpenFileName();
    loadTableFromFile(t_fileName);
}

void MainWindow::sFileOpenRecent()
{
    QAction *t_pAction = qobject_cast<QAction*>(sender());
    if (t_pAction)
    {
        QString t_fileName = t_pAction->data().toString();
        loadTableFromFile(t_fileName);
    }
}

void MainWindow::sFileSave()
{
    if (m_activeWindow)
    {
        if (m_activeWindow->getFileName() == "")
            sFileSaveAs();
        else
            saveTableToFile();
    }
}

void MainWindow::sFileSaveAs()
{
    QString t_fileName = QFileDialog::getSaveFileName(nullptr, m_activeWindow->getFileName());

    if (!t_fileName.isEmpty())
    {
        m_activeWindow->setFileName(t_fileName);
        saveTableToFile();
    }
}

void MainWindow::sCloseAllSubWindows()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::sSetActiveWindow(QMdiSubWindow* t_mdiSubWindow)
{
    if (t_mdiSubWindow)
    {
        m_activeWindow = qobject_cast<SubWindowView*>(t_mdiSubWindow->widget());
        emit hasActiveWindow();
    }
    else
    {
        emit noActiveWindow();
    }
}

void MainWindow::sMenuBarLocker()
{
    ui->actionSave->setEnabled(false);
    ui->actionSave_as->setEnabled(false);
    ui->actionSave_all->setEnabled(false);
    ui->actionClose_all_windows->setEnabled(false);
}

void MainWindow::sMenuBarUnlocker()
{
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionSave_all->setEnabled(true);
    ui->actionClose_all_windows->setEnabled(true);
}

void MainWindow::addToRecentFileList(QString fileName)
{
    if (!m_recentFiles.contains(fileName))
    {
        if (m_recentFiles.size() == 5)
            m_recentFiles.pop_front();
        m_recentFiles.push_back(fileName);
        updateRecentFileList();
    }
}

void MainWindow::updateRecentFileList()
{
    m_aActions.at(0)->setText("(none)");
    m_aActions.at(0)->setData("");
    m_aActions.at(0)->setEnabled(false);
    for (size_t i = 1; i < 5; ++i)
    {
        m_aActions.at(i)->setEnabled(false);
        m_aActions.at(i)->setVisible(false);
    }

    for (size_t i = 0; i < m_recentFiles.size(); ++i)
    {
        m_aActions.at(i)->setText(m_recentFiles[i]);
        m_aActions.at(i)->setData(m_recentFiles[i]);
        m_aActions.at(i)->setEnabled(true);
        m_aActions.at(i)->setVisible(true);
    }

    if (m_recentFiles.size())
    {
        QFile t_file("recent_files.txt");
        if (t_file.open(QIODevice::WriteOnly | QIODevice::Text))
            for (int i = 0; i < m_recentFiles.size(); ++i)
                QTextStream(&t_file) << m_recentFiles[i].toUtf8() + ';';
    }
}

void MainWindow::loadTableFromFile(QString fileName)
{
    sFileNew();

    addToRecentFileList(fileName);
    m_activeWindow->setFileName(fileName);

    TableModel* t_pModel = m_activeWindow->getModel();
    QStringList t_buffer;
    t_pModel->loadTableFromFile(fileName);
}

void MainWindow::saveTableToFile()
{
    QString t_fileName = m_activeWindow->getFileName();
    TableModel* t_pModel = m_activeWindow->getModel();
    t_pModel->saveTableToFile(t_fileName);
    addToRecentFileList(t_fileName);
    m_activeWindow->updateWindowTitle();
}
