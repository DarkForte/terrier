//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// buffer_proxy.h
//
// Identification: src/include/execution/proxy/buffer_proxy.h
//
// Copyright (c) 2015-2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include "execution/proxy/proxy.h"
#include "execution/runtime/buffer.h"

namespace terrier::execution {


PROXY(Buffer) {
  // Member Variables
  DECLARE_MEMBER(0, char *, buffer_start);
  DECLARE_MEMBER(1, char *, buffer_pos);
  DECLARE_MEMBER(2, char *, buffer_end);
  DECLARE_TYPE;

  DECLARE_METHOD(Init);
  DECLARE_METHOD(Destroy);
  DECLARE_METHOD(Append);
  DECLARE_METHOD(Reset);
};

TYPE_BUILDER(Buffer, util::Buffer);


}  // namespace terrier::execution