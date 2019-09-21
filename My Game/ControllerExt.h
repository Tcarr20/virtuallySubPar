#pragma once
#include <Controller.h>
class ControllerExt :
	private CXBoxController
	 
{
private:
	//bool m_bButtonADowns = m_bButtonADown;

public:
	ControllerExt();
	~ControllerExt();
	bool GetButtonADown();
};

