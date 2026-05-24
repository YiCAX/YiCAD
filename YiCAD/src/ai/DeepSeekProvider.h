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

/// @file DeepSeekProvider.h
/// @brief DeepSeek API 调用提供者 —— 最小可用文本请求
///
/// 职责：
///   - 从 LLMSettingsService 读取 BaseUrl / Model / API Key / Timeout
///   - 发起一次 /chat/completions 文本请求
///   - 返回 AI 回复文本或错误信息
///
/// 使用方式：
///   @code
///     auto* provider = new DeepSeekProvider(this);
///     connect(provider, &DeepSeekProvider::responseReceived,
///             dialog, &AIDialog::appendMessage);
///     connect(provider, &DeepSeekProvider::errorOccurred,
///             this,   &MyClass::handleError);
///     provider->sendMessage("你好");
///   @endcode
///
/// 错误覆盖：
///   - 未配置 Key
///   - 网络失败
///   - 返回格式异常

#ifndef DEEPSEEKPROVIDER_H
#define DEEPSEEKPROVIDER_H

#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;

class DeepSeekProvider : public QObject
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父对象指针
    explicit DeepSeekProvider(QObject* parent = nullptr);

    /// @brief 析构函数
    ~DeepSeekProvider() override;

    /// @brief 发送一条用户消息到 DeepSeek API
    /// @param [in] userMessage 用户输入文本
    ///
    /// 调用前应确保 LLMSettingsService 已初始化且 API Key 已配置。
    /// 结果通过 responseReceived / errorOccurred 信号异步返回。
    void sendMessage(const QString& userMessage);

signals:
    /// @brief 成功收到 AI 回复
    /// @param [in] responseText AI 返回的文本内容
    void responseReceived(const QString& responseText);

    /// @brief 请求过程中发生错误
    /// @param [in] errorMessage 人类可读的错误描述
    void errorOccurred(const QString& errorMessage);

private slots:
    /// @brief 处理网络回复完成事件
    /// @param [in] reply 完成的网络回复对象
    void onReplyFinished(QNetworkReply* reply);

private:
    /// @brief 构建 Chat Completions 请求体 JSON
    /// @param [in] model       模型名称
    /// @param [in] userMessage 用户消息文本
    /// @param [in] temperature 温度参数
    /// @return JSON 字节数组
    QByteArray buildRequestBody(const QString& model,
                                const QString& userMessage,
                                double temperature) const;

    /// @brief 从 DeepSeek 响应 JSON 中提取 assistant 文本
    /// @param [in] responseData 响应体 JSON
    /// @param [out] outText     提取的文本（成功时）
    /// @return true 解析成功
    static bool parseResponseContent(const QByteArray& responseData,
                                     QString& outText);

    /// @brief 从 DeepSeek 错误响应 JSON 中提取错误信息
    /// @param [in] responseData 响应体 JSON
    /// @param [out] outError    提取的错误描述（成功时）
    /// @return true 识别为错误响应格式
    static bool parseErrorResponse(const QByteArray& responseData,
                                   QString& outError);

private:
    QNetworkAccessManager* m_networkManager;  ///< Qt 网络管理器
};

#endif // DEEPSEEKPROVIDER_H
