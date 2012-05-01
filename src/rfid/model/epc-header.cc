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

#include <iomanip>
#include <iostream>
#include "ns3/assert.h"
#include "ns3/log.h"

#include "epc-header.h"

NS_LOG_COMPONENT_DEFINE("EpcHeader");

namespace ns3
{
  NS_OBJECT_ENSURE_REGISTERED(EpcHeader);

  TypeId
  EpcHeader::GetTypeId (void)
  {
    static TypeId tid =
        TypeId("ns3::EpcHeader").SetParent<Header>().AddConstructor<EpcHeader>();
    return tid;
  }
  TypeId
  EpcHeader::GetInstanceTypeId (void) const
  {
    return GetTypeId();
  }
  EpcHeader::EpcHeader (void)
  {

  }
  EpcHeader::~EpcHeader (void)
  {

  }
  uint32_t
  EpcHeader::GetSerializedSize (void) const
  {
    return Mac16Address::LENGTH + Mac16Address::LENGTH + sizeof (m_protocol);
  }
  void
  EpcHeader::Serialize (Buffer::Iterator start) const
  {
    Buffer::Iterator i = start;
    Write(i, m_from);
    Write(i, m_to);
    i.WriteHtonU16 (m_protocol);
  }
  void
  EpcHeader::Write(Buffer::Iterator &i, Mac16Address const &ad) const
  {
    Mac16Address::Buffer_t epc;
    ad.CopyTo(epc);
    i.Write(epc, Mac16Address::LENGTH);
  }
  void
  EpcHeader::Read(Buffer::Iterator &i, Mac16Address &ad)
  {
    Mac16Address::Buffer_t epc;
    i.Read(epc, Mac16Address::LENGTH);
    ad.CopyFrom(epc);
  }
  uint32_t
  EpcHeader::Deserialize (Buffer::Iterator start)
  {
    Buffer::Iterator i = start;
    Read(i, m_from);
    Read(i, m_to);
    m_protocol = i.ReadNtohU16 ();
    return GetSerializedSize();
  }
  void
  EpcHeader::Print (std::ostream &os) const
  {
    os << m_from << " " << m_to << " " << m_protocol;
  }
  void
  EpcHeader::SetFromAddress (Mac16Address const &address)
  {
    m_to = address;
  }
  Mac16Address
  EpcHeader::GetFromAddress (void) const
  {
    return m_from;
  }

  void
  EpcHeader::SetToAddress (Mac16Address const &address)
  {
    m_to = address;
  }
  Mac16Address
  EpcHeader::GetToAddress (void) const
  {
    return m_to;
  }

  void
  EpcHeader::SetProtocolNumber (uint16_t proto)
  {
    m_protocol = proto;
  }
  uint16_t
  EpcHeader::GetProtocolNumber (void) const
  {
    return m_protocol;
  }
}

