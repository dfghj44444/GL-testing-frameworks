
/* Acrodea OpenKODE Core
* helloworld.c - Example code
* Based on the Cube example from Hybrid's Rasteroid
*/

#include <KD/kd.h>
#include <EGL/egl.h>
#include "GLESCube.h"

/* Application-defined callback for input events */
void myKeyHandler(const KDEvent *event)
{
    /* We should only get input events, but just in case, forward the rest to kdDefaultEvent */
    if(event->type != KD_EVENT_INPUT)
    {
        kdDefaultEvent(event);
        return;
    }
    switch(event->data.input.index)
    {
    case KD_INPUT_GAMEKEYS_A:
    case KD_INPUT_GAMEKEYS_B:
    case KD_INPUT_GAMEKEYS_C:
    case KD_INPUT_GAMEKEYS_D:
        /* Set the 'dying' flag, so we quit */
        GLOBALS->dying = 1;
        break;

    case KD_INPUT_GAMEKEYS_FIRE:
        /* Rotate the pause state on key-down */
        if(event->data.input.value.i == 1)
        {
            GLOBALS->paused = !GLOBALS->paused;
            if(GLOBALS->paused != 0)
                GLOBALS->pauseTime = kdGetTimeUST();
        }
        break;
    }

}

/* Application entry point */
KDint kdMain(KDint argc, const KDchar *const *argv)
{
    /* Our application window */
    KDWindow *window = KD_NULL;
    KDust time;

    /* Create global struct and store it to TLS */
    struct myglobals *g = (struct myglobals *)kdMalloc(sizeof(struct myglobals));
    kdMemset(g, 0, sizeof(struct myglobals));
    kdSetTLS(g);

    if(initEGLConfig() != 0)
    {
        kdLogMessage("Unable to initialize EGL!");
        kdFree(g);
        kdExit(-1);
    }
    /* Create a normal window and exit on failure */
    window = kdCreateWindow(g->eglDisplay, g->eglConfig, KD_NULL);
    if(!window)
    {
        kdLogMessage("Unable to create a window!");
        deinitEGL();
        kdFree(g);
        kdExit(-1);
    }
    
    /* Install a callback to listen to all input events */
    kdInstallCallback(myKeyHandler, KD_EVENT_INPUT, KD_NULL);

    /* Initialize OpenGL ES render context on the KDWindow using EGL */
    initEGL(window);

    /* Main loop */
    for(;;)
    {
        /* Wait for the event with a timeout of 5 milliseconds*/
        const KDEvent *e = kdWaitEvent(5 * 1000000);
        if(e)
        {
            /* Exit the application if our window is closed */
            if(e->type == KD_EVENT_WINDOW_CLOSE)
                break;
            /* Pass the event back to the OpenKODE implementation for default handling */
            kdDefaultEvent(e);
        }

        /* Break the loop if we're quitting */
        if(GLOBALS->dying)
            break;

        /* If we are paused, use the previous pause time, otherwise use current time */
        if(GLOBALS->paused)
            time = GLOBALS->pauseTime;
        else
            time = kdGetTimeUST();

        /* Render the cube */
        /* This function will also call eglSwapBuffers() */
        renderGLESScene			((KDuint32)(time / 100000LL));
    }

    /* Application shutdown: deinitialize EGL */
    deinitEGL();
    /* Destroy the window */
    kdDestroyWindow(window);
    window = KD_NULL;

    kdFree(g);

    /* Application lifetime ends when it returns from kdMain */
    return 0;
}
