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

#include "network.hpp"
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QElapsedTimer>
#include <list>
#include "models/api.hpp"
#include "models/param.hpp"
#include "models/header.hpp"
#include "models/body.hpp"
#include "models/formData.hpp"
#include "models/xWwwFormUrlEncoded.hpp"
#include "models/body.hpp"
#include "models/raw.hpp"
#include "models/binary.hpp"
#include "models/response.hpp"

Network::Network(QObject *parent)
    : QObject{parent}, manager{nullptr}, reply{nullptr}, isGoing{false}
{
    timer = new QElapsedTimer();
}

Network::~Network()
{
    delete timer;
}

void Network::request(Model::Api *api)
{
    isGoing = true;
    if (reply)
    {
        reply->abort();
        reply = nullptr;
    }

    if (!manager)
    {
        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &Network::handleResult);
    }

    QNetworkRequest request;
    QUrl url(api->getUrl());
    QUrlQuery query;
    std::list<Model::Param *> params = api->getParams();
    for (auto &&param : params)
    {
        query.addQueryItem(param->getKey(), param->getValue());
    }
    url.setQuery(query);
    request.setUrl(url);

    std::list<Model::Header *> headers = api->getHeaders();
    for (auto &&header : headers)
    {
        request.setRawHeader(header->getKey().toUtf8(), header->getValue().toUtf8());
    }

    Model::Method method = api->getMethod();
    if (method == Model::Method::GET || method == Model::Method::HEAD || method == Model::Method::DELETE || method == Model::Method::CONNECT || method == Model::Method::OPTIONS || method == Model::Method::TRACE)
    {
        timer->start();
        reply = manager->get(request);
    }
    else
    {
        switch (api->getBody()->getType())
        {
        case Model::Body::Type::None:
        {
            QByteArray data;
            timer->start();
            reply = manager->post(request, data);

            break;
        }

        case Model::Body::Type::FormData:
        {
            // request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");

            QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
            const std::list<Model::FormData *> formDatas = api->getBody()->getFormDatas();
            for (auto &&formData : formDatas)
            {
                if (!formData->getEnabled())
                {
                    continue;
                }

                const Model::FormData::Type type = formData->getType();
                if (type == Model::FormData::Type::Text)
                {
                    QHttpPart requestPart;
                    requestPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
                    const QString header = QString("form-data; name=\"%1\"").arg(formData->getKey());
                    requestPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
                    requestPart.setBody(formData->getValue().toUtf8());
                    multiPart->append(requestPart);
                }
                else if (type == Model::FormData::Type::File)
                {
                    QHttpPart requestPart;
                    const QString header = QString("form-data; name=\"%1\"").arg(formData->getKey());
                    // TODO
                    // requestPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
                    requestPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
                    QFile *file = new QFile(formData->getValue());
                    file->open(QIODevice::ReadOnly);
                    requestPart.setBodyDevice(file);
                    multiPart->append(requestPart);
                }
            }
            timer->start();
            reply = manager->post(request, multiPart);

            break;
        }

        case Model::Body::Type::XWwwFormUrlEncoded:
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            QUrlQuery postData;
            std::list<Model::XWwwFormUrlEncoded *> xWwwFormUrlEncodeds = api->getBody()->getXWwwFormUrlEncodeds();
            for (auto &&xWwwFormUrlEncoded : xWwwFormUrlEncodeds)
            {
                if (!xWwwFormUrlEncoded->getEnabled())
                {
                    continue;
                }

                postData.addQueryItem(xWwwFormUrlEncoded->getKey(), xWwwFormUrlEncoded->getValue());
            }

            timer->start();
            reply = manager->post(request, postData.toString().toUtf8());
            break;
        }

        case Model::Body::Type::Raw:
        {
            Model::Raw *raw = api->getBody()->getRaw();
            switch (raw->getType())
            {
            case Model::Raw::Type::Text:
            {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
                break;
            }

            case Model::Raw::Type::JavaScript:
            {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/javascript");
                break;
            }

            case Model::Raw::Type::Json:
            {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                break;
            }

            case Model::Raw::Type::Html:
            {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html");
                break;
            }

            case Model::Raw::Type::Xml:
            {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
                break;
            }

            default:
                break;
            }

            timer->start();
            reply = manager->post(request, raw->getData().toUtf8());

            break;
        }

        case Model::Body::Type::Binary:
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

            QFile *compressedFile = new QFile(api->getBody()->getBinary()->getPath());
            compressedFile->open(QIODevice::ReadOnly);

            timer->start();
            reply = manager->post(request, compressedFile);
            compressedFile->setParent(reply);

            break;
        }

        default:
            break;
        }
    }
}

void Network::cancel()
{
    if (isGoing)
    {
        reply->abort();
        // reply->close();
        reply = nullptr;
    }
}

void Network::handleResult(QNetworkReply *reply)
{
    isGoing = false;

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    Model::Response response;
    response.setStatus(statusCode.toInt());
    response.setMillseconds(timer->elapsed());
    for (auto &&i : reply->rawHeaderPairs())
    {
        response.addHeader(Model::Response::Header{i.first, i.second});
    }

    const QString content = reply->readAll();
    response.setBody(content);
    emit foundResponse(response);

    reply->deleteLater();
    reply = nullptr;
    this->reply = nullptr;
}
