/**
 * @file   Resource.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_RESOURCE_HPP_
#define _IWBAN_RESOURCE_HPP_

#include <Global.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <atomic>

/* TODO Copied snippet here to not delete it...

class Material : public res::Resource
{
private:
    // Data members
    sf::Texture     _texture;

    sf::Texture     _border_texture;

    unsigned        _border_width;

    #####// TO DO Footstep sound

    // Flags
    bool            _smooth;
    bool            _dangerous;
    bool            _liquid;


public:
    bool loadFromFile(const Path & filename);

};
// class Material

// ---- ---- ---- ----

namespace impl
{

// Allocator
class MaterialAllocator
{
public:
    Material * construct(const Path & file)
    {
        IWBAN_LOG_INFO("Loading material %s", file.c_str());
        Material * mat = new Material();
        if (!mat->loadFromFile(file))
        {
            #####// TO DO Throw? Crash?
            IWBAN_LOG_ERROR("Could not load material %s", file.c_str());
            delete mat;
            mat = 0;
        }
        return mat;
    }

    void destroy(Material * tex)
    {
        delete tex;
    }

};
// class MaterialAllocator

}
// namespace impl

// ---- ---- ---- ----

typedef res::Manager<Material, Path,
                     impl::MaterialAllocator> MaterialManager;

inline MaterialManager & getMaterialManager()
{
    static MaterialManager s_material_manager;
    return s_material_manager;
}

}
// namespace gfx

 */

namespace res
{

class Resource
{
public:
    // Template typedef for derived classes
    template<class T>
    using PtrTpl = boost::intrusive_ptr<T>;

    // Generic Resource pointer
    // Shadow it with your own type (e.g. PtrTpl<Texture>)
    typedef PtrTpl<Resource>    Ptr;


private:
    // Atomic reference count
    mutable std::atomic<int>    _references;


public:
    // Constructor
                    Resource();

    // Destructor
    virtual         ~Resource();

    // Accessors
    int             getReferencesCount() const;


private:
    // intrusive_ptr access functions
    friend void intrusive_ptr_add_ref(const Resource * rc)
    {
        rc->_references.fetch_add(1, std::memory_order_relaxed);
    }

    friend void intrusive_ptr_release(const Resource * rc)
    {
        rc->_references.fetch_sub(1, std::memory_order_relaxed);
    }

};
// class Resource

// ---- ---- ---- ----

inline Resource::Resource() : _references(0)
{
}

inline Resource::~Resource()
{
    IWBAN_ASSERT_MSG(_references.load(std::memory_order_relaxed) == 0,
                     "Resource deleted while pointers are still active");
}

inline int Resource::getReferencesCount() const
{
    return _references.load(std::memory_order_relaxed);
}

}
// namespace res

#endif // _IWBAN_RESOURCE_HPP_
