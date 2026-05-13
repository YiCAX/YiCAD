
#ifndef BASE_WRITER_H
#define BASE_WRITER_H

#ifdef _MSC_VER
#include <zipios++/zipios-config.h>
#endif
#include <zipios++/zipfile.h>
#include <zipios++/zipinputstream.h>
#include <zipios++/zipoutputstream.h>
#include <zipios++/meta-iostreams.h>

#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

class Persistence;

/** The Writer class
 * This is an important helper class for the store and retrieval system
 * of persistent objects 
 */
class Writer
{

public:
    Writer();
    virtual ~Writer();

    /// switch the writer in XML only mode (no files allowed)
    void setForceXML(bool on);
    /// check on state
    bool isForceXML();
    void setFileVersion(int);
    int getFileVersion() const;

    /// insert a file as CDATA section in the XML file
    void insertAsciiFile(const char* FileName);
    /// insert a binary file BASE64 coded as CDATA section in the XML file
    void insertBinFile(const char* FileName);

    /** @name additional file writing */
    //@{
    /// add a write request of a persistent object
    std::string addFile(const char* Name, const Persistence *Object);
    /// process the requested file storing
    virtual void writeFiles()=0;
    /// get all registered file names
    const std::vector<std::string>& getFilenames() const;
    /// Set mode
    void setMode(const std::string& mode);
    /// Set modes
    void setModes(const std::set<std::string>& modes);
    /// Get mode
    bool getMode(const std::string& mode) const;
    /// Get modes
    std::set<std::string> getModes() const;
    /// Clear mode
    void clearMode(const std::string& mode);
    /// Clear modes
    void clearModes();
    //@}

    /** @name Error handling */
    //@{
    void addError(const std::string&);
    bool hasErrors() const;
    void clearErrors();
    std::vector<std::string> getErrors() const;
    //@}

    /** @name pretty formatting for XML */
    //@{
    /// get the current indentation
    const char* ind() const {return indBuf;}
    /// increase indentation by one tab
    void incInd();
    /// decrease indentation by one tab
    void decInd();
    //@}

    virtual std::ostream &Stream()=0;

    /// name for underlying file saves
    std::string ObjectName;

protected:
    std::string getUniqueFileName(const char *Name);
    struct FileEntry {
        std::string FileName;
        const Persistence *Object;
    };
    std::vector<FileEntry> FileList;
    std::vector<std::string> FileNames;
    std::vector<std::string> Errors;
    std::set<std::string> Modes;

    short indent;
    char indBuf[1024];

    bool forceXML;
    int fileVersion;

private:
    Writer(const Writer&);
    Writer& operator=(const Writer&);
};


/** The ZipWriter class, helper class implementation for the store and retrieval system
 * of persistent objects 
 */
class ZipWriter : public Writer
{
public:
    ZipWriter(const char* FileName);
    ZipWriter(std::ostream&);
    virtual ~ZipWriter();

    virtual void writeFiles();

    virtual std::ostream &Stream(){return ZipStream;}

    void setComment(const char* str){ZipStream.setComment(str);}
    void setLevel(int level){ZipStream.setLevel( level );}
    void putNextEntry(const char* str){ZipStream.putNextEntry(str);}

private:
    zipios::ZipOutputStream ZipStream;
};

/*! The FileWriter class, who writes out the data into files
 * into a given directory name.
 */
class FileWriter : public Writer
{
public:
    FileWriter(const char* DirName);
    virtual ~FileWriter();

    void putNextEntry(const char* file);
    virtual void writeFiles();

    virtual std::ostream &Stream(){return FileStream;}
    void close() {FileStream.close();}
    /*!
     This method can be re-implemented in sub-classes to avoid
     to write out certain objects. The default implementation
     always returns true.
     */
    virtual bool shouldWrite(const std::string& name, const Persistence *Object) const;

protected:
    std::string DirName;
    std::ofstream FileStream;
};

#endif // BASE_WRITER_H
