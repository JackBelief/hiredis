#include "redis_client.h"

void RedisClient::set_addr(const std::string& host, int port)
{
    host_ = host;
    port_ = port;

    return;
}

Redis* RedisClient::get_redis()
{
    Redis* pdata = (Redis*)private_data_.get();
    if (!pdata)
    {
        pdata = new Redis();
        pdata->create_connect(host_, port_);
        private_data_.set(pdata);
    }

    return pdata;
}



