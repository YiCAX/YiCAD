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

/// @file AIIntentRouter.cpp
/// @brief AIIntentRouter 实现 —— 规则驱动的意图分类
///
/// 所有自然语言关键词数据均来自外部 JSON 文件（通过 AIIntentRouterData 加载）。
/// 本文件仅包含算法逻辑，不含任何中文匹配字符串。

#include "AIIntentRouter.h"
#include "AIIntentRouterData.h"

#include <QFileInfo>
#include <QtMath>

// ============================================================================
// 构造 / 析构
// ============================================================================

AIIntentRouter::AIIntentRouter(QObject* parent)
    : QObject(parent)
{
}

AIIntentRouter::~AIIntentRouter() = default;

// ============================================================================
// 公开接口 —— 数据加载
// ============================================================================

bool AIIntentRouter::loadKeywordsFromJson(const QString& filePath)
{
    return loadIntentKeywordsFromJson(filePath, m_keywords);
}

bool AIIntentRouter::isReady() const
{
    return m_keywords.valid;
}

// ============================================================================
// 公开接口 —— 路由
// ============================================================================

RouterResult AIIntentRouter::route(const QString& input,
                                   const QString& manualMode) const
{
    // ------ 空输入保护 ------
    if (input.trimmed().isEmpty()) {
        RouterResult r;
        r.intent           = IntentType::Uncertain;
        r.confidence       = 0.0f;
        r.reasoning        = tr("Input is empty.");
        r.suggestedAction  = tr("Please enter your question or modeling command.");
        r.needsConfirmation = false;
        return r;
    }

    // ------ 手动模式兜底 ------
    if (manualMode == QStringLiteral("qa")) {
        RouterResult r;
        r.intent           = IntentType::QA;
        r.confidence       = 1.0f;
        r.reasoning        = tr("Manually set to QA mode.");
        r.suggestedAction  = tr("Will enter QA pipeline (RAG).");
        r.needsConfirmation = false;
        return r;
    }

    if (manualMode == QStringLiteral("modeling")) {
        RouterResult r;
        r.intent           = IntentType::Modeling;
        r.confidence       = 1.0f;
        r.reasoning        = tr("Manually set to Modeling mode.");
        r.suggestedAction  = tr("Will enter modeling execution pipeline.");
        r.needsConfirmation = false;
        return r;
    }

    // ------ 自动模式：规则分类 ------
    return classifyByRules(preprocess(input));
}

// ============================================================================
// 私有方法 —— 预处理
// ============================================================================

QString AIIntentRouter::preprocess(const QString& raw)
{
    QString s = raw.trimmed().simplified();

    // 全角转半角
    s.replace(QChar(0xFF1F), QChar('?'));   // ？ → ?
    s.replace(QChar(0x3000), QChar(' '));   // 全角空格 → 半角空格

    return s;
}

// ============================================================================
// 私有方法 —— 评分
// ============================================================================

float AIIntentRouter::scoreQA(const QString& input) const
{
    float score = scoreKeywordList(input, m_keywords.qaKeywords);

    // 疑问句式或话题询问加分
    if (matchesQuestionPattern(input) || isAskingAboutTopic(input))
        score += 1.5f;

    return score;
}

float AIIntentRouter::scoreModeling(const QString& input) const
{
    float score = scoreKeywordList(input, m_keywords.modelingKeywords);

    // 包含 CAD 实体名词且已有建模关键词时加分
    if (score >= 1.0f && containsEntityNoun(input))
        score += 0.5f;

    return score;
}

float AIIntentRouter::scoreKeywordList(
        const QString&                    input,
        const std::vector<KeywordEntry>&  keywords) const
{
    float total = 0.0f;
    for (const auto& kw : keywords) {
        if (input.contains(QString::fromUtf8(kw.word.c_str())))
            total += kw.weight;
    }
    return total;
}

// ============================================================================
// 私有方法 —— 模式匹配
// ============================================================================

bool AIIntentRouter::matchesAny(const QString&                     input,
                                const std::vector<std::string>&    markers,
                                MatchMode                          mode) const
{
    for (const auto& m : markers) {
        const QString marker = QString::fromUtf8(m.c_str());
        bool hit = false;
        switch (mode) {
        case MatchMode::StartsWith: hit = input.startsWith(marker); break;
        case MatchMode::EndsWith:   hit = input.endsWith(marker);   break;
        case MatchMode::Contains:   hit = input.contains(marker);   break;
        }
        if (hit) return true;
    }
    return false;
}

bool AIIntentRouter::matchesQuestionPattern(const QString& input) const
{
    // 句首疑问标记
    if (matchesAny(input, m_keywords.questionStartMarkers, MatchMode::StartsWith))
        return true;

    // 句尾疑问标记
    if (matchesAny(input, m_keywords.questionEndMarkers, MatchMode::EndsWith))
        return true;

    // 句首标记也检查出现在句中（如 "how" / "怎么" 在句子中间）
    for (const auto& m : m_keywords.questionStartMarkers) {
        const QString marker = QString::fromUtf8(m.c_str());
        if (input.contains(marker)) return true;
    }

    // 句中疑问标记
    if (matchesAny(input, m_keywords.questionContainsMarkers, MatchMode::Contains))
        return true;

    return false;
}

bool AIIntentRouter::containsEntityNoun(const QString& input) const
{
    for (const auto& noun : m_keywords.entityNouns) {
        if (input.contains(QString::fromUtf8(noun.c_str())))
            return true;
    }
    return false;
}

bool AIIntentRouter::isAskingAboutTopic(const QString& input) const
{
    for (const auto& marker : m_keywords.topicMarkers) {
        if (input.contains(QString::fromUtf8(marker.c_str())))
            return true;
    }
    return false;
}

// ============================================================================
// 私有方法 —— 置信度映射
// ============================================================================

float AIIntentRouter::scoreToConfidence(float targetScore, float otherScore)
{
    if (targetScore <= 0.0f)
        return 0.0f;

    const float ratio   = targetScore / (targetScore + otherScore);
    const float x       = kConfidenceScale * (ratio - 0.5f);
    const float sigmoid = 1.0f / (1.0f + qExp(-x));
    return sigmoid;
}

// ============================================================================
// 私有方法 —— 分类决策树
// ============================================================================

RouterResult AIIntentRouter::classifyByRules(const QString& input) const
{
    RouterResult result;

    const float qaScore = scoreQA(input);
    float modelingScore  = scoreModeling(input);

    // 话题上下文折扣：用户明显在询问某个 CAD 命令/功能时，
    // 建模关键词是话题而非操作意图，压低得分避免误判
    if (isAskingAboutTopic(input))
        modelingScore = qMin(modelingScore, kMinCategoryThreshold * 0.5f);

    // ---- 情况 1：两类得分都低 → Uncertain ----
    if (qaScore < kMinCategoryThreshold && modelingScore < kMinCategoryThreshold) {
        result.intent     = IntentType::Uncertain;
        result.confidence = 0.0f;
        result.reasoning  = tr("No matching QA or modeling keywords found; "
                               "unable to determine intent "
                               "(QA_score=%1, Modeling_score=%2)")
                                .arg(qaScore, 0, 'f', 1)
                                .arg(modelingScore, 0, 'f', 1);
        result.suggestedAction = tr(
            "Please provide more details: are you looking for information "
            "about a feature, or do you want to perform a modeling operation?");
        result.needsConfirmation = true;
        return result;
    }

    // ---- 情况 2：两类得分都 ≥ 阈值 → Mixed ----
    if (qaScore >= kMixedDualThreshold && modelingScore >= kMixedDualThreshold) {
        result.intent     = IntentType::Mixed;
        result.confidence = 0.85f;
        result.reasoning  = tr("Both QA and modeling keywords matched; "
                               "classified as mixed intent "
                               "(QA_score=%1, Modeling_score=%2)")
                                .arg(qaScore, 0, 'f', 1)
                                .arg(modelingScore, 0, 'f', 1);
        result.suggestedAction = tr(
            "Provide instructions first, then ask whether to execute "
            "the modeling operation.");
        result.needsConfirmation = true;
        return result;
    }

    // ---- 情况 3：仅 QA 得分高 → QA ----
    if (qaScore >= kMinCategoryThreshold && modelingScore < kMinCategoryThreshold) {
        result.intent     = IntentType::QA;
        result.confidence = scoreToConfidence(qaScore, modelingScore);
        result.reasoning  = tr("QA keywords matched; classified as QA intent "
                               "(QA_score=%1, Modeling_score=%2)")
                                .arg(qaScore, 0, 'f', 1)
                                .arg(modelingScore, 0, 'f', 1);
        result.suggestedAction  = tr("Will enter QA pipeline (RAG).");
        result.needsConfirmation = false;
        return result;
    }

    // ---- 情况 4：仅 Modeling 得分高 → Modeling ----
    if (modelingScore >= kMinCategoryThreshold && qaScore < kMinCategoryThreshold) {
        result.intent     = IntentType::Modeling;
        result.confidence = scoreToConfidence(modelingScore, qaScore);
        result.reasoning  = tr("Modeling keywords matched; "
                               "classified as modeling intent "
                               "(QA_score=%1, Modeling_score=%2)")
                                .arg(qaScore, 0, 'f', 1)
                                .arg(modelingScore, 0, 'f', 1);
        result.suggestedAction  = tr("Will enter modeling execution pipeline.");
        result.needsConfirmation = false;
        return result;
    }

    // ---- 情况 5：边缘 —— 取高者 ----
    if (qaScore >= modelingScore) {
        result.intent     = IntentType::QA;
        result.confidence = scoreToConfidence(qaScore, modelingScore);
        result.reasoning  = tr("QA score is higher "
                               "(QA=%1 > Modeling=%2); classified as QA.")
                                .arg(qaScore, 0, 'f', 1)
                                .arg(modelingScore, 0, 'f', 1);
        result.suggestedAction  = tr("Will enter QA pipeline (RAG).");
        result.needsConfirmation = false;
    } else {
        result.intent     = IntentType::Modeling;
        result.confidence = scoreToConfidence(modelingScore, qaScore);
        result.reasoning  = tr("Modeling score is higher "
                               "(Modeling=%1 > QA=%2); classified as modeling.")
                                .arg(modelingScore, 0, 'f', 1)
                                .arg(qaScore, 0, 'f', 1);
        result.suggestedAction  = tr("Will enter modeling execution pipeline.");
        result.needsConfirmation = false;
    }

    return result;
}
