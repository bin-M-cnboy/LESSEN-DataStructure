#pragma once

#include "list.h"

namespace DATA_STRUCTURE {

template<class data_base>
class queue : public list<data_base> {
    public:
        queue() = default;
        ~queue() = default;

        bool empty() override {
            // TODO
            return true;
        }
        void pop() override {
            // TODO
        }
        data_base top() override {
            // TODO
            data_base tmp;
            return tmp;
        }
        void push(data_base push_data) override {
            // TODO
        }
        void clear() override {
            // TODO
        }
    private:
        int top_index = 0;
        int tail_index = 0;
        data_base list_data[MAX_LINK_LIST_NUM];
};

}
