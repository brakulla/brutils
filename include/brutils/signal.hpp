/*
 * Copyright 2019 Burak Ongay
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file SigSlot.h
 *
 * Description
 */

#ifndef UTILS_SIGSLOT_H
#define UTILS_SIGSLOT_H

#include <map>
#include <functional>

namespace brutils
{
    template<typename... Args>
    class signal
    {
    public:
        signal() : _lastId(0) {};
        signal(const signal &) = delete;
        signal &operator=(const signal &) = delete;

        ~signal() = default;

    public:
        int connect(std::function<void(Args...)> const &slot) const
        {
            _slots.insert(std::make_pair(++_lastId, slot));
            return _lastId;
        }

        template<typename T>
        int connect(T *inst, void (T::*func)(Args...) const)
        {
            return connect([=](Args... args) {
                (inst->*func)(args...);
            });
        }

        void disconnect(int id)
        {
            _slots.erase(id);
        }

        void disconnectAll()
        {
            _slots.clear();
        }

        void emit(Args... p)
        {
            for (auto &slot: _slots)
            {
                slot.second(p...);
            }
        }

    private:
        mutable std::map<int, std::function<void(Args...)>> _slots;
        mutable int _lastId;
    };
}

#endif //UTILS_SIGSLOT_H
