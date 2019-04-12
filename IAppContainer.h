#pragma once

#include "IApp.h"

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
		/// A container of an IApp.
		/// </summary>
		class IAppContainer : public virtual IApp
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
		};
	}
}