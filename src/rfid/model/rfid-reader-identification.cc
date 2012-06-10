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
#include "rfid-preamble.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"

#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <algorithm>



NS_LOG_COMPONENT_DEFINE("RfidReaderIdentification");

namespace ns3
{
  namespace rfid
  {

    NS_OBJECT_ENSURE_REGISTERED(ReaderIdentification);

    TypeId
    ReaderIdentification::GetTypeId (void)
    {
      static TypeId tid = TypeId("ns3::rfid::ReaderIdentification")
        .SetParent<Object>()
        .AddConstructor<ReaderIdentification> ()  
        .AddAttribute ("MaxCollision",
                   "Max collision allowed before sending QueryAdjust",
                   UintegerValue (1),
                   MakeUintegerAccessor (&ReaderIdentification::m_maxCollisionAllowed),
                   MakeUintegerChecker<uint32_t> ())          
        .AddAttribute ("MaxNoResponse",
                   "Max no response allowed before sending QueryAdjust",
                   UintegerValue (100),
                   MakeUintegerAccessor (&ReaderIdentification::m_maxNoResponseAllowed),
                   MakeUintegerChecker<uint32_t> ())       
        .AddAttribute ("Target",
                   "Target in select message",
                   UintegerValue (rfid::S0),
                   MakeUintegerAccessor (&ReaderIdentification::m_target),
                   MakeUintegerChecker<uint16_t> ())  
        .AddAttribute ("Action",
                   "Action to receive select message",
                   UintegerValue (0x01),
                   MakeUintegerAccessor (&ReaderIdentification::m_action),
                   MakeUintegerChecker<uint16_t> ())     
        .AddAttribute ("MemBanck",
                   "Selected memory bank",
                   UintegerValue (rfid::TAGID),
                   MakeUintegerAccessor (&ReaderIdentification::m_memBank),
                   MakeUintegerChecker<uint16_t> ())  
        .AddAttribute ("Pointer",
                   "Address to the mask marked by the pointer",
                   UintegerValue (0),
                   MakeUintegerAccessor (&ReaderIdentification::m_pointer),
                   MakeUintegerChecker<uint32_t> ())  
        .AddAttribute ("Length",
                   "Length of mask",
                   UintegerValue (0),
                   MakeUintegerAccessor (&ReaderIdentification::m_length),
                   MakeUintegerChecker<uint16_t> ())  
        .AddAttribute ("Mask",
                   "Mask to match in the tag memory",
                   UintegerValue (0),
                   MakeUintegerAccessor (&ReaderIdentification::m_mask),
                   MakeUintegerChecker<uint16_t> ())    
        .AddAttribute ("Truncate",
                   "Able or disable epc truncation",
                   UintegerValue (0),
                   MakeUintegerAccessor (&ReaderIdentification::m_truncate),
                   MakeUintegerChecker<uint16_t> ())   
        .AddAttribute ("DivideRatio",
                   "equal to 8 or 64/3",
                   DoubleValue (8.0),
                   MakeDoubleAccessor (&ReaderIdentification::m_dr),
                   MakeDoubleChecker<double> ()) 
        .AddAttribute ("Modulation",
                   "equal to 1, 2, 4 or 8",
                   UintegerValue (1),
                   MakeUintegerAccessor (&ReaderIdentification::m_m),
                   MakeUintegerChecker<uint16_t> ())  
        .AddAttribute ("TRext",
                   "Preamble type",
                   UintegerValue (0),
                   MakeUintegerAccessor (&ReaderIdentification::m_trext),
                   MakeUintegerChecker<uint16_t> ())  
        .AddAttribute ("Sel",
                   "Selected flag type",
                   UintegerValue (0x0),
                   MakeUintegerAccessor (&ReaderIdentification::m_sel),
                   MakeUintegerChecker<uint16_t> ()) 
        .AddAttribute ("Session",
                   "Session type",
                   UintegerValue (S0),
                   MakeUintegerAccessor (&ReaderIdentification::m_session),
                   MakeUintegerChecker<uint16_t> ()) 
        .AddAttribute ("ValueTarget",
                   "Value of session",
                   UintegerValue (A),
                   MakeUintegerAccessor (&ReaderIdentification::m_valueTarget),
                   MakeUintegerChecker<uint16_t> ()) 
;
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
      SetReceiving (true);
      SetCollision (false);
      m_tag_number = 0;
      m_conf.SetM(m_m);
      m_conf.SetDR(m_dr);
      SetState (IDLE_READER);
      m_noResponseCounter = 0;
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
    ReaderIdentification::SetQForQuery (void)
    {
      srand(time(0));
      m_qf = ((double)rand()/(double)RAND_MAX)* 0x0F;
      m_q = (int)m_qf;
      std::cout << " ***m_q*** " << m_qf << " " << m_q << std::endl ;
    }

    void
    ReaderIdentification::SetQForQueryAdjust (void)
    {
      if (!GetCollision ()) 
      {
        m_qf = std::max ( 0.0, m_qf-0.5);
        m_q = (int)m_qf;
        std::cout << " ***m_q*** " << m_qf << " " << m_q << std::endl ;
      }
      else 
         {
           m_qf = std::min ( 15.0, m_qf+0.5);   
           m_q = (int)m_qf;
           std::cout << " ***m_q*** " << m_qf << " " << m_q << std::endl ;
         }
    }

    void
    ReaderIdentification::SendQueryRepOrAdjust (void)
    { 
        Ptr<Packet> packet = Create<Packet> (0);
      if (!GetReceiving ()) 
      { 
        std::cout << " no response +++++++++++++++++ " << m_noResponseCounter << " " <<  m_maxNoResponseAllowed << std::endl;
        m_duration = 0;
        if (m_maxNoResponseAllowed ==  m_noResponseCounter || GetCollision ())
        {
                std::cout << " ******** QueryAdjust ******** " << std::endl;
                m_noResponseCounter = 0;
                SetState(QUERY_REP);
                SetQForQueryAdjust ();
                AddEpcHeader (packet,m_q);
                AddEpcHeader (packet,m_session);
                m_header = 0x09;
                m_duration = GetPacketDuration ( 4 , m_header) + GetPacketDuration ( 4 , m_q) + GetPacketDuration ( 2 , m_session) + m_conf.GetFrameSyncDuration ();
                SetCollision (false);
        }
        else 
        {
                std::cout << " ******** QueryRep ******** " << std::endl;
                m_noResponseCounter++;
                SetState(QUERY_REP);
                AddEpcHeader (packet,m_session);
                m_header = 0x00;
                m_duration = GetPacketDuration ( 2 , m_header) + GetPacketDuration ( 2 , m_session) + m_conf.GetFrameSyncDuration ();
        }
        AddEpcHeader (packet,m_header);
        Send (packet);
      }
    }

    void
    ReaderIdentification::SetEquipementState (Ptr<Packet> packet, uint16_t header) 
    { 
      m_duration = 0 ; 
      std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
      if (GetEquipement () == READER)
        {
                 switch (GetState ())
                 {

                  case (IDLE_READER):
                  SetState(SELECT);
                  AddEpcHeader (packet,m_truncate); 
                  AddEpcHeader (packet,m_mask);
                  AddEpcHeader (packet,m_length);
                  AddEpcHeader (packet,m_pointer);
                  AddEpcHeader (packet,m_memBank);
                  AddEpcHeader (packet,m_action);
                  AddEpcHeader (packet,m_target);
                  m_header=0x0A;
                  m_duration += GetPacketDuration ( 4 , m_header) + GetPacketDuration ( 3 , m_target) + GetPacketDuration ( 3 , m_action) + GetPacketDuration ( 2 , m_memBank); 
                  m_duration += GetPacketDuration ( 8 , m_pointer) + GetPacketDuration ( 8 , m_length) + GetPacketDuration ( 8 , m_mask) + GetPacketDuration ( 1 , m_truncate);
                  m_preamble = RFID_FRAME_SYNC;
                  break;

                  case (SELECT):
                  SetState(QUERY);
                  SetQForQuery();
                  AddEpcHeader (packet,m_q);
                  AddEpcHeader (packet,m_valueTarget);
                  AddEpcHeader (packet,m_session);
                  AddEpcHeader (packet,m_sel);
                  AddEpcHeader (packet,m_trext);
                  AddEpcHeader (packet,m_conf.ModulationToHeader (m_m));
                  AddEpcHeader (packet,m_conf.RatioToHeader (m_dr));
                  m_header=0x08;
                  m_duration += GetPacketDuration ( 4 , m_header) + GetPacketDuration ( 1 , m_conf.RatioToHeader (m_dr)) + GetPacketDuration ( 2 , m_conf.ModulationToHeader (m_m));
                  m_duration += GetPacketDuration ( 1 , m_trext) + GetPacketDuration ( 2 , m_sel) + GetPacketDuration ( 2 , m_session) + GetPacketDuration ( 2 ,m_valueTarget) + GetPacketDuration ( 4 , m_q);
                  m_preamble = RFID_PREAMBLE;
                  break;

                  case (QUERY):
                  SetState(ACK);
                  m_header=0x01;
                  m_duration += GetPacketDuration ( 2 , m_header) + GetPacketDuration ( 16 , RemoveEpcHeader( packet-> Copy ()));
                  m_preamble = RFID_FRAME_SYNC;
                  break;

                  case (QUERY_REP):
                  SetState(ACK);
                  m_header=0x01;
                  m_duration += GetPacketDuration ( 2 , m_header) + GetPacketDuration ( 16 , RemoveEpcHeader( packet-> Copy ()));
                  m_preamble = RFID_FRAME_SYNC;
                  break;

                  default:
                  // NAK if we have problem with verification crc
                  SetState(QUERY_REP);
                  m_noResponseCounter = 0;
                  m_tag_number += 1; 
                  std::cout << "************************ " << m_tag_number << " tags are identified ************************" << std::endl;
                  std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
                  SetReceiving (false);
                  if (m_tag_number != GetTagNumber ()) {SendQueryRepOrAdjust ();}
                  break; 

                 }
        if (GetState () != QUERY_REP )
           {  
                AddEpcHeader (packet,m_header); 
                m_duration += (m_preamble == RFID_PREAMBLE) ? m_conf.GetPreambleDuration () : m_conf.GetFrameSyncDuration ();
                if (m_first != true) {Send(packet);}
           }
        }
      
    }

    bool ReaderIdentification::Send (Ptr<Packet> packet)
    { 
      NS_LOG_FUNCTION ("packet=" << &packet);
      if (m_first == true ) 
      { 
        SetEquipementState(packet, m_header);
        std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
        m_first = false ;
        SetReceiving (false); 
        GetRfidPhy ()->Send (packet ,m_duration );
        Simulator::Schedule (MicroSeconds (m_duration + m_conf.GetT4()), &ReaderIdentification::SetEquipementState , this , packet, m_header);
       }
          else if ( !(m_tag_number == GetTagNumber () && m_eq == READER) ) 
                {
                  std::cout << "Reader_Statut " << m_eq << " " << m_sta << std::endl;
                  SetReceiving (false);
                  GetRfidPhy ()->Send (packet ,m_duration );
                  m_id = Simulator::Schedule (MicroSeconds (m_duration + m_conf.GetT1()), &ReaderIdentification::SendQueryRepOrAdjust , this);
                }
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
      Simulator::Cancel (m_id);   
      m_forwardUp (packet, -1);
      SetEquipementState ( packet, 0);
     // Simulator::Schedule (MicroSeconds (m_conf.GetT2() - 50), &ReaderIdentification::SetEquipementState , this, packet, 0);
    }

    void 
    ReaderIdentification::SetReceiving (bool rcv)
    {
      m_rcv = rcv;
    }
    bool 
    ReaderIdentification::GetReceiving (void) const
    {
      return m_rcv;
    }

    void 
    ReaderIdentification::SetCollision (bool collision)
    {
      m_collision = collision;
    }
    bool 
    ReaderIdentification::GetCollision (void) const
    {
      return m_collision;
    }
  }
}

