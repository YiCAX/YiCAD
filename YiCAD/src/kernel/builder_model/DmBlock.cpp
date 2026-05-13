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

/// @file DmBlock.cpp
/// @brief 图块定义类的实现，包括实体管理、序列化、嵌套查找等功能

#include <iostream>
#include "DmBlock.h"

#include "DmDocument.h"
#include "DmBlockReference.h"
#include "DmLine.h"
#include "DmCircle.h"
#include "DmPoint.h"
#include "DmPolyline.h"
#include "DmArc.h"
#include "DmEllipse.h"
#include "DmSolid.h"
#include "DmSpline.h"
#include "DmRay.h"
#include "DmAttributeDefinition.h"
#include "DmAttribute.h"
#include "DmDimLinear.h"
#include "DmEntityHelper.h"

TYPESYSTEM_SOURCE(DmBlock, DmObject, 0)

DmBlockData::DmBlockData(const QString& _name, const DmVector& _basePoint, bool _frozen, const QString& path, const QString& modify)
    : name(_name)
    , basePoint(_basePoint)
    , frozen(_frozen)
    , pathName(path)
    , modifyDate(modify)
{
}

bool DmBlockData::isValid() const
{
    return (!name.isEmpty() && basePoint.valid);
}

DmBlock::DmBlock()
{

}

/// @param doc The document this block belongs to.
/// @param d The data defining the block.
DmBlock::DmBlock(DmDocument* doc, const DmBlockData& d)
    : DmObject()
    , data(d)
{
    setDocument(doc);
}

DmBlock::~DmBlock()
{
}

DM::EntityType DmBlock::getEntityType() const
{
    return DM::EntityBlock;
}

DmBlock* DmBlock::clone() const
{
    DmBlock* blk = new DmBlock(*this);
    return blk;
}

void DmBlock::setDocument(DmDocument* pDoc)
{
    DmObject::setDocument(pDoc);
    m_entityTable.setDocument(pDoc);
}

QString DmBlock::getName() const
{
    return data.name;
}

QString DmBlock::getPath() const
{
    return data.pathName;
}

void DmBlock::setPath(const QString& path)
{
    data.pathName = path;
}

QString DmBlock::getModifyDate()
{
    return data.modifyDate;
}

void DmBlock::setModifyDate(const QString& date)
{
    data.modifyDate = date;
}

DmVector DmBlock::getBasePoint() const
{
    return data.basePoint;
}

void DmBlock::setBasePoint(const DmVector& pt)
{
    data.basePoint = pt;
}

bool DmBlock::blockIsModify()
{
    return data.isMidify;
}

void DmBlock::setBlockIsModify(bool is)
{
    data.isMidify = is;
}

//bool DmBlock::save(bool isAutoSave)
//{
//    //todo :暂不支持块的保存
//    return false;
//}

//bool DmBlock::saveAs(const QString& filename, const QString& formatType, bool force)
//{
//    DmDocument* g = getDocument();
//    if (g)
//    {
//        return g->saveAs(filename, formatType, force);
//    }
//    else
//    {
//        return false;
//    }
//}

//bool DmBlock::open(const QString&)
//{
//    // do nothing
//    return false;
//}

void DmBlock::setName(const QString& n)
{
    data.name = n;
}

bool DmBlock::isFrozen() const
{
    return data.frozen;
}

void DmBlock::freeze(bool freeze)
{
    data.frozen = freeze;
}

void DmBlock::toggle()
{
    data.frozen = !data.frozen;
}

bool DmBlock::loadTemplate(const QString&)
{
    // do nothing
    return false;
}

void DmBlock::visibleInBlockList(bool v)
{
    data.visibleInBlockList = v;
}

bool DmBlock::isVisibleInBlockList() const
{
    return data.visibleInBlockList;
}

void DmBlock::selectedInBlockList(bool v)
{
    data.selectedInBlockList = v;
}

bool DmBlock::isSelectedInBlockList() const
{
    return data.selectedInBlockList;
}

QStringList DmBlock::findNestedInsert(const QString& bName)
{
    QStringList bnChain;

    for (DmEntity* e : m_entityTable)
    {
        if (e->getEntityType() == DM::EntityBlockReference)
        {
            DmBlockReference* i = ((DmBlockReference*)e);
            QString iName = i->getName();
            if (iName == bName)
            {
                bnChain << data.name;
                break;
            }
            else
            {
                DmBlockTable* blockTable = getDocument()->getBlockTable();
                if (blockTable)
                {
                    DmBlock* nestedBlock = blockTable->find(iName);
                    if (nestedBlock)
                    {
                        QStringList nestedChain;
                        nestedChain = nestedBlock->findNestedInsert(bName);
                        if (!nestedChain.empty())
                        {
                            bnChain << data.name;
                            bnChain << nestedChain;
                            break;
                        }
                    }
                }
            }
        }
    }

    return bnChain;
}

bool DmBlock::hasAttributeDefinitions() const
{
    for (auto ent : m_entityTable)
    {
        if (ent->getEntityType() == DM::EntityAttributeDefinition)
        {
            return true;
        }
    }
    return false;
}

std::list<DmAttributeDefinition*> DmBlock::getAttributeDefinitions() const
{
    std::list<DmAttributeDefinition*> attrDefs;
    for (auto ent : m_entityTable)
    {
        if (ent->getEntityType() == DM::EntityAttributeDefinition)
        {
            attrDefs.emplace_back(static_cast<DmAttributeDefinition*>(ent));
        }
    }
    return attrDefs;
}

void DmBlock::saveStream(OutputStream& wrt) const
{
    DmObject::saveStream(wrt);

    auto name = getName().toStdString();
    auto basePoint = getBasePoint();
    auto frozen = isFrozen();
    wrt << name << (double)basePoint.x << (double)basePoint.y;

    wrt << (uint32_t)m_entityTable.count();
    for (auto ent : m_entityTable)
    {
        if (isSaveEntType(ent->getEntityType()))
        {
            auto strType = DmEntityHelper::getEntityNameByType(ent->getEntityType());
            wrt << strType;
            ent->saveStream(wrt);
        }
    }
}

void DmBlock::restoreStream(InputStream& reader, const std::vector<PAIR>& revs)
{
    DmObject::restoreStream(reader, revs);

    int fileRev = getRevisionId("DmBlock", revs);
    if (revId > fileRev)
    {
        // 老文件格式
        restoreStreamWithRev(reader, fileRev);
    }
    else
    {
        std::string name;
        DmVector basePoint;
        bool frozen = false;
        uint32_t entSize = 0;
        reader >> (std::string&)name >> (double&)basePoint.x >> (double&)basePoint.y >> (uint32_t&)entSize;

        setName(QString::fromStdString(name));
        setBasePoint(basePoint);
        data.frozen = frozen;

        m_entityTable.clear_direct();
        for (uint32_t i = 0; i < entSize; ++i)
        {
            getEntity(reader, revs);
        }
    }
}

void DmBlock::restoreStreamWithRev(InputStream& rdr, int rev)
{
    if (rev == 0)
    {
    }
    else //big change, e.g. change supper class of DmBlock
    {
        //step1.
        // read all legacy data one by one
    }
}

void DmBlock::getEntity(InputStream& rdr, const std::vector<PAIR>& revs)
{
    std::string strType;
    rdr >> strType;
    DmEntity* pEntity = DmEntityHelper::createEntityByName(strType);

    if (pEntity)
    {
        pEntity->setParent(nullptr);
        pEntity->setDocument(getDocument());
        pEntity->restoreStream(rdr, revs);
        m_entityTable.add_direct(pEntity);
    }
}

bool DmBlock::isSaveEntType(const DM::EntityType type) const
{
    switch (type)
    {
    case DM::EntityLine:
    case DM::EntityCircle:
    case DM::EntityPoint:
    case DM::EntityPolyline:
    case DM::EntityArc:
    case DM::EntityEllipse:
    case DM::EntitySolid:
    case DM::EntitySpline:
    case DM::EntityRay:
    case DM::EntityXline:
    case DM::EntityBlockReference:
    case DM::EntityAttribute:
    case DM::EntityAttributeDefinition:
    case DM::EntityText:
    case DM::EntityDimAligned:
    case DM::EntityDimLinear:
    case DM::EntityDimRadial:
    case DM::EntityDimDiametric:
    case DM::EntityDimAngular:
    case DM::EntityDimLeader:
    case DM::EntityMText:
    case DM::EntityHatch:
    //case DM::EntityImage:
        return true;
    default:
        return false;
    }
}
