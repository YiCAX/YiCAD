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

/// @file GuiDialogFactory.h
/// @brief 对话框工厂类，管理对话框的创建和显示

#ifndef GUIDIALOGFACTORY_H
#define GUIDIALOGFACTORY_H

#include "GuiDialogFactoryAdapter.h"

class GuiDialogFactoryInterface;

/// @brief 便捷宏，用于获取对话框工厂对象
#define GUIDIALOGFACTORY GuiDialogFactory::instance()->getFactoryObject()

/// @brief 对话框工厂
/// @details 提供创建和显示对话框的接口，使用单例模式
class GuiDialogFactory
{
private:
    GuiDialogFactory();

public:
    virtual ~GuiDialogFactory() = default;

    /// @brief 获取单例实例
    /// @return 对话框工厂的唯一实例
    static GuiDialogFactory* instance();

    /// @brief 设置实际的工厂对象
    /// @param fo 实现了 GuiDialogFactoryInterface 的工厂对象
    void setFactoryObject(GuiDialogFactoryInterface* fo);

    /// @brief 获取工厂对象
    /// @return 工厂对象，如果未设置则返回默认适配器
    GuiDialogFactoryInterface* getFactoryObject();

    /// @brief 显示命令消息
    /// @param m 要显示的消息文本
    void commandMessage(const QString& m);

private:
    GuiDialogFactoryInterface*  m_pFactoryObject;     ///< 实际的工厂对象
    GuiDialogFactoryAdapter     m_factoryAdapter;      ///< 默认适配器
};

#endif
