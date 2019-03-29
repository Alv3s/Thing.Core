#pragma once

namespace Thing
{
	namespace Core
	{		
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