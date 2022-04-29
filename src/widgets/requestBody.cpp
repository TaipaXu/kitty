
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

#include "./requestBody.hpp"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QRadioButton>
#include <QComboBox>
#include <QPlainTextEdit>
#include "widgets/formDataTable.hpp"
#include "widgets/xWwwFormUrlEncodedTable.hpp"
#include "widgets/filePicker.hpp"
#include "models/body.hpp"
#include "models/raw.hpp"
#include "models/binary.hpp"
#include "qmodels/formData.hpp"
#include "qmodels/xWwwFormUrlEncoded.hpp"
#include "qmodels/formDataDelegate.hpp"

namespace Ui
{
    RequestBody::RequestBody(Model::Body *body, QWidget *parent)
        : QWidget{parent},
          body{body},
          raw{nullptr},
          binary{nullptr}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setAlignment(Qt::AlignLeft);

        QHBoxLayout *typesLayout = new QHBoxLayout();
        typesLayout->setAlignment(Qt::AlignLeft);

        QButtonGroup *buttons = new QButtonGroup(this);
        noneRadio = new QRadioButton(tr("none"), this);
        formDataRadio = new QRadioButton(tr("form-data"), this);
        xWwwFormUrlencodedRadio = new QRadioButton(tr("x-www-form-urlencoded"), this);
        rawRadio = new QRadioButton(tr("raw"), this);
        binaryRadio = new QRadioButton(tr("binary"), this);
        noneRadio->setChecked(true);
        buttons->addButton(noneRadio, 0);
        buttons->addButton(formDataRadio, 1);
        buttons->addButton(xWwwFormUrlencodedRadio, 2);
        buttons->addButton(rawRadio, 3);
        buttons->addButton(binaryRadio, 4);
        connect(buttons, &QButtonGroup::buttonToggled, this, &RequestBody::handleBodyTypeToggled);

        typesLayout->addWidget(noneRadio);
        typesLayout->addWidget(formDataRadio);
        typesLayout->addWidget(xWwwFormUrlencodedRadio);
        typesLayout->addWidget(rawRadio);
        typesLayout->addWidget(binaryRadio);
        mainLayout->addLayout(typesLayout);

        workStack = new QStackedWidget(this);
        noneWidget = new QWidget(workStack);
        formDataWidget = new QWidget(workStack);
        xWwwFormWidget = new QWidget(workStack);
        rawWidget = new QWidget(workStack);
        binaryWidget = new QWidget(workStack);
        QVBoxLayout *noneLayout = new QVBoxLayout(noneWidget);
        QVBoxLayout *formDataLayout = new QVBoxLayout(formDataWidget);
        QVBoxLayout *xWwwFormDataLayout = new QVBoxLayout(xWwwFormWidget);
        QVBoxLayout *rawLayout = new QVBoxLayout(rawWidget);
        QVBoxLayout *binaryLayout = new QVBoxLayout(binaryWidget);
        binaryLayout->setAlignment(Qt::AlignTop);

        workStack->addWidget(noneWidget);
        workStack->addWidget(formDataWidget);
        workStack->addWidget(xWwwFormWidget);
        workStack->addWidget(rawWidget);
        workStack->addWidget(binaryWidget);

        const Model::Body::Type type = body->getType();

        switch (type)
        {
        case Model::Body::Type::None:
        {
            raw = new Model::Raw();
            binary = new Model::Binary();
            break;
        }

        case Model::Body::Type::FormData:
        {
            formDatas = body->getFormDatas();
            raw = new Model::Raw();
            binary = new Model::Binary();
            break;
        }

        case Model::Body::Type::XWwwFormUrlEncoded:
        {
            xWwwFormUrlEncodeds = body->getXWwwFormUrlEncodeds();
            raw = new Model::Raw();
            binary = new Model::Binary();
            break;
        }

        case Model::Body::Type::Raw:
        {
            raw = body->getRaw();
            binary = new Model::Binary();
            break;
        }

        case Model::Body::Type::Binary:
        {
            raw = new Model::Raw();
            binary = body->getBinary();
            break;
        }

        default:
            break;
        }

        formDataTableView = new FormDataTable(this);
        formDataModel = new QModel::FormData(formDatas, this);
        connect(formDataModel, &QModel::FormData::dataChanged, this, &RequestBody::handleFormDataChanged);
        connect(formDataModel, &QModel::FormData::rowsInserted, this, &RequestBody::handleFormDataChanged);
        connect(formDataModel, &QModel::FormData::rowsMoved, this, &RequestBody::handleFormDataChanged);
        connect(formDataModel, &QModel::FormData::rowsRemoved, this, &RequestBody::handleFormDataChanged);
        formDataTableView->setModel(formDataModel);
        QModel::FormDataDelegate *delegate = new QModel::FormDataDelegate(this);
        formDataTableView->setItemDelegate(delegate);
        formDataLayout->addWidget(formDataTableView);

        xWwwFormUrlEncodedTableView = new XWwwFormUrlEncodedTable(this);
        xWwwFormUrlEncodedModel = new QModel::XWwwFormUrlEncoded(xWwwFormUrlEncodeds, this);
        connect(xWwwFormUrlEncodedModel, &QModel::XWwwFormUrlEncoded::dataChanged, this, &RequestBody::handleXWwwFormUrlEncodedsChanged);
        connect(xWwwFormUrlEncodedModel, &QModel::XWwwFormUrlEncoded::rowsInserted, this, &RequestBody::handleXWwwFormUrlEncodedsChanged);
        connect(xWwwFormUrlEncodedModel, &QModel::XWwwFormUrlEncoded::rowsMoved, this, &RequestBody::handleXWwwFormUrlEncodedsChanged);
        connect(xWwwFormUrlEncodedModel, &QModel::XWwwFormUrlEncoded::rowsRemoved, this, &RequestBody::handleXWwwFormUrlEncodedsChanged);
        xWwwFormUrlEncodedTableView->setModel(xWwwFormUrlEncodedModel);
        xWwwFormDataLayout->addWidget(xWwwFormUrlEncodedTableView);

        QComboBox *typeSelect = new QComboBox(this);
        typeSelect->addItems(Model::getRawTypes());
        const Model::Raw::Type rawType = raw->getType();
        typeSelect->setCurrentText(Model::getRawTypeName(rawType, true));
        connect(typeSelect, &QComboBox::currentTextChanged, this, &RequestBody::handleRawTypeChanged);
        rawLayout->addWidget(typeSelect);

        rawTextEdit = new QPlainTextEdit(this);
        connect(rawTextEdit, &QPlainTextEdit::textChanged, this, &RequestBody::handleRawTextChanged);
        if (type == Model::Body::Type::Raw)
        {
            rawTextEdit->setPlainText(raw->getData());
        }
        rawLayout->addWidget(rawTextEdit);

        FilePicker *filePicker = new FilePicker(this);
        connect(filePicker, &FilePicker::filePicked, this, &RequestBody::handleBinaryChanged);
        if (type == Model::Body::Type::Binary)
        {
            filePicker->setFile(binary->getPath());
        }
        binaryLayout->addWidget(filePicker);

        mainLayout->addWidget(workStack);

        setLayout(mainLayout);

        switch (type)
        {
        case Model::Body::Type::None:
        {
            noneRadio->click();
            break;
        }

        case Model::Body::Type::FormData:
        {
            formDataRadio->click();
            break;
        }

        case Model::Body::Type::XWwwFormUrlEncoded:
        {
            xWwwFormUrlencodedRadio->click();
            break;
        }

        case Model::Body::Type::Raw:
        {
            rawRadio->click();
            break;
        }

        case Model::Body::Type::Binary:
        {
            binaryRadio->click();
            break;
        }

        default:
            break;
        }
    }

    RequestBody::~RequestBody()
    {
        switch (body->getType())
        {
        case Model::Body::Type::None:
        {
            deleteFormDatas();
            deleteXWwwUrlEncodeds();
            deleteRaw();
            deleteBinary();
            break;
        }

        case Model::Body::Type::FormData:
        {
            deleteXWwwUrlEncodeds();
            deleteRaw();
            deleteBinary();
            break;
        }

        case Model::Body::Type::XWwwFormUrlEncoded:
        {
            deleteFormDatas();
            deleteRaw();
            deleteBinary();
            break;
        }

        case Model::Body::Type::Raw:
        {
            deleteFormDatas();
            deleteXWwwUrlEncodeds();
            deleteBinary();
            break;
        }

        case Model::Body::Type::Binary:
        {
            deleteFormDatas();
            deleteXWwwUrlEncodeds();
            deleteRaw();
            break;
        }

        default:
            break;
        }
    }

    void RequestBody::handleBodyTypeToggled(QAbstractButton *button, bool checked) const
    {
        if (!checked)
        {
            return;
        }

        if (button == noneRadio)
        {
            workStack->setCurrentWidget(noneWidget);
            // body->setType(Model::Api::Body::Type::None);
            body->removeData();
        }
        else if (button == formDataRadio)
        {
            workStack->setCurrentWidget(formDataWidget);
            // body->setType(Model::Api::Body::Type::FormData);
            body->setData(formDatas);
        }
        else if (button == xWwwFormUrlencodedRadio)
        {
            workStack->setCurrentWidget(xWwwFormWidget);
            // body->setType(Model::Api::Body::Type::XWwwFormUrlEncoded);
            body->setData(xWwwFormUrlEncodeds);
        }
        else if (button == rawRadio)
        {
            workStack->setCurrentWidget(rawWidget);
            // body->setType(Model::Api::Body::Type::Raw);
            body->setData(raw);
        }
        else if (button == binaryRadio)
        {
            workStack->setCurrentWidget(binaryWidget);
            // body->setType(Model::Api::Body::Type::Binary);
            body->setData(binary);
        }

        emit changed();
    }

    void RequestBody::handleFormDataChanged() const
    {
        std::list<Model::FormData *> items = formDataModel->getItems();
        body->setData(items);

        emit changed();
    }

    void RequestBody::handleXWwwFormUrlEncodedsChanged() const
    {
        std::list<Model::XWwwFormUrlEncoded *> items = xWwwFormUrlEncodedModel->getItems();
        body->setData(items);

        emit changed();
    }

    void RequestBody::handleRawTypeChanged(const QString &text) const
    {
        Model::Raw *raw = body->getRaw();
        if (text == "Text")
        {
            raw->setType(Model::Raw::Type::Text);
        }
        else if (text == "JavaScript")
        {
            raw->setType(Model::Raw::Type::JavaScript);
        }
        else if (text == "Json")
        {
            raw->setType(Model::Raw::Type::Json);
        }
        else if (text == "Html")
        {
            raw->setType(Model::Raw::Type::Html);
        }
        else if (text == "Xml")
        {
            raw->setType(Model::Raw::Type::Xml);
        }

        emit changed();
    }

    void RequestBody::handleRawTextChanged() const
    {
        raw->setData(rawTextEdit->toPlainText());

        emit changed();
    }

    void RequestBody::handleBinaryChanged(const QString &filePath) const
    {
        binary->setPath(filePath);

        emit changed();
    }

    void RequestBody::deleteFormDatas()
    {
        for (auto &&i : formDatas)
        {
            delete i;
        }
        formDatas = {};
    }

    void RequestBody::deleteXWwwUrlEncodeds()
    {
        for (auto &&i : xWwwFormUrlEncodeds)
        {
            delete i;
        }
        xWwwFormUrlEncodeds = {};
    }

    void RequestBody::deleteRaw()
    {
        delete raw;
        raw = nullptr;
    }

    void RequestBody::deleteBinary()
    {
        delete binary;
        binary = nullptr;
    }
} // namespace Ui
