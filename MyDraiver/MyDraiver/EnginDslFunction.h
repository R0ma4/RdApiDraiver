#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "ErrorValidity.h"
#include "Variables.h"

class EnginDslFunction
{

public:
	int line, word;
	ErrorValidity error_info;
	std::vector < Variables > variad;

};

