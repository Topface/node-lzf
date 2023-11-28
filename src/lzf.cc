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
//     return Nan::ThrowError(Exception::Error(Nan::New<String>(what)));
// }
NAN_METHOD(compress) {
    if (info.Length() < 1 || !Buffer::HasInstance(info[0])) {
        return Nan::ThrowError("First argument must be a Buffer");
    }

    Local<Value> bufferIn  = info[0];
    size_t bytesIn         = Buffer::Length(bufferIn);
    char * dataPointer     = Buffer::Data(bufferIn);
    size_t bytesCompressed = bytesIn + 100;
    char * bufferOut       = (char*) malloc(bytesCompressed);

    if (!bufferOut) {
        return Nan::ThrowError("LZF malloc failed!");
    }

    unsigned result = lzf_compress(dataPointer, bytesIn, bufferOut, bytesCompressed);

    if (!result) {
        free(bufferOut);
        return Nan::ThrowError("Compression failed, probably too small buffer");
    }

    bufferOut = (char*) realloc (bufferOut, result);
    Nan::MaybeLocal<Object> BufferOut = Nan::NewBuffer(bufferOut, result);

    info.GetReturnValue().Set(BufferOut.ToLocalChecked());
}


NAN_METHOD(decompress) {
    if (info.Length() < 1 || !Buffer::HasInstance(info[0])) {
        return Nan::ThrowError("First argument must be a Buffer");
    }

    Local<Value> bufferIn = info[0];

    size_t bytesUncompressed = 999 * 1024 * 1024; // it's about max size that V8 supports

    if (info.Length() > 1 && info[1]->IsNumber()) { // accept dest buffer size
        bytesUncompressed = Nan::To<uint32_t>(info[1]).FromJust();
    }


    char * bufferOut = (char*) malloc(bytesUncompressed);
    if (!bufferOut) {
        return Nan::ThrowError("LZF malloc failed!");
    }

    unsigned result = lzf_decompress(Buffer::Data(bufferIn), Buffer::Length(bufferIn), bufferOut, bytesUncompressed);

    if (!result) {
        return Nan::ThrowError("Unrompression failed, probably too small buffer");
    }

    bufferOut = (char*) realloc (bufferOut, result);
    Nan::MaybeLocal<Object> BufferOut = Nan::NewBuffer(bufferOut, result);

    info.GetReturnValue().Set(BufferOut.ToLocalChecked());
}

extern "C" void init(Local<Object> exports, Local<Value> module, Local<Context> context) {
    Nan::HandleScope scope;

    if (!exports->IsObject() || exports->IsNull()) {
        Nan::ThrowTypeError("Target object is not valid");
        return;
    }

    Nan::SetMethod(exports.As<Object>(), "compress", compress);
    Nan::SetMethod(exports.As<Object>(), "decompress", decompress);
}

NODE_MODULE(lzf, init)
