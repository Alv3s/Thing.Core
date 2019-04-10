#include "gtest/gtest.h"
#include "../InputHelpers.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			TEST(DigitalReadToInputStateTest, BooleanSignatureTest)
			{
				EXPECT_EQ(Thing::Core::DigitalInputState::Inactive, Thing::Core::DigitalReadToInputState(false, false));
				EXPECT_EQ(Thing::Core::DigitalInputState::WasActivated, Thing::Core::DigitalReadToInputState(false, true));
				EXPECT_EQ(Thing::Core::DigitalInputState::Active, Thing::Core::DigitalReadToInputState(true, true));
				EXPECT_EQ(Thing::Core::DigitalInputState::WasInactivated, Thing::Core::DigitalReadToInputState(true, false));
			}

			TEST(DigitalReadToInputStateTest, DigitalValueSignatureTest)
			{
				EXPECT_EQ(Thing::Core::DigitalInputState::Inactive, Thing::Core::DigitalReadToInputState(Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::Low));
				EXPECT_EQ(Thing::Core::DigitalInputState::WasActivated, Thing::Core::DigitalReadToInputState(Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High));
				EXPECT_EQ(Thing::Core::DigitalInputState::Active, Thing::Core::DigitalReadToInputState(Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::High));
				EXPECT_EQ(Thing::Core::DigitalInputState::WasInactivated, Thing::Core::DigitalReadToInputState(Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Low));
			}
		}
	}
}