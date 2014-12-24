
/* 
* GLESCube.h
* Acrodea OpenKODE example code
* Loosely based on the Cube example from Hybrid's Rasteroid
*/

#ifndef GLESCUBE_H_INCLUDED
#define GLESCUBE_H_INCLUDED

struct myglobals
{
	/* nonzero if rotation is paused, 0 otherwise */
	KDint paused ;
	/* The time we paused is stored here */
	KDust pauseTime ;

	/* This becomes nonzero when the application is supposed to quit */
	KDint dying ;

	/* EGL variables */
	EGLDisplay eglDisplay ;
	EGLSurface eglWindowSurface ;
	EGLContext eglContextOpenGLES ;
	EGLConfig eglConfig;
        
        /* GL texture */
        KDuint tex;
        /* GL shader program (only used in GLES2Cube) */
        KDuint program;
};

#define GLOBALS ((struct myglobals *)kdGetTLS())

/* Renders the 3D scene using OpenGL ES */
void renderGLESScene(unsigned int time);

/* Initializes EGL. Returns 0 on success, nonzero on failure */
int initEGLConfig(void);
/* Initialize OpenGL ES context using EGL */
void initEGL(KDWindow *wnd);
/* Deinitialize EGL */
void deinitEGL(void);

#endif
