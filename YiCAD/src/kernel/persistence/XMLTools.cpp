#include "XMLTools.h"
#include <Tools.h>

std::unique_ptr<XERCES_CPP_NAMESPACE::XMLTranscoder> XMLTools::transcoder;

void XMLTools::initialize()
{
    XERCES_CPP_NAMESPACE_USE;
    if (!transcoder.get()) {
        XMLTransService::Codes  res;
        transcoder.reset(XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::fgTransService->makeNewTranscoderFor(XERCES_CPP_NAMESPACE_QUALIFIER XMLRecognizer::UTF_8, res, 4096, XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::fgMemoryManager));
        if (res != XMLTransService::Ok)
            throw OneException("Can\'t create transcoder");
    }
}

std::string XMLTools::toStdString(const XMLCh* const toTranscode)
{
    std::string str;

    XERCES_CPP_NAMESPACE_USE;
    initialize();

    //char outBuff[128];
    static XMLByte outBuff[128];
#if (XERCES_VERSION_MAJOR == 2)
    unsigned int outputLength;
    unsigned int eaten = 0;
    unsigned int offset = 0;
    unsigned int inputLength = XMLString::stringLen(toTranscode);
#else
    XMLSize_t outputLength;
    XMLSize_t eaten = 0;
    XMLSize_t offset = 0;
    XMLSize_t inputLength = XMLString::stringLen(toTranscode);
#endif

    while (inputLength)
    {
        outputLength = transcoder->transcodeTo(toTranscode + offset, inputLength, outBuff, 128, eaten, XMLTranscoder::UnRep_RepChar);
        str.append(reinterpret_cast<const char*>(outBuff), outputLength);
        offset += eaten;
        inputLength -= eaten;

        //  Bail out if nothing more was produced
        if (outputLength == 0)
            break;
    }

    return str;
}

std::basic_string<XMLCh> XMLTools::toXMLString(const char* const fromTranscode)
{
    std::basic_string<XMLCh> str;
    if (!fromTranscode)
        return str;

    XERCES_CPP_NAMESPACE_USE;
    initialize();

    static XMLCh outBuff[128];
    const XMLByte* xmlBytes = reinterpret_cast<const XMLByte*>(fromTranscode);
#if (XERCES_VERSION_MAJOR == 2)
    unsigned int outputLength;
    unsigned int eaten = 0;
    unsigned int offset = 0;
    unsigned int inputLength = std::string(fromTranscode).size();
#else
    XMLSize_t outputLength;
    XMLSize_t eaten = 0;
    XMLSize_t offset = 0;
    XMLSize_t inputLength = std::string(fromTranscode).size();
#endif

    unsigned char* charSizes = new unsigned char[inputLength];
    while (inputLength)
    {
        outputLength = transcoder->transcodeFrom(xmlBytes + offset, inputLength, outBuff, 128, eaten, charSizes);
        str.append(outBuff, outputLength);
        offset += eaten;
        inputLength -= eaten;

        //  Bail out if nothing more was produced
        if (outputLength == 0)
            break;
    }

    delete[] charSizes;
    return str;
}

void XMLTools::terminate()
{
    transcoder.reset();
}
