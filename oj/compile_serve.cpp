#include<unordered_map>
#include<jsoncpp/json/json.h>
#include"httplib.h"
#include"compile.hpp"

int main()
{
    using namespace httplib;
    Server server;
    server.Post("/compile", [](const Request& req,  Response& resp){
               (void)req;
               //从req中获取JSON请求
               //JSON和HTTP协议结合
               //需要的请求格式是JSON格式，而HTTP提供的是键值对，进行格式转换
               //提交的代码会包含特殊符号，浏览器会帮我们进行转义
               //然后解析这个数据整理成JSON格式
               //键值对，用哪个数据结构表示？
               //STL中的容器
               std::unordered_map<std::string, std::string> body_kv;
               UrlUtil::ParseBody(req.body, body_kv);
               Json::Value req_json;
               for(auto e : body_kv)
               {
                   req_json[e.first] = e.second;
               }
               Json::Value resp_json;
               Compiler::CompileAddRun(req_json, resp_json);
               //需要把Json::Value对象序列化成一个字符串
               Json::FastWriter writer;
               resp.set_content(writer.write(resp_json), "text/html");
               });
    //加上这个目录是为了让浏览器能够访问到静态页面。
    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0", 9092);
    return 0;
}

