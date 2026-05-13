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

/// @file ActionDrawImage.h
/// @brief 图片插入交互动作类

#ifndef ACTIONDRAWIMAGE_H
#define ACTIONDRAWIMAGE_H

#include <memory>

#include "PreviewActionInterface.h"

class ImageData;
class QImage;

/// @brief 通过指定插入点和缩放参数将位图插入当前绘图
class ActionDrawImage : public PreviewActionInterface
{
    Q_OBJECT

    /// @brief 动作状态枚举
    enum Status
    {
        ShowDialog,     ///< 显示图片选择对话框
        SetTargetPoint, ///< 设置插入参考点
        SetAngle,       ///< 在命令行设置旋转角度
        SetFactor,      ///< 在命令行设置缩放因子
        SetDPI          ///< 在命令行设置 DPI
    };

public:
    ActionDrawImage(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawImage() override;

    void init(int status = 0) override;
    void reset();
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void showOptions() override;
    void hideOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

    /// @brief 获取当前旋转角度（弧度）
    /// @return 旋转角度
    double getAngle() const;

    /// @brief 设置旋转角度
    /// @param a 角度值（弧度）
    void setAngle(double a) const;

    /// @brief 获取当前缩放因子
    /// @return 缩放因子
    double getFactor() const;

    /// @brief 设置缩放因子
    /// @param f 缩放因子
    void setFactor(double f) const;

    /// @brief DPI 值转换为缩放因子
    /// @param dpi DPI 值
    /// @return 对应的缩放因子
    double dpiToScale(double dpi) const;

    /// @brief 缩放因子转换为 DPI 值
    /// @param scale 缩放因子
    /// @return 对应的 DPI 值
    double scaleToDpi(double scale) const;

protected:
    struct AImageData;
    std::unique_ptr<AImageData> pImg;

    Status m_lastStatus{ ShowDialog }; ///< 进入选项前的上一个状态
};

#endif
