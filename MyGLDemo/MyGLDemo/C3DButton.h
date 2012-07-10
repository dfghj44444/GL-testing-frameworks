#ifndef _C_BUTTON_H_
#define _C_BUTTON_H_
#include "iobject.h"

class C3DButton :
	public IObject
{
public:
	C3DButton(void);
	~C3DButton(void);

	virtual void Render();
	virtual void FrameMove(DWORD dwEslapeTime = 0);
	virtual void HandleMouse(UINT e);
	virtual void SetFocus(BOOL bFocus);
protected:
	BOOL m_bFocus;
	BOOL m_bPress;
};
#endif