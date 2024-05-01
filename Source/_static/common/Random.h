//---------------------------------------------------------------
//
// Random.h
//

#pragma once

#include <string>
#include <vector>

namespace Common {

//===============================================================================

std::vector<int> GetRandomNumberList(int count, int lowerBound, int upperBound);
void GetRandomLowercaseString(std::string& str, int count);
int GetRandomNumber(int lowerBound, int upperBound);

//===============================================================================

} // namespace Common
