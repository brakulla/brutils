/**
 * @file singleton_crtp.h
 *
 * Description
 */

#ifndef SINGLETON_CRTP_H
#define SINGLETON_CRTP_H

namespace brutils
{

template<class Class>
class singleton_crtp
{
public:
    static Class &getInstance()
    {
        static Class instance(token{});
        return instance;
    }

protected:
    static Class *_p;
    struct token
    {
    };
    singleton_crtp() = default;
    virtual ~singleton_crtp() = default;
private:
    singleton_crtp(singleton_crtp const &) = default;
    singleton_crtp &operator=(singleton_crtp const &) = default;
};

}

#endif //SINGLETON_CRTP_H
