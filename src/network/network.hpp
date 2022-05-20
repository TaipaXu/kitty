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
#include <QObject>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QElapsedTimer;
QT_END_NAMESPACE

typedef void CURL;

namespace Model
{
    class Response;
} // namespace Model

namespace Model
{
    class Api;
}

class Network : public QObject
{
    Q_OBJECT

public:
    explicit Network(QObject *parent = nullptr);
    ~Network();

    void request(Model::Api *api);
    void cancel();

signals:
    void foundResponse(const Model::Response &response);

private slots:
    void handleResult(QNetworkReply *reply);

private:
    // QNetworkAccessManager *manager;
    // QNetworkReply *reply;
    CURL *curl;
    bool isGoing;
    // QElapsedTimer *timer;
};
