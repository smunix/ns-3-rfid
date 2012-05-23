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
#include "rfid-net-device.h"
#include "rfid-phy.h"
#include "rfid-channel.h"
#include "rfid-identification.h"

NS_LOG_COMPONENT_DEFINE("RfidChannel");

namespace ns3
{
  NS_OBJECT_ENSURE_REGISTERED(RfidChannel);

  TypeId
  RfidChannel::GetTypeId (void)
  {
    static TypeId tid =
        TypeId("ns3::RfidChannel").SetParent<Channel>().AddConstructor<
            RfidChannel>();
    return tid;
  }

  RfidChannel::~RfidChannel (void)
  {
    NS_LOG_FUNCTION_NOARGS();
  }
  uint32_t
  RfidChannel::GetNDevices (void) const
  {
    NS_LOG_FUNCTION_NOARGS();

    return m_phys.size ();
  }

  Ptr<NetDevice>
  RfidChannel::GetDevice (uint32_t i) const
  {
    NS_LOG_FUNCTION(i);
    NS_ASSERT (i < GetNDevices());
    return m_phys[i]->GetDevice();
  }

  uint16_t
  RfidChannel::GetChannelNumber (void) const
  {
    NS_LOG_FUNCTION_NOARGS();

    static uint16_t channelNumber = RfidChannel::MakeUid<uint16_t>();
    return channelNumber;
  }

  void
  RfidChannel::Send (Ptr<Packet> pkt, Ptr<RfidPhy> phy)
  {
    NS_LOG_FUNCTION ("packet=" << pkt << ", phy=" << phy);
    Sender<RfidPhy> s = MakeSender (phy, pkt);
    std::for_each(m_phys.begin(), m_phys.end(), s);
    
  }
  void
  RfidChannel::Add (Ptr<RfidPhy> phy)
  {
    Unary<RfidPhy> f = MakeFinder (phy);
    RfidPhyIt it = std::find_if(m_phys.begin(), m_phys.end(), f);
    if (m_phys.end() == it)
    {
      m_phys.push_back(phy);
    }
  }
 
} // namespace ns3


