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

/// @file UISplineOptions.cpp
/// @brief 样条线选项控件实现

#include "UISplineOptions.h"

#include "ActionDrawSpline.h"
#include "ActionDrawSplinePoints.h"
#include "DmSettings.h"
#include "ui_UISplineOptions.h"
#include "Debug.h"

namespace
{
    constexpr int DEFAULT_SPLINE_DEGREE = 3;
}

/// @brief 构造样条线选项控件
/// @param parent 父窗口指针
/// @param fl 窗口标志
UISplineOptions::UISplineOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, action(nullptr)
	, ui(new Ui::Ui_SplineOptions{})
{
	ui->setupUi(this);
}

/// @brief 析构函数，保存设置后释放资源
UISplineOptions::~UISplineOptions()
{
	saveSettings();
}

/// @brief 根据当前语言刷新子控件字符串
void UISplineOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UISplineOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Draw");
	if (action)
	{
		if (action->getEntityType() == DM::ActionDrawSpline)
		{
			DMSETTINGS->writeEntry("/SplineDegree", ui->cbDegree->currentText().toInt());
		}
		DMSETTINGS->writeEntry("/SplineClosed", (int)ui->cbClosed->isChecked());
	}
	DMSETTINGS->endGroup();
}

void UISplineOptions::setAction(ActionInterface* a, bool update)
{
	if (a->getEntityType() != DM::ActionDrawSpline && a->getEntityType() != DM::ActionDrawSplinePoints)
	{
		action = nullptr;
		return;
	}

	action = a;
	int degree = DEFAULT_SPLINE_DEGREE;
	bool closed = false;

	if (update)
	{
		if (a->getEntityType() == DM::ActionDrawSpline)
		{
			ActionDrawSpline* splineAction = static_cast<ActionDrawSpline*>(action);
			degree = splineAction->getDegree();
			closed = splineAction->isClosed();
		}
		else
		{
			ActionDrawSplinePoints* splinePointsAction = static_cast<ActionDrawSplinePoints*>(action);
			closed = splinePointsAction->isClosed();
		}
	}
	else
	{
		DMSETTINGS->beginGroup("/Draw");
		if (a->getEntityType() == DM::ActionDrawSpline)
		{
			ActionDrawSpline* splineAction = static_cast<ActionDrawSpline*>(action);
			degree = DMSETTINGS->readNumEntry("/SplineDegree", DEFAULT_SPLINE_DEGREE);
			splineAction->setDegree(degree);
			closed = DMSETTINGS->readNumEntry("/SplineClosed", 0);
			splineAction->setClosed(closed);
		}
		else
		{
			ActionDrawSplinePoints* splinePointsAction = static_cast<ActionDrawSplinePoints*>(action);
			closed = DMSETTINGS->readNumEntry("/SplineClosed", 0);
			splinePointsAction->setClosed(closed);
		}

		DMSETTINGS->endGroup();
	}
	if (a->getEntityType() == DM::ActionDrawSpline)
	{
		ui->cbDegree->setCurrentIndex(ui->cbDegree->findText(QString::number(degree)));
		ui->lDegree->show();
		ui->cbDegree->show();
	}
	else
	{
		ui->lDegree->hide();
		ui->cbDegree->hide();
	}
	ui->cbClosed->setChecked(closed);
}

void UISplineOptions::setClosed(bool c)
{
	if (!action)
	{
		return;
	}
	if (action->getEntityType() == DM::ActionDrawSpline)
	{
		ActionDrawSpline* splineAction = static_cast<ActionDrawSpline*>(action);
		splineAction->setClosed(c);
	}
	else
	{
		ActionDrawSplinePoints* splinePointsAction = static_cast<ActionDrawSplinePoints*>(action);
		splinePointsAction->setClosed(c);
	}
}

void UISplineOptions::undo()
{
	if (!action)
	{
		return;
	}
	if (action->getEntityType() == DM::ActionDrawSpline)
	{
		ActionDrawSpline* splineAction = static_cast<ActionDrawSpline*>(action);
		splineAction->undo();
	}
	else
	{
		ActionDrawSplinePoints* splinePointsAction = static_cast<ActionDrawSplinePoints*>(action);
		splinePointsAction->undo();
	}
}

void UISplineOptions::setDegree(const QString& deg)
{
	if (!action)
	{
		return;
	}
	if (action->getEntityType() == DM::ActionDrawSpline)
	{
		ActionDrawSpline* splineAction = static_cast<ActionDrawSpline*>(action);
		splineAction->setDegree(deg.toInt());
	}
}
