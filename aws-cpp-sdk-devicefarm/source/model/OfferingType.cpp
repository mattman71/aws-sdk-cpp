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
#include <aws/devicefarm/model/OfferingType.h>
#include <aws/core/utils/HashingUtils.h>
#include <aws/core/Globals.h>
#include <aws/core/utils/EnumParseOverflowContainer.h>

using namespace Aws::Utils;

static const int RECURRING_HASH = HashingUtils::HashString("RECURRING");

namespace Aws
{
  namespace DeviceFarm
  {
    namespace Model
    {
      namespace OfferingTypeMapper
      {


        OfferingType GetOfferingTypeForName(const Aws::String& name)
        {
          int hashCode = HashingUtils::HashString(name.c_str());
          if (hashCode == RECURRING_HASH)
          {
            return OfferingType::RECURRING;
          }
          EnumParseOverflowContainer* overflowContainer = Aws::GetEnumOverflowContainer();
          if(overflowContainer)
          {
            overflowContainer->StoreOverflow(hashCode, name);
            return static_cast<OfferingType>(hashCode);
          }

          return OfferingType::NOT_SET;
        }

        Aws::String GetNameForOfferingType(OfferingType enumValue)
        {
          switch(enumValue)
          {
          case OfferingType::RECURRING:
            return "RECURRING";
          default:
            EnumParseOverflowContainer* overflowContainer = Aws::GetEnumOverflowContainer();
            if(overflowContainer)
            {
              return overflowContainer->RetrieveOverflow(static_cast<int>(enumValue));
            }

            return "";
          }
        }

      } // namespace OfferingTypeMapper
    } // namespace Model
  } // namespace DeviceFarm
} // namespace Aws
