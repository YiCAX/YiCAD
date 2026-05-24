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

/// @file RAGPipeline.cpp
/// @brief RAGPipeline 实现 —— RAG 问答管线的完整编排

#include "RAGPipeline.h"
#include "ChunkSplitter.h"
#include "DeepSeekProvider.h"
#include "Retriever.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

// ============================================================================
// 构造 / 析构
// ============================================================================

RAGPipeline::RAGPipeline(QObject* parent)
    : QObject(parent)
    , m_provider(new DeepSeekProvider(this))
    , m_retriever(new KeywordRetriever)
    , m_ready(false)
{
    // 转发 provider 信号
    connect(m_provider, &DeepSeekProvider::responseReceived,
            this, &RAGPipeline::onModelResponse);
    connect(m_provider, &DeepSeekProvider::errorOccurred,
            this, &RAGPipeline::onModelError);
}

RAGPipeline::~RAGPipeline() = default;

// ============================================================================
// 公开接口 —— 初始化
// ============================================================================

bool RAGPipeline::initialize(const QString& docsDir, const QString& readmePath)
{
    ChunkSplitter splitter;
    QVector<Chunk> chunks;

    // 1. 加载 docs/ 目录下的所有 .md 文件
    if (!docsDir.isEmpty())
    {
        QVector<Chunk> docChunks = splitter.splitDirectory(docsDir);
        chunks.append(docChunks);
    }

    // 2. 加载 README.md
    if (!readmePath.isEmpty())
    {
        QVector<Chunk> readmeChunks = splitter.splitFile(readmePath, "readme");
        chunks.append(readmeChunks);
    }

    if (chunks.isEmpty())
    {
        m_ready = false;
        return false;
    }

    // 3. 建立检索索引
    m_retriever->index(chunks);
    m_allChunks = chunks;
    m_ready = true;

    return true;
}

bool RAGPipeline::isReady() const
{
    return m_ready && m_retriever && m_retriever->isReady();
}

int RAGPipeline::chunkCount() const
{
    return m_retriever ? m_retriever->chunkCount() : 0;
}

// ============================================================================
// 公开接口 —— 查询
// ============================================================================

void RAGPipeline::query(const QString& question)
{
    if (!isReady())
    {
        emit errorOccurred(tr("RAG pipeline is not initialized. "
                              "Please load knowledge sources first."));
        return;
    }

    if (question.trimmed().isEmpty())
    {
        emit errorOccurred(tr("Question is empty."));
        return;
    }

    // 1. 检索 Top-K 相关 chunk
    const int topK = 5;
    QVector<Chunk> contextChunks = m_retriever->retrieve(question, topK);

    // 保存以便响应时回填引用
    m_lastQuestion = question;
    m_lastContext  = contextChunks;

    // 2. 构建 prompt
    const QString prompt = buildPrompt(question, contextChunks);

    // 3. 调用模型（异步，结果通过 onModelResponse 返回）
    m_provider->sendMessage(prompt);
}

// ============================================================================
// 私有槽 —— 模型回复
// ============================================================================

void RAGPipeline::onModelResponse(const QString& responseText)
{
    RAGAnswer answer = parseResponse(responseText);

    // 如果模型未返回引用，用检索到的 chunk 作为 fallback citations
    if (answer.citations.isEmpty() && !m_lastContext.isEmpty())
    {
        for (const Chunk& chunk : m_lastContext)
        {
            answer.citations.append(chunkToCitation(chunk));
        }
    }

    emit answerReady(answer);
}

void RAGPipeline::onModelError(const QString& errorMessage)
{
    emit errorOccurred(errorMessage);
}

// ============================================================================
// 私有方法 —— Prompt 构建
// ============================================================================
//
// 注意：prompt 内容使用 QStringLiteral 而非 tr()，因为这些字符串是发给 LLM 的
// 指令，不应随 UI 语言切换而变化。

QString RAGPipeline::buildPrompt(const QString& question,
                                 const QVector<Chunk>& contextChunks) const
{
    // 系统指令（嵌在 user message 中，与 DeepSeekProvider 的 system prompt 叠加）
    QString prompt;

    prompt += QStringLiteral(
        "[System Instruction]\n"
        "You are YiCAD's help assistant. You MUST answer based ONLY on the "
        "provided reference materials below.\n"
        "If the materials are insufficient, clearly state: "
        "\"The current documentation does not contain enough information to fully answer this question.\"\n"
        "Keep your answer concise and accurate.\n"
        "At the end of your answer, list the sources you used in this format:\n"
        "  Sources: [1] doc_path (section_title)\n"
        "Do NOT fabricate any information not present in the references.\n\n");

    // 检索上下文
    if (contextChunks.isEmpty())
    {
        prompt += QStringLiteral(
            "[Retrieved Context]\n"
            "(No relevant documentation found for this question.)\n\n");
    }
    else
    {
        prompt += QStringLiteral("[Retrieved Context]\n");
        for (int i = 0; i < contextChunks.size(); ++i)
        {
            const Chunk& c = contextChunks[i];
            prompt += QStringLiteral("--- Reference %1 ---\n"
                                     "Source: %2\n"
                                     "Section: %3\n"
                                     "Content:\n%4\n\n")
                          .arg(i + 1)
                          .arg(c.docPath)
                          .arg(c.section)
                          .arg(c.content);
        }
    }

    // 用户问题
    prompt += QStringLiteral("[User Question]\n%1").arg(question);

    return prompt;
}

// ============================================================================
// 私有方法 —— 响应解析
// ============================================================================

RAGAnswer RAGPipeline::parseResponse(const QString& responseText) const
{
    RAGAnswer answer;

    if (responseText.isEmpty())
    {
        answer.answer       = tr("(Empty response from model)");
        answer.confidence   = 0.0f;
        answer.needFollowup = true;
        return answer;
    }

    // 尝试作为 JSON 解析（模型可能返回 JSON 格式）
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(responseText.toUtf8(), &parseError);

    if (parseError.error == QJsonParseError::NoError && doc.isObject())
    {
        const QJsonObject root = doc.object();

        // 提取 answer
        if (root.contains("answer"))
        {
            answer.answer = root["answer"].toString();
        }
        else
        {
            answer.answer = responseText;  // fallback
        }

        // 提取 confidence
        if (root.contains("confidence"))
        {
            answer.confidence = static_cast<float>(root["confidence"].toDouble(0.5));
        }

        // 提取 need_followup
        if (root.contains("need_followup"))
        {
            answer.needFollowup = root["need_followup"].toBool(false);
        }

        // 提取 citations
        if (root.contains("citations"))
        {
            const QJsonArray arr = root["citations"].toArray();
            for (const QJsonValue& val : arr)
            {
                if (!val.isObject()) continue;
                const QJsonObject cit = val.toObject();
                Citation c;
                c.docPath = cit["doc_path"].toString();
                c.title   = cit["title"].toString();
                c.chunkId = cit["chunk_id"].toString();
                c.snippet = cit["snippet"].toString();

                // 如果模型没给 snippet，尝试从 m_lastContext 回填
                if (c.snippet.isEmpty() && !c.chunkId.isEmpty())
                {
                    for (const Chunk& chunk : m_lastContext)
                    {
                        if (chunk.chunkId == c.chunkId)
                        {
                            c.snippet = chunk.content.left(120);
                            break;
                        }
                    }
                }

                if (!c.docPath.isEmpty())
                {
                    answer.citations.append(c);
                }
            }
        }
    }
    else
    {
        // 非 JSON：整个响应作为回答
        answer.answer = responseText;

        // 尝试从文本中提取 "Sources:" 行做引用
        int sourcesIdx = responseText.lastIndexOf("Sources:");
        if (sourcesIdx < 0)
        {
            sourcesIdx = responseText.lastIndexOf("sources:");
        }
        if (sourcesIdx >= 0)
        {
            // 从 m_lastContext 回填 citations
            for (const Chunk& chunk : m_lastContext)
            {
                answer.citations.append(chunkToCitation(chunk));
            }
        }
    }

    // 置信度兜底
    if (answer.confidence <= 0.0f)
    {
        answer.confidence = 0.5f;
    }

    return answer;
}

// ============================================================================
// 辅助
// ============================================================================

Citation RAGPipeline::chunkToCitation(const Chunk& chunk)
{
    Citation c;
    c.docPath = chunk.docPath;
    c.title   = chunk.title;
    c.chunkId = chunk.chunkId;
    c.snippet = chunk.content.left(120);
    return c;
}
