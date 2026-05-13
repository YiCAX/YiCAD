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

/// @file DmFlags.h
/// @brief 标志位基类，提供位标志操作及 hash 支持

#ifndef DMFLAGS_H
#define DMFLAGS_H

#include <Stream.h>
#include <Persistence.h>

#include <cstddef>
#include <functional>

inline void hash_combine(std::size_t& seed) {}

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    int i[] = {0, (hash_combine(seed, std::forward<Rest>(rest)), 0)...};
    (void)(i);
}

#define MAKE_HASHABLE(type, ...) \
    namespace std { \
        template<> struct hash<type> { \
            std::size_t operator()(const type &t) const { \
                std::size_t ret = 0; \
                hash_combine(ret, __VA_ARGS__); \
                return ret; \
            } \
        }; \
    }

// Base class for objects which have flags.
class DmFlags: public Persistence
{
    TYPESYSTEM_HEADER();

public:
    // Constructor with initialisation to the given flags.
    // Default sets all flags to 0
    DmFlags(unsigned f = 0) : flags(f) {};

    virtual ~DmFlags() = default;

    unsigned getFlags() const { return flags; };

    void resetFlags() { flags = 0; }

    void setFlags(unsigned f) { flags = f; }

    void setFlag(unsigned f) { flags |= f; }

    void delFlag(unsigned f) { flags &= ~f; }

    void toggleFlag(unsigned f) { flags ^= f; }

    bool getFlag(unsigned f) const { return flags & f; }

    // persistent helper
    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

private:
    unsigned flags = 0;
};

MAKE_HASHABLE(DmFlags, t.getFlags());

#endif // DMFLAGS_H
