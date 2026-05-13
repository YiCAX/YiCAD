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

/// @file GuiPreviewWidget.h
/// @brief OpenGL 预览控件，用于块定义等场景的预览显示

#ifndef GUIPREVIEWWIDGET_H
#define GUIPREVIEWWIDGET_H

#define GL_GLEXT_PROTOTYPES

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <QOpenGLWidget>
#include <QOpenGLContext>

#include "DmEntityContainer.h"
#include "DmCachePainter.h"

/// @brief 预览控件
class GuiPreviewWidget : public QOpenGLWidget
{
public:
    /// @brief 构造预览控件
    /// @param parent 父控件
    /// @param fl 窗口标志
    GuiPreviewWidget(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());
    virtual ~GuiPreviewWidget();

    /// @brief 指定预览的实体集
    /// @param container 预览实体容器
    void setContainer(DmEntityContainer* container);
    /// @brief 适屏显示
    void zoomAuto();
    /// @brief 检查是否已初始化
    /// @return true 表示 OpenGL 已初始化
    bool initialized() const;
    /// @brief 重绘控件
    void redraw();
    /// @brief 指示内容已修改，需要重新缓存
    void specifyModified();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    /// @brief 创建画笔
    void createPainters(unsigned int width, unsigned int height);

protected:
    DmEntityContainer*                  container;          ///< 预览实体容器
    DmColor                             background;         ///< 背景色
    bool                                m_isInitialized;    ///< 是否已初始化
    DmCachePainter*                     m_pPainter;         ///< 缓存画笔
};

#endif
