#include <EGL/egl.h>
#include <string>
#include "../gles2impl.h"

static std::string eglError(std::string message) {
  return message + " (eglError: " + std::to_string(eglGetError()) + ")";
}

namespace gles2impl {
  EGLDisplay  eglDisplay;
  EGLContext  eglContext;
  EGLSurface eglSurface = EGL_NO_SURFACE;

  static const EGLint attr[] =
  {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_NONE
  };

  static const int pbufferWidth = 9;
  static const int pbufferHeight = 9;

  static const EGLint pbufferAttribs[] = {
      EGL_WIDTH, pbufferWidth,
      EGL_HEIGHT, pbufferHeight,
      EGL_NONE,
  };


  std::string initHeadless() {
    // get an EGL display connection
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
      return std::string("Couldn't get EGL display");
    }

    // initialize the EGL display connection
    if (eglInitialize(eglDisplay, NULL, NULL) == EGL_FALSE) {
      return std::string("Unable to initialize EGL");
    }


    // get an appropriate EGL frame buffer configuration
    EGLConfig ecfg;
    EGLint numConfigs;
    if (!eglChooseConfig(eglDisplay, attr, &ecfg, 1, &numConfigs)) {
      return eglError("Failed to choose config");
    }

    if (numConfigs != 1) {
      return std::string("Got ") + std::to_string(numConfigs) + std::string(" configs (expected 1");
    }

    eglBindAPI(EGL_OPENGL_ES_API);

    // create an EGL rendering context
    EGLint ctxattr[] = {
      EGL_CONTEXT_CLIENT_VERSION, 3,
      EGL_NONE
    };
    eglContext = eglCreateContext(eglDisplay, ecfg, EGL_NO_CONTEXT, ctxattr);
    if (eglContext == EGL_NO_CONTEXT) {
      return eglError("Unable to create EGL context");
    }

    eglSurface = eglCreatePbufferSurface(eglDisplay, ecfg, pbufferAttribs);

    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
      return eglError("Unable to make EGL display current");
    }
    return "";
  }

  void nextFrame(bool swapBuffers) {
    if (swapBuffers && eglSurface != EGL_NO_SURFACE) {
      eglSwapBuffers(eglDisplay, eglSurface);
    }
  }

  void cleanup() {
    eglDestroyContext(eglDisplay, eglContext);
    if (eglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(eglDisplay, eglSurface);
    }
    eglTerminate(eglDisplay);
  }

  std::string init(int width, int height, bool fullscreen, std::string title, unsigned int layer) {
    return "Unimplemented";
  }
}
