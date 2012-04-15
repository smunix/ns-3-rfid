#ifndef RFID_PHY_READER_H
#define RFID_PHY_READER_H
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/object.h"
namespace ns3 {
class RfidChannel;
class RfidMacReader;



class RfidPhyReader : public Object 
{
public:
  static TypeId GetTypeId (void);

RfidPhyReader ();
virtual ~RfidPhyReader ();

void Send(Ptr<Packet> p);
void Receive(Ptr<Packet> p);
Ptr<Object> GetDevice (void);
private:
int m_frquency;
int m_modulation;
int m_coding;
int m_rate;
Ptr<Object> m_device;

Ptr<RfidChannel> channel;
Ptr<RfidMacReader> mac;
};

} //namespace ns3

#endif // RFID_PHY_READER_H

