/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

<<<<<<< HEAD

=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
/*

 * Copyright (c) 2006,2007 INRIA

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

 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>

 */

#include "ns3/tag.h"

#include "ns3/packet.h"

#include "ns3/uinteger.h"

#include <iostream>


using namespace ns3;


// define this class in a public header

class MyTag : public Tag

{

public:

  static TypeId GetTypeId (void);

  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (TagBuffer i) const;

  virtual void Deserialize (TagBuffer i);

  virtual void Print (std::ostream &os) const;

  
  // these are our accessors to our tag structure

  void SetSimpleValue (uint8_t value);

  uint8_t GetSimpleValue (void) const;

<<<<<<< HEAD
  void SetSourceAddress (uint8_t value);

  uint8_t GetSourceAddress (void) const;

 void SetDestAddress (uint8_t value);

  uint8_t GetDestAddress (void) const;


private:

  uint8_t m_simpleValue;
 uint8_t m_srcaddr;
 uint8_t m_destaddr;
=======
private:

  uint8_t m_simpleValue;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710

};


<<<<<<< HEAD
  void MyTag::SetSourceAddress (uint8_t value)
       {
        
        m_srcaddr = value;
}

  uint8_t MyTag::GetSourceAddress (void) const
{
        return m_srcaddr;
}

 void MyTag::SetDestAddress (uint8_t value)
{
             m_destaddr = value;
}

 uint8_t MyTag::GetDestAddress (void) const
{
         return m_destaddr;
}


=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
TypeId 

MyTag::GetTypeId (void)

{

  static TypeId tid = TypeId ("ns3::MyTag")

    .SetParent<Tag> ()

    .AddConstructor<MyTag> ()

    .AddAttribute ("SimpleValue",

                   "A simple value",

                   EmptyAttributeValue (),

                   MakeUintegerAccessor (&MyTag::GetSimpleValue),

                   MakeUintegerChecker<uint8_t> ())

    ;

<<<<<<< HEAD
/*
tid.AddAttribute ("Src Index",

                   "A simple index",

                   EmptyAttributeValue (),

                   MakeUintegerAccessor (&MyTag::GetSourceAddress),

                   MakeUintegerChecker<uint8_t> ())

    ;

tid.AddAttribute ("Dest Index",

                   "A simple index",

                   EmptyAttributeValue (),

                   MakeUintegerAccessor (&MyTag::GetDestAddress),

                   MakeUintegerChecker<uint8_t> ())

    ;
*/

=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
  return tid;

}

TypeId 

MyTag::GetInstanceTypeId (void) const

{

  return GetTypeId ();

}

uint32_t 
MyTag::GetSerializedSize (void) const

{

<<<<<<< HEAD
  return 3;
=======
  return 1;
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710

}

void 

MyTag::Serialize (TagBuffer i) const

{

  i.WriteU8 (m_simpleValue);
<<<<<<< HEAD
i.WriteU8 (m_srcaddr);
i.WriteU8 (m_destaddr);
=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710

}

void 

MyTag::Deserialize (TagBuffer i)

{

  m_simpleValue = i.ReadU8 ();
<<<<<<< HEAD
m_srcaddr = i.ReadU8 ();
m_destaddr = i.ReadU8 ();
=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710

}

void 

MyTag::Print (std::ostream &os) const

{

  os << "v=" << (uint32_t)m_simpleValue;
<<<<<<< HEAD
 os << "s=" << (uint32_t)m_srcaddr;
 os << "d=" << (uint32_t)m_destaddr;
=======
>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710

}

void 

MyTag::SetSimpleValue (uint8_t value)

{

  m_simpleValue = value;

}

uint8_t 
MyTag::GetSimpleValue (void) const

{

  return m_simpleValue;
<<<<<<< HEAD
=======

>>>>>>> 2d380d4462567a9faa321e5be3a9eb9be9b2e710
}

