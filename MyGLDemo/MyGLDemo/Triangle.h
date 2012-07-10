#ifndef _C_TRIANGLE_H_
#define _C_TRIANGLE_H_
#include "IObject.h"
class CTriangle :public IObject
{
public:
	CTriangle(void);
	virtual ~CTriangle(void);
	virtual void Render() ;
	virtual void FrameMove(DWORD dwEslapeTime = 0);
	virtual void HandleMouse(UINT e) ;
	virtual void SetFocus(BOOL) ;

};
#endif