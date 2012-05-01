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

#include "ns3/log.h"
#include "ns3/queue.h"
#include "epc-header.h"
#include "rfid-phy.h"
#include "rfid-tag-identification.h"

NS_LOG_COMPONENT_DEFINE("RfidTagIdentification");

namespace ns3
{
  namespace rfid
  {
    NS_OBJECT_ENSURE_REGISTERED(TagIdentification);

    TypeId
    TagIdentification::GetTypeId (void)
    {
      static TypeId tid = TypeId("ns3::rfid::TagIdentification").SetParent<Object>().AddConstructor<TagIdentification> ();
      return tid;
    }
    TagIdentification::TagIdentification (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    }
    TagIdentification::~TagIdentification (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    }
    Address
    TagIdentification::GetAddress (void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_address;
    }
    void
    TagIdentification::SetAddress (Address address)
    {
      NS_LOG_FUNCTION(address);
      m_address = Mac16Address::ConvertFrom (address);
    }
    void
    TagIdentification::SetQueue (Ptr<Queue> queue)
    {
      NS_LOG_FUNCTION(queue);
      m_queue = queue;
    }
    Ptr<Queue>
    TagIdentification::GetQueue (void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_queue;
    }
    void
    TagIdentification::SetRfidPhy(Ptr<RfidPhy> phy)
    {
      NS_LOG_FUNCTION(phy);
      m_phy = phy;
    }
    Ptr<RfidPhy>
    TagIdentification::GetRfidPhy(void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_phy;
    }
    void
    TagIdentification::SetForwardUpCallback (ForwardUpCb const &cb)
    {
      NS_LOG_FUNCTION(&cb);
      m_forwardUp = cb;
    }
    bool TagIdentification::Send (Ptr<Packet> packet, const Address& dest,
                uint16_t protocolNumber)
    {
      NS_LOG_FUNCTION ("packet=" << &packet << ", from=" << GetAddress() << ", to=" << dest << ", proto=" << protocolNumber);
      AddEpcHeader (packet, GetAddress(), dest, protocolNumber);
      GetRfidPhy ()->Send (packet);
      return true;
    }
    void
    TagIdentification::AddEpcHeader (Ptr<Packet> packet, Address const &from, Address const &dest, uint16_t protocolNumber)
    {
      NS_LOG_FUNCTION ("packet=" << &packet << ", from=" << from << ", to=" << dest << ", proto=" << protocolNumber);
      EpcHeader eh;
      eh.SetFromAddress(Mac16Address::ConvertFrom(from));
      eh.SetToAddress(Mac16Address::ConvertFrom(dest));
      eh.SetProtocolNumber(protocolNumber);
      packet->AddHeader(eh);
    }
    void
    TagIdentification::Receive(Ptr<Packet> packet)
    {
      EpcHeader eh;
      packet->RemoveHeader(eh);
      Mac16Address from = eh.GetFromAddress();
      Mac16Address to = eh.GetToAddress();
      uint16_t proto = eh.GetProtocolNumber();
      NS_LOG_FUNCTION ("packet=" << packet << ", from=" << from << ", to=" << to << ", proto=" << proto);
      m_forwardUp (packet, from, to, proto);
    }
  }
}

