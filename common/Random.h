//---------------------------------------------------------------
//
// Random.h
//

#pragma once

#include <vector>

namespace Common {

//===============================================================================

std::vector<int> GetRandomNumberList(int count, int lowerBound, int upperBound);
int GetRandomNumber(int lowerBound, int upperBound);

//===============================================================================

} // namespace Common
