#ifndef CPU_MEMORY_BINDING_H
#define CPU_MEMORY_BINDING_H

#include "core/slang-basic.h"

#include "core/slang-memory-arena.h"

namespace renderer_test {

struct CPUMemoryBinding
{
    struct Buffer
    {
        Buffer() : m_data(nullptr), m_sizeInBytes(0) {}
        uint8_t* m_data;
        size_t m_sizeInBytes;
    };

    struct Location
    {
        bool isValid() const { return m_cur != nullptr; }
        bool isInvalid() const { return m_cur == nullptr; }

        Location toField(const char* name) const;
        Location toIndex(int index) const;

        slang::TypeLayoutReflection* getTypeLayout() const { return m_typeLayout; }
        uint8_t* getPtr() const { return m_cur; }

        Location():m_cur(nullptr) {}

        static Location make(slang::TypeLayoutReflection* typeLayout, uint8_t* ptr)
        {
            Location loc;
            loc.m_typeLayout = typeLayout;
            loc.m_cur = ptr;
            return loc;
        }

    protected:
        slang::TypeLayoutReflection* m_typeLayout;
        uint8_t* m_cur;
    };
    
    slang::VariableLayoutReflection* getParameterByName(const char* name);

    Location find(const char* name);

    SlangResult setBufferContents(const Location& location, const void* initialData, size_t sizeInBytes);
    SlangResult setNewBuffer(const Location& location, const void* initialData, size_t sizeInBytes, Buffer& outBuffer);
    SlangResult setObject(const Location& location, void* object);
    SlangResult setInplace(const Location& location, const void* data, size_t sizeInBytes);
    SlangResult init(slang::ShaderReflection* reflection);
    CPUMemoryBinding();

    Buffer _allocateBuffer(size_t size);
    Buffer _allocateBuffer(size_t size, const void* initialData, size_t initialSize);

    SlangResult _add(slang::VariableLayoutReflection* var, slang::TypeLayoutReflection* type, void* dst, Buffer& outBuffer);

    Slang::MemoryArena m_arena;    ///< Storage for buffers

    Buffer m_rootBuffer;

    slang::ShaderReflection* m_reflection;
    
    // All buffers
    Slang::List<Buffer> m_allBuffers;
};

} // renderer_test

#endif //CPU_MEMORY_BINDING_H
