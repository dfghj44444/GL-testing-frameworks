/*
 * GLES2Cube.c
 * HelloWorld
 * Acrodea OpenKODE example code
 * Very loosely based on the Cube example from Hybrid's Rasteroid
 */

#include <KD/kd.h>
#include <EGL/egl.h>
#include <GLES/gl2.h>
#include "glescube.h"

#define F2F(x) ((KDint)(((x)*65536.0f)+0.5f))

/* matrix4 functions --------------------------------------------------------*/

void mtx4_identity(float* dm)
{
	if(!dm){
		return;
	}
	kdMemset(dm, 0, (sizeof(float)*16));
	dm[0] = dm[5] = dm[10] = dm[15] = 1.0f;
}

void mtx4_mul(float* dm, const float* sml, const float* smr)
{
	int i, j;

	if(!dm || !sml || !smr){
		return;
	}
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			*dm++ = (sml[j] * smr[4*i]) + (sml[4+j] * smr[4*i+1])
			        + (sml[8+j] * smr[4*i+2]) + (sml[12+j] * smr[4*i+3]);
		}
	}
}

void mtx4_translate(float* dm, float tx, float ty, float tz)
{
	if(!dm){
		return;
	}
	mtx4_identity(dm);
	dm[12] = tx;
	dm[13] = ty;
	dm[14] = tz;
}

void mtx4_rotatex(float* dm, float deg)
{
	float rad;
	float sval;
	float cval;

	if(!dm){
		return;
	}

	while(deg >= 360.0f){
		deg -= 360.0f;
	}
	while(deg < 0){
		deg += 360.0f;
	}

	rad = deg * (float)(KD_PI_F / 180.0);
	sval = kdSinf(rad);
	cval = kdCosf(rad);
	mtx4_identity(dm);
	dm[5]  = cval;
	dm[6]  = sval;
	dm[9]  = -sval;
	dm[10] = cval;
}

void mtx4_rotatey(float* dm, float deg)
{
	float rad;
	float sval;
	float cval;

	if(!dm){
		return;
	}

	while(deg >= 360.0f){
		deg -= 360.0f;
	}
	while(deg < 0){
		deg += 360.0f;
	}

	rad = deg * (float)(KD_PI_F / 180.0);
	sval = kdSinf(rad);
	cval = kdCosf(rad);
	mtx4_identity(dm);
	dm[0]  = cval;
	dm[2]  = -sval;
	dm[8]  = sval;
	dm[10] = cval;
}

void mtx4_rotatez(float* dm, float deg)
{
	float rad;
	float sval;
	float cval;

	if(!dm){
		return;
	}

	while(deg >= 360.0f){
		deg -= 360.0f;
	}
	while(deg < 0){
		deg += 360.0f;
	}

	rad = deg * (float)(KD_PI_F / 180.0);
	sval = kdSinf(rad);
	cval = kdCosf(rad);
	mtx4_identity(dm);
	dm[0] = cval;
	dm[1] = sval;
	dm[4] = -sval;
	dm[5] = cval;
}

void mtx4_frustum(float* dm, float left, float right, float bottom, float top, float near, float far)
{
	float tmp;
	if(!dm){
		return;
	}
	dm[1] = dm[2] = dm[3] = dm[4] = dm[6] = dm[7] = 0.0f;
	dm[12] = dm[13] = dm[15] = 0.0f;
	dm[11] = -1.0f;
	tmp = 1.0f / (right - left);
	dm[0] = tmp * (2.0f * near);
	dm[8] = tmp * (right + left);
	tmp = 1.0f / (top - bottom);
	dm[5] = tmp * (2.0f * near);
	dm[9] = tmp * (top + bottom);
	tmp = 1.0f / (far - near);
	dm[10] = -tmp * (far + near);
	dm[14] = tmp * far * near * -2.0f;
}

/* Indices for cube */
static const GLushort s_cubeIndices[] =
{
    0, 1, 2,
    2, 1, 3,
    
    4, 5, 6,
    6, 5, 7,
    
    8, 9, 10,
    10, 9, 11,
    
    12, 13, 14,
    14, 13, 15,
    
    16, 17, 18,
    18, 17, 19,
    
    20, 21, 22,
    22, 21, 23
};

/* Vertices */
static const GLbyte s_cubeVertices[] =
{

    /* FRONT */
    -10, -10,  10,
    10, -10,  10,
    -10,  10,  10,
    10,  10,  10,

    /* BACK */
    -10, -10, -10,
    -10,  10, -10,
    10, -10, -10,
    10,  10, -10,

    /* LEFT */
    -10, -10,  10,
    -10,  10,  10,
    -10, -10, -10,
    -10,  10, -10,

    /* RIGHT */
    10, -10, -10,
    10,  10, -10,
    10, -10,  10,
    10,  10,  10,

    /* TOP */
    -10,  10,  10,
    10,  10,  10,
    -10,  10, -10,
    10,  10, -10,

    /* BOTTOM */
    -10, -10,  10,
    -10, -10, -10,
    10, -10,  10,
    10, -10, -10,

};

/* Normals */
static const GLbyte s_cubeNormals[] =
{
    0, 0, 127,
    0, 0, 127,
    0, 0, 127,
    0, 0, 127,

    0, 0, -128,
    0, 0, -128,
    0, 0, -128,
    0, 0, -128,

    -128, 0, 0,
    -128, 0, 0,
    -128, 0, 0,
    -128, 0, 0,

    127, 0, 0,
    127, 0, 0,
    127, 0, 0,
    127, 0, 0,

    0, 127, 0,
    0, 127, 0,
    0, 127, 0,
    0, 127, 0,

    0, -128, 0,
    0, -128, 0,
    0, -128, 0,
    0, -128, 0,

};

/* Texture coordinates */
static const GLbyte s_cubeTexCoords[] =
{

    /* FRONT */
    0, 1,
    1, 1,
    0, 0,
    1, 0,

    /* BACK */
    1, 1,
    1, 0,
    0, 1,
    0, 0,

    /* LEFT */
    1, 1,
    1, 0,
    0, 1,
    0, 0,

    /* RIGHT */
    1, 1,
    1, 0,
    0, 1,
    0, 0,

    /* TOP */
    0, 1,
    1, 1,
    0, 0,
    1, 0,

    /* BOTTOM */
    0, 0,
    0, 1,
    1, 0,
    1, 1,

};


/*-------------------------------------------------------------------*/
/*!
* \brief	Sets the perspective matrix to OpenGL ES.
* \param	fovy	Vertical field-of-view.
* \param	aspect	Aspect ratio.
* \param	zNear	Distance to near plane.
* \param	zFar	Distance to far plane.
*//*-------------------------------------------------------------------*/
static void setGLESperspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar, GLfloat *pmat)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)kdTanf(fovy * 3.1415962f / 360.0f);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    mtx4_frustum(pmat, xmin, xmax, ymin, ymax, zNear, zFar);
}

/*-------------------------------------------------------------------*/
/*!
* \brief	Updates the OpenGL ES state
* \param	width	Rendering width.
* \param	height	Rendering height.
*//*-------------------------------------------------------------------*/
static void updateOpenGLESState(int width, int height, GLfloat *pmat, GLfloat *mvmat)
{
    /* Define the aspect ratio for the perpective calculations so that 
    no distortion happens when the screen is resized */
    float aspect		= height ? (float)width/(float)height : 1.0f;

    glViewport			(0, 0, width, height);
    glScissor			(0, 0, width, height);

    if(GLOBALS->program == 0)
    {
        GLuint vs, ps;
        const char * srcPtr;
        KDchar *vertshader =        "\
            uniform mat4 mvprojMat; \
            attribute vec3 position;   \
            attribute vec3 normal;     \
            attribute vec2 txcoord0;   \
            varying vec3 oNorm;\
            varying vec2 oTx;\
            void main(void)\
            {\
                vec4 pos = vec4(position, 1.0);\
                gl_Position = mvprojMat * pos;\
                oNorm = normal;\
                oTx=txcoord0;\
            }\
        ";
        KDchar *pixelshader =         "\
            varying vec3 oNorm;\
            varying vec2 oTx;\
            uniform sampler2D texture; \
            void main(void)\
            {\
                gl_FragColor = texture2D(texture, oTx); \
            }\
        ";
        
        vs = glCreateShader(GL_VERTEX_SHADER);
        ps = glCreateShader(GL_FRAGMENT_SHADER);
        GLOBALS->program = glCreateProgram();
        srcPtr = vertshader;
        glShaderSource(vs, 1, &srcPtr, KD_NULL);
        glCompileShader(vs);
        srcPtr = pixelshader;
        glShaderSource(ps, 1, &srcPtr, KD_NULL);
        glCompileShader(ps);
        
        glAttachShader(GLOBALS->program, vs);
        glAttachShader(GLOBALS->program, ps);
        glLinkProgram(GLOBALS->program);
    }

    /* Set the matrix mode to world coordinates */
    mtx4_identity(mvmat);

    /* Enable ligting, materials, backface culling and textures */
    glDisable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glEnable			(GL_CULL_FACE);
    glEnable                    (GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDisable			(GL_DITHER);

    /* Setup the clear color and the current color for the OpenGL context */
    glClearColor(0.1f, 0.2f, 0.4f, 1.f);

    /* Enable the use of vertex, normal and texture coordinate arrays */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Update the projection matrix */
    mtx4_identity(pmat);
    setGLESperspective	(60.f, aspect, 0.1f, 100.f, pmat);
}


/*-------------------------------------------------------------------*/
/*!
* \brief	Render the OpenGL ES scene.
* \param	time Current time in milliseconds
* \note	Renders the OpenGL ES scene
*//*-------------------------------------------------------------------*/
void renderGLESScene(unsigned int time)
{

    /* Time needed for the rotation */
    const KDfloat32 effectTime	= kdFmodf(time / 4000.0f, 360.0f);
    int width = 0;
    int height = 0;
    GLuint loc;
    GLfloat mvproj[16];
    GLfloat rmatx[16];
    GLfloat rmaty[16];
    GLfloat rmat[16];
    GLfloat tmat[16];
    GLfloat pmat[16];
    GLfloat mvmat[16];
    
    /* Query the current window surface size from EGL */
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_WIDTH, (EGLint *)&width);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_HEIGHT, (EGLint *)&height);

    /* Update the GLES state */
    updateOpenGLESState		(width, height, pmat, mvmat);
    glClear 				(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Rotate the world matrix and translate it on the z-axis */
    mtx4_identity(tmat);
    mtx4_translate(tmat, 0.0f, 0.0f, -30.0f);

    mtx4_rotatey(rmatx, effectTime*29.77f);
    mtx4_rotatez(rmaty, effectTime*22.311f);
    mtx4_mul(rmat, rmatx, rmaty);
    
    mtx4_mul(mvmat, tmat, rmat);
    
    glBindTexture(GL_TEXTURE_2D, GLOBALS->tex);

    /* Set the pointer to the arrays containing the cube vertices, normals 
    and texture coordinates */
    
    glUseProgram(GLOBALS->program);
    loc = glGetAttribLocation(GLOBALS->program, "position");
    glEnableVertexAttribArray(loc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(loc, 3, GL_BYTE, GL_FALSE, 0, s_cubeVertices);

    loc = glGetAttribLocation(GLOBALS->program, "normal");
    glEnableVertexAttribArray(loc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(loc, 3, GL_BYTE, GL_TRUE, 0, s_cubeNormals);

    loc = glGetAttribLocation(GLOBALS->program, "txcoord0");
    glEnableVertexAttribArray(loc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(loc, 2, GL_BYTE, GL_FALSE, 0, s_cubeTexCoords);
    
    loc = glGetUniformLocation(GLOBALS->program, "texture");
    glUniform1i(loc, 0);
    
    mtx4_identity(mvproj);
    mtx4_mul(mvproj, pmat, mvmat);
    loc = glGetUniformLocation(GLOBALS->program, "mvprojMat");
    glUniformMatrix4fv(loc, 1, GL_FALSE, mvproj);
    
    glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_SHORT, s_cubeIndices); 

    /* Display the result on the screen */
    eglSwapBuffers			(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface);

}


int initEGLConfig()
{
    EGLint		majorVersion;
    EGLint		minorVersion;
    EGLint numConfigs;


    /* Set the attributes for EGLConfig. */
    static const EGLint s_configAttribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLConfig eglConfig;

    GLOBALS->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(!GLOBALS->eglDisplay)
        return -1;
    if(eglInitialize (GLOBALS->eglDisplay, &majorVersion, &minorVersion) == EGL_FALSE)
        return -1;

    eglChooseConfig(GLOBALS->eglDisplay, s_configAttribs, &eglConfig, 1, &numConfigs);
    if(numConfigs == 0)
    {
        eglTerminate(GLOBALS->eglDisplay);
        return -1;
    }

    GLOBALS->eglConfig = eglConfig;
    return 0;
}

/* Initializes the OpenGL ES context into a KDWindow */
void initEGL(KDWindow *wnd)
{
    GLbyte *buf = KD_NULL;
    GLbyte *walk;
    KDint i, k;
    const KDint sizes[9] = { 256, 128, 64, 32, 16, 8, 4, 2, 1 };
    const GLbyte rs[9] = { 255, 0, 255, 0, 255, 0, 255, 0, 255 };
    const GLbyte gs[9] = { 0, 0, 255, 0, 0, 255, 0, 0, 255};
    const GLbyte bs[9] = { 0, 255, 0, 255, 0, 255, 0, 255, 0 };

    /* Set the attributes for the window surface */
    static const EGLint s_surfaceAttribs[] =
    {
        EGL_COLORSPACE,		EGL_COLORSPACE_LINEAR,
        EGL_ALPHA_FORMAT,	EGL_ALPHA_FORMAT_NONPRE,
        EGL_NONE
    };
    EGLNativeWindowType nativeType;
    if(kdRealizeWindow(wnd, &nativeType) != 0)
        kdExit(20);

    /* Create a window surface for OpenGL ES */
    GLOBALS->eglWindowSurface	= eglCreateWindowSurface(GLOBALS->eglDisplay, GLOBALS->eglConfig, nativeType, s_surfaceAttribs);

    /* Create a context for OpenGL ES */
    eglBindAPI (EGL_OPENGL_ES_API);
    GLOBALS->eglContextOpenGLES	= eglCreateContext(GLOBALS->eglDisplay, GLOBALS->eglConfig, KD_NULL, KD_NULL);
    eglMakeCurrent	(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, GLOBALS->eglWindowSurface, GLOBALS->eglContextOpenGLES);

    /* Generate a texture */
    glGenTextures(1, &GLOBALS->tex);
    glBindTexture(GL_TEXTURE_2D, GLOBALS->tex);

    buf = kdMalloc(sizes[0] * sizes[0] * 3);

    for(k = 0; k < 9; k++)
    {
        walk = buf;
        for(i = 0; i < sizes[k] * sizes[k]; i++)
        {
            *(walk++) = (i % 3 == 0) ? rs[k] : 0;
            *(walk++) = (i % 3 == 0) ? gs[k] : 0;
            *(walk++) = (i % 3 == 0) ? bs[k] : 0;
        }
        glTexImage2D(GL_TEXTURE_2D, k, GL_RGB, sizes[k], sizes[k], 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
    }
    kdFree(buf);
}

void deinitEGL()
{
    glDeleteTextures    (1, &GLOBALS->tex);
    eglMakeCurrent      (GLOBALS->eglDisplay, KD_NULL, KD_NULL, KD_NULL);
    eglDestroyContext	(GLOBALS->eglDisplay, GLOBALS->eglContextOpenGLES);
    eglDestroySurface	(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface);
    eglTerminate	(GLOBALS->eglDisplay);
    eglReleaseThread	();
}
