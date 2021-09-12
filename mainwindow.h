#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QtWidgets>

#include "tablemodel.h"
#include "subwindowview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void hasActiveWindow();
    void noActiveWindow();

private slots:
    void sFileNew();
    void sFileOpen();
    void sFileOpenRecent();
    void sFileSave();
    void sFileSaveAs();

    void sCloseAllSubWindows();

    void on_button_addRow_clicked();
    void on_button_deleteRow_clicked();

    void sSetActiveWindow(QMdiSubWindow* t_mdiSubWindow);
    void sMenuBarLocker();
    void sMenuBarUnlocker();

private:
    Ui::MainWindow *ui;

    std::array<QAction*, 5> m_aActions;

    void addToRecentFileList(QString t_fileName);
    void updateRecentFileList();

    void loadTableFromFile(QString t_fileName);
    void saveTableToFile();


    QVector<TableModel*> m_vpModels;
    QVector<SubWindowView*>  m_vpWindows;

    SubWindowView* m_activeWindow;

    QStringList m_recentFiles;
};
#endif // MAINWINDOW_H
