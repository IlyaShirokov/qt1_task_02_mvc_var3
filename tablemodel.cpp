#include "tablemodel.h"

TableModel::TableModel()
{
    m_data = {};
    m_enumeration << "One" << "Two" << "Three" << "Four";
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        switch (section)
        {
        case 0:
            return "String";
        case 1:
            return "Integer";
        case 2:
            return "Boolean";
        case 3:
            return "Enumeration";
        }

    return QAbstractTableModel::headerData(section, orientation, role);
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if ((index.isValid() && index.row() < rowCount()) && (index.column() < columnCount()) && (role == Qt::DisplayRole || role == Qt::EditRole))
        switch (index.column())
        {
        case 0:
            return m_data[index.row()].stringCell;
        case 1:
            return m_data[index.row()].intCell;
        case 2:
            return m_data[index.row()].boolCell;
        case 3:
            if (role == Qt::DisplayRole)
                return m_data[index.row()].enumCell < 0 ? "" : m_enumeration[m_data[index.row()].enumCell];
            else
                return m_data[index.row()].enumCell;
        }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value && index.row() < rowCount() && index.column() < columnCount() && role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            m_data[index.row()].stringCell = value.toString();
            break;
        case 1:
            m_data[index.row()].intCell = value.toInt();
            break;
        case 2:
            m_data[index.row()].boolCell = value.toBool();
            break;
        case 3:
            m_data[index.row()].enumCell = value.toInt();
            break;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_data.insert(row, {QString(), 0, false, -1});
    endInsertRows();

    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (m_data.size())
    {
        beginRemoveRows(parent, row, row + count - 1);
        m_data.erase(m_data.begin() + row);
        endRemoveRows();
        return true;
    }

    return false;
}

QStringList TableModel::getEnumeration() const
{
    return m_enumeration;
}

void TableModel::loadTableFromFile(QString fileName)
{
    QFile t_file(fileName);
    if (t_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList t_buffer;

        while (!t_file.atEnd())
        {
            t_buffer = QString(t_file.readLine()).remove('\n').split(',');
            insertRows(rowCount());
            for (int i = 0; i < 4; ++i)
                setData(index(rowCount() - 1, i), t_buffer.at(i));
        }
        t_file.close();
    }
}

void TableModel::saveTableToFile(QString t_fileName)
{
    QFile t_file(t_fileName);
    if (t_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        for (int i = 0; i < rowCount(); ++i)
            QTextStream(&t_file) << data(index(i, 0)).toString() << ','
                                 << data(index(i, 1)).toInt()    << ','
                                 << data(index(i, 2)).toInt()    << ','
                                 << data(index(i, 3), Qt::EditRole).toInt() << '\n';
        t_file.close();
    }
}
