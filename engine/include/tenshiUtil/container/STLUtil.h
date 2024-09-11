#pragma once

#include <concepts>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

#include "tenshiUtil/Types.h"

namespace tenshi
{
	template <typename T>
	concept IsComparable = std::equality_comparable<T>;

	struct STLUtil
	{
		// Const-Version
		// Either returns the Iterator Position to the Element in the Vector or -1 if it couldnt be found
		template <IsComparable T> static std::vector<T>::const_iterator GetItOfElementInVec(const std::vector<T>& vec, const T& element)
		{
			if (vec.empty())
			{
				std::cout << "[STLUtil] Passed Vector is empty" << std::endl;
				return vec.begin();
			}

			typename std::vector<T>::const_iterator _it = std::find(vec.begin(), vec.end(), element);
			if (_it == vec.end())
				std::cerr << "[STLUtil] Could not find given Element" << std::endl;

			return _it;
		}

		// Non-Const-Version
		// Either returns the Iterator Position to the Element in the Vector or -1 if it couldnt be found
		template <IsComparable T> static std::vector<T>::iterator GetItOfElementInVec(std::vector<T>& vec, const T& element)
		{
			if (vec.empty())
			{
				std::cout << "[STLUtil] Passed Vector is empty" << std::endl;
				return vec.begin();
			}

			typename std::vector<T>::iterator _it = std::find(vec.begin(), vec.end(), element);
			if (_it == vec.end())
				std::cerr << "[STLUtil] Could not find given Element" << std::endl;

			return _it;
		}

		template <IsComparable T> static bool IsElementInVector(const std::vector<T>& vec, const T& element)
		{
			for (auto& elem : vec)
			{
				if (elem == element)
					return true;
			}

			return false;
		}
	};
}