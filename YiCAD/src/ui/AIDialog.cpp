/*
 * Copyright (C) 2026 YiCAD
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

/// @file AIDialog.cpp
/// @brief AI 助手对话框实现

#include "AIDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCloseEvent>
#include <QScrollBar>

AIDialog::AIDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("AI Assistant"));
    setMinimumSize(480, 600);
    resize(520, 680);

    // modeless: 关闭时隐藏而非销毁
    setAttribute(Qt::WA_DeleteOnClose, false);

    setupUI();
    setupConnections();
}

QString AIDialog::currentMode() const
{
    return m_pModeCombo->currentText();
}

int AIDialog::modeIndex() const
{
    return m_pModeCombo ? m_pModeCombo->currentIndex() : 2;
}

void AIDialog::setMode(int mode)
{
    if (mode >= 0 && mode < m_pModeCombo->count())
    {
        m_pModeCombo->setCurrentIndex(mode);
    }
}

void AIDialog::closeEvent(QCloseEvent* event)
{
    // modeless: 关闭时隐藏
    hide();
    event->ignore();
}

void AIDialog::setupUI()
{
    auto* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(8, 8, 8, 8);
    pMainLayout->setSpacing(6);

    // ========== 顶部：模式状态栏 ==========
    auto* pModeLayout = new QHBoxLayout();
    m_pModeLabel = new QLabel(tr("Mode:"), this);
    m_pModeCombo = new QComboBox(this);
    m_pModeCombo->addItem(tr("Q&A"));       // 0: 问答
    m_pModeCombo->addItem(tr("Modeling"));   // 1: 建模
    m_pModeCombo->addItem(tr("Auto"));       // 2: 自动
    m_pModeCombo->setCurrentIndex(2);        // 默认自动
    pModeLayout->addWidget(m_pModeLabel);
    pModeLayout->addWidget(m_pModeCombo);
    pModeLayout->addStretch();
    pMainLayout->addLayout(pModeLayout);

    // ========== 中部：对话显示区 ==========
    m_pChatView = new QTextEdit(this);
    m_pChatView->setReadOnly(true);
    m_pChatView->setPlaceholderText(tr("AI conversation will be displayed here..."));
    m_pChatView->setMinimumHeight(200);
    pMainLayout->addWidget(m_pChatView, 1);  // stretch factor = 1

    // ========== 底部：输入区 ==========
    auto* pInputGroup = new QGroupBox(tr("Input"), this);
    auto* pInputLayout = new QVBoxLayout(pInputGroup);

    // 输入框 + 发送按钮
    auto* pInputRow = new QHBoxLayout();
    m_pInputEdit = new QTextEdit(this);
    m_pInputEdit->setPlaceholderText(tr("Enter your question or modeling command..."));
    m_pInputEdit->setMaximumHeight(80);
    m_pInputEdit->setAcceptRichText(false);
    m_pInputEdit->setTabChangesFocus(true);  // Tab 切换焦点而非插入制表符

    m_pBtnSend = new QPushButton(tr("Send"), this);
    m_pBtnSend->setDefault(true);            // 回车键触发
    m_pBtnSend->setMinimumWidth(70);

    pInputRow->addWidget(m_pInputEdit, 1);
    pInputRow->addWidget(m_pBtnSend);
    pInputLayout->addLayout(pInputRow);

    // 操作按钮行
    auto* pBtnRow = new QHBoxLayout();
    m_pBtnExecute = new QPushButton(tr("Execute"), this);
    m_pBtnCancel  = new QPushButton(tr("Cancel"), this);
    m_pBtnConfig  = new QPushButton(tr("Config"), this);
    m_pBtnHistory = new QPushButton(tr("History"), this);

    pBtnRow->addWidget(m_pBtnExecute);
    pBtnRow->addWidget(m_pBtnCancel);
    pBtnRow->addWidget(m_pBtnConfig);
    pBtnRow->addWidget(m_pBtnHistory);
    pBtnRow->addStretch();
    pInputLayout->addLayout(pBtnRow);

    pMainLayout->addWidget(pInputGroup);
}

void AIDialog::setupConnections()
{
    // 发送按钮 → 发射 sendRequested 信号
    connect(m_pBtnSend, &QPushButton::clicked, this, &AIDialog::slotSendClicked);

    // 执行按钮 → 发射 executeRequested 信号
    connect(m_pBtnExecute, &QPushButton::clicked, this, &AIDialog::slotExecuteClicked);

    // 取消按钮 → 发射 cancelRequested 信号
    connect(m_pBtnCancel, &QPushButton::clicked, this, &AIDialog::slotCancelClicked);

    // 配置按钮 → 发射 configRequested 信号（占位，后续连接 LLM 配置）
    connect(m_pBtnConfig, &QPushButton::clicked, this, &AIDialog::slotConfigClicked);

    // 历史记录按钮 → 发射 historyRequested 信号（占位，后续连接历史面板）
    connect(m_pBtnHistory, &QPushButton::clicked, this, &AIDialog::slotHistoryClicked);

    // 模式切换
    connect(m_pModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AIDialog::slotModeChanged);
}

void AIDialog::appendMessage(const QString& sender, const QString& message)
{
    m_pChatView->moveCursor(QTextCursor::End);

    // 发送者标签
    QTextCharFormat senderFmt;
    senderFmt.setFontWeight(QFont::Bold);
    senderFmt.setForeground(sender == tr("User") ? QColor(0x2D, 0x53, 0x66) : QColor(0x66, 0x66, 0x66));
    m_pChatView->setCurrentCharFormat(senderFmt);
    m_pChatView->insertPlainText(sender + "\n");

    // 消息内容
    QTextCharFormat msgFmt;
    msgFmt.setFontWeight(QFont::Normal);
    m_pChatView->setCurrentCharFormat(msgFmt);
    m_pChatView->insertPlainText(message + "\n\n");

    // 滚动到底部
    auto* sb = m_pChatView->verticalScrollBar();
    sb->setValue(sb->maximum());
}

// ==================== 槽函数 ====================

void AIDialog::slotSendClicked()
{
    QString text = m_pInputEdit->toPlainText().trimmed();
    if (text.isEmpty())
    {
        return;
    }

    QString mode = currentMode();

    // 在对话区显示用户消息
    appendMessage(tr("User"), text);

    // 清空输入框
    m_pInputEdit->clear();
    m_pInputEdit->setFocus();

    // 发射信号（外部连接 DeepSeekProvider / IntentRouter 后处理）
    emit sendRequested(text, mode);
}

void AIDialog::slotExecuteClicked()
{
    QString text = m_pInputEdit->toPlainText().trimmed();
    if (text.isEmpty())
    {
        return;
    }

    // 在对话区显示执行提示
    appendMessage(tr("System"), tr("Executing: %1").arg(text));

    // 发射信号
    emit executeRequested(text);
}

void AIDialog::slotCancelClicked()
{
    emit cancelRequested();
}

void AIDialog::slotConfigClicked()
{
    emit configRequested();
}

void AIDialog::slotHistoryClicked()
{
    emit historyRequested();
}

void AIDialog::slotModeChanged(int index)
{
    Q_UNUSED(index);
    // 占位：后续可用于 UI 状态切换
}
