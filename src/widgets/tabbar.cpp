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

#include "./tabbar.hpp"
#include <QHBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>
#include "widgets/tab.hpp"
#include "models/api.hpp"

namespace Ui
{
    Tabbar::Tabbar(QWidget *parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        QHBoxLayout *mainLayout = new QHBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);
        mainLayout->setAlignment(Qt::AlignVCenter);

        scrollArea = new QScrollArea(this);
        scrollArea->setAlignment(Qt::AlignLeft);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QWidget *scrollWidget = new QWidget(scrollArea);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(scrollWidget);
        mainLayout->addWidget(scrollArea);
        tabsLayout = new QHBoxLayout();
        tabsLayout->setSpacing(2);
        tabsLayout->setContentsMargins(0, 0, 0, 0);
        tabsLayout->setAlignment(Qt::AlignLeft);
        scrollWidget->setLayout(tabsLayout);

        setLayout(mainLayout);

        setFixedHeight(30);
    }

    void Tabbar::addApi(Model::Api *api)
    {
        Tab *tab = new Tab(api, this);
        connect(tab, &Tab::clicked, this, &Tabbar::handleTabClicked);
        connect(tab, &Tab::requestClose, this, &Tabbar::requestCloseApi);
        connect(tab, &Tab::requestSwap, this, &Tabbar::swapApi);
        tabs.push_back(tab);
        tabsLayout->addWidget(tab);
    }

    void Tabbar::removeApi(Model::Api *api)
    {
        auto result = std::find_if(tabs.begin(), tabs.end(), [api](Tab *tab) {
            return tab->contains(api);
        });
        if (result != tabs.end())
        {
            Tab *tab = *result;
            tabs.erase(result);
            tabsLayout->removeWidget(tab);
            delete tab;
        }
    }

    void Tabbar::setCurrentApi(Model::Api *api)
    {
        for (auto &&tab : tabs)
        {
            if (tab->contains(api))
            {
                tab->setActive(true);
            }
            else
            {
                tab->setActive(false);
            }
        }
    }

    void Tabbar::setApiModified(const Model::Api *api, bool modified) const
    {
        auto result = std::find_if(tabs.begin(), tabs.end(), [api](Tab *tab) {
            return tab->contains(api);
        });
        (*result)->setModified(modified);
    }

    bool Tabbar::isModified(const Model::Api *api) const
    {
        auto result = std::find_if(tabs.begin(), tabs.end(), [api](Tab *tab) {
            return tab->contains(api);
        });
        return (*result)->isModified();
    }

    void Tabbar::wheelEvent(QWheelEvent *event)
    {
        constexpr const int step = 20;
        int value = scrollArea->horizontalScrollBar()->value();
        if (event->angleDelta().ry() > 0)
        {
            value -= step;
        }
        else
        {
            value += step;
        }
        scrollArea->horizontalScrollBar()->setSliderPosition(value);
    }

    void Tabbar::handleTabClicked(Model::Api *api)
    {
        setCurrentApi(api);
        emit tabActivated(api);
    }

    void Tabbar::swapApi(Model::Api *fromApi, Model::Api *toApi)
    {
        QLayoutItem *item;
        while ((item = tabsLayout->takeAt(0)))
        {
            if (item->widget())
            {
                tabsLayout->removeWidget(item->widget());
            }
        }

        auto fromResult = std::find_if(tabs.begin(), tabs.end(), [fromApi](Tab *tab) {
            return tab->contains(fromApi);
        });
        auto toResult = std::find_if(tabs.begin(), tabs.end(), [toApi](Tab *tab) {
            return tab->contains(toApi);
        });

        std::swap(*fromResult, *toResult);

        for (auto &&i : tabs)
        {
            tabsLayout->addWidget(i);
        }
    }
} // namespace Ui
