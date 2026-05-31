/*
 * Copyright (C) 2026 YiCAD
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

/// @file AIAssistant.h
/// @brief AI 助手控制器，封装 AIDialog / AIPipeline 创建与信号连接

#ifndef AIASSISTANT_H
#define AIASSISTANT_H

#include <QObject>

class QWidget;
class DmDocument;
class GuiDocumentView;
class AIDialog;
class AIPipeline;

/// @brief AI 助手控制器
///
/// 封装 AI 对话框（AIDialog）与 AI 总调度器（AIPipeline）的
/// 懒初始化、信号连接、配置对话框等业务逻辑。
/// ApplicationWindow 仅需创建实例并通过 show() 委托即可。
class AIAssistant : public QObject
{
    Q_OBJECT

public:
    /// @param parentWindow AIDialog 的父窗口
    /// @param parent       父 QObject
    explicit AIAssistant(QWidget* parentWindow, QObject* parent = nullptr);

    ~AIAssistant() override;

    /// @brief 显示 AI 助手对话框（首次调用时创建 AIDialog + AIPipeline）
    /// @param doc     当前文档指针（可为 nullptr，仅首次调用用于 AIPipeline 初始化）
    /// @param docView 当前文档视图指针（可为 nullptr）
    void show(DmDocument* doc, GuiDocumentView* docView);

private slots:
    void onSendRequested(const QString& text, const QString& mode);
    void onConfigRequested();
    void onNewSessionRequested();
    void onPipelineResponse(const QString& sender, const QString& text);

private:
    void ensureCreated(DmDocument* doc, GuiDocumentView* docView);

    QWidget*            m_parentWindow = nullptr;
    AIDialog*           m_dialog       = nullptr;
    AIPipeline*         m_pipeline     = nullptr;
};

#endif // AIASSISTANT_H
