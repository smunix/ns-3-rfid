#ifndef RFID_NET_DEVICE_H
#define RFID_NET_DEVICE_H


#include "ns3/net-device.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "rfid-phy-reader.h"
#include "rfid-mac-reader.h"
#include "rfid-channel.h"

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

 virtual void SetIfIndex (const uint32_t index) =0;
  virtual uint32_t GetIfIndex (void) const =0;
  virtual Ptr<Channel> GetChannel (void) const =0;
  virtual void SetAddress (Address address) =0 ;
  virtual Address GetAddress (void) const =0;
  virtual bool SetMtu (const uint16_t mtu) =0;
  virtual uint16_t GetMtu (void) const =0;
  virtual bool IsLinkUp (void) const =0;
  virtual void AddLinkChangeCallback (Callback<void> callback) =0;
  virtual bool IsBroadcast (void) const =0;
  virtual Address GetBroadcast (void) const =0;
  virtual bool IsMulticast (void) const =0;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const =0; 
  virtual bool IsPointToPoint (void) const =0;
  virtual bool IsBridge (void) const =0;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) =0;
  virtual Ptr<Node> GetNode (void) const =0;
  virtual void SetNode (Ptr<Node> node) =0;
  virtual bool NeedsArp (void) const =0;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb) =0;

  virtual Address GetMulticast (Ipv6Address addr) const =0;

  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber) =0;
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb) =0;
  virtual bool SupportsSendFrom (void) const =0;
private:
Ptr<RfidMacReader> m_mac;
Ptr<RfidPhyReader> m_phy;
Ptr<RfidChannel> m_channel;
Ptr<Node> m_node;


};
} //namespace ns3
#endif /* RFID_NET_DEVICE_H */
