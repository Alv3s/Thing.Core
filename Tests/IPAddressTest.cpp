#include "IPAddressTest.h"
#include "../IP.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			void IPAddressTest::SetUp()
			{
			}

			void IPAddressTest::TearDown()
			{
			}

			TEST_F(IPAddressTest, OctectConstructorTest)
			{
				const uint8_t octet1 = 192;
				const uint8_t octet2 = 168;
				const uint8_t octet3 = 1;
				const uint8_t octet4 = 200;
				const int ip = octet1 | (octet2 << 8) | (octet3 << 16) | (octet4 << 24);

				IPAddress ipAddress(octet1, octet2, octet3, octet4);

				ASSERT_EQ(octet1, ipAddress.GetOctet(0));
				ASSERT_EQ(octet2, ipAddress.GetOctet(1));
				ASSERT_EQ(octet3, ipAddress.GetOctet(2));
				ASSERT_EQ(octet4, ipAddress.GetOctet(3));

				ASSERT_EQ(ip, ipAddress.GetIP());
			}

			TEST_F(IPAddressTest, IPConstructorTest)
			{
				const uint8_t octet1 = 192;
				const uint8_t octet2 = 168;
				const uint8_t octet3 = 1;
				const uint8_t octet4 = 200;
				const int ip = octet1 | (octet2 << 8) | (octet3 << 16) | (octet4 << 24);

				IPAddress ipAddress(ip);

				ASSERT_EQ(octet1, ipAddress.GetOctet(0));
				ASSERT_EQ(octet2, ipAddress.GetOctet(1));
				ASSERT_EQ(octet3, ipAddress.GetOctet(2));
				ASSERT_EQ(octet4, ipAddress.GetOctet(3));

				ASSERT_EQ(ip, ipAddress.GetIP());
			}
		}
	}
}