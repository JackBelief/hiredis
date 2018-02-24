#pragma once

#include "singleton.h"
#include "redis_client.h"

class RedisCluster
{
    public:
        inline size_t get_clients_count(){ return clients_.size(); }
        bool add_client(const std::string& host, const int port);
        std::shared_ptr<RedisClient> get_client(size_t index);

    private:
        std::vector< std::shared_ptr<RedisClient> > clients_;                   // 存放当前集群下的redis对象
};

#define TestRedis Singleton< MultipleInstance<RedisCluster, 0> >::instance()
