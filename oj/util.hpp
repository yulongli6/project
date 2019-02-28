#pragma once
#include<cstdlib>
#include<stdint.h>
#include<iostream>
#include<sys/time.h>
#include<fstream>
#include<string>

////////////////////////////////////////////////////////////
//准备一个时间戳工具
////////////////////////////////////////////////////////////

class TimeUtil
{
public:
    //获取当前的时间戳
    static int64_t TimStamp()
    {
        struct timeval tv;
        ::gettimeofday(&tv, NULL);
        return tv.tv_sec;
    }

    static int64_t TimeStampMS()
    {
        struct timeval tv;
        ::gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
private:

};

////////////////////////////////////////////////////////////
//打印日志工具
////////////////////////////////////////////////////////////

//期望打印的格式：
//[I1551336892 util.cpp:31] hello
//[w1551336892 util.cpp:31] hello
//[E1551336892 util.cpp:31] hello
//[F1551336892 util.cpp:31] hello
//日志的使用方式形如：
//LOG(INFO) << "hello" << "\n";
//日志的级别.
//FATAL 致命
//ERROR 错误
//WARING 警告
//INFO 提示

enum level{
    INFO,
    WARING,
    ERROR,
    FATAL,
};

inline std::ostream& log(level level,
const std::string& file_name, int line_num)
{
    std::string prefix = "[";
    if(level == INFO)
        prefix += "I";
    else if(level == WARING)
        prefix += "W";
    else if(level == ERROR)
        prefix += "E";
    else if(level == FATAL)
        prefix += "F";
    prefix += std::to_string(TimeUtil::TimStamp());
    prefix += " ";
    prefix += file_name;
    prefix += ":";
    prefix += std::to_string(line_num);
    prefix += "] ";
    std::cout << prefix;
    return std::cout;
}


#define LOG(level) log(level, __FILE__, __LINE__)

////////////////////////////////////////////////////////////
//准备一个文件相关工具类
////////////////////////////////////////////////////////////

class FileUtil
{
public:
    //传入一个文件路径，帮我们把文件的所有内容都读出来放到
    //content字符串中
    static bool Read(const std::string& file_path, std::string& content)
    {
        content.clear();
        std::ifstream file(file_path.c_str());
        if(!file.is_open()){
            return false;
        }

        std::string line;
        while(std::getline(file, line))
            content += line + "\n";
        file.close();
        return true;
    }

    static bool Write(const std::string& file_path,
                      const std::string& content)
    {
        std::ofstream file(file_path.c_str());
        if(!file.is_open())
            return false;
        file.write(content.c_str(), content.size());
        file.close();
        return true;
    }
};

