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

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPlainTextEdit;
QT_END_NAMESPACE

namespace Model
{
    class Project;
} // namespace Model

namespace Ui
{
    class ProjectDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit ProjectDialog(Model::Project *project, QWidget *parent = nullptr);
        explicit ProjectDialog(QWidget *parent = nullptr);
        ~ProjectDialog() = default;

    signals:
        void accepted(const QString &name, const QString &description);

    private slots:
        void handleAcceptButtonClicked();

    private:
        Model::Project *project;
        QLineEdit *nameEdit;
        QPlainTextEdit *descriptionEdit;
    };
} // namespace Ui
