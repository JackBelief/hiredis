#pragma once

#include "redis.h"
#include "redis_response_get.h"

/***************************************************************************************
    由于执行命令时，最终使用的是redisCommandArgv API函数，所以execute_xxx函数的实现重点
    就是将目标数据转换为vector中的一个个元素。另外，Command类型结构也是实现这一效果。
***************************************************************************************/
template <typename Command>
class RedisCommandExecutor
{
    public:
        RedisCommandExecutor(Redis* redis): redis_(redis) {
        }

        // 带有返回值的命令执行函数
        template<typename ... Args>
        bool execute_response(typename Command::ResponseType& resp, Args... args) 
        {
            // 变参模板展开，展开效果详见每个Command的实现
            auto cmd = cmd_(std::forward<Args>(args)...);

            // 执行命令，解析redis回复，然后返回目标类型的结果
            Redis::RedisReplyPtr reply_ptr = redis_->command(cmd);
            if (RedisResponseGet<typename Command::ResponseType>::get(reply_ptr, resp) < 0)
            {
                return false;
            }

            return true;
        }

        // 不带有返回值的命令执行函数
        template<typename ... Args>
        bool execute_noresponse(Args... args)
        {
            auto cmd = cmd_(std::forward<Args>(args)...);
            Redis::RedisReplyPtr reply_ptr = redis_->command(cmd); 
            if (RedisResponseGet<NoResponse>::get(reply_ptr, NoResponse()) < 0)
            {
                return false;
            }

            return true;
        }

    private:
        Command                                     cmd_;              // 命令对象
        Redis*                                    redis_;              // redis 对象
};


