#include "JavascriptWrappers.h"

void JavascriptWrappers::InjectWrappers(duk_context *ctx){
    duk_push_c_function(ctx, js_Node, 0); duk_put_prop_string(ctx, 0, "Node");
}

int JavascriptWrappers::js_Node(duk_context *ctx) {
    if(duk_is_constructor_call(ctx)){
        int nargs = duk_get_top(ctx);
        std::cout << "Num args: " << nargs << std::endl;
        Node* newNode = new Node("test");
        duk_push_this(ctx);
        duk_push_pointer(ctx, (void*)newNode);duk_put_prop_string(ctx, 0, "__id__");
        duk_push_c_function(ctx, js_Node_translate, 0); duk_put_prop_string(ctx, 0, "translate");
    }else{
        std::cout << "'Node' can only be called with new!" << std::endl;
    }
    return 1;  /* one return value */
}

int JavascriptWrappers::js_Node_translate(duk_context *ctx){
    duk_push_this(ctx);
    duk_get_prop_string(ctx, 0, "__id__");
    Node* node = (Node*)duk_get_pointer(ctx, 1);
    //std::cout << "Node name: " << node->name << std::endl;

    return 0;  /* one return value */
}
