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

/// @file DmUnits.h
/// @brief 单位转换工具类

#ifndef DMUNITS_H
#define DMUNITS_H

#include "Datamodel.h"

class DmVector;
class QString;

/// @brief 单位转换类
class DmUnits
{
public:
    static DM::Unit dxfint2unit(int dxfint);

    static QString unitToString(DM::Unit u, bool t = true);
    static DM::Unit stringToUnit(const QString& u);

    static bool isMetric(DM::Unit u);
    static double getFactorToMM(DM::Unit u);
    static double convert(double val, DM::Unit src, DM::Unit dest);
    static DmVector convert(const DmVector& val, DM::Unit src, DM::Unit dest);

    static QString unitToSign(DM::Unit u);

    static QString formatLinear(double length, DM::Unit unit, DM::LinearFormat format, int prec, bool showUnit = false);
    static QString formatScientific(double length, DM::Unit unit, int prec, bool showUnit = false);
    static QString formatDecimal(double length, DM::Unit unit, int prec, bool showUnit = false);
    static QString formatEngineering(double length, DM::Unit unit, int prec, bool showUnit = false);
    static QString formatArchitectural(double length, DM::Unit unit, int prec, bool showUnit = false);
    static QString formatFractional(double length, DM::Unit unit, int prec, bool showUnit = false);
    static QString formatArchitecturalMetric(double length, DM::Unit unit, int prec, bool showUnit = false);

    static QString formatAngle(double angle, DM::AngleFormat format, int prec);
    static DM::AngleFormat numberToAngleFormat(int num);

    static DmVector paperFormatToSize(DM::PaperFormat p);
    static DM::PaperFormat paperSizeToFormat(const DmVector& s);

    static QString paperFormatToString(DM::PaperFormat p);
    static DM::PaperFormat stringToPaperFormat(const QString& p);

    static double dpiToScale(double dpi, DM::Unit unit);
    static double scaleToDpi(double scale, DM::Unit unit);
};

#endif
