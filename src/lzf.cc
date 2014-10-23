/* node-lzf (C) 2011 Ian Babrou <ibobrik@gmail.com>  */

#include <node_buffer.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <malloc/malloc.h>
#endif

#include "nan.h"

#include "lzf/lzf.h"


using namespace v8;
using namespace node;


// Handle<Value> ThrowNodeError(const char* what = NULL) {
//     return NanThrowError(Exception::Error(NanNew<String>(what)));
// }
NAN_METHOD(compress) {
    if (args.Length() < 1 || !Buffer::HasInstance(args[0])) {
        return NanThrowError("First argument must be a Buffer");
    }
    NanScope();

    Local<Object> bufferIn = args[0]->ToObject();
    size_t bytesIn         = Buffer::Length(bufferIn);
    char * dataPointer     = Buffer::Data(bufferIn);
    size_t bytesCompressed = bytesIn + 100;
    char * bufferOut       = (char*) malloc(bytesCompressed);

    if (!bufferOut) {
        return NanThrowError("LZF malloc failed!");
    }

    unsigned result = lzf_compress(dataPointer, bytesIn, bufferOut, bytesCompressed);

    if (!result) {
        free(bufferOut);
        return NanThrowError("Compression failed, probably too small buffer");
    }

    Local<Object> BufferOut = NanNewBufferHandle(bufferOut, result);
    free(bufferOut);

    NanReturnValue(BufferOut);
}


NAN_METHOD(decompress) {
    if (args.Length() < 1 || !Buffer::HasInstance(args[0])) {
        return NanThrowError("First argument must be a Buffer");
    }

    Local<Object> bufferIn = args[0]->ToObject();

    size_t bytesUncompressed = 999 * 1024 * 1024; // it's about max size that V8 supports

    if (args.Length() > 1 && args[1]->IsNumber()) { // accept dest buffer size
        bytesUncompressed = args[1]->Uint32Value();
    }


    char * bufferOut = (char*) malloc(bytesUncompressed);
    if (!bufferOut) {
        return NanThrowError("LZF malloc failed!");
    }

    unsigned result = lzf_decompress(Buffer::Data(bufferIn), Buffer::Length(bufferIn), bufferOut, bytesUncompressed);

    if (!result) {
        return NanThrowError("Unrompression failed, probably too small buffer");
    }

    Local<Object> BufferOut = NanNewBufferHandle(bufferOut, result);

    free(bufferOut);

    NanScope();
    NanReturnValue(BufferOut);
}

extern "C" void
init (Handle<Object> target) {
    NODE_SET_METHOD(target, "compress", compress);
    NODE_SET_METHOD(target, "decompress", decompress);
}

NODE_MODULE(lzf, init)
