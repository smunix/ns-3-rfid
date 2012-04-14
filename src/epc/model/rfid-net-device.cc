#include "rfid-net-device.h"
#include "rfid-phy-reader.h"
#include "rfid-mac-reader.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/node.h"

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

}
