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

/// @file AIIntentRouterData.h
/// @brief 从 JSON 文件加载意图分类关键词数据
///
/// 所有中英文关键词字符串存放在外部 JSON 文件
/// (YiCAD/support/ai/intent_keywords.json) 中。
/// 这样自然语言数据与算法 C++ 源码分离，调整权重或新增语种无需重新编译。

#ifndef AIINTENTROUTERDATA_H
#define AIINTENTROUTERDATA_H

#include <QString>
#include <vector>

// ---------------------------------------------------------------------------
// 关键词条目（匹配单元）
// ---------------------------------------------------------------------------

struct KeywordEntry
{
    std::string word;     // 关键词文本（UTF-8，区分大小写）
    float       weight;   // 匹配权重
};

// ---------------------------------------------------------------------------
// 加载后的关键词数据
// ---------------------------------------------------------------------------

struct IntentKeywords
{
    std::vector<KeywordEntry> qaKeywords;         // 问答关键词
    std::vector<KeywordEntry> modelingKeywords;   // 建模关键词

    std::vector<std::string> entityNouns;         // CAD 实体名词
    std::vector<std::string> topicMarkers;        // 话题询问标记

    std::vector<std::string> questionStartMarkers;     // 句首疑问标记
    std::vector<std::string> questionEndMarkers;       // 句尾疑问标记
    std::vector<std::string> questionContainsMarkers;  // 句中疑问标记

    bool valid = false;  // 加载成功后为 true
};

// ---------------------------------------------------------------------------
// 加载器
// ---------------------------------------------------------------------------

/// @brief 从 JSON 文件加载关键词数据
/// @param [in]  filePath  intent_keywords.json 的绝对或相对路径
/// @param [out] out       加载成功时填充；失败则保持不变
/// @return 成功返回 true
bool loadIntentKeywordsFromJson(const QString& filePath, IntentKeywords& out);

#endif // AIINTENTROUTERDATA_H
