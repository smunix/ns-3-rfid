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

#ifndef RFID_CONFIGURATION_H_
#define RFID_CONFIGURATION_H_


#include "ns3/object.h"


namespace ns3
{
  class RfidConfiguration : public Object
   {

    public:
      static TypeId
      GetTypeId (void);

      RfidConfiguration (void);
      virtual
      ~RfidConfiguration (void);
      
      void SetData0 (double data0);
      double GetData0 (void) const ;

      void SetX (double x);
      double GetX (void) const ;

      double GetDelimiter (void) const ; 
      double GetData1 (void) const ;    
      double GetRTcal (void) const ;
      double GetTRcal (void) const ; 
      double GetPreambleDuration (void) const ; 
      double GetFrameSyncDuration (void) const ; 

      void SetReaderFrequency (double reader_frequency);
      double GetReaderFrequency (void) const ; 

      void SetTagFrequency (double tag_frequency);
      double GetTagFrequency (void) const ;   

      void SetDR (double dr);
      double GetDR (void) const ;

      void SetM (double m);
      double GetM (void) const ;

      double GetBLF (void) const ;
      double GetTpri (void) const ;

      double GetT1 (void) const ;

      void SetT2 (double t2);
      double GetT2 (void) const ;

      void SetT3 (double t3);
      double GetT3 (void) const ;

      void SetT4 (double t4);
      double GetT4 (void) const ;

      double GetTagRate (void) const ;
      double GetReaderRate (void) const ;

      double GetTagPreamble (int trext) const;
      double GetTagEndSignaling (void) const;

    private:
      double m_data0;
      double m_x; 
      double m_readerFrequency;
      double m_tagFrequency;
      double m_dr;
      double m_m;
      double m_t2;
      double m_t3;
      double m_t4;
      
   };
}

#endif /* RFID_CONFIGURATION_H_ */
