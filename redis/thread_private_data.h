#pragma once

// 线程私有锁类的特点：
// 1）类的构造和析构函数完成线程私有锁的创建和删除；
// 2）类提供仿函数实现对线程私有锁存储数据对象的获取；

template<typename T>
void thread_private_data_destruct(void* para)
{
    if (para)
    {
        delete (T*)para;
        para = NULL;
    }

    return;
}

template<typename T>
class ThreadPrivateData
{
    public:
        ThreadPrivateData()
        {
            std::string rst = 0 == pthread_key_create(&private_data_key_, thread_private_data_destruct<T>) ? "success" : "false";
            std::cout << "create thread private data key rst=" << rst << " key=" << private_data_key_ << std::endl;
        }

        ~ThreadPrivateData()
        {
            pthread_key_delete(private_data_key_);
        }

    public:
        void set(T* ptr) const
        {
            pthread_setspecific(private_data_key_, ptr);
            return;
        }

        T* get() const
        {
            std::cout << "get tid=" << pthread_self << " key=" << private_data_key_ << std::endl;
            return (T*)pthread_getspecific(private_data_key_);
        }

    private:
        pthread_key_t         private_data_key_;
};
