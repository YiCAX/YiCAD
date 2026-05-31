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

/// @file LLMSettingsService.cpp
/// @brief LLMSettingsService 实现

#include "LLMSettingsService.h"
#include "SecretStorage.h"

#include "DmSettings.h"

LLMSettingsService* LLMSettingsService::s_instance = nullptr;

LLMSettingsService::LLMSettingsService()
    : m_initialized(false)
{
}

LLMSettingsService::~LLMSettingsService()
{
    // unique_ptr 自动释放 SecretStorage
}

LLMSettingsService* LLMSettingsService::instance()
{
    if (!s_instance)
    {
        s_instance = new LLMSettingsService();
    }
    return s_instance;
}

void LLMSettingsService::init(const QString& companyKey,
                              const QString& appKey)
{
    if (m_initialized)
    {
        return;
    }

    // 创建 SecretStorage（通过工厂函数，后续替换点在此）
    m_secretStorage.reset(createSecretStorage(companyKey, appKey));
    m_initialized = true;
}

void LLMSettingsService::shutdown()
{
    if (s_instance)
    {
        delete s_instance;
        s_instance = nullptr;
    }
}

// ---------------------------------------------------------------------------
// 普通配置 —— 复用 DmSettings, group = "AI/LLM"
// ---------------------------------------------------------------------------

QString LLMSettingsService::provider() const
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    QString val = DMSETTINGS->readEntry("Provider",
                                        QLatin1String(LLMDefaults::kProvider));
    DMSETTINGS->endGroup();
    return val;
}

void LLMSettingsService::setProvider(const QString& v)
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    DMSETTINGS->writeEntry("Provider", v);
    DMSETTINGS->endGroup();
}

QString LLMSettingsService::baseUrl() const
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    QString val = DMSETTINGS->readEntry("BaseUrl",
                                        QLatin1String(LLMDefaults::kBaseUrl));
    DMSETTINGS->endGroup();
    return val;
}

void LLMSettingsService::setBaseUrl(const QString& v)
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    DMSETTINGS->writeEntry("BaseUrl", v);
    DMSETTINGS->endGroup();
}

QString LLMSettingsService::model() const
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    QString val = DMSETTINGS->readEntry("Model",
                                        QLatin1String(LLMDefaults::kModel));
    DMSETTINGS->endGroup();
    return val;
}

void LLMSettingsService::setModel(const QString& v)
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    DMSETTINGS->writeEntry("Model", v);
    DMSETTINGS->endGroup();
}

int LLMSettingsService::timeoutSecs() const
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    int val = DMSETTINGS->readNumEntry("Timeout", LLMDefaults::kTimeoutSecs);
    DMSETTINGS->endGroup();
    return val;
}

void LLMSettingsService::setTimeoutSecs(int v)
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    DMSETTINGS->writeEntry("Timeout", v);
    DMSETTINGS->endGroup();
}

double LLMSettingsService::temperature() const
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    QString val = DMSETTINGS->readEntry("Temperature",
                                        QString::number(LLMDefaults::kTemperature, 'f', 2));
    DMSETTINGS->endGroup();
    return val.toDouble();
}

void LLMSettingsService::setTemperature(double v)
{
    DMSETTINGS->beginGroup(QLatin1String(kSettingsGroup));
    DMSETTINGS->writeEntry("Temperature", v);
    DMSETTINGS->endGroup();
}

// ---------------------------------------------------------------------------
// 密钥 —— 走 SecretStorage
// ---------------------------------------------------------------------------

QString LLMSettingsService::apiKey() const
{
    if (!m_secretStorage)
    {
        return QString();
    }
    return m_secretStorage->retrieve(
        QLatin1String(LLMDefaults::kSecretKey));
}

void LLMSettingsService::setApiKey(const QString& key)
{
    if (!m_secretStorage)
    {
        return;
    }

    if (key.isEmpty())
    {
        m_secretStorage->remove(QLatin1String(LLMDefaults::kSecretKey));
    }
    else
    {
        m_secretStorage->store(QLatin1String(LLMDefaults::kSecretKey), key);
    }
}

// ---------------------------------------------------------------------------
// 校验
// ---------------------------------------------------------------------------

bool LLMSettingsService::hasApiKey() const
{
    return !apiKey().isEmpty();
}

bool LLMSettingsService::isConfigured() const
{
    return hasApiKey() && !baseUrl().isEmpty();
}
