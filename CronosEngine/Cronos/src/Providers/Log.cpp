#pragma once

#include "Providers/cnpch.h"
#include "Providers/Globals.h"

#include "Application.h"

#include "../ImGui/ImGuiLayer.h"
#include "psapi.h"

//#define BTOGB (1073741824.0f)
//#define KBTOMB 1024.0f//To GB: (1048576.0f)
//#define BTOMB (1048576.0f)

namespace Cronos {

	void Log(const char file[], int line, const char* format, ...)
	{
		static char tmp_string[4096];
		static char tmp_string2[4096];
		static va_list  ap;

		// Construct the string from variable arguments
		va_start(ap, format);
		vsprintf_s(tmp_string, 4096, format, ap);
		va_end(ap);
		sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);

		OutputDebugStringA(tmp_string2);
	}


	void LogCompilationFirstMessage()
	{
		App->EditorGUI->TestLog <<	"-- Compilation Start Succeeded --"	<<		std::endl			<<
									"	Compilation at Date "			<<		__DATE__			<<
									" and Time "						<<		__TIME__			<<
		std::endl;

		//---------------------------------- CHECKING AND PRINTING C++ VERSION USED BY COMPILER & WINDOWS VERSION ----------------------------
	//	std::string cppVersion = GetCppVersion(__cplusplus);
		//App->EditorGUI->TestLog <<	"	Standard C++ Version Implemented by Compiler: "		<< __cplusplus							<< " ("			<< cppVersion << ")" << std::endl <<
			//						"	OS Found: "											<< (__STDC_HOSTED__ ? "Yes" : "No")		<< std::endl	<<
			//						"	OS Version: "										<< GetWindowsVersion() << std::endl		<< std::endl
	//	<< std::endl;

		///__cplusplus returning values:
		///199711L (C++98 or C++03)
		///201103L (C++11)
		///201402L (C++14)
		///201703L (C++17)

		//------------------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------- OPENGL AND GPU INFO PRINT -----------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------
		//GLint GPU_TotalMem = 0;
		//GLint GPU_CurrentMem = 0;
		//glGetIntegerv(0x9048, &GPU_TotalMem); //In KB!
		//glGetIntegerv(0x9049, &GPU_CurrentMem); //In KB!

		//App->EditorGUI->TestLog << std::endl << std::endl << "---- GPU HARDWARE & OPEN GL INFO LOG -----------" << std::endl << std::endl;

		//App->EditorGUI->TestLog <<

				//	"	GPU Benchmark: "								<< glGetString(GL_VENDOR)						<< std::endl						<<
				//	"	GPU Model:     "								<< glGetString(GL_RENDERER)						<< std::endl						<<
				//	"	GPU Total VRAM Memory: "						<< GPU_TotalMem/KBTOMB							<< " MB (ONLY FOR NVIDIA GPUs!)"	<< std::endl <<
				//	"	GPU Current Availale VRAM Memory: "				<< ceil(GPU_CurrentMem/KBTOMB)					<< " MB (ONLY FOR NVIDIA GPUs!)"	<< std::endl << std::endl <<
				//	"	OpenGL Shading Language Version: "				<< glGetString(GL_SHADING_LANGUAGE_VERSION)		<< std::endl <<						//Version of GLSL supported
				//	"	OpenGL Version: "								<< glGetString(GL_VERSION)						<< std::endl <<

		//std::endl << "---- END OF GPU HARDWARE & OPEN GL INFO LOG ----" << std::endl << std::endl << std::endl;


		//------------------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------- CPU INFO PRINT ----------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------
		//cppVersion = GetCppVersion(_MSVC_LANG);
		//App->EditorGUI->TestLog << std::endl << std::endl << "---- CPU HARDWARE INFO LOG -----------" << std::endl << std::endl;
		

		//App->EditorGUI->TestLog <<

			//	"	PC RAM Storage: "		<<		(float)SDL_GetSystemRAM()/KBTOMB			<<	" GB"			<< std::endl <<		// GB of RAM memory
			//	"	Avalable CPU Cores: "	<<		SDL_GetCPUCount()							<<	std::endl		<<					// Number of available cores in CPU
			//	"	L1 Cache Line Size: "	<<		SDL_GetCPUCacheLineSize()					<<	" Bytes"		<< std::endl <<		// Size of the L1 line (little but fast) of the cache memory
			//"	RDTSC Available: " << (SDL_HasRDTSC() ? "Yes" : "No") << std::endl;		//<<					// If RDTSC is (or not) available - (Read Time Stamp Counter - Records CPU cycles since reset)
				//	"	VS Compiler Version: "	<<		_MSC_VER									<<	std::endl		<<					// Compiler Version
				//	"	C++ Version Used: "		<<		_MSVC_LANG << " (" << cppVersion << ")"		<<	std::endl		<<					// C++ standard version targeted by compiler
				//	"	Multithreaded Specified: "		<<			(_MT ? "Yes" : "No")			<<	std::endl		<< std::endl;		// If multithreaded is specified (or not)


		//SYSTEM_INFO SystemInfo;
		//GetSystemInfo(&SystemInfo);
		//std::string processorArch = GetCPUArchitecture(SystemInfo);
		
		//App->EditorGUI->TestLog <<

					//"	Number of Processors: "		<< SystemInfo.dwNumberOfProcessors	<< std::endl <<
					//"	Processor Architecture: "	<< processorArch					<< std::endl <<
					//"	Processor Revision: "		<< SystemInfo.wProcessorRevision	<<
		//std::endl;
		//App->EditorGUI->TestLog <<	std::endl	<< "---- END OF CPU HARDWARE INFO LOG ----" << std::endl << std::endl;


		//------------------------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------- MEMORU INFO PRINT -------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------
		//MEMORYSTATUSEX MemoryInfo;
		//MemoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
		//GlobalMemoryStatusEx(&MemoryInfo);
		//
		//App->EditorGUI->TestLog << std::endl << std::endl	<< "---- MEMORY HARDWARE INFO LOG -----------"				<< std::endl << std::endl	<<
		//
		//			"	Percentage of Memory in Use: "		<<			(uint)MemoryInfo.dwMemoryLoad					<<	" %"	<< std::endl	<<
		//			"	Total physical memory: "			<<				 MemoryInfo.ullTotalPhys/BTOGB				<<	" GB"	<< std::endl	<<
		//			"	Free physical memory: "				<<				 MemoryInfo.ullAvailPhys/BTOGB				<<	" GB"	<< std::endl	<<
		//			"	Used physical memory: "				<<	 (MemoryInfo.ullTotalPhys - MemoryInfo.ullAvailPhys)/BTOGB			<< " GB"		<< std::endl <<
		//			"	Total virtual memory: "				<<				 MemoryInfo.ullTotalVirtual/BTOGB			<<	" GB"	<< std::endl	<<
		//			"	Free virtual memory: "				<<				 MemoryInfo.ullAvailVirtual/BTOGB			<<	" GB"	<< std::endl	<<
		//			"	Free extended memory: "				<<				 MemoryInfo.ullAvailExtendedVirtual/BTOMB	<<	" MB"	<< std::endl	<<
		//			"	Total Page File memory: "			<<				 MemoryInfo.ullTotalPageFile/BTOGB			<<	" GB"	<< std::endl	<<
		//			"	Free Page File memory: "			<<				 MemoryInfo.ullAvailPageFile/BTOGB			<<	" GB"	<<
		//std::endl;


		//PROCESS_MEMORY_COUNTERS pmc;
		//GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		//SIZE_T virtualMemUsed = pmc.PagefileUsage;
		//SIZE_T physMemUsed = pmc.WorkingSetSize;
		//
		//App->EditorGUI->TestLog << std::endl <<
		//						"	Virtual memory used by process: "	<< virtualMemUsed/BTOMB << " MB" << std::endl <<
		//						"	Physical memory used by process: "	<< physMemUsed/BTOMB	<< " MB" <<
		//std::endl;

		//------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------
		//App->EditorGUI->TestLog << std::endl << "---- END OF MEMORY HARDWARE INFO LOG ----" << std::endl;
		//App->EditorGUI->TestLog << std::endl << std::endl;
	}
}
