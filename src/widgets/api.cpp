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

#include "./api.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include "models/api.hpp"
#include "models/method.hpp"
#include "widgets/request.hpp"
#include "widgets/response.hpp"
#include "network/network.hpp"

namespace Ui
{
    Api::Api(Model::Api *api, QWidget *parent)
        : QWidget(parent), api{api}, isModified{false}, network{nullptr}, isRequestting{false}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 5, 0);

        QHBoxLayout *layoutA = new QHBoxLayout();

        QComboBox *methodSelector = new QComboBox(this);
        methodSelector->addItems(Model::getMethodNames());
        methodSelector->setCurrentText(Model::getMethodName(api->getMethod()));
        connect(methodSelector, &QComboBox::currentTextChanged, this, &Api::handleMethodChanged);
        layoutA->addWidget(methodSelector);

        QLineEdit *urlInput = new QLineEdit(api->getUrl(), this);
        connect(urlInput, &QLineEdit::textChanged, this, &Api::handleUrlChanged);
        connect(urlInput, &QLineEdit::returnPressed, this, &Api::sendRequest);
        layoutA->addWidget(urlInput);

        QPushButton *sendButton = new QPushButton(tr("Send"), this);
        connect(sendButton, &QPushButton::clicked, this, &Api::sendRequest);
        layoutA->addWidget(sendButton);

        mainLayout->addLayout(layoutA);

        QSplitter *splitter = new QSplitter(this);
        splitter->setOrientation(Qt::Vertical);
        splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        request = new Request(api, splitter);
        connect(request, &Request::changed, this, &Api::handleRequestChanged);
        splitter->addWidget(request);

        response = new Response(splitter);
        splitter->addWidget(response);

        mainLayout->addWidget(splitter);

        setLayout(mainLayout);
    }

    void Api::handleMethodChanged(const QString &text)
    {
        isModified = true;
        api->setMethod(Model::getMethod(text));
        emit modified(api, isChanged());
    }

    void Api::handleUrlChanged(const QString &text)
    {
        isModified = true;
        api->setUrl(text);
        // emit contentChanged();
        emit modified(api, isChanged());
    }

    void Api::handleRequestChanged()
    {
        isModified = true;
        emit modified(api, isChanged());
    }

    void Api::handleFoundResponse(const Model::Response &response)
    {
        this->response->setData(response);
        isRequestting = false;
    }

    void Api::sendRequest()
    {
        if (isRequestting)
        {
            return;
        }

        isRequestting = true;
        if (network == nullptr)
        {
            network = new Network(this);
            connect(network, &Network::foundResponse, this, &Api::handleFoundResponse);
        }
        network->request(api);
    }

    bool Api::isChanged() const
    {
        return isModified;
    }
} // namespace Ui
