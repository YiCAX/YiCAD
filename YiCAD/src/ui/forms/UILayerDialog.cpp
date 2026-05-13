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

/// @file UILayerDialog.cpp
/// @brief 图层配置弹窗实现

#include "UILayerDialog.h"

#include <QPushButton>
#include <QMessageBox>

#include "DmLayer.h"
#include "DmLayerTable.h"

/// @brief 图层设置
UILayerDialog::UILayerDialog(QWidget* parent, QString name, bool modal, Qt::WindowFlags fl)
	: QDialog(parent, fl)
{
	setModal(modal);
	setObjectName(name);
	setupUi(this);

	this->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
	this->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));

	init();
}

void UILayerDialog::languageChange()
{
	retranslateUi(this);
}

void UILayerDialog::setLayer(DmLayer* l)
{
	m_pLayer = l;
	m_strLayerName = m_pLayer->getName();
	leName->setText(m_strLayerName);
	wPen->setPen(m_pLayer->getPen(), false, tr("Default Pen"));

	if (m_pLayer->getName() == "0")
	{
		leName->setEnabled(false);
	}
}

void UILayerDialog::updateLayer()
{
	m_pLayer->setName(leName->text());
	m_pLayer->setPen(wPen->getPen());
}

void UILayerDialog::validate()
{
	if (m_pLayerTable && (m_isEditLayer == false || m_strLayerName != leName->text()))
	{
		const DmLayer* l = m_pLayerTable->find(leName->text());
		if (l)
		{
			QMessageBox::information(parentWidget(), QMessageBox::tr("Layer Properties"),
				QMessageBox::tr("Layer with a name \"%1\" ""already exists. Please specify a different name.").arg(leName->text()),
				QMessageBox::Ok);
			leName->setFocus();
			leName->selectAll();
		}
		else
		{
			accept();
		}
	}
	else
	{
		accept();
	}
}

void UILayerDialog::setLayerTable(DmLayerTable* layerTable)
{
	m_pLayerTable = layerTable;
}

void UILayerDialog::init()
{
	leName->setFocus();
	m_pLayer = nullptr;
	m_pLayerTable = nullptr;
	m_strLayerName = QString();
	m_isEditLayer = false;
}

void UILayerDialog::setEditLayer(bool el)
{
	m_isEditLayer = el;
}

QLineEdit* UILayerDialog::getQLineEdit()
{
	return leName;
}
