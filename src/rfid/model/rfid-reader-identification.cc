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
#include "ns3/simulator.h"

#include <iostream>
#include <stdlib.h>



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
      m_header = 0x00;
      m_first = true;
      m_receive = true; 
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
    ReaderIdentification::SetInitialConfiguration (void)
    {
      m_q = rand() % 0x0F; std::cout << " ***m_q*** " << m_q << std::endl ; 
    }

    void
    ReaderIdentification::SendQueryRep (Ptr<Packet> packet)
    { 
      if (m_receive == false ) 
      {  std::cout << " ******** Retransmission ******** " << std::endl;
        AddEpcHeader (packet,0x00);
        Send (packet);
      }
    }

    void
    ReaderIdentification::SetEquipementState (Ptr<Packet> packet, uint16_t header) 
    {  std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
      if (GetEquipement () == READER)
        {
                 switch (GetState ())
                 {
                  case (IDLE_READER):
                  SetState(QUERY);
                  m_header=0x08;
                  AddEpcHeader (packet,m_q); 
                  break;
                  case (QUERY):
                  SetState(ACK);
                  m_header=0x01;
                  break;
                  default:
                  SetState(IDLE_READER);
                  std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
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
        SetInitialConfiguration();
        SetEquipementState(packet, m_header);
        std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
        m_first = false ;
        m_receive = false ; 
        GetRfidPhy ()->Send (packet);
       }
          else if ( !(m_sta == IDLE_READER && m_eq == READER) ) 
                {
                  std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
                  m_receive = false ;
                  GetRfidPhy ()->Send (packet);
                  
                }
       Simulator::Schedule (NanoSeconds (51001), &ReaderIdentification::SendQueryRep , this , packet);
      return true;
    }
 
 void
    ReaderIdentification::AddEpcHeader (Ptr<Packet> packet, uint16_t header)
    {
      NS_LOG_FUNCTION ("packet=" << &packet << ", header=" << header);
      EpcHeader eh;
      eh.SetHeader(header);
      packet->AddHeader(eh);
    }

    uint16_t
    ReaderIdentification::RemoveEpcHeader (Ptr<Packet> packet)
    {
      NS_LOG_FUNCTION ("packet=" << &packet);
      EpcHeader eh;
      packet->RemoveHeader(eh);
      return eh.GetHeader();
    }

    void
    ReaderIdentification::Receive(Ptr<Packet> packet)
    { 
      m_receive = true ; 
      //std::cout << GetAddress () << " got a message" << std::endl ;   
      m_forwardUp (packet, -1);
      Simulator::Schedule ( MicroSeconds (1), &ReaderIdentification::SetEquipementState , this , packet, 0);
    }
  }
}

