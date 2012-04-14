#include "select-header.h"

namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED (SelectHeader);

SelectHeader::SelectHeader()
	:m_header(0x8)
{}

SelectHeader::~SelectHeader()
{}

uint8_t
SelectHeader::GetHeader() const
{
  return m_header;
}

TypeId 
SelectHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SelectHeader")
    .SetParent<Header> ()
    .AddConstructor<SelectHeader> ()
  ;
  return tid;
}

TypeId 
SelectHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}


void 
SelectHeader::Print (std::ostream &os) const
{
  os << " Select" << std::hex << m_header << std::dec;
}


uint32_t 
SelectHeader::GetSerializedSize (void) const
{
	return 1;
}

void
SelectHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_header);
}
uint32_t
SelectHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_header = i.ReadU8 ();

  return GetSerializedSize ();
}


}//namespace ns3
