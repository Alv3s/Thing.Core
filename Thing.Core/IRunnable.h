#pragma once

#include <functional>

namespace Thing
{
	namespace Core
	{		
		using RunnableCallback = std::function<void ()>;

		/// <summary>
		/// A Runnable interface.
		/// </summary>
		class IRunnable
		{
		public:			
			/// <summary>
			/// A parameterless method that can be called to execute some code.
			/// </summary>
			virtual void Run() = 0;
		};
	}
}