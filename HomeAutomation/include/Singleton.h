#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton
{
    public:
        static Singleton& getInstance();
    protected:
        Singleton() {}
    public:
        Singleton(Singleton const&) = delete;
        void operator=(Singleton const&)  = delete;
};

#endif //SINGLETON_H