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
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "rfid-reader-identification.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

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

    void
    TagIdentification::SetEquipement (int eq)
    {
      m_eq=eq;
      m_header = 0x00;
      m_first = true;
      if (m_eq == TAG)
      { 
        SetState (ARBITRATE);
      }
    
    }
    int
    TagIdentification::GetEquipement (void) const
    { 
      return m_eq;
    }

    void
    TagIdentification::SetState (int sta)
    {
      m_sta=sta;   
    }
    int
    TagIdentification::GetState (void) const
    {
      return m_sta;
    }


    void
    TagIdentification::SetInitialConfiguration (void)
    {
      m_rn = rand () % 0xFFFF;
    }

   void
    TagIdentification::SetEquipementState (Ptr<Packet> packet, uint16_t header) 
    {  
      if (m_first == true ) { SetInitialConfiguration();}
      if (GetEquipement () == TAG )
        { m_next = false;std::cout << "Idle1 " << m_eq << " " << m_sta << std::endl;
                 switch ( header )
                 {
                  case (0x08):
                  if (m_first == true ) { m_slot_counter = pow (2, RemoveEpcHeader (packet)) - 1;  m_first = false;}
                  if ( GetState () == ARBITRATE && m_slot_counter == 0 ) 
                      { SetState(REPLY); m_next = true;SetInitialConfiguration(); AddEpcHeader (packet,m_rn);}
                  else if ( m_slot_counter != 0) 
                        { m_slot_counter -= 1; 
                          // j'ai un problème avec cette fonction :(
                         /* void (rfid::ReaderIdentification::*fp)(Ptr<Packet> packet) = &rfid::ReaderIdentification::SendQueryRep;
                          Simulator::ScheduleNow(fp,Ptr<rfid::TagIdentification>, packet);*/
                        }
                  break;
                  case (0x00):
                  if ( GetState () == ARBITRATE && m_slot_counter == 0 ) 
                      { SetState(REPLY); m_next = true; AddEpcHeader (packet,m_rn);}
                  else if ( m_slot_counter != 0) { m_slot_counter -= 1;}
                  break;
                  case (0x01):
                  if (GetState () == REPLY && RemoveEpcHeader (packet) == m_rn) {SetState(ACKNOWLEDGED); m_next = true;}
                  break;
                  default:
                  SetState(ARBITRATE);
                  break; 
                 }
        if (m_next == true) 
          {
            Send(packet);
          }
        }
      
    }

    bool TagIdentification::Send (Ptr<Packet> packet)
    {
      NS_LOG_FUNCTION ("packet=" << &packet);
      std::cout << "Idle2 " << m_eq << " " << m_sta << std::endl;
      GetRfidPhy ()->Send (packet);
      return true;
    }
 
    void
    TagIdentification::AddEpcHeader (Ptr<Packet> packet, uint16_t header)
    {
      NS_LOG_FUNCTION ("packet=" << &packet << ", header=" << header);
      EpcHeader eh;
      eh.SetHeader(header);
      packet->AddHeader(eh);
    }

    uint16_t
    TagIdentification::RemoveEpcHeader (Ptr<Packet> packet)
    {
      NS_LOG_FUNCTION ("packet=" << &packet);
      EpcHeader eh;
      packet->RemoveHeader(eh);
      return eh.GetHeader();
    }

    void
    TagIdentification::Receive(Ptr<Packet> packet)
    {
      m_header = RemoveEpcHeader (packet);
      //std::cout << GetAddress () << " got a message" << std::endl ; 
      NS_LOG_FUNCTION ("packet=" << packet << ", header=" << m_header);
      m_forwardUp (packet, m_header);
      SetEquipementState(packet, m_header);
    }
  }
}

