/*******************************************************************************
 * Copyright [2019] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       输入代理模块
*******************************************************************************/
#include "libdelege.h"

LibDelege::LibDelege(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *LibDelege::createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &,
                                 const QModelIndex &) const {
    if (tmpbox.toString() == "combobox") {                     // 下拉框
        QComboBox *editor = new QComboBox(parent);
        editor->setItemDelegate(new QStyledItemDelegate());
        editor->addItems(tmpstr.toStringList());
        return editor;
    }
    if (tmpbox.toString() == "valuebox") {                     // 输入框
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setButtonSymbols(QAbstractSpinBox::NoButtons);
        editor->setMaximum(tmpmax.toDouble());
        editor->setMinimum(tmpmin.toDouble());
        editor->setDecimals(tmpstr.toInt());
        return editor;
    }
    return NULL;                                                // 只读框
}

void LibDelege::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (tmpbox.toString() == "combobox") {                     // 下拉框
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *box = static_cast<QComboBox*>(editor);
        int tindex = box->findText(text);
        box->setCurrentIndex(tindex);
    }
    if (tmpbox.toString() == "valuebox") {                     // 输入框
        double value = index.model()->data(index, Qt::EditRole).toDouble();
        QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(editor);
        box->setValue(value);
    }
}

void LibDelege::setModelData(QWidget *editor,
                             QAbstractItemModel *model,
                             const QModelIndex &index) const {
    if (tmpbox.toString() == "combobox") {                     // 下拉框
        QComboBox *box = static_cast<QComboBox*>(editor);
        QString text = box->currentText();
        model->setData(index, text, Qt::EditRole);
    }
    if (tmpbox.toString() == "valuebox") {                     // 输入框
        QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(editor);
        box->interpretText();
        double value = box->value();
        model->setData(index, value, Qt::EditRole);
    }
}

void LibDelege::updateEditorGeometry(QWidget *editor,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &) const {
    editor->setGeometry(option.rect);
    if (tmpbox.toString() == "combobox") {                     // 下拉框
        QComboBox *box = static_cast<QComboBox*>(editor);
        box->showPopup();                                       // 自动弹出
    }
}

void LibDelege::setInit(QVariant box, QVariant min, QVariant max, QVariant str)
{
    this->tmpbox = box;
    this->tmpmax = max;
    this->tmpmin = min;
    this->tmpstr = str;
}
