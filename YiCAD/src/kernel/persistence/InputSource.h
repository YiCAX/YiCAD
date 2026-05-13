#ifndef BASE_IINPUTSOURCE_H
#define BASE_IINPUTSOURCE_H

#include <iosfwd>

#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <QTextCodec>

XERCES_CPP_NAMESPACE_BEGIN
  class BinInputStream;
XERCES_CPP_NAMESPACE_END

class StdInputStream : public XERCES_CPP_NAMESPACE_QUALIFIER BinInputStream
{
public :
  StdInputStream ( std::istream& Stream, XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager* const manager = XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::fgMemoryManager );
  virtual ~StdInputStream();

  // -----------------------------------------------------------------------
  //  Implementation of the input stream interface
  // -----------------------------------------------------------------------
#if (XERCES_VERSION_MAJOR == 2)
  virtual unsigned int curPos() const;
  virtual unsigned int readBytes( XMLByte* const toFill, const unsigned int maxToRead );
#else
  virtual XMLFilePos curPos() const;
  virtual XMLSize_t readBytes( XMLByte* const toFill, const XMLSize_t maxToRead );
  virtual const XMLCh* getContentType() const {return nullptr;}
#endif

private :
  // -----------------------------------------------------------------------
  //  Unimplemented constructors and operators
  // -----------------------------------------------------------------------
  StdInputStream(const StdInputStream&);
  StdInputStream& operator=(const StdInputStream&);

  // -----------------------------------------------------------------------
  //  Private data members
  //
  //  fSource
  //      The source file that we represent. The FileHandle type is defined
  //      per platform.
  // -----------------------------------------------------------------------
  std::istream            &stream;
  XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager* const    fMemoryManager;
  QTextCodec::ConverterState state;
};


class StdInputSource : public XERCES_CPP_NAMESPACE_QUALIFIER InputSource
{
public :
  StdInputSource ( std::istream& Stream, const char* filePath, XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager* const manager = XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::fgMemoryManager );
   ~StdInputSource();

  virtual XERCES_CPP_NAMESPACE_QUALIFIER BinInputStream* makeStream() const;

private:
  StdInputSource(const StdInputSource&);
  StdInputSource& operator=(const StdInputSource&);

  std::istream   &stream;
};

#endif // BASE_IINPUTSOURCE_H
