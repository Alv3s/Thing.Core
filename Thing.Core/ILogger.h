#pragma once

#include <stdarg.h>

namespace Thing
{
	namespace Core
	{		
		/// <summary>
		/// An interface stating how a logger should implement it's functions. NOTE: Currently, due to a problem of the Microcontroller compiler, we were force to create code inside and a wrapper function.
		/// </summary>
		class ILogger
		{
		public:			
			/// <summary>
			/// Logs a message in Trace mode.
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="">The parameters to be replaced in the formatters.</param>
			void Trace(const char* message, ...)
			{
				va_list values;
				va_start(values, message);
				TraceOverride(message, values);
				va_end(values);
			}
			
			/// <summary>
			/// Logs a message in Debug mode.
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="">The parameters to be replaced in the formatters.</param>
			void Debug(const char* message, ...)
			{
				va_list values;
				va_start(values, message);
				DebugOverride(message, values);
				va_end(values);
			}
			
			/// <summary>
			/// Logs a message in Info mode.
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="">The parameters to be replaced in the formatters.</param>
			void Info(const char* message, ...)
			{
				va_list values;
				va_start(values, message);
				InfoOverride(message, values);
				va_end(values);
			}

			/// <summary>
			/// Logs a message in Warn mode.
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="">The parameters to be replaced in the formatters.</param>
			void Warn(const char* message, ...)
			{
				va_list values;
				va_start(values, message);
				WarnOverride(message, values);
				va_end(values);
			}

			/// <summary>
			/// Logs a message in Error mode.
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="">The parameters to be replaced in the formatters.</param>
			void Error(const char* message, ...)
			{
				va_list values;
				va_start(values, message);
				ErrorOverride(message, values);
				va_end(values);
			}

			/// <summary>
			/// Logs a message in Fatal mode.
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="">The parameters to be replaced in the formatters.</param>
			void Fatal(const char* message, ...)
			{
				va_list values;
				va_start(values, message);
				FatalOverride(message, values);
				va_end(values);
			}

		protected:
			/// <summary>
			/// The implementation of the Trace logging method
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="args">The variadic arguments.</param>
			virtual void TraceOverride(const char* message, va_list args) = 0;
			/// <summary>
			/// The implementation of the Debug logging method
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="args">The variadic arguments.</param>
			virtual void DebugOverride(const char* message, va_list args) = 0;
			/// <summary>
			/// The implementation of the Info logging method
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="args">The variadic arguments.</param>
			virtual void InfoOverride(const char* message, va_list args) = 0;
			/// <summary>
			/// The implementation of the Warn logging method
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="args">The variadic arguments.</param>
			virtual void WarnOverride(const char* message, va_list args) = 0;
			/// <summary>
			/// The implementation of the Error logging method
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="args">The variadic arguments.</param>
			virtual void ErrorOverride(const char* message, va_list args) = 0;
			/// <summary>
			/// The implementation of the Fatal logging method
			/// </summary>
			/// <param name="message">The message with formatters.</param>
			/// <param name="args">The variadic arguments.</param>
			virtual void FatalOverride(const char* message, va_list args) = 0;
		};
	}
}
