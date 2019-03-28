#pragma once
//Google 开发的库
#include<vector>
#include<string>
#include<ctemplate/template.h>
#include"oj_model.hpp"

class OjView
{
public:
    //根据数据，生成HTML这个动作，网页渲染（render）
    static void RenderAllQuestions(const std::
        vector<Question>& all_questions,std::string& html)
    {
        //这个函数：all_questions ==> html
        //在 C++ 代码中直接通过字符串拼接的方式构造HTML太麻烦
        //通过网页模板的方式解决问题
        
        //1.县创建一个 ctemplate 的对象，这是一个总的组织数据的对象
        //2.循环的往这个对象中添加一些子对象
        //每个子对象再设置一一些键值对（和模板中留下的{{}}是要对应的）
        //4.进行数据的替换生成最终的html
        ctemplate::TemplateDictionary dict("all_questions");
        for(const auto& question : all_questions)
        {
            ctemplate::TemplateDictionary* table_dict
                =dict.AddSectionDictionary("question");
            table_dict->SetValue("id", question.id);
            table_dict->SetValue("name", question.name);
            table_dict->SetValue("difficult", question.difficult);
        }
        ctemplate::Template* tp1;
        tp1 = ctemplate::Template::GetTemplate(
                    "./template/all_questions.html",
                    ctemplate::DO_NOT_STRIP);
        tp1->Expand(&html, &dict);
    }


    static void RenderQuestion(const Question& question,
                               std::string& html)
    {
        ctemplate::TemplateDictionary dict("question");
        dict.SetValue("id", question.id);
        dict.SetValue("name", question.name);
        dict.SetValue("difficult", question.difficult);
        dict.SetValue("desc", question.desc);
        dict.SetValue("header", question.header_cpp);
        ctemplate::Template* tp1;
        tp1 = ctemplate::Template::GetTemplate(
                    "./template/question.html",
                    ctemplate::DO_NOT_STRIP);
        tp1->Expand(&html, &dict);
    }


    static void RenderResult(const std::string& str_stdout,
                             const std::string& reason, std::string& html)
    {
        ctemplate::TemplateDictionary dict("result");
        dict.SetValue("stdout", str_stdout);
        dict.SetValue("reason",reason);

        ctemplate::Template* tp1;
        tp1 = ctemplate::Template::GetTemplate(
                    "./template/result.html",
                    ctemplate::DO_NOT_STRIP);
        tp1->Expand(&html, &dict);
    }
};
