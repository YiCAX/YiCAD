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

/// @file ActionDimension.h
/// @brief 标注操作基类头文件

#ifndef ACTIONDIMENSION_H
#define ACTIONDIMENSION_H

#include "PreviewActionInterface.h"

struct DmDimensionData;

/// @brief 所有标注操作的基类，提供标注公共数据与选项管理。
class ActionDimension : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] name 操作名称
    /// @param [in] doc 文档对象指针
    /// @param [in] docView 文档视图指针
    explicit ActionDimension(const char* name, DmDocument* doc,
                             GuiDocumentView* docView);
    ~ActionDimension() override;

    /// @brief 重置标注数据
    virtual void reset();

    void init(int status = 0) override;

    void hideOptions() override;
    void showOptions() override;

    void updateMouseCursor() override;

    /// @brief 设置标注文本
    /// @param [in] t 标注文本字符串
    void setText(const QString& t);

    /// @brief 获取标注标签
    /// @return 标注标签字符串引用
    const QString& getLabel() const;

    /// @brief 设置标注标签
    /// @param [in] t 标签字符串
    void setLabel(const QString& t);

    /// @brief 获取上公差
    /// @return 上公差字符串引用
    const QString& getTol1() const;

    /// @brief 设置上公差
    /// @param [in] t 公差字符串
    void setTol1(const QString& t);

    /// @brief 获取下公差
    /// @return 下公差字符串引用
    const QString& getTol2() const;

    /// @brief 设置下公差
    /// @param [in] t 公差字符串
    void setTol2(const QString& t);

    /// @brief 获取直径标志
    /// @return 是否为直径标注
    bool getDiameter() const;

    /// @brief 设置直径标志
    /// @param [in] d 是否显示直径符号
    void setDiameter(bool d);

    /// @brief 判断操作类型是否为标注操作
    /// @param [in] type 操作类型
    /// @return 是标注操作返回 true
    static bool isDimensionAction(DM::ActionType type);

protected:
    std::unique_ptr<DmDimensionData> data;  ///< 通用标注数据。

    QString label;      ///< 标注标签。
    QString tol1;       ///< 上公差。
    QString tol2;       ///< 下公差。
    bool diameter;       ///< 直径标志。
};

#endif

