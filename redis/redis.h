#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <hiredis/hiredis.h>

class Redis
{
    public:
        Redis();
        ~Redis();

    public:
        bool create_connect(const std::string& host, int port);

        typedef std::shared_ptr<redisReply> RedisReplyPtr;
        RedisReplyPtr command(const std::vector<std::string>& cmds);

    private:
        bool connect();
        void clear();
        void set_addr(const std::string& host, int port);

    private:
        int                port_;                            // redis 端口
        std::string        host_;                            // redis IP地址
        redisContext*      context_;
};

