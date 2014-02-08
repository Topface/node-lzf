/* node-lzf (C) 2011 Ian Babrou <ibobrik@gmail.com>  */

#include <node_buffer.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <malloc/malloc.h>
#endif


#include "lzf/lzf.h"


using namespace v8;
using namespace node;


Handle<Value> ThrowNodeError(const char* what = NULL) {
    return ThrowException(Exception::Error(String::New(what)));
}

Handle<Value> compress(const Arguments& args) {
    if (args.Length() < 1 || !Buffer::HasInstance(args[0])) {
        return ThrowNodeError("First argument must be a Buffer");
    }

    HandleScope scope;

    Local<Object> bufferIn = args[0]->ToObject();
    size_t bytesIn         = Buffer::Length(bufferIn);
    char * dataPointer     = Buffer::Data(bufferIn);
    size_t bytesCompressed = bytesIn + 100;
    char * bufferOut       = (char*) malloc(bytesCompressed);

    if (!bufferOut) {
        return ThrowNodeError("LZF malloc failed!");
    }

    unsigned result = lzf_compress(dataPointer, bytesIn, bufferOut, bytesCompressed);

    if (!result) {
        free(bufferOut);
        return ThrowNodeError("Compression failed, probably too small buffer");
    }

    Buffer *BufferOut = Buffer::New(bufferOut, result);
    free(bufferOut);

    return scope.Close(BufferOut->handle_);
}


Handle<Value> decompress(const Arguments &args) {
    if (args.Length() < 1 || !Buffer::HasInstance(args[0])) {
        return ThrowNodeError("First argument must be a Buffer");
    }

    Local<Object> bufferIn = args[0]->ToObject();

    size_t bytesUncompressed = 999 * 1024 * 1024; // it's about max size that V8 supports

    if (args.Length() > 1 && args[1]->IsNumber()) { // accept dest buffer size
        bytesUncompressed = args[1]->Uint32Value();
    }


    char * bufferOut = (char*) malloc(bytesUncompressed);
    if (!bufferOut) {
        return ThrowNodeError("LZF malloc failed!");
    }

    unsigned result = lzf_decompress(Buffer::Data(bufferIn), Buffer::Length(bufferIn), bufferOut, bytesUncompressed);

    if (!result) {
        return ThrowNodeError("Unrompression failed, probably too small buffer");
    }

    Buffer * BufferOut = Buffer::New(bufferOut, result);

    free(bufferOut);

    HandleScope scope;
    return scope.Close(BufferOut->handle_);
}

extern "C" void
init (Handle<Object> target) {
    NODE_SET_METHOD(target, "compress", compress);
    NODE_SET_METHOD(target, "decompress", decompress);
}

NODE_MODULE(lzf, init)
