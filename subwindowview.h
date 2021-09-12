#ifndef SUBWINDOWVIEW_H
#define SUBWINDOWVIEW_H

#include <QObject>
#include <QTableView>

#include "tablemodel.h"
#include "combodelegate.h"

class SubWindowView : public QTableView
{
    Q_OBJECT
public:
    SubWindowView(TableModel* model, QString fileName = "", QWidget *parent = nullptr);
    ~SubWindowView();

    TableModel* getModel() const;

    void updateWindowTitle();

    void setFileName(QString fileName);
    QString getFileName() const;

private:
    TableModel*    m_model;
    ComboDelegate* m_delegate;
    QString m_fileName;

signals:
    void aSubWindowClosed();
};

#endif // SUBWINDOWVIEW_H
