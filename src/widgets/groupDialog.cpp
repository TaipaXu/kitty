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

#include "./groupDialog.hpp"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include "models/group.hpp"

namespace Ui
{
    GroupDialog::GroupDialog(std::list<Model::Group *> groups, Model::Group *currentGroup, QWidget *parent)
        : QDialog(parent),
          groups{groups},
          currentGroup{currentGroup}
    {
        QVBoxLayout *mainLayout = new QVBoxLayout();

        QFormLayout *formLayout = new QFormLayout();

        QComboBox *groupSelector = new QComboBox(this);
        connect(groupSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GroupDialog::handleGroupSelectorChanged);
        for (auto &&i : groups)
        {
            groupSelector->addItem(i->getName());
        }
        if (currentGroup != nullptr)
        {
            auto it = std::find(groups.begin(), groups.end(), currentGroup);
            int index = std::distance(groups.begin(), it);
            groupSelector->setCurrentIndex(index);
        }
        formLayout->addRow("Group", groupSelector);

        QLineEdit *groupNameInput = new QLineEdit(this);
        connect(groupNameInput, &QLineEdit::textChanged, this, &GroupDialog::handleNameChanged);
        formLayout->addRow("Name", groupNameInput);

        mainLayout->addLayout(formLayout);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &GroupDialog::handleAcceptButtonClicked);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &GroupDialog::reject);
        mainLayout->addWidget(buttonBox);

        setLayout(mainLayout);
        setFixedSize(400, 130);
    }

    Model::Group *GroupDialog::getCurrentGroup() const
    {
        return currentGroup;
    }

    QString GroupDialog::getName() const
    {
        return name;
    }

    void GroupDialog::handleGroupSelectorChanged(int index)
    {
        auto it = groups.begin();
        std::advance(it, index);
        currentGroup = *it;
    }

    void GroupDialog::handleNameChanged(const QString &text)
    {
        name = text;
    }

    void GroupDialog::handleAcceptButtonClicked()
    {
        if (currentGroup != nullptr && !name.isEmpty())
        {
            accept();
        }
    }
} // namespace Ui
