//MVC
//M => model:负责数据存储
//V => view:负责显示界面
//C => controller:核心业务逻辑
//给予文件的方式完成题目的存储
//每个题目对应一个目录，目录的名字是题目的id
//目录包含以下几个文件
//1）header.cpp 代码框架
//2）tail.cpp 代码测试用例
//3）desc.txt 题目详细描述
//除此之外，在写一个oj_config.cfg 文件，作为总的入口文件.
//这是一个行文本文件.
//这个文件的每一行对应一个需要被服务器加载的题目
//这一行包含以下信息：题目id，题目名字，题目难度，题目目录
#pragma once
#include<map>
#include<vector>
#include<iostream>
#include"util.hpp"
//model这个模块要做的事情就是把文件中存储的题目的信息加载
//起来供服务器使用


struct Question
{
    std::string id;
    std::string name;
    std::string dir;//目录
    std::string difficult;//难度
    std::string desc;//描述
    std::string header_cpp;//代码框架
    std::string tail_cpp;//测试用例
};


class OjModel
{
    //把文件上的数据加载起来
public:
    bool load()
    {
        //1.先打开 oj_config.cfg 文件
        std::ifstream file("./oj_data/oj_config.cfg");
        if(!file.is_open())
            return false;
        //2.按行读取 oj_config.cfg 文件，并且解析
        std::string line;
        while(std::getline(file, line))
        {
            //3.根据解析结果拼成Question结构
            std::vector<std::string> tokens;
            StringUtil::Split(line, "\t", tokens);
            if(tokens.size() != 4)
            {
                LOG(ERROR) << "config file format error" << std::endl;
                continue;
            }
            //4.把结构插入hash中
            Question q;
            q.id = tokens[0];
            q.name = tokens[1];
            q.difficult = tokens[2];
            q.dir = tokens[3];
            FileUtil::Read(q.dir + "/desc.txt", q.desc);
            FileUtil::Read(q.dir + "/header.cpp", q.header_cpp);
            FileUtil::Read(q.dir + "/tail_cpp", q.tail_cpp);
            //[]如果key不存在，就创建新的键值；如果key存在，
            //就查找对应的value
            _model[q.id] = q;
        }
        file.close();
        LOG(INFO) << "load " << _model.size() << " questions\n";
        return true;
    }

	bool GetAllQuestions(std::vector<Question>& questions)const
    {
        //遍历哈希表
        questions.clear();
        for(const auto& kv : _model)
        {
           questions.push_back(kv.second);
        }
		return true;
    }

    bool GetQuestion(const std::string& id, Question& q)const
    {
        std::map<std::string, Question>::const_iterator 
            pos =  _model.find(id);
        if(pos == _model.end())
        {
            return false;
        }
        q = pos->second;
		return true;
    }
private:
    //MD5、SHA1
    std::map<std::string, Question> _model;
};
