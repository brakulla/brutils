/**
 * @file singleton_crtp.h
 *
 * Description
 */

#ifndef SINGLETON_CRTP_HPP
#define SINGLETON_CRTP_HPP

namespace brutils
{

template<class Class>
class singleton_crtp
{
public:
    static Class &getInstance()
    {
        static Class instance;
        return &instance;
    }

protected:
    static Class *_p;
    singleton_crtp() = default;
private:
    singleton_crtp(singleton_crtp const &) = default;
    singleton_crtp &operator=(singleton_crtp const &) = default;
};

}

#endif //SINGLETON_CRTP_HPP
