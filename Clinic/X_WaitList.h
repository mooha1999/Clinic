#pragma once
#include "EU_WaitList.h"
class X_WaitList :
    public EU_WaitList
{
public:
	void cancel(int time);
};

