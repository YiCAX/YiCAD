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

/// @file LayerTableCmd.h
/// @brief 图层表命令，包含添加、修改、移除、激活图层的 Undo/Redo 命令

#ifndef LAYERTABLECMD_H
#define LAYERTABLECMD_H

#include "TableBase.h"
#include "DmLayer.h"

class DmLayerTable;
class DmLayer;

/// @brief 添加图层命令
class LayerTableAddCmd :public ICmd
{
public:
    /// @brief 构造添加图层命令
    /// @param table 图层表
    /// @param addedLayer 要添加的图层
    LayerTableAddCmd(DmLayerTable* table, DmLayer* addedLayer);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    DmObject* getObject() override {return m_addedLayer;}
    CmdType getCmdType() const override{return CmdType::LayerTableAddCmd;}

protected:
    DmLayerTable* m_table;          ///< 图层表
    DmLayer* m_addedLayer;          ///< 被添加的图层
    bool m_isOwnByCommand;           ///< 图层是否由该命令所有
};

/// @brief 修改图层命令
class LayerTableModifyCmd :public ICmd
{
public:
    LayerTableModifyCmd():m_table(nullptr),m_modifiedLayer(nullptr){}
    /// @brief 构造修改图层命令
    /// @param table 图层表
    /// @param modifiedLayer 要修改的图层
    LayerTableModifyCmd(DmLayerTable* table, DmLayer* modifiedLayer);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    DmObject* getObject() override {return m_modifiedLayer;}
    CmdType getCmdType() const override{return CmdType::LayerTableModifyCmd;}

protected:
    std::string m_originData;       ///< 原始序列化数据
    std::string m_newData;          ///< 新序列化数据
    DmLayer* m_modifiedLayer;       ///< 被修改的图层
    DmLayerTable* m_table;          ///< 图层表
};

/// @brief 移除图层命令
class LayerTableRemoveCmd :public ICmd
{
public:
    /// @brief 构造移除图层命令
    /// @param table 图层表
    /// @param removedLayer 要移除的图层
    LayerTableRemoveCmd(DmLayerTable* table, DmLayer* removedLayer);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    DmObject* getObject() override {return m_removedLayer;}
    CmdType getCmdType() const override{return CmdType::LayerTableRemoveCmd;}

protected:
    DmLayerTable* m_table;          ///< 图层表
    DmLayer* m_removedLayer;        ///< 被移除的图层
};

/// @brief 激活图层命令
class LayerTableActivateCmd :public ICmd
{
public:
    /// @brief 构造激活图层命令
    /// @param table 图层表
    /// @param activatedLayer 要激活的图层
    LayerTableActivateCmd(DmLayerTable* table, DmLayer* activatedLayer);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    CmdType getCmdType() const override{return CmdType::LayerTableActivateCmd;}

protected:
    DmLayerTable* m_table;              ///< 图层表
    DmLayer* m_activatedLayer;          ///< 被激活的图层
    DmLayer* m_originActiveLayer;       ///< 原来激活的图层
};

#endif //LAYERTABLECMD_H
