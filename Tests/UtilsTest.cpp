#include "UtilsTest.h"
#include "gtest/gtest.h"
#include "../Utils.h"
#include <string>

namespace Thing {
	namespace Core {
		namespace Tests {
			void UtilsTest::SetUp()
			{
			}

			void UtilsTest::TearDown()
			{
			}

			/// <summary>
			/// Tests convert positive int to a string.
			/// </summary>
			TEST_F(UtilsTest, TestPositiveIntToString)
			{
				std::string str = Thing::Core::Utils::IntToString(20);
				ASSERT_EQ("20", str);
			}

			/// <summary>
			/// Tests convert negative int to a string.
			/// </summary>
			TEST_F(UtilsTest, TestNegativeIntToString)
			{
				std::string str = Thing::Core::Utils::IntToString(-25);
				ASSERT_EQ("-25", str);
			}

			/// <summary>
			/// Tests string format without pass any params.
			/// </summary>
			TEST_F(UtilsTest, TestFormatStringWithoutArgs)
			{
				char buffer[30];
				std::string str = Thing::Core::Utils::FormatString(buffer, "This is a test: IoT 5 stars!");
				ASSERT_EQ("This is a test: IoT 5 stars!", str);
			}

			/// <summary>
			/// Tests string with two parameters of different types.
			/// </summary>
			TEST_F(UtilsTest, TestFormatStringMultipleArgs)
			{
				char buffer[30];
				std::string str = Thing::Core::Utils::FormatString(buffer, "This is a test: %s %d stars!", "IoT", 5);
				ASSERT_EQ("This is a test: IoT 5 stars!", str);
			}
		}
	}
}