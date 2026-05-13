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

/// @file UIIntSpinBox.cpp
/// @brief 自定义整数数字输入控件

#include "UIIntSpinBox.h"

namespace
{
    constexpr int kDefaultMaxValue = 1000;
    constexpr int kDefaultMinValue = 0;
    constexpr int kDefaultStep = 1;
}

UIIntSpinBox::UIIntSpinBox(QWidget* parent /*= nullptr*/)
    : QSpinBox(parent)
{
    setSingleStep(kDefaultStep);
    setStepType(QAbstractSpinBox::DefaultStepType);
    setMaximum(kDefaultMaxValue);
    setMinimum(kDefaultMinValue);
}
