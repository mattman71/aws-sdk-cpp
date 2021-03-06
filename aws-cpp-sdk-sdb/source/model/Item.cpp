/*
* Copyright 2010-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*
*  http://aws.amazon.com/apache2.0
*
* or in the "license" file accompanying this file. This file is distributed
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied. See the License for the specific language governing
* permissions and limitations under the License.
*/
#include <aws/sdb/model/Item.h>
#include <aws/core/utils/xml/XmlSerializer.h>
#include <aws/core/utils/StringUtils.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>

#include <utility>

using namespace Aws::SimpleDB::Model;
using namespace Aws::Utils::Xml;
using namespace Aws::Utils;

Item::Item() : 
    m_nameHasBeenSet(false),
    m_alternateNameEncodingHasBeenSet(false),
    m_attributesHasBeenSet(false)
{
}

Item::Item(const XmlNode& xmlNode) : 
    m_nameHasBeenSet(false),
    m_alternateNameEncodingHasBeenSet(false),
    m_attributesHasBeenSet(false)
{
  *this = xmlNode;
}

Item& Item::operator =(const XmlNode& xmlNode)
{
  XmlNode resultNode = xmlNode;

  if(!resultNode.IsNull())
  {
    XmlNode nameNode = resultNode.FirstChild("Name");
    if(!nameNode.IsNull())
    {
      m_name = StringUtils::Trim(nameNode.GetText().c_str());
      m_nameHasBeenSet = true;
    }
    XmlNode alternateNameEncodingNode = resultNode.FirstChild("AlternateNameEncoding");
    if(!alternateNameEncodingNode.IsNull())
    {
      m_alternateNameEncoding = StringUtils::Trim(alternateNameEncodingNode.GetText().c_str());
      m_alternateNameEncodingHasBeenSet = true;
    }
    XmlNode attributesNode = resultNode.FirstChild("Attribute");
    if(!attributesNode.IsNull())
    {
      XmlNode attributeMember = attributesNode;
      while(!attributeMember.IsNull())
      {
        m_attributes.push_back(attributeMember);
        attributeMember = attributeMember.NextNode("Attribute");
      }

      m_attributesHasBeenSet = true;
    }
  }

  return *this;
}

void Item::OutputToStream(Aws::OStream& oStream, const char* location, unsigned index, const char* locationValue) const
{
  if(m_nameHasBeenSet)
  {
      oStream << location << index << locationValue << ".Name=" << StringUtils::URLEncode(m_name.c_str()) << "&";
  }
  if(m_alternateNameEncodingHasBeenSet)
  {
      oStream << location << index << locationValue << ".AlternateNameEncoding=" << StringUtils::URLEncode(m_alternateNameEncoding.c_str()) << "&";
  }
  if(m_attributesHasBeenSet)
  {
      unsigned attributesIdx = 1;
      for(auto& item : m_attributes)
      {
        Aws::StringStream attributesSs;
        attributesSs << location << index << locationValue << ".Attribute." << attributesIdx++;
        item.OutputToStream(oStream, attributesSs.str().c_str());
      }
  }
}

void Item::OutputToStream(Aws::OStream& oStream, const char* location) const
{
  if(m_nameHasBeenSet)
  {
      oStream << location << ".Name=" << StringUtils::URLEncode(m_name.c_str()) << "&";
  }
  if(m_alternateNameEncodingHasBeenSet)
  {
      oStream << location << ".AlternateNameEncoding=" << StringUtils::URLEncode(m_alternateNameEncoding.c_str()) << "&";
  }
  if(m_attributesHasBeenSet)
  {
      unsigned attributesIdx = 1;
      for(auto& item : m_attributes)
      {
        Aws::StringStream attributesSs;
        attributesSs << location <<  ".Attribute." << attributesIdx++;
        item.OutputToStream(oStream, attributesSs.str().c_str());
      }
  }
}
