#pragma once

#include <stdint.h>

namespace Thing
{
	namespace Core
	{
		enum class PinMode : bool
		{
			Input,
			Output
		};

		enum class DigitalInputState : uint8_t
		{
			Inactive,
			WasActivated,
			Active,
			WasInactivated
		};

		enum class DigitalValue : uint8_t
		{
			Low = 0,
			High = 1,
			Toggle = 2,
		};

		enum class WiFiStatus : uint8_t
		{
			Disconnected = 0,
			Connected = 1,
			ConnectionFailed = 2,
			ConnectionLost = 3
		};

		enum class InterruptMode : uint8_t
		{
			WasActivated = 0,
			WasInactivated = 1,
			OnChange = 2
		};
	}
}