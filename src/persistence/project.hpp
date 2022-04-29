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

#include <QJsonObject>
#include <vector>
#include "persistence/persistence.hpp"

namespace Model
{
    class Project;
    class Group;
    class Api;
    class Param;
    class Header;
    class Body;
    class FormData;
    class XWwwFormUrlEncoded;
    class Raw;
    class Binary;
} // namespace Model

namespace Persistence
{
    class Project : private Persistence
    {
    public:
        ~Project();
        static Project *getInstance();

        std::vector<Model::Project *> getProjects() const;
        Model::Project *createProject(const QString &name, const QString &description) const;
        void edit(Model::Project *project, const QString &name, const QString &description) const;
        void remove(Model::Project *project) const;
        void load(Model::Project *project) const;
        void load(Model::Api *api) const;
        Model::Group *createGroup(Model::Project *project, const QString &name) const;
        void rename(Model::Group *group, const QString &name) const;
        void remove(Model::Group *group) const;
        Model::Api *createApi(Model::Group *group, const QString &nameconst) const;
        void rename(Model::Api *api, const QString &name) const;
        void remove(Model::Api *api) const;
        void save(Model::Api *api) const;

    private:
        Project();
        Project(Project &) = delete;
        Project &operator=(const Project &) = delete;

        void readSettings() override;
        void writeSettings() override;

        void checkProjectsFile() const;
        QString getProjectsFilePath() const;
        QString getProjectsContent() const;
        QString getProjectFilePath(const QString &projectId) const;
        QString getProjectDirPath(const QString &projectId) const;
        QString getProjectContent(const QString &projectId) const;
        QString getApiFilePath(const QString &projectId, const QString &apiId) const;
        QString getApiContent(const QString &projectId, const QString &apiId) const;
        // QString getProjectContent(const QString &projectId) const;

        QJsonObject convertToJson(Model::Project *project) const;
        QJsonObject convertToJson(Model::Group *group) const;
        QJsonObject convertToJson(Model::Api *api) const;
        QJsonObject convertToJson(Model::Param *param) const;
        QJsonObject convertToJson(Model::Header *header) const;
        QJsonObject convertToJson(Model::Body *body) const;
        QJsonObject convertToJson(Model::FormData *formData) const;
        QJsonObject convertToJson(Model::XWwwFormUrlEncoded *xWwwFormUrlEncoded) const;
        QJsonObject convertToJson(Model::Raw *raw) const;
        QJsonObject convertToJson(Model::Binary *binary) const;

    private:
        QString projectsFilePath;
    };
} // namespace Persistence
