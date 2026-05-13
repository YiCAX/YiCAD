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

/// @file DmLineTypeTable.h
/// @brief 线型表，管理文档中所有线型

#ifndef DMLINETYPETABLE_H
#define DMLINETYPETABLE_H

#include <QString>
#include <QFile>
#include <QList>
#include <QByteArray>
#include <QStringList>

#include "DmLineType.h"
#include "DmSystem.h"
#include "TableBase.h"


/// @brief 线型文件相关常量
namespace LineType
{
    const QString LineTypeFile = "acad.lin";
    const QString LineTypeIsoFile = "acadiso.lin";
    const QString ByLayer = "ByLayer";
    const QString ByBlock = "ByBlock";
    const QString Continuous = "Continuous";
    const QString DashLine = "DashLine";
}

class DmDocument;

/// @brief 线型表
class DmLineTypeTable : public ITable
{
public:
    using iterator = FilterIterator<std::vector<DmLineType*>::iterator>;
public:
    DmLineTypeTable();
    virtual ~DmLineTypeTable();

    void setDocument(DmDocument *pDoc) override;
    void startModify(DmObject* e) override;
    /// @brief 添加线型
    void add(DmLineType* e);
    /// @brief 通过 id 移除线型
    void remove(DmId id);
    /// @brief 移除线型
    void remove(DmLineType* e);
    /// @brief 查找线型（不能获得已删除的线型）
    DmLineType* find(const QString& name);
    /// @brief 通过 id 查找线型，采用此方法可获得已删除的线型
    DmLineType* find(const DmId& id);
    /// @brief 表中不存在该实体，直接添加
    bool add_direct(DmLineType* e);
    /// @brief 直接删除从表中实体
    bool remove_direct(DmLineType* e);

    iterator begin();
    iterator end();

    unsigned int count() const;

    /// @brief 按名称激活线型
    void activate(const QString& name);
    /// @brief 激活线型
    void activate(DmLineType* lineType);
    /// @brief 直接激活线型
    void activate_direct(DmLineType* lineType);
    /// @brief 获取当前激活的线型
    DmLineType* getActive();

    /// @brief 删除静态线型
    static void deleteStaticLineTypes();
    static DmLineType* ByLayer;
    static DmLineType* ByBlock;
    static DmLineType* Continuous;
    static DmLineType* DashLine;

private:
    std::unordered_map<DmId, DmLineType*>   m_lineTypeMap;      ///< 线型字典
    std::vector<DmLineType*>                m_lineTypes;        ///< 线型列表
    DmLineType*                             m_pActLineType;     ///< 当前激活的线型
};

#endif // !DMLINETYPETABLE_H
