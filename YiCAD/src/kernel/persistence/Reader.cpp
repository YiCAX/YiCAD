# include <xercesc/sax2/XMLReaderFactory.hpp>

#include <locale>
#include <exception>

#include "Reader.h"
#include "Base64.h"
#include "Persistence.h"
#include "InputSource.h"
#include "Stream.h"
#include "XMLTools.h"
#include <Tools.h>

#ifdef _MSC_VER
#include <zipios++/zipios-config.h>
#endif
#include <zipios++/zipinputstream.h>

XERCES_CPP_NAMESPACE_USE
using namespace std;


// ---------------------------------------------------------------------------
//  XMLReader: Constructors and Destructor
// ---------------------------------------------------------------------------

XMLReader::XMLReader(const char* FileName, std::istream& str)
  : DocumentSchema(0), ProgramVersion(""), FileVersion(0), Level(0),
    CharacterCount(0), ReadType(None), _File(FileName), _valid(false),
    _verbose(true)
{
#ifdef _MSC_VER
    str.imbue(std::locale::empty());
#else
    str.imbue(std::locale::classic());
#endif

    // create the parser
    parser = XMLReaderFactory::createXMLReader();

    parser->setContentHandler(this);
    parser->setLexicalHandler(this);
    parser->setErrorHandler(this);

    try {
        StdInputSource file(str, _File.c_str());
        _valid = parser->parseFirst(file, token);
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
    }
    catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cerr << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
    }
    catch (...) {
        cerr << "Unexpected Exception \n";
    }
}

XMLReader::~XMLReader()
{
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    delete parser;
}

const char* XMLReader::localName() const
{
    return LocalName.c_str();
}

unsigned int XMLReader::getAttributeCount() const
{
    return static_cast<unsigned int>(AttrMap.size());
}

long XMLReader::getAttributeAsInteger(const char* AttrName) const
{
    AttrMapType::const_iterator pos = AttrMap.find(AttrName);

    if (pos != AttrMap.end()) {
        return atol(pos->second.c_str());
    }
    else {
        // wrong name, use hasAttribute if not sure!
        std::ostringstream msg;
        msg << "XML Attribute: \"" << AttrName << "\" not found";
        throw OneException(msg.str().c_str());
    }
}

unsigned long XMLReader::getAttributeAsUnsigned(const char* AttrName) const
{
    AttrMapType::const_iterator pos = AttrMap.find(AttrName);

    if (pos != AttrMap.end()) {
        return strtoul(pos->second.c_str(),nullptr,10);
    }
    else {
        // wrong name, use hasAttribute if not sure!
        std::ostringstream msg;
        msg << "XML Attribute: \"" << AttrName << "\" not found";
        throw OneException(msg.str().c_str());
    }
}

double XMLReader::getAttributeAsFloat  (const char* AttrName) const
{
    AttrMapType::const_iterator pos = AttrMap.find(AttrName);

    if (pos != AttrMap.end()) {
        return atof(pos->second.c_str());
    }
    else {
        // wrong name, use hasAttribute if not sure!
        std::ostringstream msg;
        msg << "XML Attribute: \"" << AttrName << "\" not found";
        throw OneException(msg.str().c_str());
    }
}

const char*  XMLReader::getAttribute (const char* AttrName) const
{
    AttrMapType::const_iterator pos = AttrMap.find(AttrName);

    if (pos != AttrMap.end()) {
        return pos->second.c_str();
    }
    else {
        // wrong name, use hasAttribute if not sure!
        std::ostringstream msg;
        msg << "XML Attribute: \"" << AttrName << "\" not found";
        throw OneException(msg.str().c_str());
    }
}

bool XMLReader::hasAttribute (const char* AttrName) const
{
    return AttrMap.find(AttrName) != AttrMap.end();
}

bool XMLReader::read()
{
    ReadType = None;

	try {
		parser->parseNext(token);
	}
	catch (const XMLException& toCatch) {

		char* message = XMLString::transcode(toCatch.getMessage());
		std::string msg = message;
		XMLString::release(&message);
		throw OneException(msg.c_str());
	}
    catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        std::string msg = message;
        XMLString::release(&message);
        throw OneException(msg.c_str());
    }
    catch (...) {
        throw OneException("Unexpected XML exception");
    }

    return true;
}

void XMLReader::readElement(const char* ElementName)
{
    bool ok;
    int currentLevel = Level;
    std::string currentName = LocalName;
    do {
        ok = read(); if (!ok) break;
        if (ReadType == EndElement && currentName == LocalName && currentLevel >= Level) {
            // we have reached the end of the element when calling this method
            // thus we must stop reading on.
            break;
        }
        else if (ReadType == EndDocument) {
            // the end of the document has been reached but we still try to continue on reading
            throw OneException("End of document reached");
        }
    } while ((ReadType != StartElement && ReadType != StartEndElement) ||
             (ElementName && LocalName != ElementName));
}

int XMLReader::level() const {
    return Level;
}

void XMLReader::readEndElement(const char* ElementName, int level)
{
    // if we are already at the end of the current element
    if (ReadType == EndElement
            && ElementName
            && LocalName == ElementName
            && (level<0 || level==Level))
    {
        return;
    }
    else if (ReadType == EndDocument) {
        // the end of the document has been reached but we still try to continue on reading
        throw OneException("End of document reached");
    }

    bool ok;
    do {
        ok = read(); if (!ok) break;
        if (ReadType == EndDocument)
            break;
    } while (ReadType != EndElement
                || (ElementName
                    && (LocalName != ElementName
                        || (level>=0 && level!=Level))));
}

void XMLReader::readCharacters()
{
}

void XMLReader::readBinFile(const char* filename)
{
    //FileInfo fi(filename);
    Oofstream to(filename, std::ios::out | std::ios::binary);
    if (!to)
        throw OneException("XMLReader::readBinFile() Could not open file!");

    bool ok;
    do {
        ok = read(); if (!ok) break;
    } while (ReadType != EndCDATA);

    to << PersistentTools::base64_decode(Characters);
    to.close();
}

void XMLReader::readFiles(zipios::ZipInputStream &zipstream) const
{
    // It's possible that not all objects inside the document could be created, e.g. if a module
    // is missing that would know these object types. So, there may be data files inside the zip
    // file that cannot be read. We simply ignore these files.
    // On the other hand, however, it could happen that a file should be read that is not part of
    // the zip file. This happens e.g. if a document is written without GUI up but is read with GUI
    // up. In this case the associated GUI document asks for its file which is not part of the ZIP
    // file, then.
    // In either case it's guaranteed that the order of the files is kept.
    zipios::ConstEntryPointer entry;
    try {
        entry = zipstream.getNextEntry();
    }
    catch (const std::exception&) {
        // There is no further file at all. This can happen if the
        // project file was created without GUI
        return;
    }
    std::vector<FileEntry>::const_iterator it = FileList.begin();
    while (entry->isValid() && it != FileList.end()) {
        std::vector<FileEntry>::const_iterator jt = it;
        // Check if the current entry is registered, otherwise check the next registered files as soon as
        // both file names match
        while (jt != FileList.end() && entry->getName() != jt->FileName)
            ++jt;
        // If this condition is true both file names match and we can read-in the data, otherwise
        // no file name for the current entry in the zip was registered.
        if (jt != FileList.end()) {
            try {
                Reader reader(zipstream, jt->FileName, FileVersion);
                InputStream str(zipstream);
                jt->Object->restoreStream(str);
                if (reader.getLocalReader())
                    reader.getLocalReader()->readFiles(zipstream);
            }
            catch(...) {
                // For any exception we just continue with the next file.
                // It doesn't matter if the last reader has read more or
                // less data than the file size would allow.
                // All what we need to do is to notify the user about the
                // failure.
                std::string msg = "Reading failed from embedded file: "+ entry->toString() +"\n";

                throw OneException(msg.c_str());
            }
            // Go to the next registered file name
            it = jt + 1;
        }

        // In either case we must go to the next entry
        try {
            entry = zipstream.getNextEntry();
        }
        catch (const std::exception&) {
            // there is no further entry
            break;
        }
    }
}

const char *XMLReader::addFile(const char* Name, Persistence *Object)
{
    FileEntry temp;
    temp.FileName = Name;
    temp.Object = Object;

    FileList.push_back(temp);
    FileNames.push_back( temp.FileName );

    return Name;
}

const std::vector<std::string>& XMLReader::getFilenames() const
{
    return FileNames;
}

bool XMLReader::isRegistered(Persistence *Object) const
{
    if (Object) {
        for (std::vector<FileEntry>::const_iterator it = FileList.begin(); it != FileList.end(); ++it) {
            if (it->Object == Object)
                return true;
        }
    }

    return false;
}

void XMLReader::addName(const char*, const char*)
{
}

const char* XMLReader::getName(const char* name) const
{
    return name;
}

bool XMLReader::doNameMapping() const
{
    return false;
}

// ---------------------------------------------------------------------------
//  XMLReader: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void XMLReader::startDocument()
{
    ReadType = StartDocument;
}

void XMLReader::endDocument()
{
    ReadType = EndDocument;
}

void XMLReader::startElement(const XMLCh* const /*uri*/, const XMLCh* const localname, const XMLCh* const /*qname*/, const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs)
{
    Level++; // new scope
    LocalName = StrX(localname).c_str();

    // saving attributes of the current scope, delete all previously stored ones
    AttrMap.clear();
    for (unsigned int i = 0; i < attrs.getLength(); i++) {
        AttrMap[StrX(attrs.getQName(i)).c_str()] = StrXUTF8(attrs.getValue(i)).c_str();
    }

    ReadType = StartElement;
}

void XMLReader::endElement  (const XMLCh* const /*uri*/, const XMLCh *const localname, const XMLCh *const /*qname*/)
{
    Level--; // end of scope
    LocalName = StrX(localname).c_str();

    if (ReadType == StartElement)
        ReadType = StartEndElement;
    else
        ReadType = EndElement;
}

void XMLReader::startCDATA ()
{
    ReadType = StartCDATA;
}

void XMLReader::endCDATA ()
{
    ReadType = EndCDATA;
}

void XMLReader::characters(const   XMLCh* const chars, const XMLSize_t length)
{
    Characters = StrX(chars).c_str();
    ReadType = Chars;
    CharacterCount += (unsigned int)length;
}

void XMLReader::ignorableWhitespace( const   XMLCh* const /*chars*/, const XMLSize_t /*length*/)
{
}

void XMLReader::resetDocument()
{
}


// ---------------------------------------------------------------------------
//  XMLReader: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void XMLReader::error(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& e)
{
    // print some details to error output and throw an
    // exception to abort the parsing
    cerr << "Error at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber() << endl;
    throw e;
}

void XMLReader::fatalError(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& e)
{
    // print some details to error output and throw an
    // exception to abort the parsing
    cerr << "Fatal Error at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber() << endl;
    throw e;
}

void XMLReader::warning(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& e)
{
    // print some details to error output and throw an
    // exception to abort the parsing
   cerr << "Warning at file " << StrX(e.getSystemId())
        << ", line " << e.getLineNumber()
        << ", char " << e.getColumnNumber() << endl;
   throw e;
}

void XMLReader::resetErrors()
{
}

bool XMLReader::testStatus(ReaderStatus pos) const
{
    return StatusBits.test(static_cast<size_t>(pos));
}

void XMLReader::setStatus(ReaderStatus pos, bool on)
{
    StatusBits.set(static_cast<size_t>(pos), on);
}

void XMLReader::setPartialRestore(bool on)
{
    setStatus(PartialRestore, on);
    setStatus(PartialRestoreInDocumentObject, on);
    setStatus(PartialRestoreInProperty, on);
    setStatus(PartialRestoreInObject, on);
}

void XMLReader::clearPartialRestoreDocumentObject()
{
    setStatus(PartialRestoreInDocumentObject, false);
    setStatus(PartialRestoreInProperty, false);
    setStatus(PartialRestoreInObject, false);
}

void XMLReader::clearPartialRestoreProperty()
{
    setStatus(PartialRestoreInProperty, false);
    setStatus(PartialRestoreInObject, false);
}

void XMLReader::clearPartialRestoreObject()
{
    setStatus(PartialRestoreInObject, false);
}

// ----------------------------------------------------------

Reader::Reader(std::istream& str, const std::string& name, int version)
  : std::istream(str.rdbuf()), _str(str), _name(name), fileVersion(version)
{
}

std::string Reader::getFileName() const
{
    return this->_name;
}

int Reader::getFileVersion() const
{
    return fileVersion;
}

std::istream& Reader::getStream()
{
    return this->_str;
}

void Reader::initLocalReader(std::shared_ptr<XMLReader> reader)
{
    this->localreader = reader;
}

std::shared_ptr<XMLReader> Reader::getLocalReader() const
{
    return(this->localreader);
}
