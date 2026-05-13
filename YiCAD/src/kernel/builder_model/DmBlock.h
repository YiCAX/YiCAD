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

/// @file DmBlock.h
/// @brief 图块定义类，块参照使用的模板，包含实体集合

#ifndef DMBLOCK_H
#define DMBLOCK_H

#include "DmObject.h"
#include "EntityTable.h"

class DmAttributeDefinition;

/// @brief 图块定义数据结构
struct DmBlockData
{
    DmBlockData() = default;

    DmBlockData(const QString& name, const DmVector& basePoint, bool frozen, const QString& path = "", const QString& modify = "");

    bool isValid() const;

    QString name;
    DmVector basePoint;

    QString pathName;
    QString modifyDate;
    bool isMidify = false;
    bool frozen = false;               ///< Frozen flag
    bool visibleInBlockList = true;    ///< Visible in block list
    bool selectedInBlockList = false;  ///< selected in block list
};

/// @brief 图块定义
/// 块参照使用的模板
class DmBlock : public DmObject
{
    TYPESYSTEM_HEADER();
    friend class DmBlockTable;

public:
    DmBlock();
    /// @param doc The document this block belongs to.
    /// @param blockData defining data of the block.
    DmBlock(DmDocument* doc, const DmBlockData& d);

    virtual ~DmBlock();

    virtual DM::EntityType getEntityType() const;

    DmBlock* clone() const;
    void setDocument(DmDocument* pDoc) override;

    QString getPath() const;
    void setPath(const QString& path);

    QString getModifyDate();
    void setModifyDate(const QString& date);

    bool blockIsModify();
    void setBlockIsModify(bool is);
    /// @return Name of this block (the name is an Id for this block).
    QString getName() const;

    /// @return base point of this block.
    DmVector getBasePoint() const;
    void setBasePoint(const DmVector& pt);

    //bool save(bool isAutoSave = false);

    //bool saveAs(const QString& filename, const QString& formatType, bool force = false);

    //bool open(const QString&);

    bool loadTemplate(const QString&);

    // sets a new name for the block. Only called by blocklist to
    // assure that block names stay unique.
    void setName(const QString& n);

    /// @retval true if this block is frozen (invisible)
    /// @retval false if this block isn't frozen (visible)
    bool isFrozen() const;

    // Toggles the visibility of this block.
    // Freezes the block if it's not frozen, thaws the block otherwise
    void toggle();

    /// (De-)freezes this block.
    /// @param freeze true: freeze, false: defreeze
    void freeze(bool freeze);

    /// Sets the visibility of the Block in block list
    /// @param v true: visible, false: invisible
    void visibleInBlockList(bool v);

    /// @brief Returns the visibility of the Block in block list
    bool isVisibleInBlockList() const;

    /// @brief Sets selection state of the block in block list
    /// @param v true: selected, false: deselected
    void selectedInBlockList(bool v);

    /// @brief Returns selection state of the block in block list
    bool isSelectedInBlockList() const;

    QStringList findNestedInsert(const QString& bName);

    /// @brief 是否包含属性定义
    bool hasAttributeDefinitions() const;

    /// @brief 获得块中的属性定义
    std::list<DmAttributeDefinition*> getAttributeDefinitions() const;

    EntityTable& getEntityTable() { return m_entityTable; }
    const EntityTable& getEntityTable() const { return m_entityTable; }

    // persistent helper
    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;

private:
    void getEntity(InputStream& rdr, const std::vector<PAIR>& revs);
    bool isSaveEntType(const DM::EntityType type) const; // 判断该类型实体是否保存

protected:
    DmBlockData data;

private:
    EntityTable         m_entityTable;                  ///< 图块的实体集

};

#endif
