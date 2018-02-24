#pragma once

#include <string.h> 
#include "redis_command.h"
#include "data_type_cast.h"

// hiredis reply type
#define REDIS_REPLY_STRING   1   // 返回字符串，查看str,len字段
#define REDIS_REPLY_ARRAY    2   // 返回一个数组，查看elements的值（数组个数），通过element[index]的方式访问数组元素，每个数组元素是一个redisReply对象的指针(嵌套)
#define REDIS_REPLY_INTEGER  3   // 返回整数，从integer字段获取值
#define REDIS_REPLY_NIL      4   // 没有数据返回
#define REDIS_REPLY_STATUS   5   // 表示状态，内容通过str字段查看，字符串长度是len字段
#define REDIS_REPLY_ERROR    6   // 表示出错，查看出错信息，如上的str,len字段

/***********************************************************************************
    ReplyPtr存放的是命令执行后，redis回复的数据；DataType是用户期待返回的数据结构的
    类型，根据DataType的类型不同，在对ReplyPtr统一解析时，也是分情况处理的。
***********************************************************************************/
// 初始模板
template <typename ReplyPtr, typename DataType, bool IsPod>
struct RedisResponseDataGet{};

// 偏特化模板--针对基本类型(int、char、long、float、double、bool)
template <typename ReplyPtr, typename DataType> 
struct RedisResponseDataGet<ReplyPtr, DataType, true>
{
    static inline int get(const ReplyPtr& reply, DataType& data)
    {
        if (REDIS_REPLY_NIL == reply->type) {
            return 0;
        }
        else if(REDIS_REPLY_STRING == reply->type) {
            std::string str;
            str.append(reply->str, reply->len);
            data = StringCastWrapper<DataType>()(std::move(str)); 
            return 0;
        }
        else if (REDIS_REPLY_INTEGER) {
            data = StringCastWrapper<DataType>()(std::to_string(reply->integer));
            return 0;
        }
        else if(REDIS_REPLY_STATUS) {
            std::string str = strcasecmp(reply->str,"OK") == 0 ? "1" : "0";
            data = StringCastWrapper<DataType>()(str); 
            return 0;
        }

        return -1;
    }

};

// 偏特化模板--针对非POD类型(string)
template <typename Reply>
struct RedisResponseDataGet<Reply, std::string, false>
{
    static inline int get(const Reply& reply, std::string& data)
    {
        if (REDIS_REPLY_NIL == reply->type) { 
            return 0;
        }
        else if (REDIS_REPLY_STRING == reply->type) {
            data.append(reply->str,reply->len);
            return 0;
        }
        else if (REDIS_REPLY_INTEGER == reply->type) {
            data = std::to_string(reply->integer);
            return 0;
        }
        else if (REDIS_REPLY_STATUS == reply->type) {
            data = strcasecmp(reply->str,"OK") == 0 ? "1" : "0";
            return 0;
        }

        return -1;
    }

};

// 偏特化模板--针对非POD类型(vector)
template <typename Reply, typename DataType>
struct RedisResponseDataGet<Reply, std::vector<DataType>, false> 
{
    static inline int get(const Reply& reply, std::vector<DataType>& data)
    {
        if (REDIS_REPLY_NIL == reply->type) {
            return 0;
        }
        else if (REDIS_REPLY_ARRAY == reply->type) {
            std::string str;
            for (int uli = 0; uli < reply->elements; ++uli)
            {
                str.clear();
                str.append(reply->element[uli]->str, reply->element[uli]->len);
                auto&& element = StringCastWrapper<DataType>()(std::move(str));
                data.push_back(std::move(element));
            }
            return 0;
        }

        return -1;
    }

};

template <typename DataType>
struct RedisResponseGet
{
    template<typename ReplyPtr>
    static inline int get(const ReplyPtr& reply, DataType& data)
    {
        if (reply.get() == NULL) {
            return -1;
        }
 
        switch (reply->type) {
            case REDIS_REPLY_STRING:
            case REDIS_REPLY_INTEGER:
            case REDIS_REPLY_ARRAY:
            case REDIS_REPLY_NIL:
            case REDIS_REPLY_STATUS:
            {
                int ret = RedisResponseDataGet<ReplyPtr, typename std::remove_reference<DataType>::type, std::is_pod<DataType>::value>::get(reply, data);
                return ret == 0 ? ret : -2;
            }

            case REDIS_REPLY_ERROR:
                return -3;
        }

        return -4;
    }
};

template <>
struct RedisResponseGet<NoResponse>
{
    template<typename ReplyPtr>
    static inline int get(const ReplyPtr& reply, const NoResponse&)
    {
        if (reply.get() == NULL) {
            return -1;
        }

        switch(reply->type)
        {
            case REDIS_REPLY_STRING:
            case REDIS_REPLY_INTEGER:    
            case REDIS_REPLY_ARRAY:    
            case REDIS_REPLY_NIL:    
            case REDIS_REPLY_STATUS:    
                return 0;

            case REDIS_REPLY_ERROR:
                return -2;
        }

        return -3;
    }    
};

