#pragma once

#include <vector>
#include <string>

namespace AE {

//-------------------------------------------------------------------
	class IOManager 
	{
		public:
    	static bool readFileToBuffer(std::string filePath, 
						   			 std::vector<unsigned char>& buffer);
		//-----------------------------------
		static bool readFileToBuffer(std::string filePath, 
									 std::string& buffer);
		//-----------------------------------
	};
}


