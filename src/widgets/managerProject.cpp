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

#include "./managerProject.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include "models/project.hpp"

namespace Ui
{
    ManagerProject::ManagerProject(Model::Project *project, QWidget *parent)
        : QWidget(parent),
          project{project}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);

        nameLabel = new QLabel(project->getName(), this);
        nameLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(nameLabel);

        setLayout(mainLayout);
        setFixedHeight(160);
        setStyleSheet("border: 1px solid grey;");
        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::ClickFocus);

        contextMenu = new QMenu(this);

        QAction *loadAction = new QAction(tr("Open"), this);
        connect(loadAction, &QAction::triggered, this, &ManagerProject::handleLoadActionTriggered);
        contextMenu->addAction(loadAction);

        QAction *editAction = new QAction(tr("Edit"), this);
        connect(editAction, &QAction::triggered, this, &ManagerProject::handleEditActionTriggered);
        contextMenu->addAction(editAction);

        QAction *exportAction = new QAction(tr("Export"), this);
        connect(exportAction, &QAction::triggered, this, &ManagerProject::handleExportActionTriggered);
        contextMenu->addAction(exportAction);

        QAction *deleteAction = new QAction(tr("Delete"), this);
        connect(deleteAction, &QAction::triggered, this, &ManagerProject::handleDeleteActionTriggered);
        contextMenu->addAction(deleteAction);

        hover = new QWidget(this);
        hover->setContentsMargins(0, 0, 0, 0);
        hover->setStyleSheet("background-color: rgba(0, 0, 0, 120);");

        QVBoxLayout *hoverLayout = new QVBoxLayout();
        hoverLayout->setAlignment(Qt::AlignTop);
        hoverLayout->setSpacing(5);

        descriptionLabel = new QLabel(project->getDescription(), hover);
        descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        descriptionLabel->setAlignment(Qt::AlignTop);
        descriptionLabel->setContentsMargins(5, 5, 5, 5);
        descriptionLabel->setStyleSheet("border: none;");
        hoverLayout->addWidget(descriptionLabel);

        hover->setLayout(hoverLayout);
        hover->hide();
    }

    void ManagerProject::setActive(bool status)
    {
        isActive = status;
        if (status)
        {
            setStyleSheet("border: 1px solid lightgreen;");
        }
        else
        {
            setStyleSheet("border: 1px solid grey;");
        }
    }

    void ManagerProject::edit(const QString &name, const QString &description) const
    {
        nameLabel->setText(name);
        descriptionLabel->setText(description);
    }

    void ManagerProject::enterEvent(QEnterEvent *event)
    {
        hover->resize(size());
        hover->show();

        hoverOpacity = new QGraphicsOpacityEffect(this);
        hoverOpacity->setOpacity(0);
        hover->setGraphicsEffect(hoverOpacity);

        hoverAnimation = new QPropertyAnimation(hoverOpacity, "opacity", this);
        hoverAnimation->setDuration(200);
        hoverAnimation->setStartValue(0);
        hoverAnimation->setEndValue(1);
        hoverAnimation->start();
    }

    void ManagerProject::leaveEvent(QEvent *event)
    {
        hover->hide();
        hoverOpacity->deleteLater();
        hoverOpacity = nullptr;
        hoverAnimation->deleteLater();
        hoverAnimation = nullptr;
    }

    void ManagerProject::focusInEvent(QFocusEvent *event)
    {
        setActive(true);
    }

    void ManagerProject::focusOutEvent(QFocusEvent *event)
    {
        setActive(false);
    }

    void ManagerProject::mouseDoubleClickEvent(QMouseEvent *event)
    {
        emit requestLoad(project);
    }

    void ManagerProject::contextMenuEvent(QContextMenuEvent *event)
    {
        contextMenu->exec(event->globalPos());
    }

    void ManagerProject::handleLoadActionTriggered() const
    {
        emit requestLoad(project);
    }

    void ManagerProject::handleEditActionTriggered() const
    {
        emit requestEdit(project);
    }

    void ManagerProject::handleExportActionTriggered() const
    {
        emit requestExport(project);
    }

    void ManagerProject::handleDeleteActionTriggered() const
    {
        emit requestDelete(project);
    }
} // namespace Ui
