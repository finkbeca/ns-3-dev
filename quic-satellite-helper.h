/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2013 Magister Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Original work author (from packet-sink-helper.cc):
 *   - Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *
 * Converted to HTTP web browsing traffic models by:
 *   - Budiarto Herman <budiarto.herman@magister.fi>
 *
 * Adapted to ns-3.29 architecture by:
 * - Patrice Raveneau <patrice.raveneau@cnes.fr>
 *
 */

#ifndef THREE_GPP_HTTP_SATELLITE_HELPER_H
#define THREE_GPP_HTTP_SATELLITE_HELPER_H

#include <ns3/application-container.h>
#include <ns3/node-container.h>
#include <ns3/object-factory.h>
#include <ns3/three-gpp-http-helper.h>
#include <ns3/three-gpp-http-variables.h>

namespace ns3
{

/**
 * \ingroup applications
 * Helper to make it easier to instantiate an QuicClient on a set of nodes.
 */
class QuicSatelliteClientHelper
{
  public:
    /**
     * Create a QuicSatelliteClientHelper to make it easier to work with QuicClient
     * applications.
     * \param address The address of the remote server node to send traffic to.
     */
    QuicSatelliteClientHelper(const Address& address);

    /**
     * Helper function used to set the underlying application attributes, but
     * *not* the socket attributes.
     * \param name The name of the application attribute to set.
     * \param value The value of the application attribute to set.
     */
    void SetAttribute(const std::string& name, const AttributeValue& value);

    /**
     * Install a QuicClient on each node of the input container configured with
     * all the attributes set with SetAttribute().
     * \param c NodeContainer of the set of nodes on which an QuicClient
     *          will be installed.
     * \return Container of Ptr to the applications installed.
     */
    ApplicationContainer Install(NodeContainer c) const;

    /**
     * Install a QuicClient on each node of the input container
     * configured with all the attributes set with SetAttribute().
     * \param node The node on which an QuicClient will be installed.
     * \return Container of Ptr to the applications installed.
     */
    ApplicationContainer Install(Ptr<Node> node) const;

    /**
     * Install a QuicClient on each node of the input container
     * configured with all the attributes set with SetAttribute().
     * \param nodeName The name of the node on which an QuicClient
     *                 will be installed.
     * \return Container of Ptr to the applications installed.
     */
    ApplicationContainer Install(const std::string& nodeName) const;

  private:
    /**
     * \internal
     * Install a QuicClient on the node configured with all the
     * attributes set with SetAttribute().
     * \param node The node on which an QuicClient will be installed.
     * \return Ptr to the application installed.
     */
    Ptr<Application> InstallPriv(Ptr<Node> node) const;

    /// Used to instantiate an QuicClient instance.
    ObjectFactory m_factory;

}; // end of `class QuicSatelliteClientHelper`

/**
 * \ingroup http
 * Helper to make it easier to instantiate an QuicServer on a set of nodes.
 */
class QuicSatelliteServerHelper
{
  public:
    /**
     * Create a QuicSatelliteServerHelper to make it easier to work with
     * QuicServer applications.
     * \param address The address of the server.
     */
    QuicSatelliteServerHelper(const Address& address);

    /**
     * Helper function used to set the underlying application attributes, but
     * *not* the socket attributes.
     * \param name The name of the application attribute to set.
     * \param value The value of the application attribute to set.
     */
    void SetAttribute(const std::string& name, const AttributeValue& value);

    /**
     * Install an QuicServer on each node of the input container
     * configured with all the attributes set with SetAttribute().
     * \param c NodeContainer of the set of nodes on which an QuicServer
     *              will be installed.
     * \return Container of Ptr to the applications installed.
     */
    ApplicationContainer Install(NodeContainer c) const;

    /**
     * Install an QuicServer on each node of the input container
     * configured with all the attributes set with SetAttribute().
     * \param node The node on which an QuicServer will be installed.
     * \return Container of Ptr to the applications installed.
     */
    ApplicationContainer Install(Ptr<Node> node) const;

    /**
     * Install an QuicServer on each node of the input container
     * configured with all the attributes set with SetAttribute().
     * \param nodeName The name of the node on which an QuicServer
     *                             will be installed.
     * \return Container of Ptr to the applications installed.
     */
    ApplicationContainer Install(const std::string& nodeName) const;

  private:
    /**
     * \internal
     * Install an QuicServer on the node configured with all the
     * attributes set with SetAttribute().
     * \param node The node on which an QuicServer will be installed.
     * \return Ptr to the application installed.
     */
    Ptr<Application> InstallPriv(Ptr<Node> node) const;

    /// Used to instantiate a QuicServer instance.
    ObjectFactory m_factory;

}; // end of `class QuicSatelliteServerHelper`

/**
 * \ingroup http
 * \brief Helper to make it easier to instantiate a 3GPP HTTP server and a group of
 *        connected 3GPP HTTP clients.
 */
class QuicHelper
{
  public:
    /**
     * \brief Create a QuicHelper to make it easier to work with
     *        QuicClient and QuicServer applications.
     *
     * \param protocolTid The TypeId of the protocol to be used by the server
     *                    and client. Either TypeId of TcpSocketFactory or UdpSocketFactory.
     */
    QuicHelper();

    /// Instance destructor.
    virtual ~QuicHelper();

    /**
     * \brief Helper function used to set the underlying QuicTcpClient
     *        application attributes, but *not* the socket attributes.
     *
     * \param name the name of the application attribute to set
     * \param value the value of the application attribute to set
     */
    void SetClientAttribute(std::string name, const AttributeValue& value);

    /**
     * \brief Helper function used to set the underlying QuicServer
     *        application attributes, but *not* the
     *        socket attributes.
     *
     * \param name the name of the application attribute to set
     * \param value the value of the application attribute to set
     */
    void SetServerAttribute(std::string name, const AttributeValue& value);

    /**
     * \brief Helper function used to set the QuicVariables attributes
     *        used by this helper instance. Currently, only number of videos
     *        specified in the Variables class is used.
     *
     * \param name the name of the application attribute to set
     * \param value the value of the application attribute to set
     */
    void SetVariablesAttribute(std::string name, const AttributeValue& value);

    /**
     * \brief Install an Quic Server application and several Quic client
     *        applications, in which each client is connected using IPv4 to the
     *        server.
     *
     * \param serverNode the node on which an QuicServer will be installed
     * \param clientNodes the set of nodes on which QuicClient applications will
     *                    be installed
     * \return container of Ptr to the server and client applications installed
     *
     * \warning The given nodes must have Internet stack installed properly
     *          before this method can be called.
     *
     * The installed applications will be configured with all the attributes set
     * with SetClientAttribute() and SetServerAttribute(). Pointers to these
     * applications can be retrieved afterwards by calling GetClients() and
     * GetServer() methods separately.
     */
    ApplicationContainer InstallUsingIpv4(Ptr<Node> serverNode, NodeContainer clientNodes);

    /**
     * \brief Install an Quic Server application and an Quic client
     *        applications, in which each client is connected using IPv4 to the
     *        server.
     *
     * \param serverNode the node on which an QuicServer will be installed
     * \param clientNode the node on which an QuicClient will be installed
     * \return container of Ptr to the server and client applications installed
     *
     * \warning The given nodes must have Internet stack installed properly
     *          before this method can be called.
     *
     * The installed applications will be configured with all the attributes set
     * with SetClientAttribute() and SetServerAttribute(). Pointers to these
     * applications can be retrieved afterwards by calling GetClients() and
     * GetServer() methods separately.
     */
    ApplicationContainer InstallUsingIpv4(Ptr<Node> serverNode, Ptr<Node> clientNode);

    /**
     * \brief Retrieve pointers to the 3GPP HTTP clients which were installed by the
     *        previous call of Install().
     *
     * \return an application container containing 3GPP HTTP clients, or an empty
     *         container if Install() has never been called before
     */
    ApplicationContainer GetClients() const;

    /**
     * \brief Retrieve a pointer to the 3GPP HTTP server which was installed by the
     *        previous call of Install().
     *
     * \return an application container containing a single 3GPP HTTP server, or an
     *         empty container if Install() has never been called before
     */
    ApplicationContainer GetServer() const;

  private:
    QuicSatelliteServerHelper* m_serverHelper;
    QuicSatelliteClientHelper* m_clientHelper;
    Ptr<QuicVariables> m_httpVariables;
    ApplicationContainer m_lastInstalledClients;
    ApplicationContainer m_lastInstalledServer;

}; // end of `class QuicHelper`

} // namespace ns3

#endif /* THREE_GPP_HTTP_HELPER_SATELLITE_H */