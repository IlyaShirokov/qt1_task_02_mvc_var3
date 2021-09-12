#include <QtWidgets>

#include "combodelegate.h"
#include "tablemodel.h"

ComboDelegate::ComboDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
}

QWidget* ComboDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QComboBox* t_pEditor = new QComboBox(parent);
    const TableModel* t_pModel = qobject_cast <const TableModel*>(index.model());

    if (t_pModel)
        for (int i = 0; i < t_pModel->getEnumeration().size(); ++i)
            t_pEditor->addItem(t_pModel->getEnumeration()[i]);

    return t_pEditor;
}

void ComboDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    const int t_enumerate = index.model()->data(index, Qt::EditRole).toInt();
    QComboBox* t_pEditor = qobject_cast<QComboBox*>(editor);

    if (t_pEditor)
        t_pEditor->setCurrentIndex(t_enumerate);
}

void ComboDelegate::setModelData(QWidget* editor, QAbstractItemModel* itemModel, const QModelIndex& index) const
{
    QComboBox* t_pEditor = qobject_cast<QComboBox*>(editor);
    if (t_pEditor)
    {
        const int t_enumerate = t_pEditor->currentIndex();
        itemModel->setData(index, t_enumerate, Qt::EditRole);
    }
}

void ComboDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}
