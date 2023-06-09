#pragma once
#pragma once
#include "string"
#include "Windows.h"

namespace Engine {
	enum class ELogLevel {
		TRACE = 1, // Debugging
		DEBUG = 2, // For Debug Builds
		INFO = 3, // For Release Builds
		WARN = 4, // Warnings to Resolve
		ERR = 5, // Errors
		FATAL = 6 // Should crash the system
	};

	class Log {
	public:
		static void E(std::string arg) {
			if (level > ELogLevel::ERR) return;

			char buffer[100];
			sprintf_s(buffer, 100, "[ERROR] %s\n", arg.c_str());
			OutputDebugStringA(buffer);
		}

		// template<typename... Args>
		static void I(std::string arg) {
			if (level > ELogLevel::INFO) return;

			char buffer[100];
			sprintf_s(buffer, 100, "[INFO] %s\n", arg.c_str());
			OutputDebugStringA(buffer);
		}

		static void D(std::string arg) {
			if (level > ELogLevel::DEBUG) return;

			char buffer[400];
			sprintf_s(buffer, 100, "[DEBUG] %s\n", arg.c_str());
			OutputDebugStringA(buffer);
		}

		static void T(std::string arg) {
			if (level > ELogLevel::INFO) return;

			char buffer[100];
			sprintf_s(buffer, 100, "[TRACE] %s\n", arg.c_str());
			OutputDebugStringA(buffer);
		}
	private:
		//static void log(std::string& arg) {}
	public:
		static ELogLevel level;
	};
}