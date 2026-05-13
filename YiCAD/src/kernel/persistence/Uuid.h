#ifndef UUID_TOOL_H
#define UUID_TOOL_H
#include <string>

/** Base class for creating uuids
* 
 */
class Uuid
{
public:
    /// Construction
    Uuid();
    Uuid(const Uuid&) = default;
    Uuid& operator=(const Uuid&) = default;
    /// Destruction
    virtual ~Uuid();

    void setValue(const char* sString);
    void setValue(const std::string &sString);
    const std::string& getValue() const;
    static std::string createUuid();

    bool operator==(const Uuid &other) const {return _uuid == other._uuid;}
    bool operator<(const Uuid &other) const {return _uuid < other._uuid;}

private:
    std::string _uuid;
};

#endif // UUID_TOOL_H

