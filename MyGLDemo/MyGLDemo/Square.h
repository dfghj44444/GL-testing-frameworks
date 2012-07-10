#pragma once
#include "iobject.h"

class CSquare :
	public IObject
{
public:
	CSquare(void);
	~CSquare(void);
	virtual void Render();
	virtual void FrameMove( DWORD dwEslapeTime = 0 );
	virtual void HandleMouse(UINT e);
	virtual void SetFocus(BOOL) ;

};
