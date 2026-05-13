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

/// @file DmFontList.h
/// @brief 全局字体列表（单例模式），管理字体文件的加载与检索

#ifndef DMFONTLIST_H
#define DMFONTLIST_H
#include <QString>
#include <memory>
#include <vector>
#include <map>
class DmFont;

/// @brief 全局字体列表。实现为单例，使用 DmFontList::instance() 获取实例
class DmFontList
{
public:
    /// @brief 获取字体列表唯一实例
    /// @return 字体列表实例指针
    static DmFontList* instance();

    /// @brief 析构函数
    virtual ~DmFontList();

    /// @brief 删除字体列表实例
    void deleteFontList();

    /// @brief 读取所有字体文件的头
    void readAllFontFiles();

    /// @brief 获得指定字体的族名（对于shx字体返回字体文件名）
    /// @param font 字体指针
    /// @param isBold 输出：是否粗体
    /// @param isItalic 输出：是否斜体
    /// @return 族名
    QString getFontFamilyName(const DmFont* font, bool& isBold, bool& isItalic);

    /// @brief 获得系统字体映射的常量引用
    /// @return 字体映射的常量引用
    const std::map<QString, std::vector<DmFont*>>& getSysFontsMapConstRef() const;

    /// @brief 根据输入获得系统文字样式名
    /// @param isBold 是否粗体
    /// @param isItalic 是否斜体
    /// @return 常规/斜体/粗体/粗斜体之一
    const QString getSysFontStyleName(const bool isBold, const bool isItalic) const;

    /// @brief 清空所有字体
    void clearFonts();

    /// @brief 字体数量
    /// @return 字体数量
    size_t countFonts() const;

    /// @brief 获得指定文件名的字体
    /// @param fileName 文件名（须带上后缀）
    /// @param getDefaultIfNotFound 如果未找到，是否使用默认字体
    /// @return 字体指针
    DmFont* requestFont(const QString& fileName, bool getDefaultIfNotFound = true);

    /// @brief 获得指定名字及样式的系统字体
    /// @param family 族名
    /// @param isBlod 是否粗体
    /// @param isItalic 是否斜体
    /// @return 字体指针
    DmFont* requestSysFont(const QString& family, bool isBlod, bool isItalic);

    /// @brief 获得近似的匹配系统字体
    /// @param family 族名
    /// @param isBlod 输入输出：是否粗体
    /// @param isItalic 输入输出：是否斜体
    /// @return 近似匹配的字体指针
    DmFont* requestSysFontCloset(const QString& family, bool& isBlod, bool& isItalic);

    /// @brief 获得近似的匹配字体（对于shx获得名字匹配的）
    /// @param family 族名
    /// @param isBold 输入输出：是否粗体
    /// @param isItalic 输入输出：是否斜体
    /// @return 近似匹配的字体指针
    DmFont* requestFontCloset(const QString& family, bool& isBold, bool& isItalic);

    /// @brief 系统字体族名（未翻译）-翻译的映射
    /// @return 映射表常量引用
    const std::map<QString, QString>& FamilyToTranslateMap();

    /// @brief 系统字体族名（已翻译）-未翻译的映射
    /// @return 映射表常量引用
    const std::map<QString, QString>& TranslateToFamilyMap();

    /// @brief 迭代器 begin
    std::vector<DmFont*>::const_iterator begin() const;

    /// @brief 迭代器 end
    std::vector<DmFont*>::const_iterator end() const;

private:
    DmFontList();
    DmFontList(DmFontList const&) = delete;
    DmFontList& operator=(DmFontList const&) = delete;

private:
    /// @brief 获得系统字体索引
    int getSysFontIdx(bool isBold, bool isItalic) const;

    /// @brief 通过索引获得系统字体样式
    void getSysFontStyleByIdx(int idx, bool& isBold, bool& isItalic) const;

private:
    static DmFontList* m_pUniqueInstance;
    std::map<QString, QString> s_familyToTranslateMap; ///< 系统字体族名（未翻译）-翻译的映射
    std::map<QString, QString> s_translateToFamilyMap; ///< 系统字体族名（已翻译）-未翻译的映射
    std::vector<DmFont*> m_fonts;                      ///< fonts in the document
    std::map<QString, std::vector<DmFont*>> m_sysFontsMap; ///< 系统字体族
    bool m_bHasReadAll{false}; ///< 是否读取了所有字体文件
};


/// @brief 全局字体列表访问宏
#define DMFONTLIST DmFontList::instance()

#endif
