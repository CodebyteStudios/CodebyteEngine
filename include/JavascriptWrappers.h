#ifndef JAVASCRIPTWRAPPERS_H
#define JAVASCRIPTWRAPPERS_H
#pragma once

#include "Tools.h"
#include "Node.h"

namespace JavascriptWrappers{

    void InjectWrappers(duk_context *ctx);

    int js_Node(duk_context *ctx);
    int js_Node_translate(duk_context *ctx);

    int js_Transform(duk_context *ctx);
    int js_Transform_translate(duk_context *ctx);
    int js_Transform_rotate(duk_context *ctx);
    int js_Transform_scale(duk_context *ctx);

}


#endif /* JAVASCRIPTWRAPPERS_H */
