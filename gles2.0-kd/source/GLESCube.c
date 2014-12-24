/* 
* GLESCube.h
* Acrodea OpenKODE example code
* based on the Cube example from Hybrid's Rasteroid
*/

#include <KD/kd.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include "glescube.h"

/* Uncomment this to demonstrate use of KHR_lock_surface extension functionality */
/*#define TEST_LOCKSURFACE*/

/*-------------------------------------------------------------------------
* OpenGL ES code
*-----------------------------------------------------------------------*/

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

#define F2F(x) ((KDint)(((x)*65536.0f)+0.5f))

/*-------------------------------------------------------------------*/
/*!
* \brief	Sets the perspective matrix to OpenGL ES.
* \param	fovy	Vertical field-of-view.
* \param	aspect	Aspect ratio.
* \param	zNear	Distance to near plane.
* \param	zFar	Distance to far plane.
*//*-------------------------------------------------------------------*/
static void setGLESperspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)kdTanf(fovy * 3.1415962f / 360.0f);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    /*    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);*/
    glFrustumx(F2F(xmin), F2F(xmax), F2F(ymin), F2F(ymax), F2F(zNear), F2F(zFar)); 
}

/*-------------------------------------------------------------------*/
/*!
* \brief	Updates the OpenGL ES state
* \param	width	Rendering width.
* \param	height	Rendering height.
*//*-------------------------------------------------------------------*/
static void updateOpenGLESState(int width, int height)
{
    /* Define lights and materials for the OpenGL ES scene */
    static const GLfixed light_position[]	= { F2F(-50.f), F2F(50.f), F2F(50.f), F2F(0.0f) };
    static const GLfixed light_ambient[]	= { F2F(0.125f), F2F(0.125f), F2F(0.125f), F2F(1.0f) };
    static const GLfixed light_diffuse[]	= { F2F(0.7f), F2F(0.7f), F2F(0.7f), F2F(1.0f) };
    static const GLfixed material_spec[]	= { F2F(0.7f), F2F(0.7f), F2F(0.7f), F2F(0.0f) };
    static const GLfixed zero_vec4[]		= { F2F(0.0f), F2F(0.0f), F2F(0.0f), F2F(0.0f) };

    /* Define the aspect ratio for the perpective calculations so that 
    no distortion happens when the screen is resized */
    float aspect		= height ? (float)width/(float)height : 1.0f;

    glViewport			(0, 0, width, height);
    glScissor			(0, 0, width, height);

    /* Set the matrix mode to world coordinates */
    glMatrixMode		(GL_MODELVIEW);
    glLoadIdentity		();

    /* Setup lights and materials with the values given above */
    glLightxv			(GL_LIGHT0, GL_POSITION, light_position);
    glLightxv			(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightxv			(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightxv			(GL_LIGHT0, GL_SPECULAR, zero_vec4);
    glMaterialxv		(GL_FRONT_AND_BACK, GL_SPECULAR, material_spec);

    /* Enable ligting, materials, backface culling and textures */
    glDisable			(GL_NORMALIZE);
    glEnable			(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);
    
    glEnable			(GL_LIGHT0);
    glEnable			(GL_COLOR_MATERIAL);
    glEnable			(GL_CULL_FACE);
    glEnable			(GL_TEXTURE_2D);

    /* Tell OpenGL to use nicest perspective correction */
    glHint				(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /* Use flat shading and disable dithering */
    glShadeModel		(GL_FLAT);
    glDisable			(GL_DITHER);

    /* Setup the clear color and the current color for the OpenGL context */
    glClearColorx		(F2F(0.1f), F2F(0.2f), F2F(0.4f), F2F(1.f));
    glColor4x			(0x10000, 0x10000, 0x10000, 0x10000);

    /* Enable the use of vertex, normal and texture coordinate arrays */
    glEnableClientState	(GL_VERTEX_ARRAY);
    glEnableClientState	(GL_NORMAL_ARRAY);
    glEnableClientState	(GL_TEXTURE_COORD_ARRAY);

    /* Mipmaps do not work on all BREW devices, so disable it */
/*    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Update the projection matrix */
    glMatrixMode		(GL_PROJECTION);
    glLoadIdentity		();
    setGLESperspective	(60.f, aspect, 0.1f, 100.f);
}

/*-------------------------------------------------------------------*/
/*!
* \brief	Render the OpenGL ES scene.
* \param	time Current time in milliseconds
* \note	Renders the OpenGL ES scene
*//*-------------------------------------------------------------------*/
void renderGLESScene(unsigned int time)
{

    int width = 0;
    int height = 0;
#if defined(TEST_LOCKSURFACE)
    KDuint16 *fb = KD_NULL;
    int y = 0;
    EGLint pitch = 0;
    EGLint origin = 0;
    EGLint r, g, b;
#else
    /* Time needed for the rotation */
    const KDfloat32 effectTime	= kdFmodf(time / 4000.0f, 360.0f);
#endif

    /* Query the current window surface size from EGL */
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_WIDTH, (EGLint *)&width);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_HEIGHT, (EGLint *)&height);

#if !defined(TEST_LOCKSURFACE)
    /* Update the GLES state */
    updateOpenGLESState		(width, height);
    glClear 				(GL_COLOR_BUFFER_BIT);

    /* Rotate the world matrix and translate it on the z-axis */
    glMatrixMode			(GL_MODELVIEW);
    glLoadIdentity			();
    glTranslatex			(F2F(0.f), F2F(0.f), F2F(-30.f));
    glRotatex				(F2F((float)(effectTime*29.77f)), F2F(1.0f), F2F(2.0f), F2F(0.0f));
    glRotatex				(F2F((float)(effectTime*22.311f)), F2F(-0.1f), F2F(0.0f), -F2F(5.0f));

    glBindTexture(GL_TEXTURE_2D, GLOBALS->tex);

    /* Set the pointer to the arrays containing the cube vertices, normals 
    and texture coordinates */
    glVertexPointer			(3, GL_BYTE, 0, s_cubeVertices);
    glNormalPointer			(GL_BYTE, 0, s_cubeNormals);
    glTexCoordPointer		(2, GL_BYTE, 0, s_cubeTexCoords);

    /* Draw the cube one face at a time in the same order thay have been defined 
    in the s_cubeVertices array */
    glDrawArrays			(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays			(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays			(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays			(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays			(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays			(GL_TRIANGLE_STRIP, 20, 4);

#else

    /* Lock the surface and query for the properties */
    eglLockSurfaceKHR(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, KD_NULL);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_BITMAP_POINTER_KHR, (EGLint *)&fb);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_BITMAP_PITCH_KHR, &pitch);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_BITMAP_ORIGIN_KHR, &origin);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_BITMAP_PIXEL_RED_OFFSET_KHR, &r);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR, &g);
    eglQuerySurface(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR, &b);

    /* Fill the surface with random noise */
    for(y = 0; y < height; y++)
    {
        kdCryptoRandom((KDuint8 *)fb, width * 2);
        fb += pitch / 2;
    }

    eglUnlockSurfaceKHR(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface);
#endif

    /* Display the result on the screen */
    eglSwapBuffers			(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface);

}

KDint initEGLConfig(void)
{
    EGLint		majorVersion;
    EGLint		minorVersion;
    EGLint numConfigs;

    /* Set the attributes for EGLConfig. */
    static const EGLint s_configAttribs[] =
    {
#ifdef TEST_LOCKSURFACE        
        EGL_SURFACE_TYPE,			EGL_WINDOW_BIT | EGL_LOCK_SURFACE_BIT_KHR | EGL_OPTIMAL_FORMAT_BIT_KHR,
        EGL_MATCH_FORMAT_KHR,                   EGL_FORMAT_RGB_565_EXACT_KHR,
#else
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,		EGL_OPENGL_ES_BIT,
#endif
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
    const GLubyte rs[9] = { 255, 0, 255, 0, 255, 0, 255, 0, 255 };
    const GLubyte gs[9] = { 0, 0, 255, 0, 0, 255, 0, 0, 255};
    const GLubyte bs[9] = { 0, 255, 0, 255, 0, 255, 0, 255, 0 };

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
#ifndef TEST_LOCKSURFACE
    eglBindAPI (EGL_OPENGL_ES_API);
    GLOBALS->eglContextOpenGLES	= eglCreateContext(GLOBALS->eglDisplay, GLOBALS->eglConfig, KD_NULL, KD_NULL);
    eglMakeCurrent	(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface, GLOBALS->eglWindowSurface, GLOBALS->eglContextOpenGLES);
#endif

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

void deinitEGL(void)
{
#ifndef TEST_LOCKSURFACE
    glDeleteTextures    (1, &GLOBALS->tex);
#endif

    eglMakeCurrent	(GLOBALS->eglDisplay, KD_NULL, KD_NULL, KD_NULL);

#ifndef TEST_LOCKSURFACE
    eglDestroyContext	(GLOBALS->eglDisplay, GLOBALS->eglContextOpenGLES);
#endif
    
    eglDestroySurface	(GLOBALS->eglDisplay, GLOBALS->eglWindowSurface);
    eglTerminate	(GLOBALS->eglDisplay);
    eglReleaseThread	();
}
