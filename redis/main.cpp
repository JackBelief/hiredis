
#include <unistd.h>
#include "redis_cluster.h"
#include "redis_command_executor.h"

void* func(void* para)
{
    std::shared_ptr<RedisClient> pp = TestRedis.get_client(0);
    if (NULL == pp.get())             // TODO
    {
        std::cout << "NULL" << std::endl;
        return NULL;
    }

    Redis* p = pp->get_redis();
    if (*(int*)para % 2 == 1)
    {
        std::string set_lua = "                                  \
            for i=1,#(KEYS) do                                   \
                local ref = redis.call('set', KEYS[i], ARGV[i])  \
            end                                                  \
        ";

        std::vector<std::string> key_vec = {"name", "age", "addr"};
        RedisCommandExecutor<eval<NoResponse>> cmd_1(p);
        cmd_1.execute_noresponse(set_lua, key_vec.size(), key_vec, "tqy", 20, "shanghai");
    }
    else
    {
    for (int uli = 0; uli < 10; ++uli)
    {
        std::string name_str;
        RedisCommandExecutor<get<std::string>> cmd_1(p);
        cmd_1.execute_response(name_str, "name");

        std::string get_lua = "                                  \
                return redis.call('get', KEYS[1])                \
        ";

        int age = -1;
        RedisCommandExecutor<eval<int>> cmd_2(p);
        cmd_2.execute_response(age, get_lua, 1, "age");
        std::cout << "di" << uli << " ci " << name_str << " " << age << std::endl;
        sleep(1);
    }
    }

    return NULL;
}

int main()
{
    TestRedis.add_client("127.0.0.1", 6379);

    int a = 1;
    int b = 2;
    pthread_t tid;
    pthread_create(&tid, NULL, func, &a);

    sleep(1);
    pthread_t tid_1;
    pthread_create(&tid_1, NULL, func, &b);

    pthread_join(tid, NULL);
    pthread_join(tid_1, NULL);

    return 0;
}

