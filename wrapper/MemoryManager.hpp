// SPDX-License-Identifier: MIT
// Copyright (c) 2025 EmbedDIP

#pragma once

extern "C" {
#include "core/memory_manager.h"
}

#include <cstddef>
#include <cstring>
#include <utility>

namespace embedDIP
{

/**
 * @brief Wrapper for raw memory allocated by memory_alloc/memory_free.
 * @see memory_alloc For underlying C allocation function
 * @see memory_free For underlying C deallocation function
 */
class Memory
{
  public:
    /**
     * @brief Creates an empty memory handle.
     */
    Memory() : ptr_(nullptr), size_(0) {}

    /**
     * @brief Allocates raw memory buffer.
     * @param[in] size Requested size in bytes
     * @note Check allocation success with data() or size()
     * @see memory_alloc For underlying C allocation function
     */
    explicit Memory(std::size_t size) : ptr_(memory_alloc(size)), size_(size)
    {
        // On failure, ptr_ will be nullptr - check with get() or bool operator
        // No exceptions in embedded systems
    }

    /**
     * @brief Frees owned memory block.
     * @see memory_free For underlying C deallocation function
     */
    ~Memory()
    {
        if (ptr_) {
            memory_free(ptr_);
        }
    }

    // Non-copyable
    Memory(const Memory &) = delete;
    Memory &operator=(const Memory &) = delete;

    // Movable
    /**
     * @brief Move-constructs ownership from another wrapper.
     * @param[in,out] other Source wrapper (becomes null after move)
     */
    Memory(Memory &&other) noexcept : ptr_(other.ptr_), size_(other.size_)
    {
        other.ptr_ = nullptr;
        other.size_ = 0;
    }

    /**
     * @brief Move-assigns ownership from another wrapper.
     * @param[in,out] other Source wrapper (becomes null after move)
     * @return Reference to this object
     */
    Memory &operator=(Memory &&other) noexcept
    {
        if (this != &other) {
            if (ptr_) {
                memory_free(ptr_);
            }
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    /**
     * @brief Returns mutable pointer to allocated memory.
     * @return Pointer to memory buffer, or nullptr if not allocated
     */
    void *data()
    {
        return ptr_;
    }
    /**
     * @brief Returns const pointer to allocated memory.
     * @return Const pointer to memory buffer, or nullptr if not allocated
     */
    const void *data() const
    {
        return ptr_;
    }

    /**
     * @brief Returns allocation size in bytes.
     * @return Size of allocated buffer in bytes
     */
    std::size_t size() const
    {
        return size_;
    }

  private:
    void *ptr_;
    std::size_t size_;
};

/**
 * @brief RAII wrapper for typed memory (like malloc() + array of T)
 */
template <typename T> class MemoryBlock
{
  public:
    /**
     * @brief Allocates typed array storage.
     * @param count Number of elements to allocate.
     */
    explicit MemoryBlock(std::size_t count)
        : ptr_(static_cast<T *>(memory_alloc(sizeof(T) * count))),
          count_(count)
    {
        // On failure, ptr_ will be nullptr - check with get() or bool operator
        // No exceptions in embedded systems
    }

    /**
     * @brief Frees typed allocation.
     */
    ~MemoryBlock()
    {
        if (ptr_) {
            memory_free(ptr_);
        }
    }

    // Disable copy
    MemoryBlock(const MemoryBlock &) = delete;
    MemoryBlock &operator=(const MemoryBlock &) = delete;

    // Enable move
    /**
     * @brief Move-constructs typed allocation ownership.
     * @param other Source wrapper.
     */
    MemoryBlock(MemoryBlock &&other) noexcept : ptr_(other.ptr_), count_(other.count_)
    {
        other.ptr_ = nullptr;
        other.count_ = 0;
    }

    /**
     * @brief Move-assigns typed allocation ownership.
     * @param other Source wrapper.
     * @return Reference to this object.
     */
    MemoryBlock &operator=(MemoryBlock &&other) noexcept
    {
        if (this != &other) {
            if (ptr_) {
                memory_free(ptr_);
            }
            ptr_ = other.ptr_;
            count_ = other.count_;
            other.ptr_ = nullptr;
            other.count_ = 0;
        }
        return *this;
    }

    /**
     * @brief Returns mutable typed pointer.
     */
    T *data()
    {
        return ptr_;
    }
    /**
     * @brief Returns const typed pointer.
     */
    const T *data() const
    {
        return ptr_;
    }

    /**
     * @brief Returns element count.
     */
    std::size_t size() const
    {
        return count_;
    }

    /**
     * @brief Provides mutable indexed element access.
     * @param i Element index.
     * @return Reference to element.
     */
    T &operator[](std::size_t i)
    {
        return ptr_[i];
    }
    /**
     * @brief Provides const indexed element access.
     * @param i Element index.
     * @return Const reference to element.
     */
    const T &operator[](std::size_t i) const
    {
        return ptr_[i];
    }

  private:
    T *ptr_;
    std::size_t count_;
};

}  // namespace embedDIP
