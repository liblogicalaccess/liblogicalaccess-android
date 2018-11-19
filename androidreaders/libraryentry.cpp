#include "androidreaderprovider.hpp"
#include "morphosamreaderprovider.hpp"
#include <string>

extern "C" {
 char *getLibraryName()
{
    return (char *)READER_ANDROID;
}

 void
getAndroidReader(std::shared_ptr<logicalaccess::AndroidReaderProvider> *rp)
{
    if (rp != NULL)
    {
        *rp = logicalaccess::AndroidReaderProvider::getSingletonInstance();
    }
}

 void
getMORPHO_SAMReader(std::shared_ptr<logicalaccess::MorphoSAMReaderProvider> *rp)
{
    if (rp != NULL)
    {
        *rp = logicalaccess::MorphoSAMReaderProvider::getSingletonInstance();
    }
}

 bool getReaderInfoAt(unsigned int index, char *readername,
                                          size_t readernamelen, void **getterfct)
{
    bool ret = false;
    if (readername != NULL && readernamelen == PLUGINOBJECT_MAXLEN && getterfct != NULL)
    {
        switch (index)
        {
        case 0:
        {
            *getterfct = (void *)&getAndroidReader;
            sprintf(readername, READER_ANDROID);
            ret = true;
        }
        case 1:
        {
            *getterfct = (void *)&getMORPHO_SAMReader;
            sprintf(readername, READER_MORPHO_SAM);
            ret = true;
        }
        break;
        }
    }

    return ret;
}
}
