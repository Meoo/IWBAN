/**
 * @file   IndexedPool.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_INDEXEDPOOL_HPP_
#define _IWBAN_INDEXEDPOOL_HPP_

#include <Global.hpp>

namespace ut
{

template<typename T, std::size_t BlockSize, std::size_t MaxBlocks>
class IndexedPool
{
public:
    class Node
    {
    public:
        friend class IndexedPool;


    private:
        // Must be the same offset / order than in EmptyNode
        Node *      prev;
        Node *      next;

        // Data members
        std::size_t id;
        T           value;


    public:
              Node * getNext()
        {
            return next;
        }
        const Node * getNext() const
        {
            return next;
        }

              Node * getPrev()
        {
            return prev;
        }
        const Node * getPrev() const
        {
            return prev;
        }

              T & getValue()
        {
            return value;
        }
        const T & getValue() const
        {
            return value;
        }

    };

private:
    class EmptyNode
    {
    public:
        // Must be the same offset / order than in Node
        Node *      prev;
        Node *      next;

    };

    // ---- ---- ---- ----

public:
    /**
     * @warning When a list is destroyed, the elements are not freed to the
     *          IndexedPool, you have to ensure it yourself.
     *
     * @warning You should not mix elements from different IndexedPools in a
     *          single list, unless you add a mechanism to differentiate their
     *          origins, or you won't be able to free them.
     */
    class List
    {
    private:
        EmptyNode _sentry;

    public:
        List()
        {
            _sentry.next = reinterpret_cast<Node*>(&_sentry);
            _sentry.prev = reinterpret_cast<Node*>(&_sentry);
        }

        bool isEmpty() const
        {
            return _sentry.next == _sentry.prev;
        }

              Node * getFirst()
        {
            return _sentry.next;
        }
        const Node * getFirst() const
        {
            return _sentry.next;
        }

              Node * getLast()
        {
            return _sentry.prev;
        }
        const Node * getLast() const
        {
            return _sentry.prev;
        }

              void * getSentry()
        {
            return &_sentry;
        }
        const void * getSentry() const
        {
            return &_sentry;
        }

        void insertHead(Node * node)
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->next = _sentry.next;
            node->prev = reinterpret_cast<Node*>(&_sentry);
            _sentry.next = node;
        }

        void insertHead(Node * first, Node * last)
        {
            first->prev->next = last->next;
            last->next->prev = first->prev;
            last->next = _sentry.next;
            first->prev = reinterpret_cast<Node*>(&_sentry);
            _sentry.next = first;
        }

        void insertTail(Node * node)
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->prev = _sentry.prev;
            node->next = reinterpret_cast<Node*>(&_sentry);
            _sentry.prev = node;
        }

        void insertTail(Node * first, Node * last)
        {
            first->prev->next = last->next;
            last->next->prev = first->prev;
            first->prev = _sentry.prev;
            last->next = reinterpret_cast<Node*>(&_sentry);
            _sentry.prev = last;
        }

    };
    // class List

    // ---- ---- ---- ----

private:
    std::size_t _num_blocks;
    Node *      _blocks[MaxBlocks];

    List        _elements_list;


public:
    IndexedPool()
    {
        _num_blocks = 0;
        //std::memset(_blocks, 0, sizeof(_blocks));
    }

          Node * at(std::size_t i)
    {
        assert(i >= 0 && i < _num_blocks * BlockSize);
        return &_blocks[i / BlockSize][i % BlockSize];
    }
    const Node * at(std::size_t i) const
    {
        assert(i >= 0 && i < _num_blocks * BlockSize);
        return &_blocks[i / BlockSize][i % BlockSize];
    }

    Node * alloc()
    {
        if (_elements_list.isEmpty())
            reserveBlock();

        return _elements_list.getFirst();
    }

    void free(Node * node)
    {
        _elements_list.insertTail(node);
    }

    // TODO mutli alloc / free?

    std::size_t getIdFromNode(const Node * node)
    {
        return node->id;
    }


protected:
    void reserveBlock()
    {
        assert(_num_blocks < MaxBlocks);

        std::size_t base_id = _num_blocks * BlockSize;

        Node * block = new Node[BlockSize];
        _blocks[_num_blocks++] = block;
        
        // Initialize linked list
        for (std::size_t i = 0; i < BlockSize; ++i)
        {
            block[i].prev = &block[i-1];
            block[i].next = &block[i+1];
            block[i].id   = base_id + i;
        }
        
        // Make a fake sentry to be modified in insert_tail function
        // Sentry's values are modified but no used, so they can be garbage
        EmptyNode fake_sentry;

        block[0].prev           = reinterpret_cast<Node*>(&fake_sentry);
        block[BlockSize-1].next = reinterpret_cast<Node*>(&fake_sentry);

        _elements_list.insertTail(&block[0], &block[BlockSize-1]);
    }

};
// class IndexedPool

}
// namespace ut

#endif // _IWBAN_INDEXEDPOOL_HPP_
