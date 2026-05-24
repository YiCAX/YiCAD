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

/// @file SecretStorage.cpp
/// @brief SecretStorage 接口及 XorSecretStorage 临时实现

#include "SecretStorage.h"

#include <QByteArray>
#include <QSettings>

// ---------------------------------------------------------------------------
// 工厂函数
// ---------------------------------------------------------------------------

SecretStorage* createSecretStorage(const QString& companyKey,
                                   const QString& appKey)
{
    // ★ 替换点：根据平台条件编译返回不同实现
    // #ifdef Q_OS_WIN
    //     return new DpapiSecretStorage(companyKey, appKey);
    // #elif defined(Q_OS_LINUX)
    //     return new LibsecretSecretStorage(companyKey, appKey);
    // #elif defined(Q_OS_MACOS)
    //     return new KeychainSecretStorage(companyKey, appKey);
    // #else
    //     return new XorSecretStorage(companyKey, appKey);
    // #endif

    return new XorSecretStorage(companyKey, appKey);
}

// ---------------------------------------------------------------------------
// XorSecretStorage（临时实现）
// ---------------------------------------------------------------------------

namespace {

/// 固定的 XOR 盐值（仅用于临时实现，不做安全保证）
/// 长度选择 32 字节以增加轻微混淆度
constexpr char kXorSalt[] = "YiCAD-AI-Secret-Salt-2026-v0.5";
constexpr int  kSaltLen   = sizeof(kXorSalt) - 1;  // 不含 '\0'

/// QSettings 存储时使用的组名
constexpr char kSettingsGroup[] = "AI/Secrets";

}  // namespace

XorSecretStorage::XorSecretStorage(const QString& companyKey,
                                   const QString& appKey)
    : m_companyKey(companyKey)
    , m_appKey(appKey)
{
}

bool XorSecretStorage::isAvailable() const
{
    // 临时实现始终可用
    return true;
}

QByteArray XorSecretStorage::obfuscate(const QByteArray& data)
{
    QByteArray result = data;
    for (int i = 0; i < result.size(); ++i)
    {
        result[i] = result[i] ^ kXorSalt[i % kSaltLen];
    }
    return result;
}

QByteArray XorSecretStorage::deobfuscate(const QByteArray& data)
{
    // XOR 是对称操作，反混淆即再次混淆
    return obfuscate(data);
}

bool XorSecretStorage::store(const QString& key, const QString& secret)
{
    if (key.isEmpty())
    {
        return false;
    }

    const QByteArray plainBytes = secret.toUtf8();
    const QByteArray obfuscated = obfuscate(plainBytes);
    const QString    encoded    = QString::fromLatin1(obfuscated.toBase64());

    // 存入 QSettings（经过 Base64 编码的混淆数据）
    QSettings s(m_companyKey, m_appKey);
    s.beginGroup(QLatin1String(kSettingsGroup));
    s.setValue(key, encoded);
    s.endGroup();

    return true;
}

QString XorSecretStorage::retrieve(const QString& key) const
{
    QSettings s(m_companyKey, m_appKey);
    s.beginGroup(QLatin1String(kSettingsGroup));

    if (!s.contains(key))
    {
        s.endGroup();
        return QString();
    }

    const QString    encoded    = s.value(key).toString();
    const QByteArray obfuscated = QByteArray::fromBase64(encoded.toLatin1());
    const QByteArray plainBytes = deobfuscate(obfuscated);

    s.endGroup();
    return QString::fromUtf8(plainBytes);
}

bool XorSecretStorage::remove(const QString& key)
{
    QSettings s(m_companyKey, m_appKey);
    s.beginGroup(QLatin1String(kSettingsGroup));
    s.remove(key);
    s.endGroup();
    return true;
}
