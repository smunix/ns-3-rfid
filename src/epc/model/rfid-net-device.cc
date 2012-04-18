#include "rfid-net-device.h"
#include "rfid-phy-reader.h"
#include "rfid-mac-reader.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/log.h"


NS_LOG_COMPONENT_DEFINE ("RfidNetDevice");
namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED (RfidNetDevice);
TypeId
RfidNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RfidNetDevice")
    .SetParent<NetDevice> ()
;
  return tid;
}

RfidNetDevice::RfidNetDevice()
{}

RfidNetDevice::~RfidNetDevice()
{}

/*
Ptr<NetDeviceContainer>
RfidNetDevice::Install (Ptr<RfidPhyReader> phy,Ptr<RfidMacReader> mac, NodeContainer c) 
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<RfidNetDevice> device = CreateObject<RfidNetDevice> ();
      device->SetMac (mac);
      device->SetPhy (phy);
      node->AddDevice (device);
      devices.Add (device);
    }
  return devices;
}
*/

void
RfidNetDevice::SetMac (Ptr<RfidMacReader> mac)
{
  m_mac = mac;
}

void
RfidNetDevice::SetPhy (Ptr<RfidPhyReader> phy)
{
  m_phy = phy;
}

void
RfidNetDevice::SetChannel (Ptr<RfidChannel> channel)
{
  m_channel = channel;
}

Ptr<Node>
RfidNetDevice::GetNode (void) const
{
  return m_node;
}



/// virtual fonctions ************************
// j'ai défini le contenu des fonctions d'une manière aléatoire, puisque je n'aurais pas besoin de ses fonctions, juste pour assurer la bonne compilation 


void
RfidNetDevice::SetIfIndex (const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t
RfidNetDevice::GetIfIndex (void) const
{
  return m_ifIndex;
}

Ptr<Channel>
RfidNetDevice::GetChannel (void) const
{
  //return m_phy->GetChannel ();
return 0;
}


void
RfidNetDevice::SetAddress (Address address)
{
 // m_mac->SetAddress (Mac48Address::ConvertFrom (address));
}


Address
RfidNetDevice::GetAddress (void) const
{
  //return m_mac->GetAddress ();
  return m_address;
}

bool
RfidNetDevice::SetMtu (const uint16_t mtu)
{

  m_mtu = mtu;
  return true;
}
uint16_t
RfidNetDevice::GetMtu (void) const
{
  return m_mtu;
}
bool
RfidNetDevice::IsLinkUp (void) const
{
//  return m_phy != 0 && m_linkUp;
	return true;
}
void
RfidNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
 // m_linkChanges.ConnectWithoutContext (callback);
}
bool
RfidNetDevice::IsBroadcast (void) const
{
  return true;
}

Address
RfidNetDevice::GetBroadcast (void) const
{
  //return Mac48Address::GetBroadcast ();
 return m_address;
}
bool
RfidNetDevice::IsMulticast (void) const
{
  return true;
}


Address
RfidNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
 // return Mac48Address::GetMulticast (multicastGroup);
 return m_address;
}
Address 
RfidNetDevice::GetMulticast (Ipv6Address addr) const
{
 // return Mac48Address::GetMulticast (addr);
 return m_address;
}

bool
RfidNetDevice::IsPointToPoint (void) const
{
  return false;
}
bool
RfidNetDevice::IsBridge (void) const
{
  return false;
}
bool
RfidNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  /*NS_ASSERT (Mac48Address::IsMatchingType (dest));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_mac->NotifyTx (packet);
  m_mac->Enqueue (packet, realTo);*/
  return true;
}

void
RfidNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
 // CompleteConfig ();
}
bool
RfidNetDevice::NeedsArp (void) const
{
  return true;
}
void
RfidNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_forwardUp = cb;
}





bool
RfidNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  /*NS_ASSERT (Mac48Address::IsMatchingType (dest));
  NS_ASSERT (Mac48Address::IsMatchingType (source));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);
  Mac48Address realFrom = Mac48Address::ConvertFrom (source);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_mac->NotifyTx (packet);
  m_mac->Enqueue (packet, realTo, realFrom);*/

  return true;
}

void
RfidNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
/*  m_promiscRx = cb;
  m_mac->SetPromisc();*/
}

bool
RfidNetDevice::SupportsSendFrom (void) const
{
  return false;
// return m_mac->SupportsSendFrom ();
}

}
