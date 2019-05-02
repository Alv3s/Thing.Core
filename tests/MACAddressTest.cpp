#include "MACAddressTest.h"
#include "../MACAddress.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			void MACAddressTest::SetUp()
			{
			}

			void MACAddressTest::TearDown()
			{
			}

			TEST_F(MACAddressTest, OctectConstructorTest)
			{
				const uint8_t octet1 = 0X0A;
				const uint8_t octet2 = 0x0B;
				const uint8_t octet3 = 0x0C;
				const uint8_t octet4 = 0x0D;
				const uint8_t octet5 = 0x0E;
				const uint8_t octet6 = 0x0F;

				const uint8_t address[] = { octet1, octet2, octet3, octet4, octet5, octet6 };

				MACAddress macAddress(octet1, octet2, octet3, octet4, octet5, octet6);

				ASSERT_EQ(octet1, macAddress.GetOctet(0));
				ASSERT_EQ(octet2, macAddress.GetOctet(1));
				ASSERT_EQ(octet3, macAddress.GetOctet(2));
				ASSERT_EQ(octet4, macAddress.GetOctet(3));
				ASSERT_EQ(octet5, macAddress.GetOctet(4));
				ASSERT_EQ(octet6, macAddress.GetOctet(5));

				ASSERT_TRUE(memcmp(address, macAddress.GetAddress(), 6) == 0);
			}

			TEST_F(MACAddressTest, BufferConstructorTest)
			{
				const uint8_t octet1 = 0X0A;
				const uint8_t octet2 = 0x0B;
				const uint8_t octet3 = 0x0C;
				const uint8_t octet4 = 0x0D;
				const uint8_t octet5 = 0x0E;
				const uint8_t octet6 = 0x0F;

				const uint8_t address[] = { octet1, octet2, octet3, octet4, octet5, octet6 };

				MACAddress macAddress((uint8_t*)address);

				ASSERT_EQ(octet1, macAddress.GetOctet(0));
				ASSERT_EQ(octet2, macAddress.GetOctet(1));
				ASSERT_EQ(octet3, macAddress.GetOctet(2));
				ASSERT_EQ(octet4, macAddress.GetOctet(3));
				ASSERT_EQ(octet5, macAddress.GetOctet(4));
				ASSERT_EQ(octet6, macAddress.GetOctet(5));

				ASSERT_TRUE(memcmp(address, macAddress.GetAddress(), 6) == 0);
			}
		}
	}
}