#include"httplib.h"
#include<jsoncpp/json/json.h>
#include"util.hpp"
#include"oj_model.hpp"
#include"oj_view.hpp"
#include"compile.hpp"

//controller 作为服务器的核心逻辑，需要创建好对应的服务器框架代码
//在这个框架中组织逻辑

int main()
{
    //数据来自_model
    OjModel model;
    model.load();
    using namespace httplib;
    Server server;
    server.Get("/all_questions", [&model](const Request& req, 
                                    Response& resp){
               (void)req;
               //数据来自_model
               std::vector<Question> all_questions;
               model.GetAllQuestions(all_questions);
               //如何借助 all_questions数据得到最终的HTML
               std::string html;
               OjView::RenderAllQuestions(all_questions, html);
               resp.set_content(html, "text/html");
               });

    //R"()" C++11引入的语法，原始字符串（忽略字符串中的转义字符串）
    //\d+ 正则表达式 
    server.Get(R"(/question/(\d+))", [&model](const Request& req, 
                               Response& resp){
               Question question;
               model.GetQuestion(req.matches[1].str(), question);
               std::string html;
               OjView::RenderQuestion(question, html);
               resp.set_content(html, "text/html");
               });
    /*server.Post(R"(/compile/(/d+))", [&model](const Request& req, 
                               Response& resp){
                //1.先根据 ID 获取题目信息
               Question question;
               model.GetQuestion(req.matches[1].str(), question);

               //解析body，获取用户提交代码
               std::unordered_map<std::string, std::string> body_kv;
               UrlUtil::ParseBody(req.body, body_kv);
               const std::string& user_code = body_kv["code"];

               //2.构造 JSON 结构的参数
               Json::Value req_json;
               //3.真实需要编译的代码
               req_json["code"] = user_code + question.tail_cpp;
               Json::Value resp_json;
               //4.调用编译模块进行编译
               Compiler::CompileAddRun(req_json, resp_json);

               //5.根据编译结果构造最终网页
               std::string html;
               OjView::RebderResult(resp_json["stdout"].asString(), 
                                    resp_json["reason"].asString());
                });*/
    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0", 9092);
    return 0;
}

