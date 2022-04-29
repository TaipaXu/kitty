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

#include "./tab.hpp"
#include <QHBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include "models/api.hpp"
#include "utils/utils.hpp"

namespace Ui
{
    const QString Tab::format = "kitty/tab";

    Tab::Tab(Model::Api *api, QWidget *parent)
        : QWidget(parent), api{api}, isHovered{false}, isActive{false}, pressed{false}, modified{false}
    {
        QHBoxLayout *mainLayout = new QHBoxLayout();
        mainLayout->setAlignment(Qt::AlignVCenter);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(10, 0, 0, 0);

        label = new QLabel(api->getName(), this);
        mainLayout->addWidget(label);

        closeButton = new QPushButton(this);
        closeButton->setFlat(true);
        closeButton->setToolTip(tr("Close(Ctrl+w)"));
        closeButton->setFixedSize(20, 20);
        closeButton->setIcon(QIcon::fromTheme("tab-close"));
        connect(closeButton, &QPushButton::clicked, this, &Tab::handleCloseButtonClicked);
        mainLayout->addWidget(closeButton);

        setLayout(mainLayout);

        setFixedSize(150, 28);

        contextMenu = new QMenu(this);
        QAction *closeAction = new QAction(tr("Close"), this);
        QAction *closeOthersAction = new QAction(tr("Close others"), this);
        QAction *closeSavedAction = new QAction(tr("Close saved"), this);
        QAction *closeAllAction = new QAction(tr("Close all tabs"), this);
        contextMenu->addAction(closeAction);
        contextMenu->addAction(closeOthersAction);
        contextMenu->addAction(closeSavedAction);
        contextMenu->addAction(closeAllAction);

        setCursor(Qt::PointingHandCursor);

        setMouseTracking(true);
        setAcceptDrops(true);
    }

    bool Tab::contains(const Model::Api *api) const
    {
        return this->api == api;
    }

    void Tab::setActive(bool status)
    {
        isActive = status;
        update();
    }

    bool Tab::isModified() const
    {
        return modified;
    }

    void Tab::setModified(bool modified)
    {
        this->modified = modified;
        if (modified)
        {
            closeButton->setIcon(QIcon(":/images/dot"));
        }
        else
        {
            closeButton->setIcon(QIcon::fromTheme(QString("tab-close")));
        }
    }

    void Tab::mousePressEvent(QMouseEvent *event)
    {
        if (!(event->buttons() & Qt::LeftButton))
        {
            return;
        }

        startPos = event->pos();

        isActive = true;
        pressed = true;

        update();

        emit clicked(api);
    }

    void Tab::mouseMoveEvent(QMouseEvent *event)
    {
        const double distance = std::abs(std::pow(event->pos().x() - startPos.x(), 2) - std::pow(event->pos().y() - startPos.y(), 2));
        if (pressed && distance > 100)
        {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            mimeData->setData(format, Utils::pointerToString(api).toUtf8());
            QPixmap *widgetPixmap = new QPixmap(size());
            render(widgetPixmap);
            drag->setMimeData(mimeData);
            drag->setPixmap(*widgetPixmap);
            drag->setHotSpot(event->pos());
            Qt::DropAction dropAction = drag->exec();

            pressed = false;
        }
    }

    void Tab::mouseReleaseEvent(QMouseEvent *event)
    {
        pressed = false;
    }

    void Tab::enterEvent(QEnterEvent *event)
    {
        isHovered = true;

        update();
    }

    void Tab::leaveEvent(QEvent *event)
    {
        isHovered = false;

        update();
    }

    void Tab::wheelEvent(QWheelEvent *event)
    {
        event->ignore();
    }

    void Tab::dragEnterEvent(QDragEnterEvent *event)
    {
        const QMimeData *mimeData = event->mimeData();
        if (mimeData->hasFormat(format))
        {
            isHovered = true;
            update();

            event->accept();
        }
        else
        {
            event->ignore();
        }
    }

    void Tab::dragLeaveEvent(QDragLeaveEvent *event)
    {
        isHovered = false;
        update();
    }

    void Tab::dropEvent(QDropEvent *event)
    {
        if (event->source() == this && event->possibleActions() & Qt::MoveAction)
        {
            return;
        }

        const QMimeData *mimeData = event->mimeData();
        Model::Api *fromApi = Utils::stringToPointer<Model::Api>(mimeData->data(format));
        emit requestSwap(fromApi, api);

        isHovered = false;
        update();
    }

    void Tab::contextMenuEvent(QContextMenuEvent *event)
    {
        contextMenu->exec(event->globalPos());
    }

    void Tab::paintEvent(QPaintEvent *event)
    {
        static QColor bgColor = palette().color(QWidget::backgroundRole());
        QPalette pal = palette();
        if (isActive)
        {
            pal.setColor(QPalette::Window, bgColor.darker());
        }
        else if (isHovered)
        {
            pal.setColor(QPalette::Window, bgColor.darker());
        }
        else
        {
            pal.setColor(QPalette::Window, bgColor.lighter());
        }
        setAutoFillBackground(true);
        setPalette(pal);
    }

    void Tab::handleCloseButtonClicked()
    {
        emit requestClose(api);
    }
} // namespace Ui
