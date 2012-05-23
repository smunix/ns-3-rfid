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

#ifndef RFID_PHY_H_
#define RFID_PHY_H_

#include <stdint.h>
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "rfid-phy-state.h"


namespace ns3
{
  class RfidPhy : public Object
  {

  public:
    typedef Callback<void, Ptr<Packet> > ForwardUpCb;
    typedef Callback<void, bool> ReceivingCb;
  public:
    static TypeId
    GetTypeId(void);
    
    RfidPhy (void);
    virtual ~RfidPhy (void);
    virtual void SetChannel (Ptr<class RfidChannel> channel);
    virtual Ptr<class RfidChannel> GetChannel (void) const;
    virtual void SetDevice (Ptr<class RfidNetDevice> dev);
    virtual Ptr<class RfidNetDevice> GetDevice (void) const;
    virtual void SetMobility (Ptr<Object> mobility);
    virtual Ptr<Object> GetMobility (void) const;
    virtual void Send (Ptr<Packet> pkt, double duration);
    virtual void StartRecv (Ptr<Packet> pkt, double duration);
    virtual void EndRecv (Ptr<Packet> pkt);

    void SetForwardUpCallback (ForwardUpCb cb);
    void SetReceivingCallback (ReceivingCb cb);

    void SetDuration ( double duration);
    double GetDuration (void) const;
  private:
    Ptr<class RfidChannel> m_channel;
    Ptr<class RfidNetDevice> m_device;
    Ptr<Object> m_mobility;
    ForwardUpCb m_forwardUp;
    ReceivingCb m_receiving;
    Ptr<class RfidPhyState> m_state;
    double m_duration;
  };
}

#endif /* RFID_PHY_H_ */
