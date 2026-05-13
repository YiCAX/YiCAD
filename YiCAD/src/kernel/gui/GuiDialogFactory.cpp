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

/// @file GuiDialogFactory.cpp
/// @brief 对话框工厂类实现

#include "GuiDialogFactory.h"

// 私有构造函数
GuiDialogFactory::GuiDialogFactory()
    : m_pFactoryObject(nullptr)
{
}

/// @brief 获取单例实例
/// @return 对话框工厂的唯一实例
GuiDialogFactory* GuiDialogFactory::instance()
{
    static GuiDialogFactory* uniqueInstance = new GuiDialogFactory{};
    return uniqueInstance;
}

/// @brief 设置实际的工厂对象
/// @param fo 实现了 GuiDialogFactoryInterface 的工厂对象
void GuiDialogFactory::setFactoryObject(GuiDialogFactoryInterface* fo)
{
    m_pFactoryObject = fo;
}

/// @brief 获取工厂对象
/// @return 工厂对象（永不为 nullptr），如果未设置则返回默认适配器
GuiDialogFactoryInterface* GuiDialogFactory::getFactoryObject()
{
    return m_pFactoryObject ? m_pFactoryObject : &m_factoryAdapter;
}

/// @brief 显示命令消息
/// @param m 要显示的消息文本
void GuiDialogFactory::commandMessage(const QString& m)
{
    if (m_pFactoryObject)
    {
        m_pFactoryObject->commandMessage(m);
    }
}
