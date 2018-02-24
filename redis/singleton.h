
#pragma once

// 单例模型
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

// 分类别的多单例模型，类型不同生成的单例对象也不同，如：MultipleInstance<T, 0>和MultipleInstance<T, 1>就是两种不同的类型，会分别生成一个单例对象，如：
// Singleton<MultipleInstance<T, 0>>、Singleton<MultipleInstance<T, 1>>
template <typename T, int index>
class MultipleInstance : public T
{

};
