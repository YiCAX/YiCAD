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

/// @file ActionDefineAttributes.cpp
/// @brief 定义属性操作类实现文件

#include "ActionDefineAttributes.h"

#include <QMouseEvent>

#include "DmAttributeDefinition.h"
#include "GuiCommandEvent.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "DmDocument.h"
#include "DmLine.h"
#include "Preview.h"
#include "Transaction.h"

ActionDefineAttributes::ActionDefineAttributes(DmDocument* doc, GuiDocumentView* docView)
	: PreviewActionInterface("Define Attributes", doc, docView)
	, pPoints(new Points{})
	, textChanged(true)
{
	actionType = DM::ActionDefineAttributes;
	reset();
}

void ActionDefineAttributes::init(int status)
{
	ActionInterface::init(status);

	switch (status)
	{
	case ShowDialog:
	{
		reset();

		DmAttributeDefinition tmp(nullptr, *textData, *attrData);
		tmp.setDocument(pDocument);

		if (GUIDIALOGFACTORY->requestDefineAttributesDialog(&tmp))
		{
			textData.reset(new TextData(tmp.getData()));
			attrData.reset(new AttributeDefinitionData(tmp.getAttributeData()));
			setStatus(SetPos);
		}
		else
		{
			setFinished();
		}
	}
	break;

	case SetPos:
		deletePreview();
		preview->setVisible(true);
		preparePreview();
		break;

	default:
		break;
	}
}

void ActionDefineAttributes::reset()
{
	const QString text = textData.get() ? textData->getTextString() : "";
	DmTextStyle* pStyle = pDocument->getTextStyleTable()->getActive();
	textData.reset(new TextData(
		DmVector(0.0, 0.0), 1.0,
		ETextVertMode::kTextBase, ETextHorzMode::kTextLeft,
		text, pStyle, 0.0, EUpdateMode::Update));
	attrData.reset(new AttributeDefinitionData("", ""));
}

void ActionDefineAttributes::trigger()
{
	if (pPoints->pos.valid)
	{
		deletePreview();

		Transaction t(tr("Create Attributes").toStdString(), pDocument);
		t.start();

		DmAttributeDefinition* text =
			new DmAttributeDefinition(nullptr, *textData, *attrData);
		text->setDocument(pDocument);
		text->update();
		pDocument->getEntityTable()->add(text);

		t.commit();

		textChanged = true;
		pPoints->secPos = {};
		setFinished();
	}
}

void ActionDefineAttributes::preparePreview()
{
	ETextMode mode = textData->getTextMode();

	if (mode != ETextMode::kTextAligned && mode != ETextMode::kTextFit)
	{
		setDataWithOnePoint();

		DmAttributeDefinition* text =
			new DmAttributeDefinition(nullptr, *textData, *attrData);
		text->update();
		preview->addEntity(text);
		textChanged = false;
	}
}

void ActionDefineAttributes::mouseMoveEvent(QMouseEvent* e)
{
	if (getStatus() == SetPos)
	{
		DmVector mouse = snapPoint(e);
		DmVector mov = mouse - pPoints->pos;
		pPoints->pos = mouse;

		ETextMode mode = textData->getTextMode();

		if (mode != ETextMode::kTextAligned && mode != ETextMode::kTextFit)
		{
			if (textChanged || pPoints->pos.valid == false || preview->isEmpty())
			{
				deletePreview();
				preparePreview();
			}
			else
			{
				preview->move(mov);
			}

			preview->setVisible(true);
		}

		drawPreview();
	}
	else if (getStatus() == SetSecPos)
	{
		pPoints->secPos = snapPoint(e);
		deletePreview();
		preview->appendEntity(
			new DmLine(nullptr, LineData(pPoints->pos, pPoints->secPos)));
		drawPreview();
	}
}

void ActionDefineAttributes::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		GuiCoordinateEvent ce(snapPoint(e));
		coordinateEvent(&ce);
	}
	else if (e->button() == Qt::RightButton)
	{
		deletePreview();
		finish(false);
	}
}

void ActionDefineAttributes::coordinateEvent(GuiCoordinateEvent* e)
{
	if (e == nullptr)
	{
		return;
	}

	DmVector mouse = e->getCoordinate();

	switch (getStatus())
	{
	case ShowDialog:
		break;

	case SetPos:
	{
		textData->setAlignment(mouse);

		// 对齐和布满需要选择2个点
		ETextMode mode = textData->getTextMode();

		if (mode == ETextMode::kTextAligned || mode == ETextMode::kTextFit)
		{
			textData->setAlignment(mouse);
			setStatus(SetSecPos);
		}
		else
		{
			setDataWithOnePoint();
			trigger();
		}
	}
	break;

	case SetSecPos:
	{
		DmVector tmp = textData->getAlignment();
		textData->setAlignment(mouse);
		textData->setPosition(tmp);
		trigger();
	}
	break;

	default:
		break;
	}
}

void ActionDefineAttributes::commandEvent(GuiCommandEvent* e)
{
	QString c = e->getCommand().toLower();

	if (checkCommand("help", c))
	{
		GUIDIALOGFACTORY->commandMessage(
			msgAvailableCommands() + getAvailableCommands().join(", "));
		return;
	}
}

QStringList ActionDefineAttributes::getAvailableCommands()
{
	QStringList cmd;

	if (getStatus() == SetPos)
	{
		cmd += command("text");
	}

	return cmd;
}

void ActionDefineAttributes::updateMouseButtonHints()
{
	switch (getStatus())
	{
	case SetPos:
		GUIDIALOGFACTORY->updateMouseWidget(
			tr("Specify insertion point"), tr("Cancel"));
		break;

	case SetSecPos:
		GUIDIALOGFACTORY->updateMouseWidget(
			tr("Specify second point"), tr("Cancel"));
		break;

	case ShowDialog:
		GUIDIALOGFACTORY->updateMouseWidget(
			tr("Enter attribute definition data:"), tr("Back"));
		break;

	default:
		GUIDIALOGFACTORY->updateMouseWidget();
		break;
	}
}

void ActionDefineAttributes::updateMouseCursor()
{
	docView->setMouseCursor(DM::CadCursor);
}

void ActionDefineAttributes::setDataWithOnePoint()
{
	ETextMode mode = textData->getTextMode();

	if (mode != ETextMode::kTextAligned && mode != ETextMode::kTextFit)
	{
		if (mode == ETextMode::kTextLeft)
		{
			// 左对齐的对齐点为0，位置随Position
			textData->setPosition(pPoints->pos);
			textData->setAlignment(DmVector(0.0, 0.0));
		}
		else
		{
			textData->setAlignment(pPoints->pos);
		}
	}
}
