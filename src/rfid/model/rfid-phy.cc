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

#include "rfid-phy.h"
#include "rfid-channel.h"
#include "rfid-net-device.h"

NS_LOG_COMPONENT_DEFINE("RfidPhy");

namespace ns3
{
  NS_OBJECT_ENSURE_REGISTERED(RfidPhy);

  TypeId
  RfidPhy::GetTypeId(void)
  {
    static TypeId tid = TypeId("ns3::RfidPhy").SetParent<Object>().AddConstructor<RfidPhy> ();
    return tid;
  }

  RfidPhy::~RfidPhy(void)
  {
    NS_LOG_FUNCTION_NOARGS ();
  }

  void
  RfidPhy::SetChannel(Ptr<RfidChannel> channel)
  {
    NS_LOG_FUNCTION (channel);
    m_channel = channel;
    m_channel->Add (this);
  }

  Ptr<RfidChannel>
  RfidPhy::GetChannel (void) const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_channel;
  }
  void
  RfidPhy::SetDevice(Ptr<RfidNetDevice> dev)
  {
    NS_LOG_FUNCTION (dev);
    m_device = dev;
    m_device->SetPhy(this);
  }

  Ptr<RfidNetDevice>
  RfidPhy::GetDevice (void) const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_device;
  }
  void
  RfidPhy::SetMobility(Ptr<Object> mobility)
  {
    NS_LOG_FUNCTION (mobility);
    m_mobility = mobility;
  }
  Ptr<Object>
  RfidPhy::GetMobility(void) const
  {
    NS_LOG_FUNCTION_NOARGS();
    return m_mobility;
  }
  void
  RfidPhy::Send(Ptr<Packet> pkt)
  {
    NS_LOG_FUNCTION (pkt);
    m_channel->Send (pkt, this);
  }
  void
  RfidPhy::Recv (Ptr<Packet> pkt)
  {
    NS_LOG_FUNCTION (pkt);
    m_forwardUp (pkt);
  }
  void
  RfidPhy::SetForwardUpCallback (ForwardUpCb cb)
  {
    m_forwardUp = cb;
  }
}
