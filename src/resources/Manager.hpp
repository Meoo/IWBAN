/**
 * @file   Manager.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_MANAGER_HPP_
#define _IWBAN_MANAGER_HPP_

#include <Global.hpp>

#include <resources/Resource.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/unordered_map.hpp>
#include <boost/utility/enable_if.hpp>

namespace res
{

namespace impl
{

/**
 * Default resource allocator.
 *
 * Do not allocate anything, and use @c delete keyword for destruction.
 *
 * @tparam T Resource type.
 * @tparam I Identifier type.
 */
template<class T, class I>
class DefaultAllocator
{
public:
    /**
     * Allocate and initialize a resource.
     *
     * @param id Resource identifier.
     * @return Pointer to a valid resource, or null pointer.
     */
    T *     construct(const I & id) { return nullptr; }

    /**
     * Destroy and deallocate a resource.
     *
     * @param res Pointer to the resource to destroy.
     */
    void    destroy(T * res)        { delete res; }

};
// class DefaultAllocator

}
// namespace impl

// ---- ---- ---- ----

/**
 * Resource manager. TODO Multithread support?
 *
 * By default, a manager cannot allocate resources on it's own, and will use
 * the keyword @c delete to deallocate a resource.
 * Use your own allocator type to override this behavior.
 *
 * @tparam T Resource type.
 * @tparam I Identifier type.
 * @tparam A Allocator class. Default is @ref impl::DefaultAllocator.
 */
template<class T, class I,
         class A = impl::DefaultAllocator<T, I> >
class Manager
{
    BOOST_STATIC_ASSERT_MSG((boost::is_base_of<Resource, T>::value),
                            "T must be derived from Resource");
public:
    using   TPtr = typename Resource::PtrTpl<T>;

    typedef boost::unordered::unordered_map<I, T *> ResMap;

private:
    // Data members
    A       _allocator;

    ResMap  _resource_map;

public:
    // Constructor TODO Private with friend declaration?
    explicit Manager(const A & allocator = A())
        : _allocator(allocator)
    {}

    // Destructor TODO Deallocate everything
    ~Manager() {}

    // Functions
    /**
     * Find a resource in the manager.
     *
     * If the resource does not exist, the allocator is called to create it.
     * If the allocator return a null pointer, this function does too.
     *
     * @param id Resource identifier
     * @return Smart pointer to the resource, or an empty smart pointer.
     */
    TPtr get(const I & id)
    {
        typename ResMap::iterator it = _resource_map.find(id);

        if (it != _resource_map.end())
            return TPtr(it->second);

        else
        {
            T * resource = _allocator.construct(id);
            if (resource)
                _resource_map.insert(typename ResMap::value_type(id, resource));

            return TPtr(resource);
        }
    }

    /**
     * Register @a resource into the manager manually.
     *
     * @param id Resource identifier.
     * @param resource Resource pointer to register.
     * @return Smart pointer to the resource.
     * @throw TODO ???_exception A resource with this identifier already exists.
     */
    TPtr put(const I & id, T * resource)
    {
        typename ResMap::iterator it = _resource_map.find(id);

        if (it != _resource_map.end())
        {
            _resource_map.insert(typename ResMap::value_type(id, resource));
            return TPtr(resource);
        }
        else
            throw "Resource already exists"; // TODO Better throw, this one is stupid
    }

    /**
     * Clean unused resources.
     */
    void clean()
    {
        for (typename ResMap::iterator it = _resource_map.begin();
             it != _resource_map.end(); ++it)
        {
            if (it->second->getReferencesCount() == 0)
            {
                _allocator.destroy(it->second);
                it = _resource_map.erase(it);

                if (it == _resource_map.end())
                    return;
            }
        }
    }

};
// class Manager

}
// namespace res

#endif // _IWBAN_MANAGER_HPP_
