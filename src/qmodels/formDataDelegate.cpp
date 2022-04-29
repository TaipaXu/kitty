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

#include "./formDataDelegate.hpp"
#include <QComboBox>
#include "widgets/filePicker.hpp"
#include "qmodels/formData.hpp"

namespace QModel
{
    FormDataDelegate::FormDataDelegate(QObject *parent)
        : QStyledItemDelegate{parent}
    {
    }

    QWidget *FormDataDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        const int column = index.column();
        switch (column)
        {
        case 0:
        {
            QComboBox *selector = new QComboBox(parent);
            selector->addItem(QStringLiteral("Text"));
            selector->addItem(QStringLiteral("File"));
            return selector;
        }

        case 1:
        case 3:
        {
            return QStyledItemDelegate::createEditor(parent, option, index);
        }

        case 2:
        {
            const QString type = index.model()->data(index.model()->index(index.row(), 0), Qt::DisplayRole).toString();
            if (type == QStringLiteral("Text"))
            {
                return QStyledItemDelegate::createEditor(parent, option, index);
            }
            else
            {
                Ui::FilePicker *filePicker = new Ui::FilePicker(parent);
                return filePicker;
            }
        }

        default:
        {
        }
        }
    }

    void FormDataDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        const int column = index.column();
        switch (column)
        {
        case 0:
        {
            QComboBox *selector = qobject_cast<QComboBox *>(editor);
            Q_ASSERT(selector);
            const QString currentText = index.data(Qt::EditRole).toString();
            const int selectorIndex = selector->findText(currentText);
            if (selectorIndex >= 0)
            {
                selector->setCurrentIndex(selectorIndex);
            }
            break;
        }

        case 1:
        case 3:
        {
            QStyledItemDelegate::setEditorData(editor, index);
            break;
        }

        case 2:
        {
            const QString type = index.model()->data(index.model()->index(index.row(), 0), Qt::DisplayRole).toString();
            if (type == QStringLiteral("Text"))
            {
                QStyledItemDelegate::setEditorData(editor, index);
            }
            else
            {
                const QString data = index.data(Qt::EditRole).toString();
                if (!data.isEmpty())
                {
                    Ui::FilePicker *filePicker = qobject_cast<Ui::FilePicker *>(editor);
                    filePicker->setFile(index.data(Qt::EditRole).toString());
                }
            }
            break;
        }

        default:
            break;
        }
    }

    void FormDataDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        const int column = index.column();
        switch (column)
        {
        case 0:
        {
            QComboBox *selector = qobject_cast<QComboBox *>(editor);
            Q_ASSERT(selector);
            const QString currentType = index.model()->data(index.model()->index(index.row(), 0), Qt::DisplayRole).toString();
            const QString nextType = selector->currentText();
            if (currentType != nextType)
            {
                model->setData(index.model()->index(index.row(), 2), QStringLiteral(""), Qt::EditRole);
            }

            model->setData(index, nextType, Qt::EditRole);
            break;
        }

        case 1:
        case 3:
        {
            QStyledItemDelegate::setModelData(editor, model, index);
            break;
        }

        case 2:
        {
            const QString type = index.model()->data(index.model()->index(index.row(), 0), Qt::DisplayRole).toString();
            if (type == QStringLiteral("Text"))
            {
                QStyledItemDelegate::setModelData(editor, model, index);
            }
            else
            {
                Ui::FilePicker *filePicker = qobject_cast<Ui::FilePicker *>(editor);
                model->setData(index, filePicker->getFilePath(), Qt::EditRole);
            }
            break;
        }

        default:
            break;
        }
    }
} // namespace QModel
