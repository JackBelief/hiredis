#pragma once

#include <sstream>

// 本结构的作用是将所有的命令参数存放到std::vector<std::string>
struct RedisCommandParasBuilder
{
    template <typename T>
    RedisCommandParasBuilder& operator << (const T& data)
    {
        oss.str("");
        oss << data;
        command_paras.push_back(oss.str());

        return *this;   
    }

    template <typename T>
    RedisCommandParasBuilder& operator << (const std::vector<T>& data)
    {
        for (auto& element : data)
        {
            oss.str("");
            oss << element;
            command_paras.push_back(oss.str());
        }

        return *this;
    }

    std::vector<std::string> get()
    {
        return std::move(command_paras);
    }

    std::ostringstream oss;
    std::vector<std::string> command_paras;
};

// response类型
struct NoResponse{};

template <class T>
using response_remove_reference = typename std::remove_reference<T>::type;

#define DeclareType(exp) response_remove_reference<decltype(exp)>


// redis命令结构定义
struct NX{};                             // 只有当键不存在时，才对键进行设置操作，如:SET key value NX，一般用于redis锁操作
struct XX{};                             // 只有当键存在时，才对键进行设置操作，如:SET key value XX
struct PX{};                             // 设置键的过期时间为毫秒，如:SET key value PX millisecond
struct EX{};                             // 设置键的过期时间为秒，如:SET key value EX second

template <typename R = NoResponse>
struct set
{
    using ResponseType = R;
    template <typename T>
    std::vector<std::string> operator()(const std::string& key, const T& value)
    {
        RedisCommandParasBuilder para_builder;
        para_builder << "SET" << key << value;

        return std::move(para_builder.get());
    }

    template <typename T, typename ET>
    std::vector<std::string> operator()(const std::string& key, const T& value, EX&&, ET expire_time)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "EX" << expire_time;
          return std::move(para_builder.get());
    }

    template <typename T, typename PT>
    std::vector<std::string> operator()(const std::string& key, const T& value, PX&&, PT expire_time)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "PX" << expire_time;
          return std::move(para_builder.get());
    }

    template <typename T>
    std::vector<std::string> operator()(const std::string& key, const T& value, NX&&)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "NX";
          return std::move(para_builder.get());
    }

    template <typename T>
    std::vector<std::string> operator()(const std::string& key, const T& value, XX&&)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "XX";
          return std::move(para_builder.get());
    }

    template <typename T, typename ET>
    std::vector<std::string> operator()(const std::string& key, const T& value, EX&&, ET expire_time, NX&&)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "EX" << expire_time << "NX";
          return std::move(para_builder.get());
    }

    template <typename T, typename ET>
    std::vector<std::string> operator()(const std::string& key, const T& value, EX&&, ET expire_time, XX&&)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "EX" << expire_time << "XX";
          return std::move(para_builder.get());
    }

    template <typename T, typename PT>
    std::vector<std::string> operator()(const std::string& key, const T& value, PX&&, PT expire_time, NX&&)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "PX" << expire_time << "NX";
          return std::move(para_builder.get());
    }

    template <typename T, typename PT>
    std::vector<std::string> operator()(const std::string& key, const T& value, PX&&, PT expire_time, XX&&)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "SET" << key << value << "PX" << expire_time << "XX";
          return std::move(para_builder.get());
    }
};

template <typename R>
struct get
{
    using ResponseType = R;

    std::vector<std::string> operator()(const std::string& key)
    {
          RedisCommandParasBuilder para_builder;
          para_builder << "GET" << key;
          return std::move(para_builder.get());
    }
};

template <typename R>
struct eval
{
    using ResponseType = R;

    template <typename ... ARGS>
    std::vector<std::string> operator()(const std::string& script, ARGS&& ... args)
    {
        RedisCommandParasBuilder para_builder;
        para_builder << "EVAL" << script;
        auto ans = {(para_builder << std::forward<ARGS>(args), 0)...};
        (void)ans;

        return std::move(para_builder.get());
    }
};

