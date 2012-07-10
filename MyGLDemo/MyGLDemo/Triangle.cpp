#include "StdAfx.h"
#include "Triangle.h"

CTriangle::CTriangle(void)
{
}

CTriangle::~CTriangle( void )
{

}

void CTriangle::Render()
{
	glLoadIdentity();	//reset 

	glTranslatef(-m_pos.X,-m_pos.Y,-m_pos.Z);
	//glRotatef(m_fRotationRate,m_rotation.X,m_rotation.Y,m_rotation.Z);
	glRotatef(m_fRotSpeed,0.f,1.f,1.f);
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f,0.0f,0.0f);		
	glVertex3f( 0.0f, 1.0f, 0.0f);	
	glColor3f(0.0f,1.0f,0.0f);		
	glVertex3f(-1.0f,-1.0f, 1.0f);	
	glColor3f(0.0f,0.0f,1.0f);		
	glVertex3f( 1.0f,-1.0f, 1.0f);	
	glColor3f(1.0f,0.0f,0.0f);		
	glVertex3f( 0.0f, 1.0f, 0.0f);	
	glColor3f(0.0f,0.0f,1.0f);		
	glVertex3f( 1.0f,-1.0f, 1.0f);	
	glColor3f(0.0f,1.0f,0.0f);		
	glVertex3f( 1.0f,-1.0f, -1.0f);	
	glColor3f(1.0f,0.0f,0.0f);		
	glVertex3f( 0.0f, 1.0f, 0.0f);	
	glColor3f(0.0f,1.0f,0.0f);		
	glVertex3f( 1.0f,-1.0f, -1.0f);	
	glColor3f(0.0f,0.0f,1.0f);		
	glVertex3f(-1.0f,-1.0f, -1.0f);	
	glColor3f(1.0f,0.0f,0.0f);		
	glVertex3f( 0.0f, 1.0f, 0.0f);	
	glColor3f(0.0f,0.0f,1.0f);		
	glVertex3f(-1.0f,-1.0f,-1.0f);	
	glColor3f(0.0f,1.0f,0.0f);		
	glVertex3f(-1.0f,-1.0f, 1.0f);	
	glEnd();

	glRotatef(-m_fRotSpeed,1.f,1.f,0.f);
	glTranslatef(m_pos.X,m_pos.Y,m_pos.Z);
}

void CTriangle::FrameMove( DWORD dwEslapeTime /*= 0*/ )
{
	m_fRotSpeed += 2.f * dwEslapeTime * 0.001f;
}

void CTriangle::HandleMouse( UINT e )
{

}

void CTriangle::SetFocus( BOOL )
{

}
