/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 SMUNIX
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
 * Author: Providence M. SALUMU <Providence.Salumu@smunix.com>
 */

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "rfid-channel.h"
#include "rfid-phy.h"
#include "rfid-net-device.h"
#include "rfid-identification.h"

NS_LOG_COMPONENT_DEFINE("RfidNetDevice");

namespace ns3
{
  NS_OBJECT_ENSURE_REGISTERED(RfidNetDevice);

  TypeId
  RfidNetDevice::GetTypeId (void)
  {
    static TypeId tid =
        TypeId("ns3::RfidNetDevice").SetParent<NetDevice>().AddConstructor<
            RfidNetDevice>().AddAttribute("Channel",
            "The channel attached to this device", PointerValue(),
            MakePointerAccessor(&RfidNetDevice::DoGetChannel),
            MakePointerChecker<RfidChannel>()).AddAttribute("Phy",
            "The PHY layer attached to this device.", PointerValue(),
            MakePointerAccessor(&RfidNetDevice::GetPhy, &RfidNetDevice::SetPhy),
            MakePointerChecker<RfidPhy>()).AddAttribute("Identification",
            "The identification layer attached to this device.",
            PointerValue(),
            MakePointerAccessor(&RfidNetDevice::GetIdentification,
                &RfidNetDevice::SetIdentification),
            MakePointerChecker<rfid::Identification>());
    return tid;
  }
  RfidNetDevice::RfidNetDevice (void)
  {
    NS_LOG_FUNCTION_NOARGS ();
  }
  RfidNetDevice::~RfidNetDevice (void)
  {
    NS_LOG_FUNCTION_NOARGS();
  }

  void
  RfidNetDevice::DoDispose (void)
  {
    NS_LOG_FUNCTION_NOARGS ();
    m_node = 0;
    m_Identification = 0;
    m_phy = 0;
    // chain up.
    NetDevice::DoDispose();
  }
  void
  RfidNetDevice::SetIfIndex (const uint32_t index)
  {
    NS_LOG_FUNCTION (index);
    m_ifIndex = index;
  }
  uint32_t
  RfidNetDevice::GetIfIndex (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_ifIndex;
  }
  Ptr<Channel>
  RfidNetDevice::GetChannel (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_channel;
  }
  bool
  RfidNetDevice::SetMtu (const uint16_t mtu)
  {
    NS_LOG_FUNCTION(mtu);
    m_mtu = mtu;
    NS_LOG_LOGIC("mtu = " << mtu);
    return true;
  }
  uint16_t
  RfidNetDevice::GetMtu (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_mtu;
  }
  void
  RfidNetDevice::SetAddress (Address address)
  {
    NS_LOG_FUNCTION(address);
    m_Identification->SetAddress(address);
  }
  Address
  RfidNetDevice::GetAddress (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_Identification->GetAddress();
  }
  bool
  RfidNetDevice::IsLinkUp (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return GetPhy() != 0 && m_linkUp;
  }
  void
  RfidNetDevice::AddLinkChangeCallback (Callback<void> callback)
  {
    NS_LOG_FUNCTION_NOARGS();
  }
  bool
  RfidNetDevice::IsBroadcast (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return true;
  }
  Address
  RfidNetDevice::GetBroadcast (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return Mac16Address("ff:ff");
  }
  bool
  RfidNetDevice::IsMulticast (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return true;
  }

  Ptr<RfidChannel>
  RfidNetDevice::DoGetChannel () const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_channel;
  }

  void
  RfidNetDevice::SetChannel (Ptr<RfidChannel> channel)
  {
    NS_LOG_FUNCTION(channel);
    m_channel = channel;
  }

  Ptr<rfid::Identification>
  RfidNetDevice::GetIdentification () const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_Identification;
  }

  void
  RfidNetDevice::SetIdentification (
      Ptr<rfid::Identification> Identification)
  {
    NS_LOG_FUNCTION(Identification);
    m_Identification = Identification;
    if (m_Identification and m_phy)
      m_Identification->SetRfidPhy(m_phy);
  }

  void
  RfidNetDevice::SetPhy (Ptr<RfidPhy> phy)
  {
    NS_LOG_FUNCTION(phy);
    m_phy = phy;
    if (m_Identification and m_phy)
      m_Identification->SetRfidPhy(m_phy);
  }

  Ptr<RfidPhy>
  RfidNetDevice::GetPhy (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_phy;
  }

  void
  RfidNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
  {
    NS_LOG_FUNCTION(&cb);
  }
  bool
  RfidNetDevice::SupportsSendFrom (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return false;
  }
  void
  RfidNetDevice::NotifyLinkUp (void)
  {
    NS_LOG_FUNCTION_NOARGS();
  }
  bool
  RfidNetDevice::SendFrom (Ptr<Packet> packet, const Address& source,
      const Address& dest, uint16_t protocolNumber)
  {
    NS_LOG_FUNCTION("source=" << source << ", dest=" << dest << ", proto=" << protocolNumber);
    // TODO : smunix : implement me
    return false;
  }
  bool
  RfidNetDevice::IsBridge (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return false;
  }
  bool
  RfidNetDevice::Send (Ptr<Packet> packet, const Address& dest,
      uint16_t protocolNumber)
  {
    NS_LOG_FUNCTION("dest=" << dest << ", proto=" << protocolNumber);
    //return m_Identification->Send (packet, dest, protocolNumber);
    return false;
  }

  bool
  RfidNetDevice::Send (Ptr<Packet> packet)
  {
   
    return m_Identification->Send (packet);
  }

  Ptr<Node>
  RfidNetDevice::GetNode (void) const
  {
    return m_node;
  }
  void
  RfidNetDevice::SetNode (Ptr<Node> node)
  {
    NS_LOG_FUNCTION (node);
    m_node = node;
  }
  bool
  RfidNetDevice::NeedsArp (void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return false;
  }
  void
  RfidNetDevice::SetReceiveCallback (ReceiveCallback cb)
  {
    NS_LOG_FUNCTION (&cb);
    m_rxCallback = cb;
  }
  Address
  RfidNetDevice::GetMulticast (Ipv4Address multicastGroup) const
  {
    NS_LOG_FUNCTION (multicastGroup);
    return Mac16Address::GetMulticast(multicastGroup);
  }
  Address
  RfidNetDevice::GetMulticast (Ipv6Address addr) const
  {
    NS_LOG_FUNCTION (addr);
    return Mac16Address::GetMulticast(addr);
  }
  bool
  RfidNetDevice::IsPointToPoint (void) const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return false;
  }
  void
  RfidNetDevice::ForwardUp (Ptr<Packet> packet, Mac16Address from, Mac16Address to, uint16_t proto)
  {
    NS_LOG_FUNCTION ("packet=" << packet << ", from=" << from << ", proto=" << proto);
    m_rxCallback (this, packet, proto, from);
  }

    void 
    RfidNetDevice::SetReceiveRfidCallback (ReceiveRfidCallback cb)
    {
    NS_LOG_FUNCTION (&cb);
    m_rxRfidCallback = cb;
    }    

    void 
    RfidNetDevice::ForwardRfidUp (Ptr<Packet> packet, uint16_t header)
    {
    NS_LOG_FUNCTION ("packet=" << packet <<  ", header=" << header);
    m_rxRfidCallback (this, packet, header);
    }
}

