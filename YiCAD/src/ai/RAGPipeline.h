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

/// @file RAGPipeline.h
/// @brief RAG 问答管线 —— 编排检索、提示词拼装、模型调用与结果解析
///
/// 职责：
///   - 持有 ChunkSplitter + IRetriever + DeepSeekProvider
///   - initialize() 加载知识源并建立索引
///   - query() 执行完整 RAG 流程：检索 → 拼 prompt → 调模型 → 解析引用
///
/// 使用方式：
///   @code
///     auto* rag = new RAGPipeline(this);
///     connect(rag, &RAGPipeline::answerReady,
///             this, &MyClass::onRagAnswer);
///     connect(rag, &RAGPipeline::errorOccurred,
///             this, &MyClass::onRagError);
///     rag->initialize("path/to/docs", "path/to/README.md");
///     rag->query("修剪命令怎么用");
///   @endcode
///
/// 说明：
///   - 首版不支持多轮对话，每次 query() 独立
///   - 首版检索器为 KeywordRetriever，后续可替换为向量检索

#ifndef RAGPIPELINE_H
#define RAGPIPELINE_H

#include "RAGTypes.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>

class DeepSeekProvider;
class IRetriever;

class RAGPipeline : public QObject
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父对象指针
    explicit RAGPipeline(QObject* parent = nullptr);

    /// @brief 析构函数
    ~RAGPipeline() override;

    /// @brief 初始化管线：加载知识源、切分、建索引
    /// @param docsDir   docs/ 目录的绝对路径
    /// @param readmePath README.md 的绝对路径
    /// @return 成功返回 true
    ///
    /// 可以多次调用以重建索引（如文档更新后）。
    bool initialize(const QString& docsDir, const QString& readmePath);

    /// @brief 是否已完成初始化并可查询
    bool isReady() const;

    /// @brief 发起一次 RAG 问答
    /// @param question 用户原始问题
    ///
    /// 结果通过 answerReady / errorOccurred 信号异步返回。
    /// 如果未初始化或检索器为空，会直接发出 errorOccurred。
    void query(const QString& question);

    /// @brief 当前索引中的 chunk 总数（调试用）
    int chunkCount() const;

signals:
    /// @brief RAG 问答成功完成
    /// @param answer 包含主回答、引用列表和置信度
    void answerReady(const RAGAnswer& answer);

    /// @brief 管线执行过程中发生错误
    /// @param errorMessage 错误描述
    void errorOccurred(const QString& errorMessage);

private slots:
    /// @brief 接收 DeepSeekProvider 的模型回复
    void onModelResponse(const QString& responseText);

    /// @brief 接收 DeepSeekProvider 的错误
    void onModelError(const QString& errorMessage);

private:
    /// @brief 构建发送给模型的系统提示词 + 上下文 + 用户问题
    QString buildPrompt(const QString& question,
                        const QVector<Chunk>& contextChunks) const;

    /// @brief 解析模型回复，提取 answer + citations
    RAGAnswer parseResponse(const QString& responseText) const;

    /// @brief 从 chunk 构建 Citation
    static Citation chunkToCitation(const Chunk& chunk);

    DeepSeekProvider*          m_provider;   ///< 模型调用（不持有）
    std::unique_ptr<IRetriever> m_retriever;  ///< 检索器
    QVector<Chunk>             m_allChunks;   ///< 全部 chunk（调试用）
    QString                    m_lastQuestion;///< 最近一次问题
    QVector<Chunk>             m_lastContext; ///< 最近一次检索到的上下文
    bool                       m_ready;       ///< 是否已初始化
};

#endif // RAGPIPELINE_H
