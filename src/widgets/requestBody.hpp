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
#include <list>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QRadioButton;
class QStackedWidget;
class QPlainTextEdit;
QT_END_NAMESPACE

namespace Model
{
    class Body;
    class FormData;
    class XWwwFormUrlEncoded;
    class Raw;
    class Binary;
} // namespace Model

namespace QModel
{
    class FormData;
    class XWwwFormUrlEncoded;
} // namespace QModel

namespace Ui
{
    class FormDataTable;
    class XWwwFormUrlEncodedTable;

    class RequestBody : public QWidget
    {
        Q_OBJECT

    public:
        explicit RequestBody(Model::Body *body, QWidget *parent = nullptr);
        ~RequestBody();

    signals:
        void changed() const;

    private slots:
        void handleBodyTypeToggled(QAbstractButton *button, bool checked) const;
        void handleFormDataChanged() const;
        void handleXWwwFormUrlEncodedsChanged() const;
        void handleRawTypeChanged(const QString &text) const;
        void handleRawTextChanged() const;
        void handleBinaryChanged(const QString &filePath) const;

    private:
        void deleteFormDatas();
        void deleteXWwwUrlEncodeds();
        void deleteRaw();
        void deleteBinary();

    private:
        Model::Body *body;
        QStackedWidget *workStack;
        QRadioButton *noneRadio;
        QRadioButton *formDataRadio;
        QRadioButton *xWwwFormUrlencodedRadio;
        QRadioButton *rawRadio;
        QRadioButton *binaryRadio;

        QWidget *noneWidget;
        QWidget *formDataWidget;
        QWidget *xWwwFormWidget;
        QWidget *rawWidget;
        QWidget *binaryWidget;

        std::list<Model::FormData *> formDatas;
        std::list<Model::XWwwFormUrlEncoded *> xWwwFormUrlEncodeds;
        Model::Raw *raw;
        Model::Binary *binary;

        FormDataTable *formDataTableView;
        QModel::FormData *formDataModel;
        XWwwFormUrlEncodedTable *xWwwFormUrlEncodedTableView;
        QModel::XWwwFormUrlEncoded *xWwwFormUrlEncodedModel;
        QPlainTextEdit *rawTextEdit;
    };
} // namespace Ui
