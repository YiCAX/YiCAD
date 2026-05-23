# YiCAD

YiCAD 是一个开源的 2D CAD 应用程序，功能类似 AutoCAD，使用 **C++23** 和 **Qt 5.15** 开发，基于 **OpenGL** 进行高性能图形渲染。

![YiCAD 界面](screenshot.png)

当前版本：**v0.5**

## 功能特性

- **2D 绘图实体**：直线、圆弧、圆、椭圆、多段线、样条曲线、填充(Hatch)、实体(Solid)
- **块(Block)系统**：块定义、块引用、块属性，支持嵌套块
- **标注系统**：线性标注、对齐标注、角度标注、半径/直径标注
- **文字系统**：单行文字、多行文字(MTEXT)
- **图层管理**：支持图层的创建、锁定、冻结、可见性控制
- **线型/颜色/线宽**：丰富的实体属性设置
- **图像支持**：插入和管理光栅图像
- **捕捉系统**：端点、中点、圆心、交点、垂足等多种捕捉模式
- **修剪/延伸**：支持 Trim 和 Extend 操作
- **Undo/Redo**：完整的撤销/重做框架，基于命令栈实现
- **Office Ribbon 风格 UI**：基于 SARibbonBar 的现代化功能区界面
- **命令行输入**：支持命令行快捷操作

## 构建

### 第三方库

CMake 配置时会自动检查 `ThirdParty/` 目录，如不存在则自动下载第三方库压缩包并解压。也可以手动从以下地址下载：

> **手动下载地址：** [https://github.com/YiCAX/YiCAD/releases/download/thirdparty%2F2026-05-12/ThirdParty.zip](https://github.com/YiCAX/YiCAD/releases/download/thirdparty%2F2026-05-12/ThirdParty.zip)

将下载的 `ThirdParty.zip` 解压到项目根目录即可。

| 库 | 用途 |
|----|------|
| [SARibbonBar](https://github.com/czyt1988/SARibbonBar) | Office 风格 Ribbon 界面 |
| [Boost 1.90](https://www.boost.org/) | 一元四次方程求解等 |
| [CGAL 6.1](https://www.cgal.org/) | 计算几何算法库 |
| [muparser](https://beltoforion.de/en/muparser/) | 数学表达式解析 |
| [nlohmann/json](https://github.com/nlohmann/json) | JSON 序列化 |
| [mimalloc](https://github.com/microsoft/mimalloc) | 高性能内存分配 |

### 依赖

- **CMake** 3.10+
- **Visual Studio 2022** (Windows)
- **Qt 5.15**, **Boost 1.90**, **CGAL 6.1**, **OpenGL** / GLEW / GLM, **Xerces-C**, **FreeType**, **mimalloc**, **Eigen**（均内置于 ThirdParty）
- **SARibbonBar**
- **muparser**

### Windows

```bash
# 生成 Visual Studio 2022 项目 (v143 工具链)
cmake -G "Visual Studio 17 2022" -T "v143" -S . -B build

# 编译 Release
cmake --build build --config Release

# 编译 Debug
cmake --build build --config Debug

# 安装
cmake --install build --config Release

# 输出位置: build/bin/Release/YiCAD.exe
```

> **注意：** Linux 后续支持。

## 架构概览

项目采用 **MVC + Action 模式** 架构：

| 层次 | 路径 | 说明 |
|------|------|------|
| **数据模型** | `YiCAD/src/kernel/data_model/` | Dm* 类 — CAD 实体数据 |
| **视图** | `YiCAD/src/kernel/gui/` | QOpenGLWidget 子类，4 层渲染 |
| **动作** | `YiCAD/src/actions/` | ~75 个 Action 类处理用户交互 |
| **命令** | `YiCAD/src/cmd/` | 命令行输入解析与分发 |
| **Undo/Redo** | `YiCAD/src/kernel/history/` | 命令栈、事务、宏命令 |
| **数学计算** | `YiCAD/src/kernel/math/` | 计算几何、KD树、R树、Delaunay三角剖分 |
| **渲染** | `YiCAD/src/kernel/painters/` | OpenGL 绘制抽象层 |
| **持久化** | `YiCAD/src/kernel/persistence/` | XML 序列化 (Xerces-C) |

## 开发

- **代码规范**: UTF-8 with BOM 编码
- **命名约定**: `Dm*` (数据模型), `Action*` (交互命令), `UI*` (界面组件), `GL*` (OpenGL), `Meta*` (序列化), `Filter*` (文件格式)

## License

本项目基于 **Apache License 2.0** 开源。详见 [LICENSE](LICENSE.txt) 文件。

Licensed under the **Apache License, Version 2.0**. See [LICENSE](LICENSE.txt) for the full text.
