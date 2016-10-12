#pragma once

#include <string>
#include <map>

#include "Common.h"
#include "CallableManager.h"

#define FUN(fn) decltype(&fn), &fn
#define MET(fn) decltype(&fn), &fn

class TaskManager : public FunctionManager, public ClassManager
{
public:
	TaskManager():FunctionManager(), ClassManager(){}
	virtual ~TaskManager(){}
};
