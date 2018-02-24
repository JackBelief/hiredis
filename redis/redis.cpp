#include "redis.h"


Redis::Redis() : context_(NULL)
{

}

Redis::~Redis()
{
    clear();
}

void Redis::clear()
{
    if (context_)
    {
        redisFree(context_);
        context_ = NULL;
    }

    return;
}

bool Redis::create_connect(const std::string& host, int port)
{
    set_addr(host, port);
    return connect();
}

void Redis::set_addr(const std::string& host, int port)
{
    host_ = host;
    port_ = port;

    return;
}

bool Redis::connect()
{
    clear();

    struct timeval timeout = {1, 500000};
    context_ = redisConnectWithTimeout(host_.c_str(), port_, timeout);
    if (!context_) {
        std::cout << "connect redis fail ip= " << host_ << " port=" << port_ << std::endl;
        return false;
    }
    else if (0 != context_->err) {
        std::cout << "connect redis fail ip= " << host_ << " port=" << port_ << " err=" << context_->errstr << std::endl;
        clear();
        return false;
    }

    redisEnableKeepAlive(context_);
    redisSetTimeout(context_, timeout); // TODO

    return true;
}

Redis::RedisReplyPtr Redis::command(const std::vector<std::string>& cmds)
{
    if (!context_ && !connect()) {
        return std::shared_ptr<redisReply>(NULL);
    }

    std::vector<size_t> each_cmd_length;
    std::vector<const char*> each_cmd_content;
    for (auto& element : cmds) {
        each_cmd_length.emplace_back(element.size());
        each_cmd_content.emplace_back(element.c_str());
    }

    redisReply* reply = (redisReply*)redisCommandArgv(context_, static_cast<int>(cmds.size()), &(each_cmd_content[0]), &(each_cmd_length[0]));
    if (!reply) {
        std::cout << "command execute fail, connect redis again!" << std::endl;
        if (connect()) {
            reply = (redisReply*)redisCommandArgv(context_, static_cast<int>(cmds.size()), &(each_cmd_content[0]), &(each_cmd_length[0]));
        }
    }

    // reply的释放需要借助API函数freeReplyObject
    return std::shared_ptr<redisReply>(reply, [](redisReply* reply){ if (reply) { freeReplyObject(reply); } });
}


