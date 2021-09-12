#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H


#include <QStyledItemDelegate>
class ComboDelegate : public QStyledItemDelegate
{
public:
    ComboDelegate(QObject *parent = nullptr);
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *t_pEditor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *t_pEditor, QAbstractItemModel *t_pModel, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *t_pEditor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // COMBODELEGATE_H
