#include "ack-header.h"

namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED (AckHeader);

AckHeader::AckHeader()
	:m_header(1) // 01
{}

AckHeader::~AckHeader()
{}

uint8_t
AckHeader::GetHeader() const
{
  return m_header;
}

TypeId 
AckHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AckHeader")
    .SetParent<Header> ()
    .AddConstructor<AckHeader> ()
  ;
  return tid;
}

TypeId 
AckHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}


void 
AckHeader::Print (std::ostream &os) const
{
  os << " Ack" << std::hex << m_header << std::dec;
}


uint32_t 
AckHeader::GetSerializedSize (void) const
{
	return 1;
}

void
AckHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_header);
}
uint32_t
AckHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_header = i.ReadU8 ();

  return GetSerializedSize ();
}


}//namespace ns3
