#pragma once

namespace Thing
{
	namespace Core
	{		
		/// <summary>
		/// Interface defining the methods each concrete App has to implement.
		/// </summary>
		class IApp
		{
		public:			
			/// <summary>
			/// Method called once in the lifetime of the Application. This method should setup the application.
			/// </summary>
			virtual void Setup() = 0;			
			/// <summary>
			/// Method constantly being called. This method should process the application data.
			/// </summary>
			virtual void Loop() = 0;
		};
	}
}