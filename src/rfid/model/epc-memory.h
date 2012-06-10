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

#ifndef EPC_MEMORY_H_
#define EPC_MEMORY_H_

#include "ns3/object.h"



namespace ns3
{
  class EpcMemory : public Object
  {
  public:
    static TypeId GetTypeId (void);

    EpcMemory (void);
    virtual ~EpcMemory (void);

    void SetKillPassword ( uint32_t killPassword );
    uint32_t GetKillPassword ( void ) const ;

    void SetAccessPassword ( uint32_t accessPassword );
    uint32_t GetAccessPassword ( void ) const ;

    void SetStoredCRC ( uint16_t storedCRC );
    uint16_t GetStoredCRC ( void ) const ;

    void SetStoredPC ( uint16_t storedPC );
    uint16_t GetStoredPC ( void ) const ;

    void SetEpc ( uint32_t epc );
    uint32_t GetEpc ( void ) const ;

    void SetUserInformation ( uint32_t UserInformation );
    uint32_t GetUserInformation ( void ) const ;
  };
}

#endif /* EPC_MEMORY_H_ */
