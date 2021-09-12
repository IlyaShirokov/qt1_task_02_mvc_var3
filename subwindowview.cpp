#include "subwindowview.h"

SubWindowView::SubWindowView(TableModel* model, QString fileName, QWidget* parent) : QTableView(parent), m_model(model), m_fileName(fileName)
{
    updateWindowTitle();

    m_delegate = new ComboDelegate(m_model);
    setItemDelegateForColumn(3, m_delegate);

    setModel(m_model);
}

SubWindowView::~SubWindowView()
{
    delete m_delegate;
    emit aSubWindowClosed();
}

TableModel* SubWindowView::getModel() const
{
    return m_model;
}

void SubWindowView::updateWindowTitle()
{
    if (m_fileName.size())
        setWindowTitle(m_fileName);
    else
        setWindowTitle("New Table");
}

void SubWindowView::setFileName(QString fileName)
{
    m_fileName = fileName;
    updateWindowTitle();
}

QString SubWindowView::getFileName() const
{
    return m_fileName;
}
