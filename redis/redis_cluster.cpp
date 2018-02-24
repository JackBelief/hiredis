#include <iostream>
#include "redis_cluster.h"

// 获取redis对象
std::shared_ptr<RedisClient> RedisCluster::get_client(size_t index)
{
    return index >= clients_.size() ? std::shared_ptr<RedisClient>(NULL) : clients_[index];
}

// 增加redis对象
bool RedisCluster::add_client(const std::string& host, int port)
{
    auto it = clients_.begin();
    std::string new_client_addr = host + ":" + std::to_string(port);

    for (; it != clients_.end(); ++it) {
        std::string cur_client_addr = (*it)->get_host() + ":" + std::to_string((*it)->get_port());
        if (new_client_addr < cur_client_addr) {
            break;
        }
        else if (new_client_addr == cur_client_addr) {
            std::cout << "redis addr repeat addr=" << cur_client_addr << std::endl;
            return false;
        }
    }

    auto new_client = std::make_shared<RedisClient>();
    new_client->set_addr(host, port);

    clients_.insert(it, new_client);
    std::cout << "new redis add success addr=" << new_client_addr << std::endl;
    return true;
}

