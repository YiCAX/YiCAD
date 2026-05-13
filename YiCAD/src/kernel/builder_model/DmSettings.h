/*
 * Copyright (C) 2026 YiCAD Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file DmSettings.h
/// @brief 应用程序设置管理类，支持读写配置和缓存

#ifndef DMSETTINGS_H
#define DMSETTINGS_H

#include <QString>
#include <map>

class QVariant;

// ---------------------------------------------------------------------------
// Default Settings
// ---------------------------------------------------------------------------

namespace Colors
{
    const QString SNAP_INDICATOR = "#FFC200";
    const QString BACKGROUND     = "#1E1E1E";
    const QString GRID           = "#323748";
    const QString META_GRID      = "#494F69";
    const QString SELECT         = "#4D80FF";
    const QString HIGHLIGHT      = "#FFFFFF";
    const QString START_HANDLE   = "Cyan";
    const QString HANDLE         = "Blue";
    const QString END_HANDLE     = "Blue";
}  // namespace Colors

// ---------------------------------------------------------------------------

// TODO: 考虑改为 constexpr 函数
#define DMSETTINGS DmSettings::instance()

/// This class can store and reload settings from a
/// configuration file or the windoze registry.
/// Please note that the Qt default implementation doesn't
/// work as one would expect. That's why this class overwrites
/// most of the default behaviour.
class DmSettings
{
public:
    ~DmSettings();

    void deleteDmStettings();

    /// @return Instance to the unique settings object.
    static DmSettings* instance();

    /// @brief Initialize the system.
    /// @param companyKey Company Key
    /// @param appKey Application key
    void init(const QString& companyKey, const QString& appKey);

    void beginGroup(const QString& group);
    void endGroup();

    bool writeEntry(const QString& key, int value);
    bool writeEntry(const QString& key, double value);
    bool writeEntry(const QString& key, const QVariant& value);
    bool writeEntry(const QString& key, const QString& value);
    QString readEntry(const QString& key, const QString& def = QString(), bool* ok = 0);
    QByteArray readByteArrayEntry(const QString& key, const QString& def = QString(), bool* ok = 0);
    int readNumEntry(const QString& key, int def = 0);
    void clear_all();
    void clear_geometry();
    static bool save_is_allowed;

private:
    DmSettings();
    DmSettings(DmSettings const&) = delete;
    DmSettings& operator=(DmSettings const&) = delete;
    QVariant readEntryCache(const QString& key);
    void addToCache(const QString& key, const QVariant& value);

private:
    static DmSettings*          m_pUniqueInstance;

    std::map<QString, QVariant> cache;
    QString                     companyKey;
    QString                     appKey;
    QString                     group;
    bool                        initialized;
};

#endif
