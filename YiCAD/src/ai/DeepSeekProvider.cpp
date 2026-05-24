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

/// @file DeepSeekProvider.cpp
/// @brief DeepSeekProvider 实现 —— 最小可用文本请求

#include "DeepSeekProvider.h"
#include "LLMSettingsService.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QTimer>

// ---------------------------------------------------------------------------
// 构造 / 析构
// ---------------------------------------------------------------------------

DeepSeekProvider::DeepSeekProvider(QObject* parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    // 连接 finished 信号到处理槽
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &DeepSeekProvider::onReplyFinished);
}

DeepSeekProvider::~DeepSeekProvider() = default;

// ---------------------------------------------------------------------------
// 公共接口
// ---------------------------------------------------------------------------

void DeepSeekProvider::sendMessage(const QString& userMessage)
{
    // ---- 1. 读取配置 ----
    LLMSettingsService* svc = LLMSettingsService::instance();
    if (!svc || !svc->isInitialized())
    {
        emit errorOccurred(tr("LLM configuration service is not initialized."));
        return;
    }

    // ---- 2. 校验 API Key ----
    if (!svc->hasApiKey())
    {
        emit errorOccurred(tr("API Key not configured. Please set it in AI settings."));
        return;
    }

    const QString baseUrl     = svc->baseUrl();
    const QString model       = svc->model();
    const QString apiKey      = svc->apiKey();
    const int     timeoutSecs = svc->timeoutSecs();
    const double  temperature = svc->temperature();

    // ---- 3. 构建请求 ----
    const QUrl url(baseUrl + "/chat/completions");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json");
    request.setRawHeader("Authorization",
                         "Bearer " + apiKey.toUtf8());
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    request.setTransferTimeout(timeoutSecs * 1000);  // Qt 5.15+
#endif

    const QByteArray body = buildRequestBody(model, userMessage, temperature);

    // ---- 4. 发起 POST ----
    QNetworkReply* reply = m_networkManager->post(request, body);

    // 兜底超时（与 setTransferTimeout 双保险）
    QTimer::singleShot(timeoutSecs * 1000 + 500, reply, [reply]() {
        if (reply && reply->isRunning())
        {
            reply->abort();
        }
    });
}

// ---------------------------------------------------------------------------
// 槽：网络回复完成
// ---------------------------------------------------------------------------

void DeepSeekProvider::onReplyFinished(QNetworkReply* reply)
{
    if (!reply)
    {
        return;
    }

    // reply 将在本函数返回后由 Qt 自动调度删除（deleteLater）

    const int     statusCode  = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QByteArray body      = reply->readAll();
    const QString     errorStr = reply->errorString();

    // ---- 情况 1：网络层错误（非 HTTP 错误，如 DNS/连接失败） ----
    if (reply->error() != QNetworkReply::NoError)
    {
        // 如果有 HTTP 状态码，说明是服务端返回的错误
        if (statusCode > 0)
        {
            // 尝试解析 DeepSeek 错误 JSON 格式
            QString deepseekError;
            if (parseErrorResponse(body, deepseekError))
            {
                emit errorOccurred(tr("DeepSeek API error (HTTP %1): %2")
                                    .arg(statusCode)
                                    .arg(deepseekError));
            }
            else
            {
                // 返回非 JSON 错误（如 HTML 页面）
                QString preview = QString::fromUtf8(body.left(300));
                emit errorOccurred(tr("HTTP %1: %2")
                                    .arg(statusCode)
                                    .arg(preview.isEmpty() ? errorStr : preview));
            }
        }
        else
        {
            // 纯网络错误：无法连接、超时、DNS 等
            emit errorOccurred(tr("Network request failed: %1").arg(errorStr));
        }
        reply->deleteLater();
        return;
    }

    // ---- 情况 2：HTTP 200，解析正常响应 ----
    if (statusCode != 200)
    {
        emit errorOccurred(tr("Unknown status code %1").arg(statusCode));
        reply->deleteLater();
        return;
    }

    QString content;
    if (parseResponseContent(body, content))
    {
        emit responseReceived(content);
    }
    else
    {
        emit errorOccurred(tr("Invalid response format: unable to parse AI reply content."));
    }

    reply->deleteLater();
}

// ---------------------------------------------------------------------------
// 请求体构建
// ---------------------------------------------------------------------------

QByteArray DeepSeekProvider::buildRequestBody(const QString& model,
                                              const QString& userMessage,
                                              double temperature) const
{
    QJsonObject systemMsg;
    systemMsg["role"]    = QStringLiteral("system");
    systemMsg["content"] = QStringLiteral("You are a helpful CAD assistant.");

    QJsonObject userMsg;
    userMsg["role"]    = QStringLiteral("user");
    userMsg["content"] = userMessage;

    QJsonArray messages;
    messages.append(systemMsg);
    messages.append(userMsg);

    QJsonObject root;
    root["model"]       = model;
    root["messages"]    = messages;
    root["temperature"] = temperature;
    root["stream"]      = false;

    QJsonDocument doc(root);
    return doc.toJson(QJsonDocument::Compact);
}

// ---------------------------------------------------------------------------
// 响应解析
// ---------------------------------------------------------------------------

bool DeepSeekProvider::parseResponseContent(const QByteArray& responseData,
                                            QString& outText)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        return false;
    }

    const QJsonObject root = doc.object();

    // DeepSeek / OpenAI 格式: { "choices": [ { "message": { "content": "..." } } ] }
    const QJsonArray choices = root["choices"].toArray();
    if (choices.isEmpty())
    {
        return false;
    }

    const QJsonObject firstChoice = choices[0].toObject();
    const QJsonObject message     = firstChoice["message"].toObject();

    if (!message.contains("content"))
    {
        return false;
    }

    outText = message["content"].toString();
    return true;
}

bool DeepSeekProvider::parseErrorResponse(const QByteArray& responseData,
                                          QString& outError)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        return false;
    }

    const QJsonObject root = doc.object();

    // DeepSeek / OpenAI 错误格式: { "error": { "message": "..." } }
    if (!root.contains("error"))
    {
        return false;
    }

    const QJsonObject errorObj = root["error"].toObject();
    outError = errorObj["message"].toString(
        tr("(no message)"));

    // 附加 type / code 信息
    const QString type = errorObj["type"].toString();
    const QString code = errorObj["code"].toString();
    if (!type.isEmpty())
    {
        outError += tr(" [%1]").arg(type);
    }
    if (!code.isEmpty() && code != type)
    {
        outError += tr(" code=%1").arg(code);
    }

    return true;
}
