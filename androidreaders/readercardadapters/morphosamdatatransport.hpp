#pragma once

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <logicalaccess/readerproviders/datatransport.hpp>
#include <string>
#include <vector>

namespace logicalaccess
{

// Data transport to talk to SAM on a morpho
// tablet
// This code is very similar to androiddatatransport.
// We should probably refactor.
class MorphoSAMDataTransport : public DataTransport
{

  public:
    MorphoSAMDataTransport() = default;

    /**
     * \brief Get the transport type of this instance.
     * \return The transport type.
     */
    virtual std::string getTransportType() const
    {
        return "MorphoSAMDataTransport";
    };

    /**
     * \brief Serialize the current object to XML.
     * \param parentNode The parent node.
     */
    void serialize(boost::property_tree::ptree &parentNode)
    {
        boost::property_tree::ptree node;
        parentNode.add_child(getDefaultXmlNodeName(), node);
    }

    /**
     * \brief UnSerialize a XML node to the current object.
     * \param node The XML node.
     */
    void unSerialize(boost::property_tree::ptree &node)
    {
    }

    /**
     * \brief Get the default Xml Node name for this object.
     * \return The Xml node name.
     */
    virtual std::string getDefaultXmlNodeName() const
    {
        return "MorphoSAMDataTransport";
    };

    virtual bool connect()
    {
        return true;
    };

    virtual void disconnect(){};

    virtual bool isConnected()
    {
        return true;
    };

    virtual std::string getName() const
    {
        return "MorphoSAM";
    };


  protected:
    void send(const std::vector<unsigned char> &data) override;

    std::vector<unsigned char> receive(long int timeout) override;

    // The result we store when send() is called.
    ByteVector result_;
};
}
