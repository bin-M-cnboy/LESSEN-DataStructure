#pragma once

#include "list.h"

namespace DATA_STRUCTURE {

template<class data_base>
class queue : public list<data_base> {
    public:
        queue() = default;
        ~queue() = default;

        bool empty() override {
            return top_index == tail_index;
        }

        void pop() override {
            if (!empty()) {
                top_index = (top_index + 1) % MAX_LINK_LIST_NUM;
            }
        }

        data_base top() override {
            if (empty()) return data_base{};
            return list_data[top_index];
        }

        void push(data_base push_data) override {
            if ((tail_index + 1) % MAX_LINK_LIST_NUM == top_index) return;

            list_data[tail_index] = push_data;
            tail_index = (tail_index + 1) % MAX_LINK_LIST_NUM;
        }

        void clear() override {
            top_index = tail_index = 0;
        }

    private:
        int top_index = 0;
        int tail_index = 0;
        data_base list_data[MAX_LINK_LIST_NUM];
    };

}