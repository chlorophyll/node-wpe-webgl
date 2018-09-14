#include <cstring>
#include <vector>
#include <iostream>

#include "gles2platform.h"
#include "gles2impl.h"

namespace gles2platform {

using namespace node;
using namespace v8;
using namespace std;

NAN_METHOD(init) {
  Nan::HandleScope scope;

  int width = info[0]->Int32Value();
  int height = info[1]->Int32Value();
  bool fullscreen = info[2]->BooleanValue();

  Nan::Utf8String title(info[3]->ToString());
  unsigned int layer = info[4]->Uint32Value();

  std::string message = gles2impl::init(width, height, fullscreen, *title, layer);
  if (message.size()) {
    Nan::ThrowRangeError(message.c_str());
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(initHeadless) {
  std::string message = gles2impl::initHeadless();
  if (message.size()) {
    Nan::ThrowRangeError(message.c_str());
  }
  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(nextFrame) {
  Nan::HandleScope scope;

  bool swapBuffers = info[0]->BooleanValue();

  gles2impl::nextFrame(swapBuffers);

  info.GetReturnValue().Set(Nan::Undefined());
}

void AtExit() {
  gles2impl::cleanup();
}

} // end namespace gles2platform
