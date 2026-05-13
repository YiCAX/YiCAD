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

/// @file UIDlgCmdsSetting.cpp
/// @brief 命令快捷键设置对话框实现

#include "UIDlgCmdsSetting.h"

#include "Commands.h"

#include <QPushButton>

UIDlgCmdsSetting::UIDlgCmdsSetting(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(true);
    setupUi(this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotOk()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(slotBtnClicked(QAbstractButton*)));
}

void UIDlgCmdsSetting::setGroup(const QString& group)
{
    m_group = group;
    init();
}

void UIDlgCmdsSetting::init()
{
    auto data = Commands::readConfigFile(COMMANDS->getConfigFile(), m_group, false);
    auto userData = Commands::readConfigFile(COMMANDS->getUserConfig(), m_group, true);
    m_data = (userData.size() == 0) ? data : userData;

    loadDataToUI();
}

void UIDlgCmdsSetting::loadDataToUI()
{
    tblCmds->clear();

    tblCmds->setColumnCount(2);
    tblCmds->setRowCount(m_data.size());
    tblCmds->setHorizontalHeaderLabels(QStringList{ tr("Command"), tr("Keys") });
    tblCmds->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //隐藏竖直向标题
    tblCmds->verticalHeader()->hide();

    //设置列标题不可选（不可选整列）
    tblCmds->horizontalHeader()->setSectionsClickable(false);

    //设置单击编辑
    tblCmds->setEditTriggers(QAbstractItemView::AllEditTriggers);

    //tblCmds->setStyleSheet("QHeaderView::section { background-color:gray }");	//该对话框指定了父窗体之后，样式设置无效？
    tblCmds->setSortingEnabled(false);

    //设置数据
    for (int i = 0; i < m_data.size(); i++)
    {
        auto item = m_data.at(i);
        QString desr = std::get<1>(item);
        QStringList keys = std::get<2>(item);

        QTableWidgetItem* desrItem = new QTableWidgetItem(desr);
        desrItem->setFlags(desrItem->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
        desrItem->setTextAlignment(Qt::AlignCenter);
        tblCmds->setItem(i, 0, desrItem);

        QString keysStr = keys.join(", ");
        QTableWidgetItem* keysItem = new QTableWidgetItem(keysStr);
        keysItem->setTextAlignment(Qt::AlignCenter);
        keysItem->setToolTip(tr("The last set keys of command: %1 is %2. One command can map multiple keys, keys should be separated by comma(,). ")
            .arg(desr).arg(keysStr));
        tblCmds->setItem(i, 1, keysItem);
    }
}

void UIDlgCmdsSetting::slotBtnClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton btn = buttonBox->standardButton(button);
    if (btn == QDialogButtonBox::StandardButton::RestoreDefaults)
    {
        auto data = Commands::readConfigFile(COMMANDS->getConfigFile(), m_group, false);
        m_data = data;
        loadDataToUI();
    }
}

void UIDlgCmdsSetting::slotOk()
{
    for (int i = 0; i < m_data.size(); i++)
    {
        auto& item = m_data.at(i);
        QString keysStr = tblCmds->item(i, 1)->text().trimmed().toLower();
        keysStr.replace(QString::fromLocal8Bit(","), ",");
        QStringList keys = keysStr.split(",", Qt::SkipEmptyParts);
        std::get<2>(item) = keys;
    }

    //保存到文件
    Commands::saveToFile(m_data, m_group, COMMANDS->getUserConfig());
    QDialog::accept();
}
