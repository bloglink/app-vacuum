/*******************************************************************************
 * Copyright [2019] <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     0.1
 * author:      zhaonanlin
 * brief:       输入代理模块
*******************************************************************************/
#ifndef LIBDELEGE_H
#define LIBDELEGE_H

#include <QObject>
#include <QSpinBox>
#include <QListView>
#include <QComboBox>
#include <QModelIndex>
#include <QItemDelegate>
#include <QDoubleSpinBox>
#include <QStyledItemDelegate>

class LibDelege : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LibDelege(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setInit(QVariant box, QVariant min, QVariant max, QVariant str);
    QVariant tmpbox;
    QVariant tmpmax;
    QVariant tmpmin;
    QVariant tmpstr;
};
#endif // LIBDELEGE_H
