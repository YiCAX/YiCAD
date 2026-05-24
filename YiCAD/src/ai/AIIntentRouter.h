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

/// @file AIIntentRouter.h
/// @brief AI 意图路由器 —— 规则驱动的输入分类，分为问答、建模、混合或不确定
///
/// 职责：
///   - 基于关键词和规则的意图分类（首版，不依赖大模型）
///   - 支持手动模式兜底：QA / Modeling / Auto
///   - 混合输入返回"先说明，再确认是否执行"的中间态结果
///   - 不触发 RAG 或建模执行，只返回路由结果
///
/// 关键词数据通过 loadKeywordsFromJson() 在启动时从外部 JSON 文件加载。
/// 所有自然语言字符串均存放于该文件中。
///
/// UI 发送的手动模式 token：
///   - "auto"     → 规则分类（默认）
///   - "qa"       → 强制问答
///   - "modeling" → 强制建模

#ifndef AIINTENTROUTER_H
#define AIINTENTROUTER_H

#include "AIIntentRouterData.h"

#include <QObject>
#include <QString>

// ---------------------------------------------------------------------------
// 意图类型
// ---------------------------------------------------------------------------

enum class IntentType
{
    QA,         ///< 问答：询问用法、概念等帮助类问题
    Modeling,   ///< 建模：执行绘制、修改等 CAD 操作
    Mixed,      ///< 混合：既有提问又有操作意图，先回答再确认
    Uncertain   ///< 不确定：无法判断意图，需追问
};

// ---------------------------------------------------------------------------
// 路由结果
// ---------------------------------------------------------------------------

struct RouterResult
{
    IntentType intent            = IntentType::Uncertain;  ///< 分类结果
    float      confidence        = 0.0f;                   ///< 置信度 [0.0, 1.0]
    QString    reasoning;        ///< 分类理由（人类可读，用于调试/日志）
    QString    suggestedAction;  ///< 建议的下一步动作
    bool       needsConfirmation = false;  ///< 是否需要用户确认后再执行
};

// ---------------------------------------------------------------------------
// AIIntentRouter
// ---------------------------------------------------------------------------

class AIIntentRouter : public QObject
{
    Q_OBJECT

public:
    explicit AIIntentRouter(QObject* parent = nullptr);
    ~AIIntentRouter() override;

    /// @brief 从 JSON 文件加载关键词数据
    /// @return 成功返回 true。必须在 route() 之前调用，否则始终返回 Uncertain。
    bool loadKeywordsFromJson(const QString& filePath);

    /// @brief 关键词数据是否已成功加载
    bool isReady() const;

    /// @brief 对用户输入进行意图分类
    /// @param input      用户原始文本
    /// @param manualMode 手动模式："auto" / "qa" / "modeling"（来自 UI 切换）
    RouterResult route(const QString& input,
                       const QString& manualMode = QStringLiteral("auto")) const;

private:
    // ---- 匹配模式 ----
    enum class MatchMode { StartsWith, EndsWith, Contains };

    // ---- 预处理 ----
    static QString preprocess(const QString& raw);

    // ---- 评分 ----
    float scoreQA(const QString& input) const;
    float scoreModeling(const QString& input) const;
    float scoreKeywordList(const QString& input,
                           const std::vector<KeywordEntry>& keywords) const;

    // ---- 模式匹配（数据驱动） ----
    bool matchesAny(const QString& input,
                    const std::vector<std::string>& markers,
                    MatchMode mode) const;
    bool matchesQuestionPattern(const QString& input) const;
    bool containsEntityNoun(const QString& input) const;
    bool isAskingAboutTopic(const QString& input) const;

    // ---- 置信度映射 ----
    static float scoreToConfidence(float targetScore, float otherScore);

    // ---- 分类决策 ----
    RouterResult classifyByRules(const QString& input) const;

    // ---- 阈值常量 ----
    static constexpr float kMinCategoryThreshold = 1.0f;
    static constexpr float kMixedDualThreshold   = 1.0f;
    static constexpr float kConfidenceScale      = 3.0f;

    // ---- 数据 ----
    IntentKeywords m_keywords;
};

#endif // AIINTENTROUTER_H
