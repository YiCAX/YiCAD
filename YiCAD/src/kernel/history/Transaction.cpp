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

/// @file Transaction.cpp
/// @brief 事务(Transaction)和事务组(TransactionGroup)类实现

#include "DmDocument.h"
#include "Transaction.h"

Transaction::Transaction(const std::string& name, DmDocument* doc)
    : m_name(name)
    , m_pDocument(doc)
    , m_curState(TransactionState::Invalid)
{
}

void Transaction::start()
{
    if (m_pDocument == nullptr)
    {
        throw std::exception("Transaction start with document null.");
    }

    if (m_curState == TransactionState::Started)
    {
        return;
    }

    m_pDocument->getCmdManager()->start(m_name);
    m_curState = TransactionState::Started;
}

void Transaction::commit()
{
    if (m_curState != TransactionState::Started)
    {
        throw std::exception("Transaction has not started.");
    }

    m_pDocument->getCmdManager()->commit();
    m_pDocument->regenerate();
    m_curState = TransactionState::Committed;
}

Transaction::~Transaction()
{
    m_curState = TransactionState::Invalid;
}

void Transaction::rollback()
{
    if (m_curState != TransactionState::Started)
    {
        throw std::exception("Transaction has not started.");
    }

    m_pDocument->getCmdManager()->rollback();
    m_curState = TransactionState::Rollbacked;
}

TransactionGroup::TransactionGroup(const std::string& name, DmDocument* doc)
    : m_name(name)
    , m_pDocument(doc)
    , m_curState(TransactionState::Invalid)
{
}

TransactionGroup::~TransactionGroup()
{
    m_curState = TransactionState::Invalid;
}

void TransactionGroup::start()
{
    if (m_pDocument == nullptr)
    {
        throw std::exception("TransactionGroup start with document null.");
    }

    if (m_curState == TransactionState::Started)
    {
        return;
    }

    m_pDocument->getCmdManager()->startGroup(m_name);
    m_curState = TransactionState::Started;
}

void TransactionGroup::commit()
{
    if (m_curState != TransactionState::Started)
    {
        throw std::exception("TransactionGroup has not started.");
    }

    m_pDocument->getCmdManager()->commitGroup();
    m_pDocument->regenerate();
    m_curState = TransactionState::Committed;
}

void TransactionGroup::rollback()
{
    if (m_curState != TransactionState::Started)
    {
        throw std::exception("TransactionGroup has not started.");
    }

    m_pDocument->getCmdManager()->rollbackGroup();
    m_curState = TransactionState::Rollbacked;
}
