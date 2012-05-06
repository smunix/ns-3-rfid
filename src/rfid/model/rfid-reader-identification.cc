/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 ABID
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
 * Author: ABID AYOUB <abid.ayoub@gmail.com>
 */

#include "ns3/log.h"
#include "ns3/queue.h"
#include "epc-header.h"
#include "rfid-phy.h"
#include "rfid-reader-identification.h"
#include <iostream>

NS_LOG_COMPONENT_DEFINE("RfidReaderIdentification");

namespace ns3
{
  namespace rfid
  {

    NS_OBJECT_ENSURE_REGISTERED(ReaderIdentification);

    TypeId
    ReaderIdentification::GetTypeId (void)
    {
      static TypeId tid = TypeId("ns3::rfid::ReaderIdentification").SetParent<Object>().AddConstructor<ReaderIdentification> ();
      return tid;
    }
    ReaderIdentification::ReaderIdentification (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    }
    ReaderIdentification::~ReaderIdentification (void)
    {
      NS_LOG_FUNCTION_NOARGS();
    }
    Address
    ReaderIdentification::GetAddress (void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_address;
    }
    void
    ReaderIdentification::SetAddress (Address address)
    {
      NS_LOG_FUNCTION(address);
      m_address = Mac16Address::ConvertFrom (address);
    }
    void
    ReaderIdentification::SetQueue (Ptr<Queue> queue)
    {
      NS_LOG_FUNCTION(queue);
      m_queue = queue;
    }
    Ptr<Queue>
    ReaderIdentification::GetQueue (void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_queue;
    }
    void
    ReaderIdentification::SetRfidPhy(Ptr<RfidPhy> phy)
    {
      NS_LOG_FUNCTION(phy);
      m_phy = phy;
    }
    Ptr<RfidPhy>
    ReaderIdentification::GetRfidPhy(void) const
    {
      NS_LOG_FUNCTION_NOARGS();
      return m_phy;
    }
    void
    ReaderIdentification::SetForwardUpCallback (ForwardUpCb const &cb)
    {
      NS_LOG_FUNCTION(&cb);
      m_forwardUp = cb;
    }

    void
    ReaderIdentification::SetEquipement (int eq)
    {
      m_eq=eq;
      m_header = 0;
      m_first = true;
      if (m_eq == READER)
      { 
        SetState (IDLE_READER);
      }
    }
    int
    ReaderIdentification::GetEquipement (void) const
    { 
      return m_eq;
    }

    void
    ReaderIdentification::SetState (int sta)
    {
      m_sta=sta;   
    }
    int
    ReaderIdentification::GetState (void) const
    {
      return m_sta;
    }

    void
    ReaderIdentification::NextStep (Ptr<Packet> packet, uint16_t header) 
    {  std::cout << "Idle1 " << m_eq << " " << m_sta << std::endl;
      if (GetEquipement () == READER)
        {
                 switch (GetState ())
                 {
                  case (IDLE_READER):
                  SetState(QUERY);
                  m_header=1000;
/*m_dataSize = 5; m_data = "1100";
//memcpy (m_data, "Hello", 5);
packet = Create<Packet> (reinterpret_cast<uint8_t *> (m_data), m_dataSize);

m_dataSize = 5; m_data = 1100;
packet = Create<Packet> (m_data, m_dataSize);*/
                  break;
                  case (QUERY):
                  SetState(ACK);
                  m_header=0001;
                  break;
                  default:
                  SetState(IDLE_READER);
                  std::cout << "Idle2 " << m_eq << " " << m_sta << std::endl;
                  break; 
                 }
        AddEpcHeader (packet,m_header); 
        if (m_first == false) {Send(packet);}
        }
      
    }

    bool ReaderIdentification::Send (Ptr<Packet> packet)
    {
      NS_LOG_FUNCTION ("packet=" << &packet);

      if (m_first == true ) 
      {  
        NextStep(packet, m_header);
        std::cout << "Idle2 " << m_eq << " " << m_sta << std::endl;
        m_first = false ; GetRfidPhy ()->Send (packet); 
       }
          else if ( !(m_sta == IDLE_READER && m_eq == READER) ) 
                {
                  std::cout << "Idle2 " << m_eq << " " << m_sta << std::endl; 
                  GetRfidPhy ()->Send (packet);
                }
      return true;
    }
 
 void
    ReaderIdentification::AddEpcHeader (Ptr<Packet> packet, uint16_t m_header)
    {
      NS_LOG_FUNCTION ("packet=" << &packet << ", header=" << m_header);
      EpcHeader eh;
      eh.SetHeader(m_header);
      packet->AddHeader(eh);
    }


    void
    ReaderIdentification::Receive(Ptr<Packet> packet)
    {
      //std::cout << GetAddress () << " got a message" << std::endl ;   
      m_forwardUp (packet, 0);
      NextStep(packet, 0);
    }
  }
}

