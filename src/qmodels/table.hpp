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

#include <QAbstractTableModel>
#include <QMimeData>
#include <list>

namespace QModel
{
    template <typename T>
    class Table : public QAbstractTableModel
    {
    public:
        Table(const std::list<T *> &items, QObject *parent = nullptr);
        ~Table();

        T *getItem(const QModelIndex &index);
        std::list<T *> getItems();
        void addItem();
        void insertBefore(const QModelIndex &index);
        void insertAfter(const QModelIndex &index);
        void deleteItem(const QModelIndex &index);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        QStringList mimeTypes() const override;
        QMimeData *mimeData(const QModelIndexList &indexes) const override;
        Qt::DropActions supportedDragActions() const override;
        Qt::DropActions supportedDropActions() const override;
        bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    protected:
        std::list<T *> items;
        static const QString mimeType;
    };

    template <typename T>
    const QString Table<T>::mimeType = "kitty/table";

    template <typename T>
    Table<T>::Table(const std::list<T *> &items, QObject *parent)
        : QAbstractTableModel(parent),
          items{items}
    {
    }

    template <typename T>
    Table<T>::~Table()
    {
        for (auto &&item : items)
        {
            delete item;
        }
    }

    template <typename T>
    T *Table<T>::getItem(const QModelIndex &index)
    {
        const int row = index.row();
        auto it = items.begin();
        std::advance(it, row);
        return *it;
    }

    template <typename T>
    std::list<T *> Table<T>::getItems()
    {
        return items;
    }

    template <typename T>
    void Table<T>::addItem()
    {
        insertAfter(createIndex(items.size() - 1, 1));
    }

    template <typename T>
    void Table<T>::insertBefore(const QModelIndex &index)
    {
        const int row = index.row();
        beginInsertRows(QModelIndex(), row, row);
        auto it = items.begin();
        std::advance(it, row);
        items.insert(it, new T());
        endInsertRows();
        // emit rowsInserted(QModelIndex(), row, row);
    }

    template <typename T>
    void Table<T>::insertAfter(const QModelIndex &index)
    {
        const int row = index.row() + 1;
        beginInsertRows(QModelIndex(), row, row);
        auto it = items.begin();
        std::advance(it, row);
        items.insert(it, new T());
        endInsertRows();
    }

    template <typename T>
    void Table<T>::deleteItem(const QModelIndex &index)
    {
        T *item = getItem(index);
        beginRemoveRows(QModelIndex(), index.row(), index.row() + 1);
        items.remove(item);
        delete item;
        item = nullptr;
        endRemoveRows();

        // removeRow(index.row(), QModelIndex());
    }

    template <typename T>
    int Table<T>::rowCount(const QModelIndex & /*parent*/) const
    {
        return items.size();
    }

    template <typename T>
    int Table<T>::columnCount(const QModelIndex & /*parent*/) const
    {
        return 3;
    }

    template <typename T>
    Qt::ItemFlags Table<T>::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return Qt::NoItemFlags;
        }

        const int column = index.column();
        if (column == 0)
        {
            return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
        }
        return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
    }

    template <typename T>
    QStringList Table<T>::mimeTypes() const
    {
        return {mimeType};
    }

    template <typename T>
    QMimeData *Table<T>::mimeData(const QModelIndexList &indexes) const
    {
        QMimeData *mimeData = new QMimeData();
        const QModelIndex index = indexes.at(0);
        mimeData->setData(mimeType, QByteArray::number(index.row()));
        return mimeData;
    }

    template <typename T>
    inline Qt::DropActions Table<T>::supportedDragActions() const
    {
        return Qt::MoveAction;
    }

    template <typename T>
    inline Qt::DropActions Table<T>::supportedDropActions() const
    {
        return Qt::MoveAction;
    }

    template <typename T>
    bool Table<T>::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
    {
        return data && action == Qt::MoveAction && data->hasFormat(mimeType);
    }

    template <typename T>
    bool Table<T>::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
    {
        if (action == Qt::IgnoreAction)
        {
            return true;
        }

        if (!data || action != Qt::MoveAction || !data->hasFormat(mimeType))
        {
            return false;
        }

        const int fromRow = data->data(mimeType).toInt();
        const int toRow = parent.row();

        beginMoveRows(QModelIndex(), fromRow, fromRow, QModelIndex(), fromRow < toRow ? toRow + 1 : toRow);
        auto itFrom = items.begin();
        std::advance(itFrom, fromRow);
        auto itTo = items.begin();
        std::advance(itTo, toRow);
        std::swap(*itFrom, *itTo);
        endMoveRows();

        return true;
    }

    template <typename T>
    QVariant Table<T>::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return tr("Key");
            case 1:
                return tr("Value");
            case 2:
                return tr("Description");
            }
        }
        return QVariant();
    }

    template <typename T>
    QVariant Table<T>::data(const QModelIndex &index, int role) const
    {
        const int row = index.row();
        const int column = index.column();
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            auto it = items.begin();
            std::advance(it, row);
            switch (column)
            {
            case 0:
                return (*it)->getKey();
                break;
            case 1:
                return (*it)->getValue();
                break;
            case 2:
                return (*it)->getDescription();
                break;

            default:
                break;
            }
        }
        else if (role == Qt::CheckStateRole)
        {
            if (column == 0)
            {
                auto it = items.begin();
                std::advance(it, index.row());
                return (*it)->getEnabled() ? Qt::Checked : Qt::Unchecked;
            }
        }

        return QVariant();
    }

    template <typename T>
    bool Table<T>::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        const int row = index.row();
        if (role == Qt::CheckStateRole && index.column() == 0)
        {
            auto it = items.begin();
            std::advance(it, index.row());
            (*it)->setEnabled(!(*it)->getEnabled());
            QModelIndex topLeft = createIndex(row, 0);
            emit dataChanged(topLeft, topLeft, {Qt::DisplayRole});
            return true;
        }

        if (role == Qt::EditRole)
        {
            if (!checkIndex(index))
                return false;

            const QString content = value.toString();
            T *param = getItem(index);
            switch (index.column())
            {
            case 0:
                param->setKey(content);
                break;
            case 1:
                param->setValue(content);
                break;
            case 2:
                param->setDescription(content);
                break;

            default:
                break;
            }

            emit dataChanged(index, index, {Qt::DisplayRole});

            return true;
        }
        return false;
    }
} // namespace QModel
