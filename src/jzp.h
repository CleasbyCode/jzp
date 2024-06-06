#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "searchFunc.cpp"
#include "valueUpdater.cpp"
#include "adjustZip.cpp"
#include "jzp.cpp"
#include "information.cpp"

template <uint_fast8_t N>
uint_fast32_t searchFunc(std::vector<uint_fast8_t>&, uint_fast32_t, uint_fast8_t, const uint_fast8_t (&)[N]);

void 
	startJzp(const std::string&, const std::string&),
	adjustZipOffsets(std::vector<uint_fast8_t>&),
	valueUpdater(std::vector<uint_fast8_t>&, uint_fast32_t, const uint_fast32_t, uint_fast8_t, bool),
	displayInfo();
