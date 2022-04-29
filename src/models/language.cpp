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

#include "./language.hpp"

namespace Model
{
    Language getLanguage(const QString &name)
    {
        if (name == QStringLiteral("English"))
        {
            return Language::En;
        }
        else if (name == QStringLiteral("中文"))
        {
            return Language::Zh;
        }
    }

    QString getLanguageName(Language language)
    {
        switch (language)
        {
        case Language::En:
        {
            return QStringLiteral("English");
        }

        case Language::Zh:
        {
            return QStringLiteral("中文");
        }

        default:
        {
            break;
        }
        }
    }

    QStringList getLanguageNames()
    {
        QStringList names;
        for (auto &&language : languages)
        {
            names << getLanguageName(language);
        }

        return names;
    }
} // namespace Model
