/**
 * \file ANDROIDDATATRANSPORT_HPP.hpp
 * \author Adrien J. <adrien.jund@islog.com>
 * \brief Android DataTransport.
 */

#ifndef ANDROIDDATATRANSPORT_HPP
#define ANDROIDDATATRANSPORT_HPP

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "logicalaccess/readerproviders/serialportdatatransport.hpp"

#include <string>
#include <vector>

namespace logicalaccess
{
class LIBLOGICALACCESS_API AndroidDataTransport : public DataTransport
{
  public:
    AndroidDataTransport(){};

    /**
     * \brief Get the transport type of this instance.
     * \return The transport type.
     */
    virtual std::string getTransportType() const
    {
        return "AndroidDataTransport";
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
        return "AndroidDataTransport";
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
        return "Android";
    };

    virtual void send(const std::vector<unsigned char> &data);

    virtual std::vector<unsigned char> receive(long int timeout);

  protected:
    std::vector<unsigned char> m_result;
};
}

#endif /* ANDROIDDATATRANSPORT_HPP */