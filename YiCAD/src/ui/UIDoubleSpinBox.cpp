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

/// @file UIDoubleSpinBox.cpp
/// @brief 自定义双精度浮点数字输入控件，支持高精度小数输入

#include "UIDoubleSpinBox.h"

namespace
{
    constexpr double kDefaultStep = 0.0005;
    constexpr double kMaxValue = 1e10;
    constexpr double kMinValue = -1e10;
    constexpr int kDisplayPrecision = 15;
}

UIDoubleSpinBox::UIDoubleSpinBox(QWidget* parent /*= nullptr*/)
    : QDoubleSpinBox(parent)
{
    setDecimals(4);
    setSingleStep(kDefaultStep);
    setStepType(QAbstractSpinBox::DefaultStepType);
    setMaximum(kMaxValue);
    setMinimum(kMinValue);
}

QString UIDoubleSpinBox::textFromValue(double value) const
{
    //QString vStr = QLocale().toString(value, 'g', 10);
    QString vStr = QString::number(value, 'g', kDisplayPrecision);
    return vStr;
}
