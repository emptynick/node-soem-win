// node-soem-master.h

#include <node.h>
#include <nan.h>

#include <stdlib.h>
#include <string>
#include "ethercat.h"


using namespace v8;

class NodeSoemMaster : public Nan::ObjectWrap {

    public:
        static NAN_MODULE_INIT(Init) {
            Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
            tpl->SetClassName(Nan::New("NodeSoemMaster").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            SetPrototypeMethod(tpl, "init", init);
            SetPrototypeMethod(tpl, "configInit", configInit);
            SetPrototypeMethod(tpl, "configMap", configMap);
            SetPrototypeMethod(tpl, "configDC", configDC);

            SetPrototypeMethod(tpl, "sendProcessdata", sendProcessdata);
            SetPrototypeMethod(tpl, "receiveProcessdata", receiveProcessdata);

            SetPrototypeMethod(tpl, "writeState", writeState);
            SetPrototypeMethod(tpl, "readState", readState);
            SetPrototypeMethod(tpl, "statecheck", statecheck);

            SetPrototypeMethod(tpl, "getSlaves", getSlaves);
            SetPrototypeMethod(tpl, "getInterfaceName", getInterfaceName);
            SetPrototypeMethod(tpl, "getAdapters", getAdapters);
            SetPrototypeMethod(tpl, "getMap", getMap);

            SetPrototypeMethod(tpl, "getExpectedWC", getExpectedWC);

            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());

            Nan::Set(target, Nan::New("NodeSoemMaster").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());

        }

    private:
        explicit NodeSoemMaster(char *ifname) : ifname_(ifname) { }
        ~NodeSoemMaster() { }

        static NAN_METHOD(New) {

            if (info.IsConstructCall()) {
                // invoke as constructor: 'new NodeSoemMaster(...)'
                // get interface name from arguments

                char *ifname = (char*) calloc(1, 20);

                Isolate* isolate = info.GetIsolate();

                if (info[0]->IsUndefined() || !info[0]->IsString()) {

                    ifname = (char *) "eth0";

                } else {
                    v8::Local<v8::String> fileName = info[0].As<v8::String>();
                    (*info[0].As<v8::String>())->WriteUtf8(isolate, ifname);
                }

                NodeSoemMaster* obj = new NodeSoemMaster(ifname);

                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());

            } else {

                // invoked as plain function 'NodeSoemMaster(...)', turn into constructor call.

                const int argc = 1;
                Local<Value> argv[argc] = { info[0] };
                Local<Function> cons = Nan::New(constructor());
                info.GetReturnValue().Set(Nan::NewInstance(cons,argc, argv).ToLocalChecked());

            }

        }

        static NAN_METHOD(init) {
            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            int retVal = ec_init(obj->ifname_);

            info.GetReturnValue().Set(Number::New(isolate, retVal));
        }

        static NAN_METHOD(configInit) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_config_init(FALSE);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(configMap) {

            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            int retVal = ec_config_map(&obj->ioMap_);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(configDC) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_configdc();

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(sendProcessdata) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_send_processdata();

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(receiveProcessdata) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_receive_processdata(5000);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(writeState) {

            Isolate* isolate = info.GetIsolate();

            int slave;
            int reqstate;


            if (info[0]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(Nan::MaybeLocal<String>(String::NewFromUtf8(isolate, "No slave defined in writeState.")).ToLocalChecked()));
                return;
            }

            //~ slave = info[0]->Uint32Value(Nan::GetCurrentContext());
            slave = info[0]->Uint32Value(Nan::GetCurrentContext()).FromJust();

            if (info[1]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(Nan::MaybeLocal<String>(String::NewFromUtf8(isolate, "No state defined in writeState.")).ToLocalChecked()));
                return;
            }

            //~ reqstate = info[1]->Uint32Value(Nan::GetCurrentContext());
            reqstate = info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust();

            ec_slave[slave].state = reqstate;

            int retVal = ec_writestate(slave);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(readState) {

            Isolate* isolate = info.GetIsolate();

            int retVal = ec_readstate();

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(statecheck) {

            Isolate* isolate = info.GetIsolate();

            int slave;
            int reqstate;
            int timeout = 1000;

            if (info[0]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(Nan::MaybeLocal<String>(String::NewFromUtf8(isolate, "No slave defined in statecheck.")).ToLocalChecked()));
                return;
            }

            slave = info[0]->Uint32Value(Nan::GetCurrentContext()).FromJust();

            if (info[1]->IsUndefined()) {
                isolate->ThrowException(Exception::TypeError(Nan::MaybeLocal<String>(String::NewFromUtf8(isolate, "No requested state in statecheck.")).ToLocalChecked()));
            }

            reqstate = info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust();

            int retVal = ec_statecheck(slave, reqstate, timeout);

            info.GetReturnValue().Set(Number::New(isolate, retVal));

        }

        static NAN_METHOD(getSlaves) {

            Isolate* isolate = info.GetIsolate();
            Local<Context> context = isolate->GetCurrentContext();

            Local<Array> arr = Array::New(isolate);

            int i = 1;

            while (i <= ec_slavecount) {

                Local<Object> slave = Object::New(isolate);

                slave->Set(context, String::NewFromUtf8(isolate, "name").ToLocalChecked(), String::NewFromUtf8(isolate, ec_slave[i].name).ToLocalChecked());
                slave->Set(context, String::NewFromUtf8(isolate, "state").ToLocalChecked(), Number::New(isolate, ec_slave[i].state));
                slave->Set(context, String::NewFromUtf8(isolate, "ALStatusocde").ToLocalChecked(), Number::New(isolate, ec_slave[i].ALstatuscode));
                slave->Set(context, String::NewFromUtf8(isolate, "configadr").ToLocalChecked(), Number::New(isolate, ec_slave[i].configadr));
                slave->Set(context, String::NewFromUtf8(isolate, "aliasadr").ToLocalChecked(), Number::New(isolate, ec_slave[i].aliasadr));

                int numbytes = ec_slave[i].Obytes;
                if ((numbytes == 0) && (ec_slave[i].Obits > 0)) numbytes = 1;
                if (numbytes > 8) numbytes = 8;

                slave->Set(context, String::NewFromUtf8(isolate, "Obits").ToLocalChecked(), Uint32::New(isolate, ec_slave[i].Obits));
                slave->Set(context, String::NewFromUtf8(isolate, "Obytes").ToLocalChecked(), Uint32::New(isolate, ec_slave[i].Obytes));
                slave->Set(context, String::NewFromUtf8(isolate, "outputs").ToLocalChecked(), ArrayBuffer::New(isolate, numbytes));

                numbytes = ec_slave[i].Ibytes;
                if ((numbytes == 0) && (ec_slave[i].Ibits > 0)) numbytes = 1;
                if (numbytes > 8) numbytes = 8;

                slave->Set(context, String::NewFromUtf8(isolate, "Ibits").ToLocalChecked(), Uint32::New(isolate, ec_slave[i].Ibits));
                slave->Set(context, String::NewFromUtf8(isolate, "Ibytes").ToLocalChecked(), Uint32::New(isolate, ec_slave[i].Ibytes));
                slave->Set(context, String::NewFromUtf8(isolate, "inputs").ToLocalChecked(), ArrayBuffer::New(isolate, numbytes));

                slave->Set(context, String::NewFromUtf8(isolate, "pdelay").ToLocalChecked(), Uint32::New(isolate, ec_slave[i].pdelay));

                arr->Set(context, i-1, slave);

                i++;

            }

            info.GetReturnValue().Set(arr);

        }

        static NAN_METHOD(getInterfaceName) {

            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            info.GetReturnValue().Set(String::NewFromUtf8(isolate, obj->ifname_).ToLocalChecked());

        }

        static NAN_METHOD(getAdapters) {
            ec_adaptert * adapter = NULL;
            Isolate* isolate = info.GetIsolate();
            int counter = 0;
            Local<Array> data = Array::New(isolate);

            adapter = ec_find_adapters ();

            while (adapter != NULL)
            {
                printf ("Description : %s, Device to use for wpcap: %s\n", adapter->desc,adapter->name);
                data->Set(isolate->GetCurrentContext(), counter, String::NewFromUtf8(isolate, adapter->name).ToLocalChecked());
                adapter = adapter->next;
                counter++;
            }
            info.GetReturnValue().Set(data);
        }


        static NAN_METHOD(getMap) {

            Isolate* isolate = info.GetIsolate();

            NodeSoemMaster* obj = ObjectWrap::Unwrap<NodeSoemMaster>(info.Holder());

            Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, 4096);

            info.GetReturnValue().Set(buffer);

        }

        static NAN_METHOD(getExpectedWC) {

            Isolate* isolate = info.GetIsolate();

            int exp = ec_group[0].outputsWKC * 2 + ec_group[0].inputsWKC;

            info.GetReturnValue().Set(Uint32::New(isolate, exp));

        }

        static inline Nan::Persistent<Function> & constructor() {
            static Nan::Persistent<v8::Function> my_constructor;
            return my_constructor;
        };

        char *ifname_;
        char ioMap_[4096];
};

NODE_MODULE(objectwrapper, NodeSoemMaster::Init)
