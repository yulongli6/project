#include"httplib.h"
#include"compile.hpp"
#include<jsoncpp/json/json.h>

int main()
{
    using namespace httplib;
    Server server;
    server.Get("/compile", [](const Request& req,  Response& resp){
               (void)req;
               Json::Value req_json;
               Json::Value resp_json;
               Compiler::CompileAddRun(req_json, resp_json);
               //需要把Json::Value对象序列化成一个字符串
               Json::FastWriter writer;
               resp.set_content(writer.write(resp_json), "text/html");
               });
    server.listen("0.0.0.0", 9092);
    return 0;
}

