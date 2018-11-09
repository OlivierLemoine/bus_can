#include <node.h>
#include <v8.h>

#include <cstring>

#include <libpcan.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

using namespace v8;

struct Can_opt
{
    const char *szDevNode;
    HANDLE h;
    TPCANRdMsg pMsgBuff;

    bool isInitialize = false;
};

Can_opt can_opt;

void initialize(const v8::FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    if (args.Length() < 1)
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    if (!args[0]->IsString())
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }

    const char *szDevNode = "/dev/pcanusb32";
    // const char *szDevNode = args[0].StringValue().c_str();
    // const char *szDevNode = Local<String>::Cast(args[0]).c_str();

    can_opt.h = LINUX_CAN_Open(szDevNode, O_RDWR);

    CAN_Init(can_opt.h, CAN_BAUD_500K, CAN_INIT_TYPE_ST);
    CAN_Status(can_opt.h);

    args.GetReturnValue().Set(Undefined(isolate));
}

void getValue(const v8::FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    if (!can_opt.isInitialize)
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Initialize before use")));
        return;
    }

    TPCANRdMsg pMsgBuff;

    LINUX_CAN_Read_Timeout(can_opt.h, &pMsgBuff, 1);
    
    Local<Array> result_list = Array::New(isolate);

    result_list->Set(0, Number::New(isolate, pMsgBuff.Msg.ID));
    
    for(size_t i = 0; i < 8; i++)
    {
        result_list->Set(i+1, Number::New(isolate, pMsgBuff.Msg.DATA[i]));
    }

    args.GetReturnValue().Set(result_list);
}

void sendValue(const v8::FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    
    if (!can_opt.isInitialize)
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Initialize before use")));
        return;
    }

    if (args.Length() < 1)
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    if (!args[0]->IsArray())
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }

    Local<Array> data = Local<Array>::Cast(args[0]);

    TPCANMsg msgBuff;

    msgBuff.ID = 0x1;
    msgBuff.MSGTYPE = MSGTYPE_STANDARD;
    msgBuff.LEN = data->Length();

    
    for(size_t i = 0; i < data->Length(); i++)
    {
        msgBuff.DATA[i] = data->Get(i)->NumberValue();
    }

    LINUX_CAN_Write_Timeout(can_opt.h, &msgBuff, 0);

    args.GetReturnValue().Set(Undefined(isolate));
}


void Init(Handle<Object> exports)
{
    NODE_SET_METHOD(exports, "initialize", initialize);
    NODE_SET_METHOD(exports, "getValue", getValue);
    NODE_SET_METHOD(exports, "sendValue", sendValue);
}

NODE_MODULE(can_lib, Init)