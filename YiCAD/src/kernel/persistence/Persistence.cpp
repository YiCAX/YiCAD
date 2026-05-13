#include "Persistence.h"

#include <cassert>

#include "Base64.h"
#include "Reader.h"
#include "Writer.h"
#include <Tools.h>
//**************************************************************************
// Construction/Destruction
TYPESYSTEM_SOURCE_ABSTRACT(Persistence, MetaType, 0) //use abstract macro means this class cannot be initialized independent

//**************************************************************************
// separator for other implementation aspects
unsigned int Persistence::getMemSize () const
{
    // you have to implement this method in all descending classes!
    assert(0);
    return 0;
}

void Persistence::saveXML (Writer &/*writer*/) const
{
    // you have to implement this method in all descending classes!
    assert(0);
}

void Persistence::restoreXML(XMLReader &/*reader*/)
{
    // you have to implement this method in all descending classes!
    assert(0);
}

void Persistence::saveStream (OutputStream& /*str*/) const
{
}

void Persistence::restoreStream(InputStream& rdr, const std::vector<PAIR>& revs)
{
}

void Persistence::restoreStream(InputStream& rdr)
{
}

void Persistence::restoreStreamWithRev(InputStream& rdr, int rev)
{
}

std::string Persistence::encodeAttribute(const std::string& str)
{
    std::string tmp;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (*it == '<')
            tmp += "&lt;";
        else if (*it == '\"')
            tmp += "&quot;";
        else if (*it == '\'')
            tmp += "&apos;";
        else if (*it == '&')
            tmp += "&amp;";
        else if (*it == '>')
            tmp += "&gt;";
        else if (*it == '\r')
            tmp += "&#13;";
        else if (*it == '\n')
            tmp += "&#10;";
        else if (*it == '\t')
            tmp += "&#9;";
        else
            tmp += *it;
    }

    return tmp;
}

void Persistence::dumpToStream(std::ostream& stream, int compression)
{
    //we need to close the zipstream to get a good result, the only way to do this is to delete the ZipWriter.
    //Hence the scope...
    {
        //create the writer
        ZipWriter writer(stream);
        writer.setLevel(compression);
        writer.putNextEntry("Persistence.xml");
        writer.setMode("BinaryBrep");

        //save the content (we need to encapsulte it with xml tags to be able to read single element xmls like happen for properties)
        writer.Stream() << "<Content>" << std::endl;
        OutputStream str(writer.Stream());
        saveStream(str);
        writer.Stream() << "</Content>";
        writer.writeFiles();
    }
}

void Persistence::restoreFromStream(std::istream& stream)
{
    zipios::ZipInputStream zipstream(stream);
    XMLReader reader("", zipstream);

    if (!reader.isValid())
        throw OneException("Unable to construct reader");

    reader.readElement("Content");
    InputStream str(zipstream);
    restoreStream(str);
    reader.readFiles(zipstream);
    restoreFinished();
}

int Persistence::getRevisionId(const std::string& type, const std::vector<PAIR>& revs)
{
    for (auto& rev : revs)
    {
        if (rev.first == type)
        {
            return rev.second;
        }
    }

    return 0;
}

std::string Persistence::encode(const std::string& str) const
{
    if (isBase64)
    {
        return PersistentTools::base64_encode(reinterpret_cast<unsigned char const*>(str.c_str()), str.length());
    }
    else
    {
        return str;
    }
}

std::string Persistence::decode(const std::string& str) const
{
    if (isBase64)
    {
        return PersistentTools::base64_decode(str);
    }
    else
    {
        return str;
    }
}
