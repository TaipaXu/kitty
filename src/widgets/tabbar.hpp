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
class QHBoxLayout;
class QScrollArea;
QT_END_NAMESPACE

namespace Model
{
    class Api;
} // namespace Model

namespace Ui
{
    class Tab;

    class Tabbar : public QWidget
    {
        Q_OBJECT

    public:
        explicit Tabbar(QWidget *parent = nullptr);
        ~Tabbar() = default;

        void addApi(Model::Api *api);
        void removeApi(Model::Api *api);
        void setCurrentApi(Model::Api *api);
        void setApiModified(const Model::Api *api, bool modified) const;
        bool isModified(const Model::Api *api) const;

    signals:
        void tabActivated(Model::Api *api) const;
        void requestCloseApi(Model::Api *api) const;

    protected:
        void wheelEvent(QWheelEvent *event) override;

    private slots:
        void handleTabClicked(Model::Api *api);
        // void handleTabRequestClose(Model::Api *api);
        void swapApi(Model::Api *fromApi, Model::Api *toApi);

    private:
        QScrollArea *scrollArea;
        QHBoxLayout *tabsLayout;
        std::list<Tab *> tabs;
    };
} // namespace Ui
