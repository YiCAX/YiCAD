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

/// @file SecretStorage.h
/// @brief 密钥安全存储接口
///
/// 设计意图：
///   - 抽象密钥存储，使上层代码不关心底层实现
///   - 调用方通过接口写入/读取敏感数据（API Key 等）
///
/// 当前实现：XorSecretStorage（临时，仅作接口验证）
///   - 基于 XOR + 固定盐值的简单混淆
///   - 密文以 Base64 存入 QSettings
///   - 不满足生产安全要求，仅用于开发期闭环
///
/// 后续替换点（Windows）：
///   - 替换为 DpapiSecretStorage，使用 CryptProtectData / CryptUnprotectData
///   - 实现位置：SecretStorage.cpp 中新增类，工厂函数 createSecretStorage()
///     根据平台返回对应实现
///
/// 后续替换点（Linux/macOS）：
///   - 使用 libsecret (GNOME Keyring) 或 macOS Keychain
///   - 同样通过工厂函数切换

#ifndef SECRETSTORAGE_H
#define SECRETSTORAGE_H

#include <QString>

/// @brief 密钥存储抽象接口
class SecretStorage
{
public:
    virtual ~SecretStorage() = default;

    /// @brief 检查当前后端是否可用
    virtual bool isAvailable() const = 0;

    /// @brief 存储密钥
    /// @param key 密钥标识（如 "llm_api_key"）
    /// @param secret 明文密钥数据
    /// @return true 成功
    virtual bool store(const QString& key, const QString& secret) = 0;

    /// @brief 读取密钥
    /// @param key 密钥标识
    /// @return 明文密钥，不存在则返回空字符串
    virtual QString retrieve(const QString& key) const = 0;

    /// @brief 删除密钥
    /// @param key 密钥标识
    /// @return true 成功
    virtual bool remove(const QString& key) = 0;
};

// ---------------------------------------------------------------------------
// 工厂函数 —— 这是后续替换物理实现的单一入口
// ---------------------------------------------------------------------------

/// @brief 根据当前平台创建合适的 SecretStorage 实现
///
/// 当前返回 XorSecretStorage（临时）。
/// 后续 Windows 下替换为 DpapiSecretStorage，
/// Linux 下替换为 LibsecretSecretStorage，
/// macOS 下替换为 KeychainSecretStorage。
SecretStorage* createSecretStorage(const QString& companyKey,
                                   const QString& appKey);

// ---------------------------------------------------------------------------
// 临时实现：XorSecretStorage
// ---------------------------------------------------------------------------

/// @brief 基于 XOR 混淆 + Base64 + QSettings 的临时密钥存储
///
/// 【安全警告】
///   此实现不提供真正的加密保护。XOR 混淆可被轻易逆向。
///   仅用于开发阶段闭环验证接口，不得用于生产环境。
///
/// 【替换时机】
///   正式发布前，必须替换为 DpapiSecretStorage（Windows）或等价系统级存储。
class XorSecretStorage : public SecretStorage
{
public:
    XorSecretStorage(const QString& companyKey, const QString& appKey);
    ~XorSecretStorage() override = default;

    bool isAvailable() const override;
    bool store(const QString& key, const QString& secret) override;
    QString retrieve(const QString& key) const override;
    bool remove(const QString& key) override;

private:
    /// @brief XOR 混淆（临时）
    static QByteArray obfuscate(const QByteArray& data);

    /// @brief XOR 反混淆（临时）
    static QByteArray deobfuscate(const QByteArray& data);

    QString m_companyKey;
    QString m_appKey;
};

#endif // SECRETSTORAGE_H
