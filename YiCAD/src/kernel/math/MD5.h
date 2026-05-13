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

/// @file MD5.h
/// @brief MD5哈希值计算类，参考：https://baike.baidu.com/item/MD5/212708?fr=aladdin

#ifndef MD5_H
#define MD5_H

#include <string>

/// @brief 计算MD5值
class MD5
{
public:
    /// @brief 计算字符串的MD5值
    /// @param [in] source 原始字符串
    /// @return MD5哈希值字符串
    static std::string getMD5(const std::string& source);

private:
    /// @brief MD5主循环
    /// @param [in] M 512位消息块拆分的16个32位字
    static void mainLoop(unsigned int M[]);

    /*
     * 填充函数
     * 处理后应满足bits≡448(mod512),字节就是bytes≡56（mode64)
     * 填充方式为先加一个1,其它位补零
     * 最后加上64位的原来长度
     */
    /// @brief 填充函数
    /// @param [in] str 原始字符串
    /// @return 填充后的消息块数组指针
    static unsigned int* add(std::string str);

    /// @brief 将整数转为十六进制字符串
    /// @param [in] a 整数值
    /// @return 十六进制字符串
    static std::string changeHex(int a);

private:
    static unsigned int strlength;       ///< strByte的长度
    static unsigned int atemp;           ///< A的临时变量
    static unsigned int btemp;           ///< B的临时变量
    static unsigned int ctemp;           ///< C的临时变量
    static unsigned int dtemp;           ///< D的临时变量
    static const unsigned int k[];       ///< 常量ti unsigned int(abs(sin(i+1))*(2pow32))
    static const unsigned int s[];       ///< 向左位移数
    static const char str16[];           ///< 十六进制字符表
};

#endif //MD5_H
