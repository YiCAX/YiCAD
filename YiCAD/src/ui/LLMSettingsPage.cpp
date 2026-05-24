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

/// @file LLMSettingsPage.cpp
/// @brief LLM 配置对话框实现

#include "LLMSettingsPage.h"
#include "LLMSettingsService.h"

#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>

LLMSettingsPage::LLMSettingsPage(QWidget* parent)
    : QDialog(parent)
    , m_providerEdit(nullptr)
    , m_baseUrlEdit(nullptr)
    , m_modelEdit(nullptr)
    , m_timeoutSpin(nullptr)
    , m_temperatureSpin(nullptr)
    , m_apiKeyEdit(nullptr)
{
    setWindowTitle(tr("LLM 配置"));
    setMinimumWidth(480);

    setupUi();
    loadSettings();

    // 连接保存按钮
    QDialogButtonBox* btnBox = findChild<QDialogButtonBox*>();
    if (btnBox)
    {
        connect(btnBox, &QDialogButtonBox::accepted, this, &LLMSettingsPage::onSave);
        connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }
}

void LLMSettingsPage::setupUi()
{
    auto* mainLayout = new QVBoxLayout(this);

    // ---- 顶部说明 ----
    auto* headerLabel = new QLabel(tr("配置 AI 大语言模型连接参数。\n"
                                      "不填 API Key 则 AI 功能不可用。"));
    headerLabel->setWordWrap(true);
    mainLayout->addWidget(headerLabel);

    // ---- 表单区 ----
    auto* formLayout = new QFormLayout();

    m_providerEdit = new QLineEdit(this);
    m_providerEdit->setPlaceholderText(tr("deepseek"));
    formLayout->addRow(tr("Provider:"), m_providerEdit);

    m_baseUrlEdit = new QLineEdit(this);
    m_baseUrlEdit->setPlaceholderText("https://api.deepseek.com/v1");
    formLayout->addRow(tr("Base URL:"), m_baseUrlEdit);

    m_modelEdit = new QLineEdit(this);
    m_modelEdit->setPlaceholderText("deepseek-chat");
    formLayout->addRow(tr("Model:"), m_modelEdit);

    m_timeoutSpin = new QSpinBox(this);
    m_timeoutSpin->setRange(5, 600);
    m_timeoutSpin->setSuffix(tr(" 秒"));
    m_timeoutSpin->setToolTip(tr("请求超时时间（秒）"));
    formLayout->addRow(tr("Timeout:"), m_timeoutSpin);

    m_temperatureSpin = new QDoubleSpinBox(this);
    m_temperatureSpin->setRange(0.0, 2.0);
    m_temperatureSpin->setSingleStep(0.1);
    m_temperatureSpin->setDecimals(2);
    m_temperatureSpin->setToolTip(tr("生成随机性，0=确定，2=最随机"));
    formLayout->addRow(tr("Temperature:"), m_temperatureSpin);

    m_apiKeyEdit = new QLineEdit(this);
    m_apiKeyEdit->setEchoMode(QLineEdit::Password);
    m_apiKeyEdit->setPlaceholderText(tr("sk-..."));
    m_apiKeyEdit->setToolTip(tr("API Key 通过加密存储，不会以明文保存"));
    formLayout->addRow(tr("API Key:"), m_apiKeyEdit);

    mainLayout->addLayout(formLayout);

    // ---- 底部提示 ----
    auto* noteLabel = new QLabel(tr("注意：API Key 使用 XOR 混淆存储（临时）。\n"
                                    "正式版将替换为系统级安全存储（DPAPI/Keychain）。"));
    noteLabel->setWordWrap(true);
    noteLabel->setStyleSheet("color: #888; font-size: 11px;");
    mainLayout->addWidget(noteLabel);

    // ---- 按钮 ----
    auto* btnBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    btnBox->button(QDialogButtonBox::Ok)->setText(tr("保存"));
    btnBox->button(QDialogButtonBox::Cancel)->setText(tr("取消"));
    mainLayout->addWidget(btnBox);
}

void LLMSettingsPage::loadSettings()
{
    LLMSettingsService* svc = LLMSettingsService::instance();
    if (!svc || !svc->isInitialized())
    {
        return;
    }

    m_providerEdit->setText(svc->provider());
    m_baseUrlEdit->setText(svc->baseUrl());
    m_modelEdit->setText(svc->model());
    m_timeoutSpin->setValue(svc->timeoutSecs());
    m_temperatureSpin->setValue(svc->temperature());

    // API Key 不预填（出于安全，用户需手动输入查看）
    // 如果已有 Key，用占位提示
    if (svc->hasApiKey())
    {
        m_apiKeyEdit->setPlaceholderText(tr("（已设置，输入新 Key 覆盖）"));
    }
}

void LLMSettingsPage::onSave()
{
    LLMSettingsService* svc = LLMSettingsService::instance();
    if (!svc || !svc->isInitialized())
    {
        QMessageBox::warning(this, tr("错误"),
                             tr("LLM 配置服务未初始化。"));
        return;
    }

    svc->setProvider(m_providerEdit->text());
    svc->setBaseUrl(m_baseUrlEdit->text());
    svc->setModel(m_modelEdit->text());
    svc->setTimeoutSecs(m_timeoutSpin->value());
    svc->setTemperature(m_temperatureSpin->value());

    // 仅当用户输入了新 Key 才更新（避免空输入覆盖已有 Key）
    const QString newKey = m_apiKeyEdit->text().trimmed();
    if (!newKey.isEmpty())
    {
        svc->setApiKey(newKey);
    }

    accept();
}
