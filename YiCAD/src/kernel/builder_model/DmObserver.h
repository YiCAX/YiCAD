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

/// @file DmObserver.h
/// @brief 观察者模式接口，用于响应实体变更事件

#ifndef DMOBSERVER_H
#define DMOBSERVER_H

#include "Datamodel.h"

class DmEntity;

/// @brief 观察者抽象接口，响应实体变更通知
class DmObserver
{
public:
    DmObserver()
    {
    }

    virtual ~DmObserver() = default;

    /// @brief 响应实体变更
    /// @param ent 发生变更的实体指针
    virtual void respond(DmEntity* ent = nullptr) = 0;

    /// @brief 获取观察者类型
    /// @return 观察者类型枚举值
    virtual DM::ObserverType getObserverType() const = 0;

private:
};

#endif // !DMOBSERVER_H
