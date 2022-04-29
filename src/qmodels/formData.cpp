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

#include "./formData.hpp"

namespace QModel
{
    FormData::FormData(const std::list<Model::FormData *> &items, QObject *parent)
        : Table<Model::FormData>(items, parent)
    {
    }

    int FormData::columnCount(const QModelIndex &parent) const
    {
        return 4;
    }

    Qt::ItemFlags FormData::flags(const QModelIndex &index) const
    {
        const int column = index.column();
        if (column == 0)
        {
            return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
        }
        return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
    }

    QVariant FormData::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
            {
                return tr("Type");
            }

            case 1:
            {
                return tr("Key");
            }

            case 2:
            {
                return tr("Value");
            }

            case 3:
            {
                return tr("Description");
            }
            }
        }

        return QVariant();
    }

    QVariant FormData::data(const QModelIndex &index, int role) const
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
            {
                return (*it)->getType() == Model::FormData::Type::Text ? tr("Text") : tr("File");
            }

            case 1:
            {
                return (*it)->getKey();
            }

            case 2:
            {
                return (*it)->getValue();
            }

            case 3:
            {
                return (*it)->getDescription();
            }

            default:
            {
                break;
            }
            }
        }
        else if (role == Qt::CheckStateRole && column == 0)
        {
            auto it = items.begin();
            std::advance(it, row);

            return (*it)->getEnabled() ? Qt::Checked : Qt::Unchecked;
        }

        return QVariant();
    }

    bool FormData::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        const int row = index.row();
        if (role == Qt::CheckStateRole && index.column() == 0)
        {
            auto it = items.begin();
            std::advance(it, row);
            (*it)->setEnabled(!(*it)->getEnabled());
        }
        else if (role == Qt::EditRole)
        {
            if (!QAbstractTableModel::checkIndex(index))
            {
                return false;
            }

            switch (index.column())
            {
            case 0:
            {
                Model::FormData *item = getItem(index);
                const QString type = value.toString();
                if (type == QStringLiteral("Text"))
                {
                    item->setType(Model::FormData::Type::Text);
                }
                else
                {
                    item->setType(Model::FormData::Type::File);
                }
                break;
            }

            case 1:
            {
                Model::FormData *item = getItem(index);
                item->setKey(value.toString());
                break;
            }

            case 2:
            {
                Model::FormData *item = getItem(index);
                const QString data = value.toString();
                item->setValue(data);
                break;
            }

            case 3:
            {
                Model::FormData *item = getItem(index);
                const QString data = value.toString();
                item->setDescription(data);
                break;
            }

            default:
                break;
            }

            QModelIndex topLeft = createIndex(index.row(), 0);
            emit dataChanged(topLeft, topLeft, {Qt::DisplayRole});
            return true;
        }
        return false;
    }
} // namespace QModel
