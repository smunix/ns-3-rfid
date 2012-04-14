#ifndef RFID_CHANNEL_H
#define RFID_CHANNEL_H

#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/net-device.h"
namespace ns3 {
class RfidChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
  RfidChannel ();
  virtual ~RfidChannel ();
  void Send(Ptr<Packet> p);

  virtual uint32_t GetNDevices (void) const = 0;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const = 0;

private:
void Receive (uint32_t i, Ptr<Packet> packet) const;
};

} // namespace ns3

#endif /* RFID_CHANNEL_H */
