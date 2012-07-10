#ifndef _I_OBJECT_H_
#define _I_OBJECT_H_
#include "Vector3d.h"

class IObject
{
public:
	IObject(void):m_fRotSpeed(0.f),m_pos(0.f),m_rot(0.f),m_IsBtn(FALSE){};
	virtual ~IObject(void) {};
	virtual void Render() = 0;
	virtual void FrameMove(DWORD dwEslapeTime = 0) = 0;
	virtual void HandleMouse(UINT e) = 0;;
	inline void SetRotSpeed(float fRotationRate , vector3d<float> rotation){ 		
		m_fRotSpeed = fRotationRate;		
		m_rot = rotation;
	};
	inline void SetPos(vector3d<float> pos){
		m_pos = pos;
	}
	inline BOOL IsBtn() const{
		return m_IsBtn;
	};
	virtual void SetFocus(BOOL) = 0;
protected:
	float m_fRotSpeed;
	vector3d<float> m_pos;
	vector3d<float> m_rot;
	BOOL m_IsBtn;

};

#endif // ifdef _I_OBJECT_H_