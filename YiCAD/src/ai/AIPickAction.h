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

/// @file AIPickAction.h
/// @brief AI 补输入拾取动作 —— 在画布上引导用户点击/选择，用于补全 missing_inputs
///
/// 职责：
///   - 作为 ActionInterface 子类，占用事件处理栈
///   - 根据 pickType（PickPoint / PickEntity）处理鼠标点击
///   - 拾取成功后通过回调通知 AIPickSession
///   - 在自身 finish() 后由 GuiEventHandler::cleanUp 自动回收
///
/// 使用方式（由 AIPickSession 调用）：
///   @code
///     auto* action = new AIPickAction(doc, docView, task, callback);
///     docView->setCurrentAction(action);
///   @endcode
///
/// 已知限制（当前版本）：
///   - 仅支持左键拾取 / 右键取消，不支持键盘输入坐标
///   - 不支持拾取多实体（每次只选一个）
///   - 拾取实体时不限制实体类型（任何实体都可以被选中）

#ifndef AIPICKACTION_H
#define AIPICKACTION_H

#include "ActionInterface.h"
#include "AIPickTypes.h"

#include <functional>

class QMouseEvent;
class QKeyEvent;

/// @brief AI 补输入拾取动作
///
/// 在画布上等待用户拾取一个点或一个实体。
/// 生命周期由 GuiEventHandler 管理（通过 finish/cleanUp）。
class AIPickAction : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 拾取完成回调类型
    /// @param result  拾取结果（success==false 表示用户取消）
    using PickCallback = std::function<void(const PickResult& result)>;

    /// @brief 构造函数
    /// @param doc      文档指针
    /// @param docView  文档视图指针
    /// @param task     拾取任务（定义拾取类型和提示文本）
    /// @param callback 拾取完成回调（拾取成功或取消时调用一次）
    AIPickAction(DmDocument*        doc,
                 GuiDocumentView*   docView,
                 const PickTask&    task,
                 PickCallback       callback);

    ~AIPickAction() override = default;

    // ---- ActionInterface 重写 ----

    void init(int status = 0) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void updateMouseCursor() override;
    void updateMouseButtonHints() override;

    /// @brief 将 DM::EntityType 转为人类可读的显示名称
    static QString entityTypeDisplayName(DM::EntityType t);

    /// @brief 是否可被打断：AI 拾取过程中不允许其他操作打断
    bool canBeInterrupt() override { return false; }

private:
    // ---- 内部实现 ----

    /// @brief 执行点拾取：使用 Snapper 获取捕捉坐标
    void doPickPoint(QMouseEvent* e);

    /// @brief 执行实体拾取：使用 Snapper 捕获实体
    void doPickEntity(QMouseEvent* e);

    /// @brief 用户取消拾取
    void doCancel();

    PickTask     m_task;        ///< 本次拾取任务
    PickCallback m_callback;    ///< 完成回调（只调用一次）
    bool         m_called = false;  ///< 防止重复回调
};

#endif // AIPICKACTION_H
