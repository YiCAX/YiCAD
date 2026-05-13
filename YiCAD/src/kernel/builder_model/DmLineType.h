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

/// @file DmLineType.h
/// @brief 线型类，管理线型的名称、外观和虚线模式数据

#ifndef DMLINETYPE_H
#define DMLINETYPE_H

#include <cstddef>
#include <vector>

#include "DmSystem.h"
#include "Datamodel.h"
#include "DmObject.h"

class DmLineTypeTable;

/// @brief 线型数据结构
struct DmLineTypeData
{
    DmLineTypeData() = default;

    /// @brief 构造线型数据
    /// @param [in] _name 线型名称
    /// @param [in] _desp 线型描述
    /// @param [in] _outward 线型外观
    /// @param [in] _pattern 虚线模式数据
    DmLineTypeData(const QString& _name, const QString& _desp,
        const QString& _outward, const std::vector<double>& _pattern);

    QString m_LineTypeName;                ///< 名字
    QString m_LineTypeOutward;             ///< 外观
    QString m_LineTypeDesp;                ///< 描述
    std::vector<double> m_LineTypePattern; ///< 模式数据
};

/// @brief 线型
class DmLineType : public DmObject
{
    TYPESYSTEM_HEADER();
public:
    DmLineType();
    explicit DmLineType(const QString& _name);
    DmLineType(DmLineTypeData _data);
    DmLineType(DmLineType* _linetype);
    ~DmLineType() = default;

    /// @brief 设置线型名称
    void setLineTypeName(QString name);

    /// @brief 设置线型外观
    void setLineTypeOutWard(QString outward);

    /// @brief 设置线型描述
    void setLineTypeDesp(QString desp);

    /// @brief 通过逗号分隔的字符串设置线型数据
    void setLineTypeData(QString _data);

    /// @brief 通过向量设置线型数据
    void setLineTypeData(std::vector<double> _pattern);

    /// @brief 设置线型模式数据
    void setLineTypePattern(std::vector<double> _pattern);

    /// @brief 获取线型名称
    QString getLineTypeName();

    /// @brief 获取线型外观
    QString getLineTypeOutWard();

    /// @brief 获取线型描述
    QString getLineTypeDesp();

    /// @brief 获取线型数据（模式向量引用）
    std::vector<double>& getLineTypeData();

    /// @brief 获取模式数据元素个数
    size_t getNum();

    /// @brief 清空线型数据
    void clear();

    // persistent helper
    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& reader,
        const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

protected:
    DmLineTypeData data;
};

#endif // DMLINETYPE_H
