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

/// @file UICurrentActivePen.cpp
/// @brief 当前激活画笔控件，集成颜色、线宽和线型选择，修改当前文档和选中实体的笔属性

#include "UICurrentActivePen.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QAbstractItemView>

#include "UILineTypeBox.h"
#include "UIColorBox.h"
#include "UIWidthBox.h"
#include "ApplicationWindow.h"
#include "DmDocument.h"
#include "DocumentCmd.h"
#include "Transaction.h"

UICurrentActivePen::UICurrentActivePen(QWidget* parent)
	: QWidget(parent)
{
 	QLabel* penColor = new QLabel(tr("Color:"), parent);
	penColor->setMinimumWidth(30);
	m_pCurrentColor = new UIColorBox(parent);
	m_pCurrentColor->init(true);

	QLabel* penLineWidth = new QLabel(tr("LineWindth:"), parent);
	penLineWidth->setMinimumWidth(30);
	m_pCurrentWidth = new UIWidthBox(parent);
	m_pCurrentWidth->init(true);

	QLabel* penLineType = new QLabel(tr("LineType:"), parent);
	penLineType->setMinimumWidth(30);
	m_pCurrentLineType = new UILineTypeBox(parent);
	//m_pCurrentLineType->view()->setTextElideMode(Qt::ElideLeft);
	m_pCurrentLineType->init(true);

	QHBoxLayout* pLayout = new QHBoxLayout();
	pLayout->setContentsMargins(10, 0, 0, 0);
	pLayout->setSpacing(10);
	pLayout->addWidget(penColor);
	pLayout->addWidget(m_pCurrentColor);
	pLayout->addWidget(penLineWidth);
	pLayout->addWidget(m_pCurrentWidth);
	pLayout->addWidget(penLineType);
	pLayout->addWidget(m_pCurrentLineType);
	pLayout->setStretch(0, 1);
	pLayout->setStretch(1, 2);
	pLayout->setStretch(2, 1);
	pLayout->setStretch(3, 3);
	pLayout->setStretch(4, 1);
	pLayout->setStretch(5, 3);
	parent->setLayout(pLayout);

	connect(m_pCurrentColor, SIGNAL(colorChanged(const DmColor&)), this, SLOT(slotSelectChanged()));
	connect(m_pCurrentWidth, SIGNAL(widthChanged(DM::LineWidth)), this, SLOT(slotSelectChanged()));
	connect(m_pCurrentLineType, SIGNAL(lineTypeChanged(DmLineType*)), this, SLOT(slotSelectChanged()));
}

UICurrentActivePen::~UICurrentActivePen()
{
}

void UICurrentActivePen::setPen(DmDocument* doc)
{
	auto activePen = doc->getActivePen();
	m_pCurrentColor->setColor(activePen.getColor());
	m_pCurrentWidth->setWidth(activePen.getWidth());
	m_pCurrentLineType->setLineType(activePen.getLineType(), doc);
}

DmLineType* UICurrentActivePen::getLineType()
{
	return m_pCurrentLineType->getLineType();
}

DM::LineWidth UICurrentActivePen::getLineWidth()
{
	return m_pCurrentWidth->getWidth();
}

DmColor UICurrentActivePen::getColor()
{
	return m_pCurrentColor->getColor();
}

void UICurrentActivePen::update(DmDocument* doc)
{
    DmPen pen = doc->getActivePen();
    m_pCurrentColor->setColor(pen.getColor());
    m_pCurrentWidth->setWidth(pen.getWidth());
	m_pCurrentLineType->updateLineTypeTable(doc);
}

void UICurrentActivePen::slotSelectChanged()
{
	//修改当前文档的当前pen
	DmDocument* doc = ApplicationWindow::getAppWindow()->getDocument();
	DmPen pen(getColor(), getLineWidth(), getLineType());
    DmPen originPen = doc->getActivePen();
    
    TransactionGroup tg(tr("Modify current pen").toStdString(), doc);
    tg.start();
    Transaction t(tr("Modify current pen").toStdString(), doc);
    t.start();
    ModifyDocPenCmd* cmd = new ModifyDocPenCmd(doc, pen, originPen);
    doc->getCmdManager()->addAndExecuteCmd(cmd);
    if (sender() == m_pCurrentLineType)
    {
        doc->getLineTypeTable()->activate(m_pCurrentLineType->getLineType());
    }
    t.commit();
    
	//修改当前选择实体的pen
	unsigned int selectedCount = doc->getEntityTable()->countSelect();
	if (selectedCount == 0)
	{
        tg.commit();
		return;
	}

    Transaction t2(tr("Modify current selected entities").toStdString(), doc);
    t2.start();
    EntityTable* entityTable = doc->getEntityTable();
	for (auto& e : *entityTable)
	{
		if (e->isSelected())
		{
            entityTable->startModify(e);
			e->setPen(pen);
			//对于简单实体（比如直线）不用update()。
			//但是对于文字（DmText）这种包含DmBlockReference的实体，里面的DmBlockReference的子实体颜色要从DmText及DmBlock获得，因此要update()
			e->update();
		}
	}
    t2.commit();
    tg.commit();
}
