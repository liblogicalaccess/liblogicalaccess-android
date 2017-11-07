/**
 * \file STidSTRReaderUnitConfiguration.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief STidSTR reader unit configuration.
 */

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <logicalaccess/readerproviders/readerunitconfiguration.hpp>

#include "androidreaderprovider.hpp"
#include "androidreaderunitconfiguration.hpp"
#include "logicalaccess/logs.hpp"

namespace logicalaccess
{
AndroidReaderUnitConfiguration::AndroidReaderUnitConfiguration()
    : ReaderUnitConfiguration(READER_ANDROID)
{
}

AndroidReaderUnitConfiguration::~AndroidReaderUnitConfiguration()
{
}

std::string AndroidReaderUnitConfiguration::getDefaultXmlNodeName() const
{
    return "AndroidReaderUnitConfiguration";
}
}
