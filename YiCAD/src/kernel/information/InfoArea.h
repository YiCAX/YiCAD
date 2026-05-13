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

/// @file InfoArea.h
/// @brief 面积与周长计算类，用于计算多边形区域的面积和周长

#ifndef INFOAREA_H
#define INFOAREA_H

#include <vector>

#include "DmVector.h"

class QPolygon;

/// @brief 面积与周长计算类，用于计算多边形区域的面积和周长
class InfoArea
{
public:
	InfoArea();

	/// @brief 重置所有点和计算结果
	void reset();

	/// @brief 添加点到点列表
	/// @param p 点的坐标
	void push_back(const DmVector& p);

	/// @brief 检查点是否已存在于轮廓中
	/// @param p 待检查的点坐标
	/// @return true 表示点重复，false 表示点不在轮廓中
	bool duplicated(const DmVector& p);

	/// @brief 移除最后一个点
	void pop_back();

	/// @brief 获取计算后的面积
	/// @return 面积值
	double getArea() const;

	/// @brief 获取周长，需要时自动计算
	/// @return 周长值
	double getCircumference();

	/// @brief 获取点数量
	/// @return 点数量
	int size();

	/// @brief 获取指定索引的点
	/// @param i 索引
	/// @return 点的常量引用
	const DmVector& at(const int i) const;

	/// @brief 计算QPolygon的面积（静态方法）
	/// @param polygon 多边形
	/// @return 面积值
	static double getArea(const QPolygon& polygon);

private:
	/// @brief 计算面积和周长
	void calculate();

	/// @brief 计算子区域面积
	/// @param p1 第一个点
	/// @param p2 第二个点
	/// @return 子区域面积
	double calcSubArea(const DmVector& p1, const DmVector& p2);

private:
	std::vector<DmVector> m_thePoints;          ///< 点列表
	double m_dBaseY = 0.0;                       ///< 基准Y坐标
	double m_dArea = 0.0;                        ///< 面积
	double m_dCircumference = 0.0;               ///< 周长
	bool isCalculationNeeded = true;             ///< 是否需要重新计算
};

#endif
