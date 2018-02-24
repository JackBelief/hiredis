
#pragma once

// ����ģ��
template <typename T>
class Singleton
{
    private:
        Singleton()
        {

        }

        ~Singleton()
        {

        }

    public:
        static T& instance()
        {
            static T temp;
            return temp;
        }
};

// �����Ķ൥��ģ�ͣ����Ͳ�ͬ���ɵĵ�������Ҳ��ͬ���磺MultipleInstance<T, 0>��MultipleInstance<T, 1>�������ֲ�ͬ�����ͣ���ֱ�����һ�����������磺
// Singleton<MultipleInstance<T, 0>>��Singleton<MultipleInstance<T, 1>>
template <typename T, int index>
class MultipleInstance : public T
{

};
