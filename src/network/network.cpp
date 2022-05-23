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
#include <QMimeDatabase>
#include <list>
extern "C"
{
#include <curl/curl.h>
}
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
#include "persistence/apiSettings.hpp"

size_t receiveData(void *contents, size_t size, size_t nmemb, std::string *userdata)
{
    size_t newLength = size * nmemb;
    try
    {
        userdata->append(( char * )contents, newLength);
    }
    catch (std::bad_alloc &e)
    {
        return 0;
    }
    return newLength;
}

Network::Network(QObject *parent)
    : QObject{parent}, curl{nullptr}, isGoing{false}
{
}

Network::~Network()
{
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
}

void Network::request(Model::Api *api)
{
    if (!curl)
    {
        curl = curl_easy_init();
    }
    else
    {
        curl_easy_reset(curl);
    }

    QUrl url(api->getUrl());
    QUrlQuery query;
    for (auto &&param : api->getParams())
    {
        query.addQueryItem(param->getKey(), param->getValue());
    }
    url.setQuery(query);
    curl_easy_setopt(curl, CURLOPT_URL, url.toEncoded().data());

    Persistence::ApiSettings *apiSettings = Persistence::ApiSettings::getInstance();
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, apiSettings->getAutoFollowRedirects() ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, apiSettings->getEnableSslVerification() ? 1L : 0L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receiveData);
    std::string responseHeadersStr;
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeadersStr);
    std::string responseContent;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseContent);

    struct curl_slist *headers = nullptr;
    for (auto &&header : api->getHeaders())
    {
        headers = curl_slist_append(headers, QString("%1: %2").arg(header->getKey()).arg(header->getValue()).toStdString().c_str());
    }

    const Model::Method method = api->getMethod();
    switch (method)
    {
    case Model::Method::GET:
    {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, nullptr);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

        curl_easy_perform(curl);
        break;
    }

    case Model::Method::POST:
    case Model::Method::PUT:
    case Model::Method::PATCH:
    {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, nullptr);

        switch (method)
        {
        case Model::Method::POST:
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

            break;
        }

        case Model::Method::PUT:
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

            break;
        }

        case Model::Method::PATCH:
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");

            break;
        }

        default:
            break;
        }

        switch (api->getBody()->getType())
        {
        case Model::Body::Type::None:
        {
            curl_easy_perform(curl);

            break;
        }

        case Model::Body::Type::FormData:
        {
            curl_httppost *post = nullptr;
            curl_httppost *last = nullptr;
            for (auto &&formData : api->getBody()->getFormDatas())
            {
                if (!formData->getEnabled())
                {
                    continue;
                }

                const Model::FormData::Type type = formData->getType();
                if (type == Model::FormData::Type::Text)
                {
                    curl_formadd(&post, &last, CURLFORM_COPYNAME, formData->getKey().toStdString().c_str(), CURLFORM_COPYCONTENTS, formData->getValue().toStdString().c_str(), CURLFORM_END);
                }
                else if (type == Model::FormData::Type::File)
                {
                    curl_formadd(&post,
                                 &last,
                                 CURLFORM_COPYNAME, formData->getKey().toStdString().c_str(),
                                 CURLFORM_FILE, formData->getValue().toStdString().c_str(),
                                 CURLFORM_CONTENTTYPE, QMimeDatabase().mimeTypeForFile(formData->getValue()).name().toStdString().c_str(),
                                 CURLFORM_END);
                }
            }
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_perform(curl);

            curl_formfree(post);

            break;
        }

        case Model::Body::Type::XWwwFormUrlEncoded:
        {
            std::string content;
            for (auto &&xWwwFormUrlEncoded : api->getBody()->getXWwwFormUrlEncodeds())
            {
                if (!xWwwFormUrlEncoded->getEnabled())
                {
                    continue;
                }

                if (!content.empty())
                {
                    content += "&";
                }
                content += QString("%1=%2").arg(xWwwFormUrlEncoded->getKey()).arg(xWwwFormUrlEncoded->getValue()).toStdString();
            }

            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(content.length()));
            curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, content.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_perform(curl);

            break;
        }

        case Model::Body::Type::Raw:
        {
            Model::Raw *raw = api->getBody()->getRaw();
            const std::string content = raw->getData().toStdString();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(content.length()));
            curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, content.c_str());

            std::string contentTypeHeader;
            switch (raw->getType())
            {
            case Model::Raw::Type::Text:
            {
                contentTypeHeader = "Content-Type: text/plain";
                break;
            }

            case Model::Raw::Type::JavaScript:
            {
                contentTypeHeader = "Content-Type: application/javascript";
                break;
            }

            case Model::Raw::Type::Json:
            {
                contentTypeHeader = "Content-Type: application/json";
                break;
            }

            case Model::Raw::Type::Html:
            {
                contentTypeHeader = "Content-Type: text/html";
                break;
            }

            case Model::Raw::Type::Xml:
            {
                contentTypeHeader = "Content-Type: application/xml";
                break;
            }

            default:
                break;
            }
            headers = curl_slist_append(headers, contentTypeHeader.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_perform(curl);

            break;
        }

        case Model::Body::Type::Binary:
        {
            QFile file(api->getBody()->getBinary()->getPath());
            if (!file.open(QIODevice::ReadOnly))
                return;
            QByteArray data = file.readAll();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(data.length()));
            curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, data.data());
            headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_perform(curl);

            break;
        }

        default:
            break;
        }

        break;
    }

    case Model::Method::HEAD:
    {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, nullptr);

        curl_easy_perform(curl);
        break;
    }

    case Model::Method::DELETE:
    {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 0L);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

        curl_easy_perform(curl);
        break;
    }

    case Model::Method::OPTIONS:
    {
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");

        curl_easy_perform(curl);
        break;
    }

        // case Model::Method::TRACE:
        // {
        // }

    default:
        break;
    }

    long responsecode;
    double totalTime;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responsecode);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &totalTime);

    double nameLookupTime;
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &nameLookupTime);
    double connectTime;
    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connectTime);
    double appConnectTime;
    curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &appConnectTime);
    double pretransferTime;
    curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &pretransferTime);
    double startTransferTime;
    curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &startTransferTime);
    double redirectTime;
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_TIME, &redirectTime);

    double dnsLookupTime = nameLookupTime * 1000;
    double tcpHandshakeTime = (connectTime - nameLookupTime) * 1000;
    double sslHandshakeTime = (appConnectTime - connectTime) * 1000;
    double transferStartTime = (startTransferTime - pretransferTime) * 1000;
    double downloadTime = (totalTime - startTransferTime) * 1000;

    Model::Response response;
    response.setStatus(responsecode);
    for (auto &&i : QString::fromStdString(responseHeadersStr).split("\r\n"))
    {
        if (i.contains(":"))
        {
            auto &&header = i.split(":");
            const QString key = header[0].trimmed();
            const QString value = header[1].trimmed();
            response.addHeader(Model::Response::Header{key, value});
        }
    }
    response.setTotalTime(totalTime * 1000);
    response.setDnsLookupTime(dnsLookupTime);
    response.setTcpHandshakeTime(tcpHandshakeTime);
    response.setSslHandshakeTime(sslHandshakeTime);
    response.setTransferStartTime(transferStartTime);
    response.setDownloadTime(downloadTime);
    response.setBody(QString::fromStdString(responseContent));
    emit foundResponse(response);

    curl_slist_free_all(headers);
}

void Network::cancel()
{
    if (isGoing)
    {
    }
}

void Network::handleResult(QNetworkReply *reply)
{
    isGoing = false;
}
