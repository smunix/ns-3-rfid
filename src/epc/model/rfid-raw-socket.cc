#include "rfid-raw-socket.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("RfidRawSocket");
namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED (RfidRawSocket);
TypeId
RfidRawSocket::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RfidRawSocket")
    .SetParent<Socket> ()
;
  return tid;
}

RfidRawSocket::RfidRawSocket()
{}

RfidRawSocket::~RfidRawSocket()
{}






}
