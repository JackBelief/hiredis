#pragma once

// �߳�˽��������ص㣺
// 1����Ĺ����������������߳�˽�����Ĵ�����ɾ����
// 2�����ṩ�º���ʵ�ֶ��߳�˽�����洢���ݶ���Ļ�ȡ��

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
