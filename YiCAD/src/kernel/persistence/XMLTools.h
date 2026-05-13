#ifndef XML_TOOLS_H
#define XML_TOOLS_H

#include <memory>
#include <iostream>
#include <xercesc/util/TransService.hpp>

/// <summary>
/// Helper class for wrapping Xerces, the third party lib
/// </summary>
XERCES_CPP_NAMESPACE_BEGIN
    class DOMNode;
    class DOMElement;
    class DOMDocument;
XERCES_CPP_NAMESPACE_END

// Helper class
class XMLTools
{
public:
    static std::string toStdString(const XMLCh* const toTranscode);
    static std::basic_string<XMLCh> toXMLString(const char* const fromTranscode);
    static void initialize();
    static void terminate();

private:
    static std::unique_ptr<XERCES_CPP_NAMESPACE::XMLTranscoder> transcoder;
};

//**************************************************************************
//**************************************************************************
// StrXLocal
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class StrX
{
public :
    StrX(const XMLCh* const toTranscode);
    ~StrX();

    /// Getter method
    const char* c_str() const;

private :
    //  This is the local code page form of the string.
    char*   fLocalForm;
};

inline std::ostream& operator<<(std::ostream& target, const StrX& toDump)
{
    target << toDump.c_str();
    return target;
}

inline StrX::StrX(const XMLCh* const toTranscode)
{
    // Call the private transcoding method
    fLocalForm = XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode(toTranscode);
}

inline StrX::~StrX()
{
    XERCES_CPP_NAMESPACE_QUALIFIER XMLString::release(&fLocalForm);
}


// -----------------------------------------------------------------------
//  Getter methods
// -----------------------------------------------------------------------
inline const char* StrX::c_str() const
{
    return fLocalForm;
}

//**************************************************************************
//**************************************************************************
// StrXUTF-8
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class StrXUTF8
{
public :
    StrXUTF8(const XMLCh* const toTranscode);

    /// Getter method
    const char* c_str() const;
    /// string which holds the UTF-8 form
    std::string  str;
};

inline std::ostream& operator<<(std::ostream& target, const StrXUTF8& toDump)
{
    target << toDump.c_str();
    return target;
}

inline StrXUTF8::StrXUTF8(const XMLCh* const toTranscode)
{
    str = XMLTools::toStdString(toTranscode);
}

// -----------------------------------------------------------------------
//  Getter methods
// -----------------------------------------------------------------------
inline const char* StrXUTF8::c_str() const
{
    return str.c_str();
}


//**************************************************************************
//**************************************************************************
// XStr
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class XStr
{
public :
    ///  Constructors and Destructor
    XStr(const char* const toTranscode);
    ///
    ~XStr();


    ///  Getter method
    const XMLCh* unicodeForm() const;

private :
    /// This is the Unicode XMLCh format of the string.
    XMLCh*   fUnicodeForm;
};


inline XStr::XStr(const char* const toTranscode)
{
    fUnicodeForm = XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode(toTranscode);
}

inline XStr::~XStr()
{
    XERCES_CPP_NAMESPACE_QUALIFIER XMLString::release(&fUnicodeForm);
}


// -----------------------------------------------------------------------
//  Getter methods
// -----------------------------------------------------------------------
inline const XMLCh* XStr::unicodeForm() const
{
    return fUnicodeForm;
}

//**************************************************************************
//**************************************************************************
// XUTF-8Str
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class XUTF8Str
{
public :
    XUTF8Str(const char* const fromTranscode);
    ~XUTF8Str();

    /// Getter method
    const XMLCh* unicodeForm() const;

private :
    std::basic_string<XMLCh>  str;
};

inline XUTF8Str::XUTF8Str(const char* const fromTranscode)
{
    str = XMLTools::toXMLString(fromTranscode);
}

inline XUTF8Str::~XUTF8Str()
{
}


// -----------------------------------------------------------------------
//  Getter methods
// -----------------------------------------------------------------------
inline const XMLCh* XUTF8Str::unicodeForm() const
{
    return str.c_str();
}

#endif // XML_TOOLS_H
