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

#include "./responseHeader.hpp"
#include <QMimeData>

namespace QModel
{
    ResponseHeader::ResponseHeader(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }

    void ResponseHeader::setHeaders(const std::list<Model::Response::Header> &headers)
    {
        this->headers.clear();

        beginResetModel();
        this->headers = headers;
        endResetModel();
    }

    int ResponseHeader::rowCount(const QModelIndex &parent) const
    {
        return headers.size();
    }

    int ResponseHeader::columnCount(const QModelIndex &parent) const
    {
        return 2;
    }

    QVariant ResponseHeader::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
            {
                return tr("Key");
            }

            case 1:
            {
                return tr("Value");
            }
            }
        }
        return QVariant();
    }

    QVariant ResponseHeader::data(const QModelIndex &index, int role) const
    {
        const int row = index.row();
        const int column = index.column();
        if (role == Qt::DisplayRole)
        {
            auto it = headers.begin();
            std::advance(it, row);
            switch (column)
            {
            case 0:
            {
                return (*it).key;
            }

            case 1:
            {
                return (*it).value;
            }

            default:
                break;
            }
        }

        return QVariant();
    }
} // namespace QModel
