
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
#include "rfid-preamble.h"


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
      SetState (IDLE_TAG);
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
      m_rn = rand () % 0xFFFF; std::cout << " rn " << m_rn << std::endl;
    }
     
    uint16_t 
    TagIdentification::CalculateSlotCounter (uint16_t q)
    {
      uint16_t slot;
      slot = pow (2, q) - 1;std::cout << " slot " << slot << std::endl;
      if (  slot != 0) { slot = rand () % (slot + 1) ;  } 
      return slot;
    }

    void 
    TagIdentification::SetStateIfTimeout (void)
    {
      if (!GetReceiving () ) {SetState (ARBITRATE);}
    }

    Ptr<Packet>
    TagIdentification::GenerateRnMessage (Ptr<Packet> packet)
    {
      if ( m_slot_counter == 0 ) 
      { 
        SetState(REPLY); 
        m_next = true; 
        AddEpcHeader (packet,m_rn); 
        m_duration += GetPacketDuration ( 16 , m_rn) + m_conf.GetTagPreamble (m_conf.GetM ());
        Simulator::Schedule (MicroSeconds (m_duration + m_conf.GetT2()), &TagIdentification::SetStateIfTimeout, this);
      }
      std::cout << " slot counter " << m_slot_counter << std::endl;
      return packet;
    }

    Ptr<Packet> 
    TagIdentification::SetResponseToQuery ( Ptr<Packet> packet)
    {
      m_slot_counter = CalculateSlotCounter (RemoveEpcHeader (packet));
      SetState(ARBITRATE);
      m_conf.SetM (1.0);
      m_conf.SetDR (8.0);
      return GenerateRnMessage (packet);
    } 

    Ptr<Packet> 
    TagIdentification::SetResponseToQueryRep ( Ptr<Packet> packet)
    {
      m_slot_counter --;
      return GenerateRnMessage (packet);
    } 

    Ptr<Packet> 
    TagIdentification::SetResponseToAck ( Ptr<Packet> packet)
    {
      if (RemoveEpcHeader (packet) == m_rn) 
      { 
        SetState(ACKNOWLEDGED); 
        m_next = true;
        m_duration += m_conf.GetTagPreamble (m_conf.GetM ());
        Simulator::Schedule (MicroSeconds (m_duration + m_conf.GetT2()), &TagIdentification::SetStateIfTimeout, this);
      }
      else 
        {
          SetState (ARBITRATE);
          m_slot_counter = 0X7FFF;
        }
      return packet;
    } 

    Ptr<Packet> 
    TagIdentification::SetResponseToQueryAdjust (Ptr<Packet> packet)
    {
      m_slot_counter = CalculateSlotCounter (RemoveEpcHeader (packet));
      SetState(ARBITRATE);
      return GenerateRnMessage (packet);
    }

    void
    TagIdentification::SetEquipementState (Ptr<Packet> packet, uint16_t header) 
    {  
      m_duration = 0 ;
 
      if (GetEquipement () == TAG )
        { 
            m_next = false;
            std::cout << "Tag_Statut " << m_eq << " " << m_sta << std::endl;
            switch ( GetState () )
            {
              case (IDLE_TAG):
                 if ( header == 0x0A ) 
                 {
                   std::cout << "************* Tag have received select message ****************" << std::endl;
                   SetState(READY); 
                   SetInitialConfiguration();
                 }
              break;


              case (READY):
                 switch ( header )
                 {
                   case ( 0x0A ): 
                      SetState(READY);
                   break;

                   case ( 0x08 ): 
                      packet = SetResponseToQuery ( packet);
                   break;

                   default:
                      SetState(READY);
                   break;
                 }                    
              break;


              case (ARBITRATE):
                 switch ( header )
                 {
                   case ( 0x0A ): 
                      SetState(READY);
                   break;

                   case ( 0x08 ): 
                      packet = SetResponseToQuery ( packet);
                   break;
                      
                   case ( 0x00): 
                      packet = SetResponseToQueryRep ( packet);
                   break;

                   case ( 0x09):    
                      packet = SetResponseToQueryAdjust (packet);
                   break;

                   default:
                      SetState(ARBITRATE);
                   break;
                 }
              break;


              case (REPLY):
                 switch ( header )
                 {
                   case ( 0x0A ): 
                      SetState(READY);
                   break;

                   case ( 0x08 ): 
                      packet = SetResponseToQuery ( packet);
                   break;

                   case ( 0x01): 
                      packet = SetResponseToAck ( packet);
                   break;

                   case ( 0x09): 
                      packet = GenerateRnMessage (packet);
                   break;

                   default:
                      SetState(ARBITRATE);
                   break;
                 }
              break;


              case (ACKNOWLEDGED):
                 switch ( header )
                 {
                   case ( 0x0A ): 
                      SetState(READY);
                   break;

                   case ( 0x08 ): 
                      packet = SetResponseToQuery ( packet);
                   break;

                   case ( 0x00): 
                      SetState(READY);
                   break;

                   case ( 0xC1): 
                      //not implemented yet
                   break;

                   case ( 0x01): 
                      packet = SetResponseToAck ( packet);
                   break;

                   case ( 0x09): 
                      SetState(READY);
                   break;

                   default:
                      SetState(ARBITRATE);
                   break;
                 }
              break;


              default:
                 std::cout << " ******** Not Matching Message ******** " << std::endl;
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
      std::cout << "Tag_Statut " << m_eq << " " << m_sta << std::endl;
      GetRfidPhy ()->Send (packet ,m_duration );
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
      NS_LOG_FUNCTION ("packet=" << packet << ", header=" << m_header);
      m_forwardUp (packet, m_header);
      SetEquipementState ( packet, m_header);
    }
    
        void 
    TagIdentification::SetReceiving (bool rcv)
    {
      m_rcv = rcv;
    }
    bool 
    TagIdentification::GetReceiving (void) const
    {
      return m_rcv;
    }

  }
}

