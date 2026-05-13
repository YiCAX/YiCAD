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

/// @file Transaction.h
/// @brief 事务(Transaction)和事务组(TransactionGroup)类，封装 Undo/Redo 的开始/提交/回滚操作

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "MacroCmd.h"
#include <string>

/// @brief 事务状态枚举
enum class TransactionState
{
    Invalid,    ///< 无效状态
    Started,    ///< 已开始
    Committed,  ///< 已提交
    Rollbacked  ///< 已回滚
};

class DmDocument;

/// @brief 事务类，用于生成一个整体Undo/Redo操作
class Transaction
{
public:
    /// @brief 构造函数
    /// @param [in] name 事务名称
    /// @param [in] doc 文档指针
    Transaction(const std::string& name, DmDocument* doc);

    Transaction() = delete;
    Transaction(const Transaction&) = delete;

    ~Transaction();

    /// @brief 开始事务
    void start();

    /// @brief 提交事务
    void commit();

    /// @brief 回滚事务
    void rollback();

private:
    std::string m_name;                ///< 事务名称
    DmDocument* m_pDocument = nullptr; ///< 关联的文档指针
    TransactionState m_curState = TransactionState::Invalid; ///< 当前事务状态
};

/// @brief 事务组类，用于生成一组整体Undo/Redo操作
class TransactionGroup
{
public:
    /// @brief 构造函数
    /// @param [in] name 事务组名称
    /// @param [in] doc 文档指针
    TransactionGroup(const std::string& name, DmDocument* doc);

    ~TransactionGroup();

    /// @brief 开始事务组
    void start();

    /// @brief 提交事务组
    void commit();

    /// @brief 回滚事务组
    void rollback();

private:
    std::string m_name;                ///< 事务组名称
    DmDocument* m_pDocument = nullptr; ///< 关联的文档指针
    TransactionState m_curState = TransactionState::Invalid; ///< 当前事务状态
};


#endif //TRANSACTION_H
