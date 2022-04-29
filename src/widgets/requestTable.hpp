/*
 * This file is part of the Kitty (https://github.com/TaipaXu/kitty).
 * Copyright (c) 2022 Taipa Xu.
 *
 * Kitty is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Kitty is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Kitty. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QHeaderView>
#include <QTableView>

namespace Ui
{
    template <typename T>
    class RequestTable : public QWidget
    {
    public:
        explicit RequestTable(QWidget *parent = nullptr);
        ~RequestTable() = default;

        void setModel(T *model);
        void setItemDelegate(QAbstractItemDelegate *delegate) const;

    protected slots:
        void handleTableViewMenuRequested(const QPoint &pos) const;
        virtual void handleAddNewParamActionTriggered() const;
        virtual void handleInsertBeforeParamActionTriggered() const;
        virtual void handleInsertAfterParamActionTriggered() const;
        virtual void handleDeleteParamActionTriggered() const;

    private:
        T *model;
        QTableView *tableView;
        QMenu *tableMenu;
        QMenu *paramMenu;
        QAction *insertBeforeParamAction;
        QAction *insertAfterParamAction;
        QAction *deleteParamAction;
    };

    template <typename T>
    RequestTable<T>::RequestTable(QWidget *parent)
        : QWidget{parent}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();

        tableView = new QTableView(this);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setDragDropMode(QAbstractItemView::InternalMove);
        tableView->setDragEnabled(true);
        tableView->setAcceptDrops(true);
        tableView->setDropIndicatorShown(true);
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tableView, &QTableView::customContextMenuRequested, this, &RequestTable::handleTableViewMenuRequested);

        mainLayout->addWidget(tableView);

        tableMenu = new QMenu(this);

        QAction *addNewParamAction = new QAction(tr("Add param"), this);
        connect(addNewParamAction, &QAction::triggered, this, &RequestTable::handleAddNewParamActionTriggered);
        tableMenu->addAction(addNewParamAction);

        paramMenu = new QMenu(this);

        insertBeforeParamAction = new QAction(tr("Insert before"), this);
        connect(insertBeforeParamAction, &QAction::triggered, this, &RequestTable::handleInsertBeforeParamActionTriggered);
        paramMenu->addAction(insertBeforeParamAction);
        insertAfterParamAction = new QAction(tr("Insert after"), this);
        connect(insertAfterParamAction, &QAction::triggered, this, &RequestTable::handleInsertAfterParamActionTriggered);
        paramMenu->addAction(insertAfterParamAction);
        deleteParamAction = new QAction(tr("Delete"), this);
        connect(deleteParamAction, &QAction::triggered, this, &RequestTable::handleDeleteParamActionTriggered);
        paramMenu->addAction(deleteParamAction);

        setLayout(mainLayout);
    }

    template <typename T>
    void RequestTable<T>::setModel(T *model)
    {
        this->model = model;
        tableView->setModel(model);
    }

    template <typename T>
    void RequestTable<T>::setItemDelegate(QAbstractItemDelegate *delegate) const
    {
        tableView->setItemDelegate(delegate);
    }

    template <typename T>
    void RequestTable<T>::handleTableViewMenuRequested(const QPoint &pos) const
    {
        QModelIndex index = tableView->indexAt(pos);
        if (index.isValid())
        {
            insertBeforeParamAction->setData(pos);
            insertAfterParamAction->setData(pos);
            deleteParamAction->setData(pos);
            paramMenu->popup(tableView->viewport()->mapToGlobal(pos));
        }
        else
        {
            tableMenu->popup(tableView->viewport()->mapToGlobal(pos));
        }
    }

    template <typename T>
    void RequestTable<T>::handleAddNewParamActionTriggered() const
    {
        model->addItem();
    }

    template <typename T>
    void RequestTable<T>::handleInsertBeforeParamActionTriggered() const
    {
        const QAction *action = qobject_cast<QAction *>(sender());
        const QPoint point = action->data().toPoint();
        QModelIndex index = tableView->indexAt(point);
        model->insertBefore(index);
    }

    template <typename T>
    void RequestTable<T>::handleInsertAfterParamActionTriggered() const
    {
        const QAction *action = qobject_cast<QAction *>(sender());
        const QPoint point = action->data().toPoint();
        QModelIndex index = tableView->indexAt(point);
        model->insertAfter(index);
    }

    template <typename T>
    void RequestTable<T>::handleDeleteParamActionTriggered() const
    {
        const QAction *action = qobject_cast<QAction *>(sender());
        const QPoint point = action->data().toPoint();
        QModelIndex index = tableView->indexAt(point);
        model->deleteItem(index);
    }
} // namespace Ui
