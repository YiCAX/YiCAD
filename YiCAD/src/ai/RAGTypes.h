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

/// @file RAGTypes.h
/// @brief RAG 管线公共数据类型 —— Chunk / Citation / RAGAnswer

#ifndef RAGTYPES_H
#define RAGTYPES_H

#include <QString>
#include <QVector>

// ---------------------------------------------------------------------------
// Chunk —— 文档切分后的一个片段
// ---------------------------------------------------------------------------

struct Chunk
{
    QString docPath;     ///< 源文件路径，如 "docs/RAG问答功能设计方案.md"
    QString title;       ///< 文档标题（从首个 # heading 提取）
    QString section;     ///< 当前所在章节，如 "7. 索引设计"
    QString chunkId;     ///< 片段编号，在同一 docPath 内唯一
    QString content;     ///< 片段正文内容
    QString language;    ///< 语言标记："zh" / "en"
    QString sourceType;  ///< 来源类型："doc" / "readme"
};

// ---------------------------------------------------------------------------
// Citation —— 答案引用
// ---------------------------------------------------------------------------

struct Citation
{
    QString docPath;     ///< 来源文件路径
    QString title;       ///< 文档标题
    QString chunkId;     ///< 片段编号
    QString snippet;     ///< 片段内容预览（前 120 字符）
};

// ---------------------------------------------------------------------------
// RAGAnswer —— 一次 RAG 问答的完整结果
// ---------------------------------------------------------------------------

struct RAGAnswer
{
    QString            answer;        ///< 主回答文本
    QVector<Citation>  citations;     ///< 引用列表
    float              confidence;    ///< 回答置信度 [0.0, 1.0]
    bool               needFollowup;  ///< 是否需要追问/补充
};

#endif // RAGTYPES_H
