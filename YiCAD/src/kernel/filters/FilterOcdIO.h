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

/// @file FilterOcdIO.h
/// @brief OCD文件读写类头文件

#ifndef FILTER_OCD_IO_H
#define FILTER_OCD_IO_H

#include "FilterInterface.h"

class DmDocument;

class OutputStream;
class InputStream;

class MetaLineTypesContainer;
class MetaLayersContainer;
class MetaViewportsContainer;
class MetaTextStylesContainer;
class MetaDimensionStylesContainer;
class MetaTableStylesContainer;
class MetaLinesContainer;
class MetaCirclesContainer;
class MetaArcsContainer;
class MetaPointsContainer;
class MetaEllipsesContainer;
class MetaRaysContainer;
class MetaXlinesContainer;
class MetaSolidsContainer;
class MetaTrianglesContainer;
class MetaPolylinesContainer;
class MetaSplinesContainer;
class MetaBlockTableRecordsContainer;
class MetaBlcokReferencesContainer;
class MetaTextsContainer;
class MetaMTextsContainer;
class MetaAttributeDefinitionsContainer;
class MetaAttributesContainer;
class MetaDimLinearsContainer;
class MetaDimAlignedsContainer;
class MetaDimAngularsContainer;
class MetaDimRadialsContainer;
class MetaDimDiametricsContainer;
class MetaDimLeadersContainer;
class MetaHatchsContainer;
class MetaArrayRectContainer;
class MetaArrayPolarContainer;

/// @brief OCD文件读写
class FilterOcdIO : public FilterInterface
{
public:
    FilterOcdIO();
    ~FilterOcdIO();

    /// @brief 判断类型是否支持导入
    bool canImport(const QString& t) const override;

    /// @brief 判断类型是否支持导出
    bool canExport(const QString& t) const override;

    /// @brief 导入ycd
    bool fileImport(DmDocument& g, const QString& file) override;
    bool fileExport(DmDocument& g, const QString& file, const QString& version) override;

    /// @brief 导出ycd
    void saveXML(Writer& writer);
    /// @brief 导入ycd
    void restoreXML(XMLReader& reader);

    // ======================== save ========================
    void saveLineTypes(Writer& writer);
    void saveLayers(Writer& writer);
    void saveTextStyles(Writer& writer);
    void saveDimStyles(Writer& writer);
    void saveBlockTableRecords(Writer& writer);
    void saveEntities(Writer& writer);

    // ====================== restore =======================
    void restoreLineTypes(XMLReader& reader);
    void restoreLayers(XMLReader& reader);
    void restoreTextStyles(XMLReader& reader);
    void restoreDimStyles(XMLReader& reader);
    void restoreBlockTableRecords(XMLReader& reader);
    void restoreEntities(XMLReader& reader);

    static FilterInterface* createFilter();

private:
    /// @brief 初始化导入导出
    void initPersist(const DmDocument& document);

private:
	DmDocument*                                          m_pDocument;                 // 当前绘图区域

    std::shared_ptr<MetaLineTypesContainer>		        m_spPersistLineTypes;
    std::shared_ptr<MetaLayersContainer>		        m_spPersistLayers;
    std::shared_ptr<MetaViewportsContainer>             m_spPersistViewports;
    std::shared_ptr<MetaTextStylesContainer>            m_spPersistTextStyles;
    std::shared_ptr<MetaDimensionStylesContainer>       m_spPersistDimensionStyles;
    std::shared_ptr<MetaTableStylesContainer>           m_spPersistTableStyles;
    std::shared_ptr<MetaLinesContainer>			        m_spPersistLines;
    std::shared_ptr<MetaCirclesContainer>		        m_spPersistCircles;
    std::shared_ptr<MetaArcsContainer>			        m_spPersistArcs;
    std::shared_ptr<MetaPointsContainer>		        m_spPersistPoints;
    std::shared_ptr<MetaEllipsesContainer>		        m_spPersistEllipses;
    std::shared_ptr<MetaRaysContainer>			        m_spPersistRays;
    std::shared_ptr<MetaXlinesContainer>		        m_spPersistXlines;
    std::shared_ptr<MetaSolidsContainer>		        m_spPersistSolids;
    std::shared_ptr<MetaTrianglesContainer>             m_spPersistTriangles;
    std::shared_ptr<MetaPolylinesContainer>		        m_spPersistPolylines;
    std::shared_ptr<MetaSplinesContainer>		        m_spPersistSplines;
    std::shared_ptr<MetaBlockTableRecordsContainer>     m_spPersistBlockTableRecords;
    std::shared_ptr<MetaBlcokReferencesContainer>       m_spPersistBlcokReferences;
    std::shared_ptr<MetaTextsContainer>                 m_spPersistTexts;
    std::shared_ptr<MetaMTextsContainer>                 m_spPersistMTexts;
    std::shared_ptr<MetaAttributeDefinitionsContainer>  m_spPersistAttributeDefinitions;
    std::shared_ptr<MetaAttributesContainer>            m_spPersistAttributes;
    std::shared_ptr<MetaDimLinearsContainer>            m_spPersistDimLinears;
    std::shared_ptr<MetaDimAlignedsContainer>           m_spPersistDimAligneds;
    std::shared_ptr<MetaDimAngularsContainer>           m_spPersistDimAngulars;
    std::shared_ptr<MetaDimRadialsContainer>            m_spPersistDimRadials;
    std::shared_ptr<MetaDimDiametricsContainer>         m_spPersistDimDiametrics;
    std::shared_ptr<MetaDimLeadersContainer>            m_spPersistDimLeaders;
    std::shared_ptr<MetaHatchsContainer>                m_spPersistHatchs;
};

#endif // FILTER_OCD_IO_H
