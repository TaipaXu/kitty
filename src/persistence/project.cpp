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

#include "./project.hpp"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include "models/project.hpp"
#include "models/group.hpp"
#include "models/api.hpp"
#include "models/method.hpp"
#include "models/param.hpp"
#include "models/header.hpp"
#include "models/body.hpp"
#include "models/formData.hpp"
#include "models/xWwwFormUrlEncoded.hpp"
#include "models/raw.hpp"
#include "models/binary.hpp"
#include "utils/utils.hpp"
#include "config.hpp"

namespace Persistence
{
    Project::Project()
        : Persistence()
    {
        projectsFilePath = getProjectsFilePath();
        checkProjectsFile();
    }

    Project::~Project()
    {
    }

    Project *Project::getInstance()
    {
        static Project instance;
        return &instance;
    }

    std::vector<Model::Project *> Project::getProjects() const
    {
        std::vector<Model::Project *> projects;

        const QString content = getProjectsContent();
        QJsonParseError jsonError;
        const QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            const QJsonArray projectArray = document.array();
            for (auto &&projectJson : projectArray)
            {
                const QJsonObject projectObject = projectJson.toObject();
                projects.push_back(new Model::Project(projectObject["id"].toString(), projectObject["name"].toString(), projectObject["description"].toString()));
            }
        }

        return projects;
    }

    Model::Project *Project::createProject(const QString &name, const QString &description) const
    {
        const QString id = Utils::getUuid();

        const QString content = getProjectsContent();
        QJsonParseError jsonError;
        const QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonArray jsonProjects = document.array();
            QJsonObject jsonProject;
            jsonProject["id"] = id;
            jsonProject["name"] = name;
            jsonProject["description"] = description;
            jsonProjects.append(jsonProject);

            QFile file(projectsFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(jsonProjects).toJson(QJsonDocument::Indented));
            file.close();
        }

        const QString projectDirPath = getProjectDirPath(id);
        const bool result = QDir().mkpath(projectDirPath);

        QJsonObject root;
        root["groups"] = QJsonArray();

        const QString projectFilePath = getProjectFilePath(id);
        QFile file(projectFilePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        file.close();

        Model::Project *project = new Model::Project(id, name, description);
        return project;
    }

    void Project::edit(Model::Project *project, const QString &name, const QString &description) const
    {
        const QString projectId = project->getId();

        const QString projectsContent = getProjectsContent();
        QJsonParseError projectsJsonError;
        QJsonDocument projectsDocument = QJsonDocument::fromJson(projectsContent.toUtf8(), &projectsJsonError);
        if (!projectsDocument.isNull() && projectsJsonError.error == QJsonParseError::NoError)
        {
            QJsonArray projectsArray = projectsDocument.array();
            for (size_t i = 0; i < projectsArray.size(); i++)
            {
                QJsonObject jsonProject = projectsArray[i].toObject();
                const QString id = jsonProject["id"].toString();
                if (id == projectId)
                {
                    jsonProject["name"] = name;
                    jsonProject["description"] = description;
                    projectsArray.replace(i, jsonProject);
                }
            }

            QFile file(projectsFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(projectsArray).toJson(QJsonDocument::Indented));
            file.close();
        }
    }

    void Project::remove(Model::Project *project) const
    {
        const QString id = project->getId();
        const QString projectDirPath = getProjectDirPath(id);
        const bool result = QDir(projectDirPath).removeRecursively();

        const QString content = getProjectsContent();
        QJsonParseError jsonError;
        const QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonArray projectsArray = document.array();
            for (size_t i = 0; i < projectsArray.size(); i++)
            {
                const QJsonObject projectObject = projectsArray[i].toObject();
                const QString projectId = projectObject["id"].toString();
                if (projectId == id)
                {
                    projectsArray.removeAt(i);
                }
            }

            const QString projectsFilePath = getProjectsFilePath();
            QFile file(projectsFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(projectsArray).toJson(QJsonDocument::Indented));
            file.close();
        }
    }

    void Project::load(Model::Project *project) const
    {
        const QString content = getProjectContent(project->getId());

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject root = document.object();
            std::list<Model::Group *> groups;
            for (auto &&groupJson : root["groups"].toArray())
            {
                QJsonObject groupObject = groupJson.toObject();
                std::list<Model::Api *> apis;
                for (auto &&apiJson : groupObject["apis"].toArray())
                {
                    QJsonObject apiObject = apiJson.toObject();
                    apis.push_back(new Model::Api(apiObject["id"].toString(), apiObject["name"].toString()));
                }

                Model::Group *group = new Model::Group(groupObject["id"].toString(), groupObject["name"].toString(), apis);
                groups.push_back(group);
            }

            project->setGroups(groups);
        }
    }

    void Project::load(Model::Api *api) const
    {
        const QString projectId = api->getGroup()->getProject()->getId();
        const QString apiId = api->getId();
        const QString content = getApiContent(projectId, apiId);
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject apiObject = document.object();
            const QString apiId = apiId;
            const QString apiName = api->getName();
            const QString apiUrl = apiObject["url"].toString();
            const QString apiMethodStr = apiObject["method"].toString();

            std::list<Model::Param *> params;
            for (auto &&paramJson : apiObject["params"].toArray())
            {
                QJsonObject paramObject = paramJson.toObject();
                const QString key = paramObject["key"].toString();
                const QString value = paramObject["value"].toString();
                const QString description = paramObject["description"].toString();
                const bool enabled = !(paramObject.contains("enabled") && !paramObject["enabled"].toBool());
                params.push_back(new Model::Param(key, value, description, enabled));
            }

            std::list<Model::Header *> headers;
            for (auto &&headerJson : apiObject["headers"].toArray())
            {
                QJsonObject headerObject = headerJson.toObject();
                const QString key = headerObject["key"].toString();
                const QString value = headerObject["value"].toString();
                const QString description = headerObject["description"].toString();
                const bool enabled = !(headerObject.contains("enabled") && !headerObject["enabled"].toBool());
                headers.push_back(new Model::Header(key, value, description, enabled));
            }

            QJsonValue bodyValue = apiObject["body"];
            Model::Body *body = nullptr;
            if (!bodyValue.isNull())
            {
                QJsonObject bodyObject = apiObject["body"].toObject();
                const QString type = bodyObject["type"].toString();
                body = new Model::Body(Model::getBodyType(type));
                if (type == "form_data")
                {
                    std::list<Model::FormData *> formDatas;
                    for (auto &&dataJson : bodyObject["data"].toArray())
                    {
                        QJsonObject dataObject = dataJson.toObject();
                        const QString type = dataObject["type"].toString();
                        const QString key = dataObject["key"].toString();
                        const QString value = dataObject["value"].toString();
                        const QString description = dataObject["description"].toString();
                        const bool enabled = !(dataObject.contains("enabled") && !dataObject["enabled"].toBool());
                        const Model::FormData::Type FormDataType = Model::getFormDataType(type);
                        formDatas.push_back(new Model::FormData(FormDataType, key, value, description, enabled));
                    }
                    body->setData(formDatas);
                }
                else if (type == "x_www_form_urlencoded")
                {
                    std::list<Model::XWwwFormUrlEncoded *> xWwwFormUrlEncodeds;
                    for (auto &&dataJson : bodyObject["data"].toArray())
                    {
                        QJsonObject dataObject = dataJson.toObject();
                        const QString key = dataObject["key"].toString();
                        const QString value = dataObject["value"].toString();
                        const QString description = dataObject["description"].toString();
                        const bool enabled = !(dataObject.contains("enabled") && !dataObject["enabled"].toBool());
                        xWwwFormUrlEncodeds.push_back(new Model::XWwwFormUrlEncoded{key, value, description, enabled});
                    }

                    body->setData(xWwwFormUrlEncodeds);
                }
                else if (type == "raw")
                {
                    QJsonObject dataObject = bodyObject["data"].toObject();
                    const QString type = dataObject["type"].toString();
                    const QString data = dataObject["data"].toString();
                    Model::Raw *raw = new Model::Raw(Model::getRawType(type), data);

                    body->setData(raw);
                }
                else if (type == "binary")
                {
                    const QString data = bodyObject["data"].toString();
                    Model::Binary *binary = new Model::Binary(data);
                    body->setData(binary);
                }
            }
            else
            {
                body = new Model::Body(Model::Body::Type::None);
            }

            Model::Method method = Model::getMethod(apiMethodStr);
            api->setData(apiUrl, method, params, headers, body);
        }
    }

    Model::Group *Project::createGroup(Model::Project *project, const QString &name) const
    {
        const QString dataPath = getDataDirPath();
        const QString projectsFile = QString("%1/%2/project.json").arg(dataPath).arg(project->getId());
        QFile file(projectsFile);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        const QString content = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            const QString id = Utils::getUuid();
            QJsonObject root = document.object();
            QJsonArray groups = root["groups"].toArray();
            QJsonObject groupObject;
            groupObject["id"] = id;
            groupObject["name"] = name;
            groupObject["apis"] = QJsonArray();
            groups.push_back(groupObject);
            root["groups"] = groups;

            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
            file.close();

            Model::Group *group = new Model::Group(id, name, {});
            return group;
        }
    }

    void Project::rename(Model::Group *group, const QString &name) const
    {
        Model::Project *project = group->getProject();
        const QString projectId = project->getId();
        const QString content = getProjectContent(projectId);

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject root = document.object();
            QJsonArray groupsArray = root["groups"].toArray();
            for (size_t i = 0; i < groupsArray.size(); i++)
            {
                QJsonObject groupObject = groupsArray[i].toObject();
                const QString groupId = groupObject["id"].toString();
                if (groupId == group->getId())
                {
                    groupObject["name"] = name;
                    groupsArray.replace(i, groupObject);
                    break;
                }
            }
            root["groups"] = groupsArray;

            const QString projectFilePath = getProjectFilePath(projectId);
            QFile file(projectFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
            file.close();
        }
    }

    void Project::remove(Model::Api *api) const
    {
        Model::Group *group = api->getGroup();
        Model::Project *project = group->getProject();
        const QString projectId = project->getId();
        const QString content = getProjectContent(projectId);

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject root = document.object();
            QJsonArray groupsArray = root["groups"].toArray();
            for (size_t i = 0; i < groupsArray.size(); i++)
            {
                QJsonObject groupObject = groupsArray[i].toObject();
                const QString groupId = groupObject["id"].toString();
                if (groupId == group->getId())
                {
                    QJsonArray apisArray = groupObject["apis"].toArray();
                    for (size_t j = 0; j < apisArray.size(); j++)
                    {
                        QJsonObject apiObject = apisArray[j].toObject();
                        if (apiObject["id"].toString() == api->getId())
                        {
                            apisArray.removeAt(j);

                            const QString apiId = apiObject["id"].toString();
                            const QString apiFilePath = getApiFilePath(projectId, apiId);
                            QFile file(apiFilePath);
                            file.remove();
                            break;
                        }
                    }

                    groupObject["apis"] = apisArray;
                    groupsArray.replace(i, groupObject);
                    break;
                }
            }
            root["groups"] = groupsArray;

            const QString projectFilePath = getProjectFilePath(projectId);
            QFile file(projectFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
            file.close();
        }
    }

    void Project::remove(Model::Group *group) const
    {
        Model::Project *project = group->getProject();
        const QString projectId = project->getId();
        const QString content = getProjectContent(projectId);

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject root = document.object();
            QJsonArray groupsArray = root["groups"].toArray();
            for (size_t i = 0; i < groupsArray.size(); i++)
            {
                QJsonObject groupObject = groupsArray[i].toObject();
                const QString groupId = groupObject["id"].toString();
                if (groupId == group->getId())
                {
                    QJsonArray apis = groupObject["apis"].toArray();
                    for (size_t j = 0; j < apis.size(); j++)
                    {
                        QJsonObject apiObject = apis[j].toObject();
                        const QString apiId = apiObject["id"].toString();
                        const QString apiFilePath = getApiFilePath(projectId, apiId);
                        QFile file(apiFilePath);
                        file.remove();
                    }
                    groupsArray.removeAt(i);
                    break;
                }
            }
            root["groups"] = groupsArray;

            const QString projectFilePath = getProjectFilePath(projectId);
            QFile file(projectFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
            file.close();
        }
    }

    Model::Api *Project::createApi(Model::Group *group, const QString &name) const
    {
        Model::Project *project = group->getProject();
        const QString projectId = project->getId();
        const QString id = Utils::getUuid();
        const QString content = getProjectContent(projectId);
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(content.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject root = document.object();
            QJsonArray groups = root["groups"].toArray();
            for (size_t i = 0; i < groups.size(); i++)
            {
                QJsonObject groupObject = groups[i].toObject();
                const QString groupId = groupObject["id"].toString();
                if (groupId == group->getId())
                {
                    QJsonArray apis = groupObject["apis"].toArray();
                    QJsonObject apiObject;
                    apiObject["id"] = id;
                    apiObject["name"] = name;
                    apis.push_back(apiObject);
                    groupObject["apis"] = apis;
                    groups.replace(i, groupObject);
                    break;
                }
            }
            root["groups"] = groups;

            const QString projectFilePath = getProjectFilePath(projectId);
            QFile projectFile(projectFilePath);
            projectFile.open(QIODevice::WriteOnly | QIODevice::Text);
            projectFile.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
            projectFile.close();

            Model::Api *api = new Model::Api(id, name, "", Model::Method::GET, {}, {}, new Model::Body(Model::Body::Type::None));
            const QString apiFilePath = getApiFilePath(project->getId(), id);
            QFile apiFile(apiFilePath);
            apiFile.open(QIODevice::WriteOnly | QIODevice::Text);
            apiFile.write(QJsonDocument(convertToJson(api)).toJson(QJsonDocument::Indented));
            apiFile.close();

            return api;
        }
    }

    void Project::rename(Model::Api *api, const QString &name) const
    {
        Model::Group *group = api->getGroup();
        Model::Project *project = group->getProject();

        const QString content = getProjectContent(project->getId());
        const QString json = getProjectContent(group->getProject()->getId());
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(json.toUtf8(), &jsonError);
        if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
        {
            QJsonObject root = document.object();
            QJsonArray groupsArray = root["groups"].toArray();
            for (size_t i = 0; i < groupsArray.size(); i++)
            {
                QJsonObject groupObject = groupsArray[i].toObject();
                const QString groupId = groupObject["id"].toString();
                if (groupId == group->getId())
                {
                    QJsonArray apisArray = groupObject["apis"].toArray();
                    for (size_t j = 0; j < apisArray.size(); j++)
                    {
                        QJsonObject apiObject = apisArray[j].toObject();
                        if (apiObject["id"].toString() == api->getId())
                        {
                            apiObject["name"] = name;
                            apisArray.replace(j, apiObject);
                            break;
                        }
                    }

                    groupObject["apis"] = apisArray;
                    groupsArray.replace(i, groupObject);
                    break;
                }
            }
            root["groups"] = groupsArray;

            const QString projectFilePath = getProjectFilePath(project->getId());
            QFile file(projectFilePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
            file.close();
        }
    }

    void Project::save(Model::Api *api) const
    {
        const QString projectId = api->getGroup()->getProject()->getId();
        const QString apiFilePath = getApiFilePath(projectId, api->getId());
        QFile file(apiFilePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(QJsonDocument(convertToJson(api)).toJson(QJsonDocument::Indented));
        file.close();
    }

    void Project::readSettings()
    {
    }

    void Project::writeSettings()
    {
    }

    void Project::checkProjectsFile() const
    {
        const QString dataDirPath = getDataDirPath();
        QDir dataDir(dataDirPath);
        if (!dataDir.exists())
        {
            dataDir.mkpath(dataDirPath);
        }

        const QString projectsFilePath = getProjectsFilePath();
        QFile file(projectsFilePath);
        if (!file.exists())
        {
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write("{}");
            file.close();
        }
    }

    QString Project::getProjectsFilePath() const
    {
        const QString dataPath = getDataDirPath();
        const QString fileName = QStringLiteral("projects.json");
        QString filePath;
        filePath = QString("%1/%2").arg(dataPath).arg(fileName);

        return filePath;
    }

    QString Project::getProjectsContent() const
    {
        QFile file(projectsFilePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        const QString content = file.readAll();
        file.close();

        return content;
    }

    QString Project::getProjectFilePath(const QString &projectId) const
    {
        return QString("%1/%2/project.json").arg(dataDirPath).arg(projectId);
    }

    QString Project::getProjectDirPath(const QString &projectId) const
    {
        return QString("%1/%2").arg(dataDirPath).arg(projectId);
    }

    QString Project::getProjectContent(const QString &projectId) const
    {
        const QString projectFilePath = getProjectFilePath(projectId);
        QFile file(projectFilePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        const QString content = file.readAll();
        file.close();

        return content;
    }

    QString Project::getApiFilePath(const QString &projectId, const QString &apiId) const
    {
        return getProjectDirPath(projectId) + "/" + apiId + ".json";
    }

    QString Project::getApiContent(const QString &projectId, const QString &apiId) const
    {
        const QString apiFilePath = getApiFilePath(projectId, apiId);
        QFile file(apiFilePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        const QString content = file.readAll();
        file.close();

        return content;
    }

    QJsonObject Project::convertToJson(Model::Project *project) const
    {
        QJsonObject json;
        json["id"] = project->getId();
        json["name"] = project->getName();
        json["description"] = project->getDescription();

        return json;
    }

    QJsonObject Project::convertToJson(Model::Group *group) const
    {
        QJsonObject json;
        json["id"] = group->getId();
        json["name"] = group->getName();
        QJsonArray apis;
        for (auto &&api : group->getApis())
        {
            apis.push_back(convertToJson(api));
        }
        json["apis"] = apis;

        return json;
    }

    QJsonObject Project::convertToJson(Model::Api *api) const
    {
        QJsonObject json;
        json["url"] = api->getUrl();
        json["method"] = Model::getMethodName(api->getMethod());

        QJsonArray paramsArray;
        for (auto &&param : api->getParams())
        {
            paramsArray.push_back(convertToJson(param));
        }
        json["params"] = paramsArray;

        QJsonArray headersArray;
        for (auto &&header : api->getHeaders())
        {
            headersArray.push_back(convertToJson(header));
        }
        json["headers"] = headersArray;

        if (api->getMethod() == Model::Method::GET)
        {
            json["body"] = QJsonValue::Null;
        }
        else
        {
            Model::Body *body = api->getBody();
            Model::Body::Type type = body->getType();

            if (type == Model::Body::Type::None)
            {
                json["body"] = QJsonValue::Null;
            }
            else
            {
                json["body"] = convertToJson(body);
            }
        }

        return json;
    }

    QJsonObject Project::convertToJson(Model::Param *param) const
    {
        QJsonObject paramObject;
        paramObject["enabled"] = param->getEnabled();
        paramObject["key"] = param->getKey();
        paramObject["value"] = param->getValue();
        paramObject["description"] = param->getDescription();

        return paramObject;
    }

    QJsonObject Project::convertToJson(Model::Header *header) const
    {
        QJsonObject headerObject;
        headerObject["enabled"] = header->getEnabled();
        headerObject["key"] = header->getKey();
        headerObject["value"] = header->getValue();
        headerObject["description"] = header->getDescription();

        return headerObject;
    }

    QJsonObject Project::convertToJson(Model::Body *body) const
    {
        QJsonObject bodyObject;
        Model::Body::Type type = body->getType();
        bodyObject["type"] = Model::getBodyTypeName(type);

        QJsonArray bodyDataArray;
        if (type == Model::Body::Type::FormData)
        {
            for (auto &&formData : body->getFormDatas())
            {
                bodyDataArray.push_back(convertToJson(formData));
            }
            bodyObject["data"] = bodyDataArray;
        }
        else if (type == Model::Body::Type::XWwwFormUrlEncoded)
        {
            for (auto &&xWwwFormUrlEncoded : body->getXWwwFormUrlEncodeds())
            {
                bodyDataArray.push_back(convertToJson(xWwwFormUrlEncoded));
            }
            bodyObject["data"] = bodyDataArray;
        }
        else if (type == Model::Body::Type::Raw)
        {
            bodyObject["data"] = convertToJson(body->getRaw());
        }
        else if (type == Model::Body::Type::Binary)
        {
            Model::Binary *binary = body->getBinary();
            bodyObject["data"] = binary->getPath();
        }

        return bodyObject;
    }

    QJsonObject Project::convertToJson(Model::FormData *formData) const
    {
        QJsonObject json;
        json["type"] = Model::getFormDataTypeName(formData->getType());
        json["key"] = formData->getKey();
        json["value"] = formData->getValue();
        json["description"] = formData->getDescription();

        return json;
    }

    QJsonObject Project::convertToJson(Model::XWwwFormUrlEncoded *xWwwFormUrlEncoded) const
    {
        QJsonObject json;
        json["key"] = xWwwFormUrlEncoded->getKey();
        json["value"] = xWwwFormUrlEncoded->getValue();
        json["description"] = xWwwFormUrlEncoded->getDescription();
        json["enableds"] = xWwwFormUrlEncoded->getEnabled();

        return json;
    }

    QJsonObject Project::convertToJson(Model::Raw *raw) const
    {
        QJsonObject json;
        json["type"] = Model::getRawTypeName(raw->getType());
        json["data"] = raw->getData();

        return json;
    }

    QJsonObject Project::convertToJson(Model::Binary *binary) const
    {
    }
} // namespace Persistence
