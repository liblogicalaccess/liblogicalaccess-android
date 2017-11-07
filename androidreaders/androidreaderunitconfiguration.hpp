/**
 * \file stidstrreaderunitconfiguration.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief STidSTR Reader unit configuration.
 */

#ifndef LOGICALACCESS_ANDROIDREADERUNITCONFIGURATION_HPP
#define LOGICALACCESS_ANDROIDREADERUNITCONFIGURATION_HPP

#include "logicalaccess/readerproviders/readerunitconfiguration.hpp"

namespace logicalaccess
{
/**
 * \brief The ANDROID reader unit configuration base class.
 */
class LIBLOGICALACCESS_API AndroidReaderUnitConfiguration : public ReaderUnitConfiguration
{
  public:
    /**
     * \brief Constructor.
     */
    AndroidReaderUnitConfiguration();

    /**
     * \brief Destructor.
     */
    virtual ~AndroidReaderUnitConfiguration();

    /**
     * \brief Serialize the current object to XML.
     * \param parentNode The parent node.
     */
    virtual void serialize(boost::property_tree::ptree &parentNode) override{};

    /**
     * \brief UnSerialize a XML node to the current object.
     * \param node The XML node.
     */
    virtual void unSerialize(boost::property_tree::ptree &node) override{};

    /**
     * \brief Get the default Xml Node name for this object.
     * \return The Xml node name.
     */
    virtual std::string getDefaultXmlNodeName() const override;

    virtual void resetConfiguration(){};

  protected:
};
}

#endif
