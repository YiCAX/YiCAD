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

/// @file UIActionHandler.h
/// @brief 操作处理器，负责触发菜单、按钮等CAD操作命令

#ifndef UIACTIONHANDLER_H
#define UIACTIONHANDLER_H

#include "ActionInterface.h"
#include "MDIWindow.h"
#include "QMdiArea"
#include "UITabDrawWidget.h"
#include "UISnapWidget.h"

class UISnapWidget;
class DmLayer;

/// @class UIActionHandler
/// @brief 这个类可以触发操作（菜单、按钮等）
class UIActionHandler : public QObject
{
    Q_OBJECT

public:
    UIActionHandler(QObject* parent);
    virtual ~UIActionHandler() = default;

    ActionInterface* getCurrentAction();
    ActionInterface* setCurrentAction(DM::ActionType id);

    void setSnapToolBar(UISnapWidget* toolbar);
    void setMDIWindow(MDIWindow* m);
    void setMdiArea(QMdiArea* m);
    void setUITabDrawWidget(UITabDrawWidget* tabDrawWidget);

    /// @brief Kills all running selection actions. Called when a selection action is launched to reduce confusion.
    void killSelectActions();
    /// @brief killAllActions kill all actions
    void killAllActions();

    bool keycode(const QString& code);
    // special handling of actions issued from command line, currently used for snap actions
    // return true if handled
    bool commandLineActions(DM::ActionType id);
    bool command(const QString& cmd);
    QStringList getAvailableCommands();
    SnapMode getSnaps();
    DM::SnapRestriction getSnapRestriction();
    void set_view(GuiDocumentView* pDocumentView);
    void set_document(DmDocument* document);
    void redrawAll();
    void updateGrids();

public slots:
    void slotFileNew();
    void slotFileOpen();
    void slotFileSave();
    void slotFileSaveAs();

    void slotFileExportImage();

    void slotZoomIn();
    void slotZoomOut();
    void slotZoomPan();

    void slotEditKillAllActions();
    void slotEditUndo();
    void slotEditRedo();
    void slotEditCut();
    void slotEditCopy();
    void slotEditPaste();

    void slotDrawPoint();
    void slotDrawLine();
    void slotDrawLineFree();
    void slotDrawLineRectangle();
    void slotDrawLineBisector();
    void slotDrawLineTangent1();
    void slotDrawLineTangent2();
    void slotDrawLineOrthTan();
    void slotDrawLinePolygon();
    void slotDrawLinePolygon3();
    void slotDrawCircle();
    void slotDrawCircle2P();
    void slotDrawCircle3P();
    void slotDrawCircleTan2();
    void slotDrawCircleTan3();
    void slotDrawArc();
    void slotDrawArc3P();
    void slotDrawArcTangential();
    void slotDrawEllipseAxis();
    void slotDrawEllipseInscribe();
    void slotDrawRay();
    void slotDrawSpline();
    void slotDrawSplinePoints();
    void slotDrawMText();
    void slotDrawText();
    void slotDrawHatch();
    void slotDrawImage();
    void slotDrawXline();
    void slotDrawPolyline();
    void slotPolylineAdd();
    void slotPolylineAppend();
    void slotPolylineDel();
    void slotCloudLineRectangle();
    void slotCloudLinePolygon();
    void slotCloudLineFree();

    void slotDimAligned();
    void slotDimLinear();
    void slotDimRadial();
    void slotDimDiametric();
    void slotDimAngular();
    void slotDimLeader();
    void slotDimBaseline();
    void slotDimStyle();
    void slotTextStyle();

    void slotModifyDelete();
    void slotModifyDeleteNoSelect();     // 已在Ribbon菜单入口屏蔽此功能
    void slotModifyCopy();
    void slotModifyMove();
    void slotModifyScale();
    void slotModifyRotate();
    void slotModifyMirror();
    void slotModifyEntity();
    void slotModifyTrim();
    void slotModifyExtend();
    void slotModifyCut();
    void slotModifyCut_2P();
    void slotModifyBevel();
    void slotModifyRound();
    void slotModifySingleOffset();
    void slotModifyExplode();

    void slotSetSnaps(SnapMode const& s);
    void slotSnapFree();
    void slotSnapGrid();
    void slotSnapEndpoint();
    void slotSnapOnEntity();
    void slotSnapCenter();
    void slotSnapMiddle();
    void slotSnapIntersection();

    void slotRestrictNothing();
    void slotRestrictOrthogonal();
    void slotRestrictHorizontal();
    void slotRestrictVertical();

    void disableSnaps();
    void disableRestrictions();

    void slotInfoDist();
    void slotInfoAngle();
    void slotInfoTotalLength();
    void slotInfoArea();
    void slotIndoSelected();

    void slotLayersFreeze();
    void slotLayersLock();
    void slotLayersPrint();
    void slotLayersColor();
    void slotLayersActivate();
    void slotLayersDelete();
    void slotLayersDefreezeAll();
    void slotLayersFreezeAll();
    void slotLayersUnlockAll();
    void slotLayersLockAll();
    void slotLayersRename();
    void slotLayersAdd();

    void slotBlocksSave();
    void slotBlocksSaveAs();
    void slotBlocksInsertPrepare();
    void slotBlocksInsert();
    void slotBlocksCreate();
    void slotBlocksDelete();
    void slotBlocksEdit();
    void slotBlocksImport();
    void slotDefineAttributes();
    void slotOptionsGeneral();
    void slotOptionsDrawing();

    void slotViewGrid();
    void slotCopyToLayer();

    void slotSecectedChanged();
private:
    UISnapWidget*       m_pSnapToolbar = nullptr;
    GuiDocumentView*    m_pView = nullptr;
    DmDocument*         m_pDocument = nullptr;
    MDIWindow*          m_pMdiWin = nullptr;
    QMdiArea*           m_pDrawingArea = nullptr;
    UITabDrawWidget*    m_pTabDrawWidget = nullptr;
};

#endif
