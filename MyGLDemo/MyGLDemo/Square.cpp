#include "StdAfx.h"
#include "Square.h"

extern unsigned int g_PixelShader;
CSquare::CSquare(void)
{
}

CSquare::~CSquare(void)
{
}

void CSquare::Render()
{
	glLoadIdentity();	
	glTranslatef(-m_pos.X,-m_pos.Y,-m_pos.Z);
	glRotatef(m_fRotSpeed,1.f,1.f,0.f);

	glUseProgram(g_PixelShader);
	glBegin(GL_QUADS);

	glColor3f(0.0f,1.0f,0.0f);			// 颜色改为蓝色
	glVertex3f( 1.0f, 1.0f,-1.0f);			// 四边形的右上顶点 (顶面)
	glVertex3f(-1.0f, 1.0f,-1.0f);			// 四边形的左上顶点 (顶面)
	glVertex3f(-1.0f, 1.0f, 1.0f);			// 四边形的左下顶点 (顶面)
	glVertex3f( 1.0f, 1.0f, 1.0f);			// 四边形的右下顶点 (顶面)
	glColor3f(1.0f,0.5f,0.0f);			// 颜色改成橙色
	glVertex3f( 1.0f,-1.0f, 1.0f);			// 四边形的右上顶点(底面)
	glVertex3f(-1.0f,-1.0f, 1.0f);			// 四边形的左上顶点(底面)
	glVertex3f(-1.0f,-1.0f,-1.0f);			// 四边形的左下顶点(底面)
	glVertex3f( 1.0f,-1.0f,-1.0f);			// 四边形的右下顶点(底面)
	glColor3f(1.0f,0.0f,0.0f);			// 颜色改成红色
	glVertex3f( 1.0f, 1.0f, 1.0f);			// 四边形的右上顶点(前面)
	glVertex3f(-1.0f, 1.0f, 1.0f);			// 四边形的左上顶点(前面)
	glVertex3f(-1.0f,-1.0f, 1.0f);			// 四边形的左下顶点(前面)
	glVertex3f( 1.0f,-1.0f, 1.0f);			// 四边形的右下顶点(前面)
	glColor3f(1.0f,1.0f,0.0f);			// 颜色改成黄色
	glVertex3f( 1.0f,-1.0f,-1.0f);			// 四边形的右上顶点(后面)
	glVertex3f(-1.0f,-1.0f,-1.0f);			// 四边形的左上顶点(后面)
	glVertex3f(-1.0f, 1.0f,-1.0f);			// 四边形的左下顶点(后面)
	glVertex3f( 1.0f, 1.0f,-1.0f);			// 四边形的右下顶点(后面)
	glColor3f(0.0f,0.0f,1.0f);			// 颜色改成蓝色
	glVertex3f(-1.0f, 1.0f, 1.0f);			// 四边形的右上顶点(左面)
	glVertex3f(-1.0f, 1.0f,-1.0f);			// 四边形的左上顶点(左面)
	glVertex3f(-1.0f,-1.0f,-1.0f);			// 四边形的左下顶点(左面)
	glVertex3f(-1.0f,-1.0f, 1.0f);			// 四边形的右下顶点(左面)
	glColor3f(1.0f,0.0f,1.0f);			// 颜色改成紫罗兰色
	glVertex3f( 1.0f, 1.0f,-1.0f);			// 四边形的右上顶点(右面)
	glVertex3f( 1.0f, 1.0f, 1.0f);			// 四边形的左上顶点(右面)
	glVertex3f( 1.0f,-1.0f, 1.0f);			// 四边形的左下顶点(右面)
	glVertex3f( 1.0f,-1.0f,-1.0f);			// 四边形的右下顶点(右面)


	glEnd();						// 立方体绘制结束

	glUseProgram(NULL);

	glTranslatef(1.5f,0.0f,6.0f);
	glRotatef(-m_fRotSpeed,1.f,1.f,0.f);
	glTranslatef(m_pos.X,m_pos.Y,m_pos.Z);

}


void CSquare::FrameMove( DWORD dwEslapeTime /*= 0*/ )
{
	m_fRotSpeed -= 2.f * dwEslapeTime * 0.001f;
}

void CSquare::HandleMouse( UINT e )
{

}

void CSquare::SetFocus( BOOL )
{

}
