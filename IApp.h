#pragma once

namespace Thing
{
	namespace Core
	{		
		/// <summary>
		/// A Listener of an IApp object.
		/// </summary>
		class IAppListener
		{
		public:			
			/// <summary>
			/// Called automatically whdn IApp Loop is finishing.
			/// </summary>
			/// <returns>false if it should still run or true if it shouldn't be called anymore by the parent IApp.</returns>
			virtual bool OnLoop() = 0;
		};
		
		/// <summary>
		/// Interface defining the methods each concrete App has to implement.
		/// </summary>
		class IApp
		{
		public:			
			/// <summary>
			/// Adds a listener to this IApp. The listener will have code called after each Loop call.
			/// </summary>
			/// <param name="listener">The listener.</param>
			virtual void AddListener(IAppListener* listener) = 0;			
			/// <summary>
			/// Removes a listener of this IApp
			/// </summary>
			/// <param name="listener">The listener.</param>
			virtual void RemoveListener(IAppListener* listener) = 0;


			/// <summary>
			/// Adds a listener to this IApp. The listener will have code called after each Loop call.
			/// </summary>
			/// <param name="listener">The listener.</param>
			virtual void AddListener(IAppListener& listener) = 0;
			/// <summary>
			/// Removes a listener of this IApp
			/// </summary>
			/// <param name="listener">The listener.</param>
			virtual void RemoveListener(IAppListener& listener) = 0;
			
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