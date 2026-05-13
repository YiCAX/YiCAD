
#include <limits>
#include <locale>

#include "Writer.h"
#include "Base64.h"
#if defined(_WIN32) && defined(ZIPIOS_UTF8)
#include <FileInfo.h>
#endif
#include "Persistence.h"
#include "Stream.h"
#include "Tools.h"

using namespace std;
using namespace zipios;


// ---------------------------------------------------------------------------
//  Writer: Constructors and Destructor
// ---------------------------------------------------------------------------

Writer::Writer()
  : indent(0),forceXML(false),fileVersion(1)
{
    indBuf[0] = '\0';
}

Writer::~Writer()
{
}

void Writer::insertAsciiFile(const char* FileName)
{
    FileInfo fi(FileName);
    Oifstream from(fi);
    if (!from)
    {
        throw OneException("Writer::insertAsciiFile() Could not open file!");
    }
    Stream() << "<![CDATA[";
    char ch;
    while (from.get(ch))
        Stream().put(ch);
    Stream() << "]]>" << endl;
}

void Writer::insertBinFile(const char* FileName)
{
    FileInfo fi(FileName);
    Oifstream from(fi, std::ios::in | std::ios::binary | std::ios::ate);
    if (!from)
    {
        throw OneException("Writer::insertAsciiFile() Could not open file!");
    }

    Stream() << "<![CDATA[";
    std::ifstream::pos_type fileSize = from.tellg();
    from.seekg(0, std::ios::beg);
    std::vector<unsigned char> bytes(static_cast<size_t>(fileSize));
    from.read(reinterpret_cast<char*>(&bytes[0]), fileSize);
    Stream() << PersistentTools::base64_encode(&bytes[0], static_cast<unsigned int>(fileSize));
    Stream() << "]]>" << endl;
}

void Writer::setForceXML(bool on)
{
    forceXML = on;
}

bool Writer::isForceXML()
{
    return forceXML;
}

void Writer::setFileVersion(int v)
{
    fileVersion = v;
}

int Writer::getFileVersion() const
{
    return fileVersion;
}

void Writer::setMode(const std::string& mode)
{
    Modes.insert(mode);
}

void Writer::setModes(const std::set<std::string>& modes)
{
    Modes = modes;
}

bool Writer::getMode(const std::string& mode) const
{
    std::set<std::string>::const_iterator it = Modes.find(mode);
    return (it != Modes.end());
}

std::set<std::string> Writer::getModes() const
{
    return Modes;
}

void Writer::clearMode(const std::string& mode)
{
    std::set<std::string>::iterator it = Modes.find(mode);
    if (it != Modes.end())
        Modes.erase(it);
}

void Writer::clearModes()
{
    Modes.clear();
}

void Writer::addError(const std::string& msg)
{
    Errors.push_back(msg);
}

bool Writer::hasErrors() const
{
    return (!Errors.empty());
}

void Writer::clearErrors()
{
    Errors.clear();
}

std::vector<std::string> Writer::getErrors() const
{
    return Errors;
}

std::string Writer::addFile(const char* Name,const Persistence *Object)
{
    // always check isForceXML() before requesting a file!
    assert(isForceXML()==false);

    FileEntry temp;
    temp.FileName = getUniqueFileName(Name);
    temp.Object = Object;

    FileList.push_back(temp);

    FileNames.push_back( temp.FileName );

    // return the unique file name
    return temp.FileName;
}

std::string Writer::getUniqueFileName(const char *Name)
{
    // name in use?
    std::string CleanName = (Name ? Name : "");
    std::vector<std::string>::const_iterator pos;
    pos = find(FileNames.begin(),FileNames.end(),CleanName);

    if (pos == FileNames.end()) {
        // if not, name is OK
        return CleanName;
    }
    else {
        std::vector<std::string> names;
        names.reserve(FileNames.size());
        FileInfo fi(CleanName);
        CleanName = fi.fileNamePure();
        std::string ext = fi.extension();
        for (pos = FileNames.begin();pos != FileNames.end();++pos) {
            fi.setFile(*pos);
            std::string FileName = fi.fileNamePure();
            if (fi.extension() == ext)
                names.push_back(FileName);
        }
        std::stringstream str;
        str << Tools::getUniqueName(CleanName, names);
        if (!ext.empty())
            str << "." << ext;
        return str.str();
    }
}

const std::vector<std::string>& Writer::getFilenames() const
{
    return FileNames;
}

void Writer::incInd()
{
    if (indent < 1020) {
        indBuf[indent  ] = ' ';
        indBuf[indent+1] = ' ';
        indBuf[indent+2] = ' ';
        indBuf[indent+3] = ' ';
        indBuf[indent+4] = '\0';
        indent += 4;
    }
}

void Writer::decInd()
{
    if (indent >= 4) {
        indent -= 4;
    }
    else {
        indent = 0;
    }
    indBuf[indent] = '\0';
}

// ----------------------------------------------------------------------------
/// ZipWriter

ZipWriter::ZipWriter(const char* FileName)
  : ZipStream(FileName)
{
#ifdef _MSC_VER
    ZipStream.imbue(std::locale::empty());
#else
    ZipStream.imbue(std::locale::classic());
#endif
    ZipStream.precision(std::numeric_limits<double>::digits10 + 1);
    ZipStream.setf(ios::fixed,ios::floatfield);
}

ZipWriter::ZipWriter(std::ostream& os)
  : ZipStream(os)
{
#ifdef _MSC_VER
    ZipStream.imbue(std::locale::empty());
#else
    ZipStream.imbue(std::locale::classic());
#endif
    ZipStream.precision(std::numeric_limits<double>::digits10 + 1);
    ZipStream.setf(ios::fixed,ios::floatfield);
}

void ZipWriter::writeFiles()
{
    // use a while loop because it is possible that while
    // processing the files new ones can be added
    size_t index = 0;
    while (index < FileList.size()) {
        FileEntry entry = FileList[index];
        ZipStream.putNextEntry(entry.FileName);
		OutputStream str(ZipStream);
        entry.Object->saveStream(str);
        index++;
    }
}

ZipWriter::~ZipWriter()
{
    ZipStream.close();
}

// ----------------------------------------------------------------------------
/// FilterWriter

FileWriter::FileWriter(const char* DirName) : DirName(DirName)
{
}

FileWriter::~FileWriter()
{
}

void FileWriter::putNextEntry(const char* file)
{
    std::string fileName = DirName + "/" + file;
    this->FileStream.open(fileName.c_str(), std::ios::out | std::ios::binary);
}

bool FileWriter::shouldWrite(const std::string& , const Persistence *) const
{
    return true;
}

void FileWriter::writeFiles()
{
    // use a while loop because it is possible that while
    // processing the files new ones can be added
    size_t index = 0;
    this->FileStream.close();
    while (index < FileList.size()) {
        FileEntry entry = FileList[index];

        if (shouldWrite(entry.FileName, entry.Object)) {
            std::string filePath = entry.FileName;
            std::string::size_type pos = 0;
            while ((pos = filePath.find('/', pos)) != std::string::npos) {
                std::string dirName = DirName + "/" + filePath.substr(0, pos);
                pos++;
                FileInfo fi(dirName);
                fi.createDirectory();
            }

            std::string fileName = DirName + "/" + entry.FileName;
            this->FileStream.open(fileName.c_str(), std::ios::out | std::ios::binary);
			OutputStream str(Stream());
            entry.Object->saveStream(str);
            this->FileStream.close();
        }

        index++;
    }
}
