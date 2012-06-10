/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/rfid-helper.h"
#include "ns3/rfid.h"
#include "ns3/rfid-net-device.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/vector.h"
#include "ns3/node.h"
#include "ns3/object-factory.h"
#include "ns3/constant-position-mobility-model.h"
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("RfidExample");


using namespace ns3;

  typedef std::vector<Ptr<RfidNetDevice> > NetDevices;

bool 
Sink (Ptr<NetDevice> dev, Ptr<const Packet> pkt, uint16_t header)
{
  NS_LOG_UNCOND ("Received : dev=" << dev->GetAddress() << ", pkt=" << pkt << ", header=" << header );
  return true;
}

void
SendOnePacket (Ptr<RfidNetDevice> dev)
{
  Ptr<Packet> p = Create<Packet>(0);
  dev->Send(p);
}

NetDevices
CreateReader (Vector pos, Ptr<RfidChannel> channel, NetDevices devices, int tagNumber)
{
  Ptr<Node> node = CreateObject<Node>();
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<
      ConstantPositionMobilityModel>();
  Ptr<RfidNetDevice> dev = CreateObject<RfidNetDevice>();
  Ptr<rfid::ReaderIdentification> readerId = CreateObject<
      rfid::ReaderIdentification>();
  Ptr<RfidPhy> phy = CreateObject<RfidPhy>();

  mobility->SetPosition(pos);

  readerId->SetRfidPhy(phy);
  readerId->SetAddress(Mac16Address::Allocate());
  dev->SetIdentification(readerId);

  phy->SetMobility(node);
  node->AggregateObject(mobility);
  phy->SetChannel(channel);
  phy->SetDevice(dev);
  node->AddDevice(dev);

  readerId->SetEquipement (rfid::READER);
  readerId->SetTagNumber (tagNumber);

  dev->SetReceiveRfidCallback(MakeCallback(&Sink));
  readerId->SetForwardUpCallback(MakeCallback (&RfidNetDevice::ForwardRfidUp, dev));
  phy->SetForwardUpCallback(MakeCallback (&rfid::ReaderIdentification::Receive, readerId));
  phy->SetReceivingCallback(MakeCallback (&rfid::ReaderIdentification::SetReceiving, readerId));
  phy->SetCollisionCallback(MakeCallback (&rfid::ReaderIdentification::SetCollision, readerId));

  Simulator::Schedule(Seconds(1.0),SendOnePacket, dev);
  devices.push_back (dev);
  return devices;
}

NetDevices
CreateTag (Vector pos, Ptr<RfidChannel> channel, NetDevices devices)
{
  Ptr<Node> node = CreateObject<Node>();
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<
      ConstantPositionMobilityModel>();
  Ptr<RfidNetDevice> dev = CreateObject<RfidNetDevice>();
  Ptr<rfid::TagIdentification> tagId = CreateObject<
      rfid::TagIdentification>();
  Ptr<RfidPhy> phy = CreateObject<RfidPhy>();

  mobility->SetPosition(pos);

  tagId->SetRfidPhy(phy);
  tagId->SetAddress(Mac16Address::Allocate());
  dev->SetIdentification(tagId);

  phy->SetMobility(node);
  node->AggregateObject(mobility);
  phy->SetChannel(channel);
  phy->SetDevice(dev);
  node->AddDevice(dev);
  
  tagId->SetEquipement (rfid::TAG);

  dev->SetReceiveRfidCallback(MakeCallback(&Sink));
  tagId->SetForwardUpCallback(MakeCallback (&RfidNetDevice::ForwardRfidUp, dev));
  phy->SetForwardUpCallback(MakeCallback (&rfid::TagIdentification::Receive, tagId));
  phy->SetReceivingCallback(MakeCallback (&rfid::TagIdentification::SetReceiving, tagId));
  phy->SetCollisionCallback(MakeCallback (&rfid::TagIdentification::SetCollision, tagId));

  devices.push_back (dev);
  return devices;
}

int 
main (int argc, char *argv[])
{

  bool verbose = true;
  int tagNumber = 1;
  NetDevices devices;

  CommandLine cmd;
  cmd.AddValue ("tagNumber", "Number of tag to be identified", tagNumber);
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);


  Ptr<RfidChannel> channel = CreateObject<RfidChannel>();
  uint32_t devicesN = channel->GetNDevices();
  NS_ASSERT_MSG (devicesN == 0, "O devices have been attached so far");

  devices = CreateReader(Vector(0.0, 0.0, 0.0), channel, devices, tagNumber);

  for (int i=1; i<= tagNumber; i++)
  {
    devices = CreateTag(Vector(5.0, 0.0, 0.0), channel, devices);
  }


  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


