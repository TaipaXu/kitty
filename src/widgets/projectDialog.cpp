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

#include "./projectDialog.hpp"
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include "models/project.hpp"

namespace Ui
{
    ProjectDialog::ProjectDialog(Model::Project *project, QWidget *parent)
        : QDialog(parent), project{project}
    {
        setWindowTitle(tr("Project"));

        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setAlignment(Qt::AlignTop);

        QLabel *nameLabel = new QLabel(tr("Name"), this);
        nameEdit = new QLineEdit(project->getName(), this);
        mainLayout->addWidget(nameLabel);
        mainLayout->addWidget(nameEdit);

        QLabel *descriptionLabel = new QLabel(tr("Description"), this);
        descriptionEdit = new QPlainTextEdit(project->getDescription(), this);
        mainLayout->addWidget(descriptionLabel);
        mainLayout->addWidget(descriptionEdit);

        QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        mainLayout->addItem(verticalSpacer);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
        mainLayout->addWidget(buttonBox);

        setLayout(mainLayout);

        resize(400, 300);

        connect(buttonBox, &QDialogButtonBox::accepted, this, &ProjectDialog::handleAcceptButtonClicked);
    }

    ProjectDialog::ProjectDialog(QWidget *parent)
        : QDialog(parent), project{nullptr}
    {
        setWindowTitle(tr("Project"));

        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->setAlignment(Qt::AlignTop);

        QLabel *nameLabel = new QLabel(tr("Name"), this);
        nameEdit = new QLineEdit(tr("Name"), this);
        mainLayout->addWidget(nameLabel);
        mainLayout->addWidget(nameEdit);

        QLabel *descriptionLabel = new QLabel(tr("Description"), this);
        descriptionEdit = new QPlainTextEdit(tr("Description"), this);
        mainLayout->addWidget(descriptionLabel);
        mainLayout->addWidget(descriptionEdit);

        QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
        mainLayout->addItem(verticalSpacer);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
        mainLayout->addWidget(buttonBox);

        setLayout(mainLayout);

        resize(400, 300);

        connect(buttonBox, &QDialogButtonBox::accepted, this, &ProjectDialog::handleAcceptButtonClicked);
    }

    void ProjectDialog::handleAcceptButtonClicked()
    {
        const QString name = nameEdit->text();
        const QString description = descriptionEdit->toPlainText();
        if (project == nullptr)
        {
            // Create
            if (!name.isEmpty())
            {
                accept();
                emit accepted(name, description);
            }
            else
            {
                reject();
            }
        }
        else
        {
            // Edit
            if (project->getName() != name || project->getDescription() != description)
            {
                accept();
                emit accepted(name, description);
            }
            else
            {
                reject();
            }
        }
    }
} // namespace Ui
