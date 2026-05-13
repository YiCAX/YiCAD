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

/// @file UIActionGroupManager.h
/// @brief 操作分组管理器，管理工具栏中各类操作的QActionGroup

#ifndef UIACTIONGROUPMANAGER_H
#define UIACTIONGROUPMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>

class QActionGroup;
class QAction;

class UIActionGroupManager : public QObject
{
    Q_OBJECT

public:
    explicit UIActionGroupManager(QObject* parent);

    QActionGroup* block = nullptr;
    QActionGroup* circle = nullptr;
    QActionGroup* curve = nullptr;
    QActionGroup* edit = nullptr;
    QActionGroup* ellipse = nullptr;
    QActionGroup* file = nullptr;
    QActionGroup* dimension = nullptr;
    QActionGroup* info = nullptr;
    QActionGroup* layer = nullptr;
    QActionGroup* line = nullptr;
    QActionGroup* modify = nullptr;
    QActionGroup* options = nullptr;
    QActionGroup* other = nullptr;
    QActionGroup* polyline = nullptr;
    QActionGroup* restriction = nullptr;
    QActionGroup* select = nullptr;
    QActionGroup* snap = nullptr;
    QActionGroup* snap_extras = nullptr;
    QActionGroup* view = nullptr;
    QActionGroup* widgets = nullptr;

    QList<QActionGroup*> toolGroups();
    QMap<QString, QActionGroup*> allGroups();
    void sortGroupsByName(QList<QActionGroup*>& list);

public slots:
    void toggleExclusiveSnapMode(bool state);
    void toggleTools(bool state);

private:
    QList<bool> snap_memory;

};

#endif // UIACTIONGROUPMANAGER_H
