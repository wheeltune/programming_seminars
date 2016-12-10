#pragma once

#include <atomic>
#include <memory>

template<typename T>
class lock_free_queue
{
public:
    lock_free_queue() : inDeque(0) {
        Node* sentinale = new Node();
        head_.store(sentinale);
        storedHead_.store(sentinale);
        tail_.store(sentinale);
    }

    ~lock_free_queue() {
        head_.store(tail_.load()->next);
        inDeque.store(1);
        tryClean();
    }

    void enqueue(T item) {
        Node* const new_node = new Node(item);

        while (true) {
            Node* curr_tail = tail_.load();
            Node* curr_tail_next = curr_tail->next.load();

            if (curr_tail == tail_.load()) {
                if (curr_tail_next == nullptr) {
                    if (tail_.load()->next.compare_exchange_weak(curr_tail_next, new_node)) {
                        tail_.compare_exchange_weak(curr_tail, new_node);
                        break;
                    }
                } else {
                    tail_.compare_exchange_weak(curr_tail, curr_tail_next);
                }
            }
        }
    }

    bool dequeue(T& item) {
        ++inDeque;
        while (true) {
            Node* curr_head = head_.load();
            Node* curr_tail = tail_.load();
            Node* curr_head_next = curr_head->next.load();

            if (curr_head == head_.load()) {
                if (curr_head == curr_tail) {
                    if (curr_head_next == nullptr) {
                        return false;
                    }

                    tail_.compare_exchange_weak(curr_head, curr_head_next); 
                } else {
                    if (head_.compare_exchange_weak(curr_head, curr_head_next)) {
                        item = curr_head_next->data;
                        tryClean();
                        break;
                    }
                }
            }
        }
        --inDeque;
        return true;
    }

private:
    struct Node
    {
        Node() : next(nullptr) {}
        Node(T data_)
            : data(data_), next(nullptr) {}

        T data;
        std::atomic<Node*> next;
    };

    void tryClean() {
        if (inDeque == 1) {
            Node* curr_head = head_.load();
            while (storedHead_.load() != curr_head) {
                Node* next = storedHead_.load()->next;
                delete storedHead_;
                storedHead_.store(next);
            }
        }
    }

    std::atomic<unsigned> inDeque;
    std::atomic<Node*> storedHead_, head_, tail_;
};