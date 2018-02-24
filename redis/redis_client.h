#pragma once

#include "redis.h"
#include "thread_private_data.h"

class RedisClient
{
    public:
        RedisClient() {}
        ~RedisClient() {}

    public:
        void set_addr(const std::string& host, int port);

        inline int get_port() const { return port_; }
        inline const std::string& get_host() const { return host_; }

        Redis* get_redis();

    private:
        int                                  port_;         // redis 端口
        std::string                          host_;         // redis IP
        ThreadPrivateData<Redis>             private_data_; // redis client 线程私有(对于同一台redis，每个工作线程都有一个连接)
};
