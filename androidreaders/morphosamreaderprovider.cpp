//
// Created by xaqq on 3/9/18.
//

#include "morphosamreaderprovider.hpp"
#include "morphosamreaderunit.hpp"
#include "logicalaccess/plugins/llacommon/logs.hpp"

namespace logicalaccess
{
MorphoSAMReaderProvider::MorphoSAMReaderProvider()
    : ReaderProvider()
{
}

void MorphoSAMReaderProvider::release()
{
}

std::shared_ptr<MorphoSAMReaderProvider> MorphoSAMReaderProvider::getSingletonInstance()
{
    LOG(LogLevel::INFOS) << "Getting MorphoSAMReaderProvider singleton instance...";
    static std::shared_ptr<MorphoSAMReaderProvider> instance;
    if (!instance)
    {
        instance.reset(new MorphoSAMReaderProvider());
        instance->refreshReaderList();
    }

    return instance;
}

std::shared_ptr<ReaderUnit> MorphoSAMReaderProvider::createReaderUnit()
{
    LOG(LogLevel::INFOS) << "Creating new MorphoSAMReaderUnit ...";

    return std::make_shared<MorphoSAMReaderUnit>();
}

bool MorphoSAMReaderProvider::refreshReaderList()
{
    LOG(LogLevel::INFOS) << "Refreshing reader list...";
    d_readers.clear();
    d_readers.push_back(std::make_shared<MorphoSAMReaderUnit>());
    return true;
}
}
