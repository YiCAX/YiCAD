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

/// @file LLMSettingsPage.h
/// @brief LLM 配置对话框（最小闭环）
///
/// 提供简单的 QDialog 表单用于编辑 LLM 配置项。
/// 所有读写走 LLMSettingsService。
/// 本对话框不依赖任何 AI 调用逻辑，纯配置层入口。

#ifndef LLMSETTINGSPAGE_H
#define LLMSETTINGSPAGE_H

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;

class LLMSettingsPage : public QDialog
{
    Q_OBJECT

public:
    explicit LLMSettingsPage(QWidget* parent = nullptr);
    ~LLMSettingsPage() override = default;

private slots:
    void onSave();
    void slotResetKey();

private:
    void loadSettings();
    void setupUi();

    QLineEdit*      m_providerEdit;
    QLineEdit*      m_baseUrlEdit;
    QLineEdit*      m_modelEdit;
    QSpinBox*       m_timeoutSpin;
    QDoubleSpinBox* m_temperatureSpin;
    QLineEdit*      m_apiKeyEdit;
};

#endif // LLMSETTINGSPAGE_H
