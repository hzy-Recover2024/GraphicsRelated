/*线程安全的内存池*/
#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include <mutex>
#include <vector>
#include <memory>
#include <unordered_set>
#include <cstdlib>
namespace GRelated {
    constexpr size_t BLOCK_SIZE = 8192; //块的大小，单位是字节
    template<typename T>
    class MemoryPool {
    public:
        MemoryPool()
        {
            static_assert(BLOCK_SIZE >= sizeof(Node), "BLOCK_SIZE too small");
            // 初始化内存池
            expandPool();
        }

        ~MemoryPool()
        {
            for (auto block : m_blocks)
            {
                operator delete[](block, static_cast<std::align_val_t>(alignof(Node)));
            }
        }

        T* allocate()
        {
            // 从内存池中分配一个对象
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_listHead == nullptr)
            {
                expandPool();
            }
            auto tmp = m_listHead;
            m_listHead = m_listHead->next;
            m_allocated.insert(reinterpret_cast<T*>(tmp));
            return reinterpret_cast<T*>(tmp);
        }

        void deallocate(T* ptr)
        {
            // 回收内存
            std::lock_guard<std::mutex> lock(m_mutex);
            auto itr = m_allocated.find(ptr);
            if (itr == m_allocated.end())
            {
                return;
            }
            m_allocated.erase(itr);
            auto node = reinterpret_cast<Node*>(ptr);
            node->next = m_listHead;
            m_listHead = node;
        }

    private:

        union alignas(alignof(T)) Node //强制要求Node按照T的大小进行对齐
        {
            Node* next;
            char data[sizeof(T)];
        };

        void expandPool()
        {
            // c++17和c11之后的动态内存对齐
            auto newBlock = static_cast<char*>(operator new[](BLOCK_SIZE,
                static_cast<std::align_val_t>(alignof(Node))));
            m_blocks.push_back(newBlock);
            Node* nodeArray = reinterpret_cast<Node*>(newBlock);
            size_t nodeNum = BLOCK_SIZE / sizeof(Node);
            for (size_t i = 0; i < nodeNum - 1; ++i)
            {
                nodeArray[i].next = &nodeArray[i + 1];
            }
            nodeArray[nodeNum - 1].next = nullptr;
            m_listHead = nodeArray;
        }


        Node* m_listHead = nullptr;
        std::vector<char*> m_blocks;
        std::unordered_set<T*> m_allocated; //防止非本池分配的内存进行错误释放
        std::mutex m_mutex;
    };
}
#endif // !_MEMORY_POOL_H

