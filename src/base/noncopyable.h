#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

/**
 * noncopyable: 禁止派生类对象进行拷贝操作。
 * noncopyable 被继承以后，派生类对象可以正常构造和析构，
 * 但派生类对象无法进行拷贝构造和拷贝赋值操作！
 * 注：声明为 protected 的成员仅向派生类（和友元类）暴露。
 */

class noncopyable
{
public:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

#endif // NONCOPYABLE_H