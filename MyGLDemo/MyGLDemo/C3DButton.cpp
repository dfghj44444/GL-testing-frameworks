#include "StdAfx.h"
#include "C3DButton.h"

void CALLBACK AddBtnToScence();


typedef struct													// Create A Structure
{
	GLubyte	*imageData;											// Image Data (Up To 32 Bits)
	GLuint	bpp;												// Image Color Depth In Bits Per Pixel.
	GLuint	width;												// Image Width
	GLuint	height;												// Image Height
	GLuint	texID;												// Texture ID Used To Select A Texture
} TextureImage;													// Structure Name

TextureImage textures;										// Storage For 10 Textures

bool LoadTGA(TextureImage *texture, char *filename)				// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if(	file==NULL ||											// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
			return FALSE;										// Return False
		else													// Otherwise
		{
			fclose(file);										// If Anything Failed, Close The File
			return FALSE;										// Return False
		}
	}

	texture->width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)

	if(	texture->width	<=0	||									// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||									// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		return FALSE;											// Return False
	}

	texture->bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||								// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)							// Was Image Data Loaded
			free(texture->imageData);							// If So, Release The Image Data

		fclose(file);											// Close The File
		return FALSE;											// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];								// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);						// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);				// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered

	if (texture[0].bpp==24)										// Was The TGA 24 Bits
	{
		type=GL_RGB;											// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;												// Texture Building Went Ok, Return True
}

C3DButton::C3DButton(void)
	:m_bPress(FALSE)
	,m_bFocus(FALSE)
{	
	m_IsBtn= TRUE;

	m_pos = vector3df(0.42f,-0.2f,1);

	LoadTGA(&textures,"blueface.tga");

}

C3DButton::~C3DButton(void)
{
}

void C3DButton::Render()
{
	glLoadIdentity();	
	glTranslatef(-m_pos.X,-m_pos.Y,-m_pos.Z);
	
	glEnable(GL_TEXTURE_2D);						
	glBindTexture(GL_TEXTURE_2D, textures.texID);				// 选择纹理
	
	glBegin(GL_QUADS);

	if (m_bPress)
		glColor3f(1.f,0.f,0.f);			
	else if(m_bFocus)
		glColor3f(1.f,0.f,1.f);			
	else
		glColor3f(1.f,1.f,1.f);		
	// 前面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.2f,  0.2f);	// 纹理和四边形的左下
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.2f, -0.2f,  0.2f);	// 纹理和四边形的右下
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.2f,  0.2f,  0.2f);	// 纹理和四边形的右上
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f,  0.2f,  0.2f);	// 纹理和四边形的左上

	glColor3f(0.f,0.f,1.f);		
	// 后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, -0.2f, -0.2f);	// 纹理和四边形的右下
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f,  0.2f, -0.2f);	// 纹理和四边形的右上
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.2f,  0.2f, -0.2f);	// 纹理和四边形的左上
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.2f, -0.2f, -0.2f);	// 纹理和四边形的左下
	// 顶面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f,  0.2f, -0.2f);	// 纹理和四边形的左上
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f,  0.2f,  0.2f);	// 纹理和四边形的左下
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.2f,  0.2f,  0.2f);	// 纹理和四边形的右下
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.2f,  0.2f, -0.2f);	// 纹理和四边形的右上
	// 底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f, -0.2f, -0.2f);	// 纹理和四边形的右上
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.2f, -0.2f, -0.2f);	// 纹理和四边形的左上
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.2f, -0.2f,  0.2f);	// 纹理和四边形的左下
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, -0.2f,  0.2f);	// 纹理和四边形的右下
	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.2f, -0.2f, -0.2f);	// 纹理和四边形的右下
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.2f,  0.2f, -0.2f);	// 纹理和四边形的右上
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.2f,  0.2f,  0.2f);	// 纹理和四边形的左上
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.2f, -0.2f,  0.2f);	// 纹理和四边形的左下
	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.2f, -0.2f);	// 纹理和四边形的左下
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.2f, -0.2f,  0.2f);	// 纹理和四边形的右下
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.2f,  0.2f,  0.2f);	// 纹理和四边形的右上
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f,  0.2f, -0.2f);	// 纹理和四边形的左上

	glEnd();

	glDisable(GL_TEXTURE_2D);						
				

	//glTranslatef(m_pos.X,m_pos.Y,m_pos.Z);
}

void C3DButton::FrameMove( DWORD dwEslapeTime /*= 0*/ )
{

}

void C3DButton::HandleMouse( UINT e )
{
	switch(e)
	{
	case WM_MOUSEMOVE:
		m_bFocus = TRUE;
		break;
	case WM_LBUTTONUP:
		m_bPress = FALSE;
		break;
	case WM_LBUTTONDOWN:
		m_bPress = TRUE;
		AddBtnToScence();
		break;
	default:
		break;;
	}
}

void C3DButton::SetFocus( BOOL bFocus )
{
	m_bFocus = bFocus;
}

