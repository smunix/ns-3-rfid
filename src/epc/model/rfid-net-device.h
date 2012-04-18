#ifndef RFID_NET_DEVICE_H
#define RFID_NET_DEVICE_H


#include "ns3/net-device.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "rfid-phy-reader.h"
#include "rfid-mac-reader.h"
#include "rfid-channel.h"


#include "ns3/packet.h"
#include "ns3/traced-callback.h"
#include "ns3/mac48-address.h"
#include <string>




namespace ns3{
class RfidMacReader;
class RfidPhyReader;
class RfidChannel;


class RfidNetDevice: public NetDevice
{
public:
static TypeId GetTypeId (void);
RfidNetDevice();
~RfidNetDevice();

// Ptr<NetDeviceContainer> Install (Ptr<RfidPhyReader> phy,Ptr<RfidMacReader> mac, NodeContainer c);
void SetMac (Ptr<RfidMacReader> mac);
void SetPhy (Ptr<RfidPhyReader> phy);
void SetChannel (Ptr<RfidChannel> channel);


  // inherited from NetDevice base class.
  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool IsBridge (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);

  virtual Address GetMulticast (Ipv6Address addr) const;

  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

private:
Ptr<RfidMacReader> m_mac;
Ptr<RfidPhyReader> m_phy;
Ptr<RfidChannel> m_channel;
Ptr<Node> m_node;
/*
  virtual void DoDispose (void);
  virtual void DoStart (void);
*/
Mac48Address m_address;
NetDevice::ReceiveCallback m_forwardUp;
uint32_t m_ifIndex;
mutable uint16_t m_mtu;
};
} //namespace ns3
#endif /* RFID_NET_DEVICE_H */
