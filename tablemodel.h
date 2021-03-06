#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractItemModel>
#include <QFile>
#include <QTextStream>

struct RowData
{
    QString stringCell;
    int     intCell;
    bool    boolCell;
    int     enumCell;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex()) override;

    QStringList getEnumeration() const;
    void loadTableFromFile(QString t_fileName);
    void saveTableToFile(QString t_fileName);

private:
    QVector<RowData> m_data;
    QStringList m_enumeration;
};

#endif // TABLEMODEL_H
