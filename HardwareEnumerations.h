#pragma once

namespace Thing
{
	namespace Core
	{
		enum class PinMode
		{
			Input,
			Output
		};

		enum class DigitalInputState
		{
			Inactive,
			WasActivated,
			Active,
			WasInactivated
		};

		enum class DigitalValue
		{
			Low = 0,
			High = 1,
			Toggle = 2,
		};

		enum class WiFiStatus {
			Disconnected = 0,
			Connected = 1,
			ConnectionFailed = 2,
			ConnectionLost = 3
		};

		enum class InterruptMode
		{
			WasActivated = 0,
			WasInactivated = 1,
			OnChange = 2
		};
	}
}