/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/rfid.h"

// An essential include is test.h
#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/vector.h"
#include "ns3/node.h"
#include "ns3/object-factory.h"
#include "ns3/constant-position-mobility-model.h"
#include <iostream>


#  define DO_LOG(x) do { std::cout << x << std::endl; } while (false)

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// This is an Channel TestCase.
class RfidTestCaseChannel : public TestCase
{
public:
  RfidTestCaseChannel ();
  virtual
  ~RfidTestCaseChannel ();

private:
  virtual void
  DoRun (void);
  void
  CreateOne (Vector pos, Ptr<RfidChannel> channel);
  void
  SendOnePacket (Ptr<RfidNetDevice> dev);
  void
  CreateTwo (Vector pos, Ptr<RfidChannel> channel);
  bool Sink (Ptr<NetDevice>, Ptr<const Packet>, uint16_t);
private:
  typedef std::vector<Ptr<RfidNetDevice> > NetDevices;
  NetDevices m_devices;
  ObjectFactory m_tagIdentification;
  ObjectFactory m_readerIdentification;
  uint16_t m_count;
  uint16_t m_tagNumber;  

};

// Add some help text to this case to describe what it is intended to test
RfidTestCaseChannel::RfidTestCaseChannel () :
    TestCase("Rfid test case (does nothing)")
{
  m_tagIdentification.SetTypeId("ns3::rfid::TagIdentification");
  m_readerIdentification.SetTypeId("ns3::rfid::ReaderIdentification");
  m_count = 0;
  m_tagNumber = 0;
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
RfidTestCaseChannel::~RfidTestCaseChannel ()
{
}
bool RfidTestCaseChannel::Sink (Ptr<NetDevice> dev, Ptr<const Packet> pkt, uint16_t header)
{
  DO_LOG ("Received : dev=" << dev->GetAddress() << ", pkt=" << pkt << ", header=" << header );
  m_count++;
  return true;
}
void
RfidTestCaseChannel::SendOnePacket (Ptr<RfidNetDevice> dev)
{
  Ptr<Packet> p = Create<Packet>(0);
  dev->Send(p);
}

void
RfidTestCaseChannel::CreateOne (Vector pos, Ptr<RfidChannel> channel)
{
  Ptr<Node> node = CreateObject<Node>();
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<
      ConstantPositionMobilityModel>();
  Ptr<RfidNetDevice> dev = CreateObject<RfidNetDevice>();
  Ptr<rfid::ReaderIdentification> readerId = m_readerIdentification.Create<
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
  readerId->SetTagNumber (m_tagNumber);

  dev->SetReceiveRfidCallback(MakeCallback(&RfidTestCaseChannel::Sink, this));
  readerId->SetForwardUpCallback(MakeCallback (&RfidNetDevice::ForwardRfidUp, dev));
  phy->SetForwardUpCallback(MakeCallback (&rfid::ReaderIdentification::Receive, readerId));
  phy->SetReceivingCallback(MakeCallback (&rfid::ReaderIdentification::SetReceiving, readerId));
  phy->SetCollisionCallback(MakeCallback (&rfid::ReaderIdentification::SetCollision, readerId));

  Simulator::Schedule(Seconds(1.0), &RfidTestCaseChannel::SendOnePacket, this,
      dev);
  m_devices.push_back (dev);
  
}

void
RfidTestCaseChannel::CreateTwo (Vector pos, Ptr<RfidChannel> channel)
{
  Ptr<Node> node = CreateObject<Node>();
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<
      ConstantPositionMobilityModel>();
  Ptr<RfidNetDevice> dev = CreateObject<RfidNetDevice>();
  Ptr<rfid::TagIdentification> tagId = m_tagIdentification.Create<
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

  dev->SetReceiveRfidCallback(MakeCallback(&RfidTestCaseChannel::Sink, this));
  tagId->SetForwardUpCallback(MakeCallback (&RfidNetDevice::ForwardRfidUp, dev));
  phy->SetForwardUpCallback(MakeCallback (&rfid::TagIdentification::Receive, tagId));
  phy->SetReceivingCallback(MakeCallback (&rfid::TagIdentification::SetReceiving, tagId));
  phy->SetCollisionCallback(MakeCallback (&rfid::TagIdentification::SetCollision, tagId));
 /*Simulator::Schedule(Seconds(1.0), &RfidTestCaseChannel::SendOnePacket, this,
      dev);*/
  m_devices.push_back (dev);
  m_tagNumber++;
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
RfidTestCaseChannel::DoRun (void)
{
  Ptr<RfidChannel> channel = CreateObject<RfidChannel>();
  uint32_t devicesN = channel->GetNDevices();
  NS_TEST_ASSERT_MSG_EQ(devicesN, 0, "O devices have been attached so far");
  

  CreateTwo(Vector(5.0, 0.0, 0.0), channel);
  CreateTwo(Vector(10.0, 0.0, 0.0), channel);
  CreateTwo(Vector(11.0, 0.0, 0.0), channel);
  CreateTwo(Vector(12.0, 0.0, 0.0), channel);
  CreateTwo(Vector(13.0, 0.0, 0.0), channel);
  CreateOne(Vector(0.0, 0.0, 0.0), channel);
 // CreateTwo(Vector(15.0, 0.0, 0.0), channel);
  Simulator::Run();
  Simulator::Destroy();
  Simulator::Stop(Seconds(10.0));
  NS_TEST_ASSERT_MSG_EQ(6, m_count, "received exactly 6 packets");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class RfidTestSuite : public TestSuite
{
public:
  RfidTestSuite ();
};

RfidTestSuite::RfidTestSuite () :
    TestSuite("rfid", UNIT)
{
  AddTestCase(new RfidTestCaseChannel);
}

// Do not forget to allocate an instance of this TestSuite
static RfidTestSuite rfidTestSuite;

