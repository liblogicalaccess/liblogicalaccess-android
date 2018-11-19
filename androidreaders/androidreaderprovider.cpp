/**
 * \file androidreaderprovider.cpp
 * \author Adrien J. <adrien-dev@islog.com>
 * \brief Reader Provider OSDP.
 */

#include "androidreaderprovider.hpp"
#include "logicalaccess/plugins/llacommon/logs.hpp"
#include "logicalaccess/readerproviders/serialportdatatransport.hpp"

namespace logicalaccess
{
AndroidReaderProvider::AndroidReaderProvider()
    : ReaderProvider()
{
}

AndroidReaderProvider::~AndroidReaderProvider()
{
}

void AndroidReaderProvider::release()
{
}

std::shared_ptr<AndroidReaderProvider> AndroidReaderProvider::getSingletonInstance()
{
    LOG(LogLevel::INFOS) << "Getting singleton instance...";
    static std::shared_ptr<AndroidReaderProvider> instance;
    if (!instance)
    {
        instance.reset(new AndroidReaderProvider());
        instance->refreshReaderList();
    }

    return instance;
}

std::shared_ptr<ReaderUnit> AndroidReaderProvider::createReaderUnit()
{
    LOG(LogLevel::INFOS) << "Creating new reader unit with empty port...";

    return std::make_shared<AndroidReaderUnit>();
}

bool AndroidReaderProvider::refreshReaderList()
{
    LOG(LogLevel::INFOS) << "Refreshing reader list...";
    d_readers.clear();
    d_readers.push_back(std::make_shared<AndroidReaderUnit>());
    return true;
}
}
