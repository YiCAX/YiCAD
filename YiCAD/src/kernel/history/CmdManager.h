/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file CmdManager.h
/// @brief 命令管理器，管理 Undo/Redo 双栈及事务提交

#ifndef CMDMANAGER_H
#define CMDMANAGER_H

#include "Cmd.h"
#include "MacroCmd.h"
#include <deque>
#include <QObject>

class DmDocument;

/// @brief 命令管理器
/// @details 维护 Undo/Redo 双栈，不能限制次数否则会内存泄漏
class CmdManager : public QObject
{
    Q_OBJECT
public:
    CmdManager();
    ~CmdManager();

    /// @brief 设置关联的文档
    void setDocument(DmDocument* doc){m_pDocument = doc;}
    /// @brief 获取关联的文档
    DmDocument* getDocument() const{return m_pDocument;}

    /// @brief 撤销
    void undo();
    /// @brief 重做
    void redo();
    /// @brief 获取命令状态信息
    /// @param[out] hasUndo 是否有可撤销的命令
    /// @param[out] undoName 可撤销命令名
    /// @param[out] hasRedo 是否有可重做的命令
    /// @param[out] redoName 可重做命令名
    void getCmdData(bool& hasUndo, std::string& undoName, bool& hasRedo, std::string& redoName);
    /// @brief 添加命令到当前事务
    void addToCurrentCmd(ICmd* cmd);
    /// @brief 添加并立即执行命令
    void addAndExecuteCmd(ICmd* cmd);
    /// @brief 获取当前事务命令
    MacroCmd* getCurrentCmd() const { return m_currentCmd; }
    /// @brief 获取当前事务组命令
    MacroCmd* getCurrentGroupCmd() const { return m_currentGroupCmd; }
    /// @brief 获取最后一个可撤销命令
    MacroCmd* getLastUndoCmd() const;
    /// @brief 查找命令在撤销栈中的索引
    int indexOfCmd(ICmd* cmd) const;
    /// @brief 回滚并删除指定索引（包含）之后的所有命令
    void rollbackAndRemoveAfter(size_t index);

    /// @brief 获取撤销栈大小
    /// @return 撤销栈中的命令数量
    size_t getUndoCount() const { return m_undoCmds.size(); }

    friend class TableBase;
    friend class Transaction;
    friend class TransactionGroup;
private:
    /// @brief 开始事务
    void start(const std::string& name);
    /// @brief 提交事务
    void commit();
    /// @brief 回滚事务
    void rollback();
    /// @brief 清空重做列表
    void clearRedo();
    /// @brief 清空撤销列表
    void clearUndo();

    /// @brief 开始事务组
    void startGroup(const std::string& name);
    /// @brief 提交事务组
    void commitGroup();
    /// @brief 回滚事务组
    void rollbackGroup();

    /// @brief 收集命令类型，发送信号（用于更新 UI）
    void emitSignals(const CmdTypeObjectVector& cmdTypes);
    /// @brief 样式修改后，通过该方法刷新该文字样式关联的文字，标注等
    void updateWhenStyleChanged(const CmdTypeObjectVector& cmdTypes);
    /// @brief 文字样式发生改变，更新样式匹配的文字、标注
    void updateWhenTextStyleChanged(const CmdTypeObjectVector& cmdTypes);
    /// @brief 标注样式发生改变，更新样式匹配的标注
    void updateWhenDimStyleChanged(const CmdTypeObjectVector& cmdTypes);
    /// @brief 块发生改变，更新块参照
    void updateWhenBlockChanged(const CmdTypeObjectVector& cmdTypes);

signals:
    /// @brief 实体修改信号
    void entityModified();
    /// @brief 图层修改信号
    void layerModified();
    /// @brief 线型修改信号
    void lineTypeModified();
    /// @brief 文字样式修改信号
    void textStyleModified();
    /// @brief 标注样式修改信号
    void dimStyleModified();
    /// @brief 块修改信号
    void blockModified();
    /// @brief 活动画笔修改信号
    void activePenModified();
    /// @brief 有提交的命令即触发，用来更新 undo 的 UI
    void cmdChanged();

private:
    std::deque<ICmd*> m_undoCmds;           ///< 撤销命令栈
    std::deque<ICmd*> m_redoCmds;           ///< 重做命令栈

    MacroCmd* m_currentCmd;                 ///< 当前事务命令
    MacroCmd* m_currentGroupCmd;            ///< 当前事务组命令
    DmDocument* m_pDocument;                ///< 关联的文档
};

#endif //CMDMANAGER_H
