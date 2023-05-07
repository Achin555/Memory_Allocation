#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include <map>
#include <unordered_set>
#include <set>
#include <queue>
#include <utility>
#include <list>
#include <string.h>
using namespace std;

// define a memory block struct

struct MemoryBlock
{
    int start_address;
    int end_address;
    bool free;
};

class MemoryManager
{
private:
    vector<MemoryBlock> memory_blocks;
    // defines a vector named memory block of type memory block
    // MemoryBlock is a user-defined struct that stores information about a memory block
    // vector to store memory blocks

public:
    MemoryManager(int size)
    {
        // initialize the memory blocks with the given size

        memory_blocks.push_back({0, size - 1, true});
    }

    // allocate memory for a process

    int allocateMemory(int size)
    {
        // find a free memory block that can accommodate the requested size

        auto it = find_if(memory_blocks.begin(), memory_blocks.end(), [&](const MemoryBlock &block)
                          { return block.free && (block.end_address - block.start_address + 1) >= size; });

        if (it == memory_blocks.end())
        {
            // no free memory block found
            return -1;
        }

        // update the found memory block and return the starting address of the allocated memory

        MemoryBlock &block = *it;
        int start_address = block.start_address;
        block.start_address += size;
        block.free = (block.end_address - block.start_address + 1) > 0;
        return start_address;
    }

    // deallocate memory for a process

    void deallocateMemory(int start_address, int size)
    {
        // find the memory block containing the specified starting address

        auto it = find_if(memory_blocks.begin(), memory_blocks.end(), [&](const MemoryBlock &block)
                          { return block.start_address == start_address; });

        if (it == memory_blocks.end())
        {
            // no memory block found with the specified starting address

            return;
        }

        // update the memory block

        MemoryBlock &block = *it;
        block.start_address -= size;
        block.free = true;

        // merge adjacent free memory blocks

        if (it != memory_blocks.begin() && (it - 1)->free)
        {
            (it - 1)->end_address = block.end_address;
            memory_blocks.erase(it);
            it = it - 1;
            block = *it;
        }
        if (it != memory_blocks.end() - 1 && (it + 1)->free)
        {
            block.end_address = (it + 1)->end_address;
            memory_blocks.erase(it + 1);
        }
    }

    // print the memory blocks

    void printMemoryBlocks()
    {
        for (const MemoryBlock &block : memory_blocks)
        {
            cout << "Start Address: " << block.start_address << ", End Address: " << block.end_address << ", Free: " << block.free << endl;
        }
    }
};
// // define a memory manager class
// class MemoryAllocator
// {
// public:
//     MemoryAllocator(size_t size)
//     {
//         m_buffer = std::malloc(size); // allocates memory of size size
//         m_size = size;
//         m_offset = 0; // current position in buffer
//       // offset is value added to current addresss to get new address
//     }

//     ~MemoryAllocator()
//     {
//         std::free(m_buffer);  // destructor to de allocate memory
//     }

//     void* allocate(size_t size) // when allocating memory to block
//   // pointer to memory of unsecifiedd type
//     {
//         if (m_offset + size > m_size)
//            // if enough space in buffer  to allocate memory
//         {
//             return nullptr;
//            // null ptr does not point to any memory location
//         }

//         void* ptr = static_cast<char*>(m_buffer) + m_offset;
// // void is casted to a appropriate data type using static cast(explicit type conversion

//         m_offset += size;
//         m_blocks[ptr] = size; // keeps track of allocated block
//         return ptr;
//     }

//     void deallocate(void* ptr)
//     {
//         auto it = m_blocks.find(ptr); // auto refer to the datatype from initial expression
//         if (it == m_blocks.end())
//         {
//             std::cerr << "Error: block not found in memory allocator" << std::endl;
//  similar to cout for error
//             return;
//         }
//         size_t size = it;
//         m_blocks.erase(it);
//         m_offset -= size;
//     }
// private:
//     void* m_buffer;
//     size_t m_size; // size of object or number of elements in sequence
//     size_t m_offset;
//     std::unordered_map<void*, size_t> m_blocks;
//  // un..map is variable with void* as key type and size_t as value . memory block as key and size as values
// };
class MemoryAllocator
{
public:
    MemoryAllocator(size_t size)
    {
        m_buffer = std::malloc(size);
        m_size = size;
        m_offset = 0;
    }

    ~MemoryAllocator()
    {
        std::free(m_buffer);
    }

    void *allocate(size_t size)
    {
        if (m_offset + size > m_size)
        {
            return nullptr;
        }

        void *ptr = static_cast<char *>(m_buffer) + m_offset;
        m_offset += size;
        m_blocks[ptr] = size;
        return ptr;
    }

    void deallocate(void *ptr)
    {
        auto it = m_blocks.find(ptr);
        if (it == m_blocks.end())
        {
            std::cerr << "Error: block not found in memory allocator" << std::endl;
            return;
        }
        size_t size = it->second;
        m_blocks.erase(it);
        m_offset += size;
    }

private:
    void *m_buffer;
    size_t m_size;
    size_t m_offset;
    std::unordered_map<void *, size_t> m_blocks;
};

class BuddyAllocator 
{
public:
    BuddyAllocator(int total_memory_bytes);
    ~BuddyAllocator();
    void* allocate(int size_bytes);
    int allocate_block(int order) ;
    void deallocate(void* ptr);
    void deallocate_block(int index);


private:
    char* memory_;
    int total_memory_bytes_;
    int last_allocated_block_;
    int block_size_;
    int max_order_;
    int* bitmap_;
    int* block_ptrs_;
    int get_block_index(int order, int offset);
    void split_block(int order, int index);
    void merge_block(int order, int index);
};

BuddyAllocator::BuddyAllocator(int total_memory_bytes)
    : memory_(nullptr), total_memory_bytes_(total_memory_bytes),
      block_size_(64), max_order_(0), bitmap_(nullptr), block_ptrs_(nullptr) 
{
    // Calculate maximum order based on total memory
    int size = total_memory_bytes_;
    while (size > 0)
    {
        size >>= 1;
        ++max_order_;
    }

    // Allocate memory for bitmap and block pointers
    int bitmap_size = pow(2, max_order_ - 1);
    bitmap_ = new int[bitmap_size];
    memset(bitmap_, 0, sizeof(int) * bitmap_size);
    block_ptrs_ = new int[bitmap_size];
    memset(block_ptrs_, -1, sizeof(int) * bitmap_size);

    // Allocate memory for buddy blocks
    memory_ = new char[total_memory_bytes_];
    memset(memory_, 0, total_memory_bytes_);
}

BuddyAllocator::~BuddyAllocator() 
{
    delete[] bitmap_;
    delete[] block_ptrs_;
    delete[] memory_;
}

void* BuddyAllocator::allocate(int size_bytes) 
{
    if (size_bytes <= 0 || size_bytes > total_memory_bytes_) 
    {
        throw std::invalid_argument("Invalid allocation size");
    }

    // Calculate order based on block size
    int order = 0;
    int size = block_size_;
    while (size < size_bytes) 
    {
        size <<= 1;
        ++order;
    }

    // Find first available block at the given order or higher
    int index = get_block_index(order, 0);
    if (index == -1) 
    {
        // No available block at the requested order, try higher orders
        for (int i = order + 1; i <= max_order_; ++i) 
        {
            index = get_block_index(i, 0);
            if (index != -1) 
            {
                // Found an available block at a higher order, split it down to the requested order
                while (i > order) 
                {
                    --i;
                    split_block(i, index * 2);
                    index *= 2;
                }
                break;
            }
        }
    }

    if (index == -1) 
    {
        throw std::bad_alloc();
    }

    // Mark block as allocated and return pointer
    bitmap_[index] = 1;
    return &memory_[index * block_size_];
}

void BuddyAllocator::deallocate(void* ptr) 
{
    if (ptr == nullptr) 
    {
        return;
    }

    //


  // Find block index from pointer
  int index = ((char*)ptr - memory_) / block_size_;

  // Mark block as free and merge with buddies
  bitmap_[index] = 0;
  while (index > 0) 
  {
    int buddy_index = index ^ 1;
    if (bitmap_[buddy_index] == 1) 
    {
      break;
    }
    merge_block(log2(index), index / 2);
    index /= 2;
  }
}

int BuddyAllocator::get_block_index(int order, int offset) 
{
    int size = pow(2, max_order_ - order);
    int start = (offset / size) * size;
    for (int i = start; i < total_memory_bytes_ / block_size_; i += size) 
    {
        if (bitmap_[i] == 0) 
        {
            return i;
        }
    }
    return -1;
}

void BuddyAllocator::split_block(int order, int index) 
{
    int size = pow(2, max_order_ - order);
    int buddy_index = index ^ 1;

    // Update block pointers
    block_ptrs_[index] = buddy_index;
    block_ptrs_[buddy_index] = index;

    // Set bitmap for new blocks
    bitmap_[index] = 0;
    bitmap_[buddy_index] = 0;

    // Split block into two buddies
    for (int i = 0; i < size; ++i) 
    {
        memory_[(buddy_index + i) * block_size_] = 0;
    }
}

void BuddyAllocator::merge_block(int order, int index) 
{
    int size = pow(2, max_order_ - order);
    int buddy_index = index ^ 1;

    // Update block pointers
    block_ptrs_[index] = -1;
    block_ptrs_[buddy_index] = -1;

    // Clear bitmap for merged blocks
    bitmap_[index] = 0;
    bitmap_[buddy_index] = 0;

    // Merge blocks
    for (int i = 0; i < size; ++i) 
    {
        memory_[(buddy_index + i) * block_size_] = 0;
    }
}
  
  int BuddyAllocator::allocate_block(int order) 
  {
  if (order > max_order_) 
  {
    return -1;
  }

  for (int i = order; i <= max_order_; ++i) 
  {
    int index = get_block_index(i, 0);
    if (index != -1) 
    {
      // Block found, split until desired order is reached
      for (int j = i; j > order; --j) 
      {
        split_block(j, index);
      }
      return index;
    }
  }
  return -1;
}

void BuddyAllocator::deallocate_block(int index) 
{
  int order = max_order_;
  while (block_ptrs_[index] == -1 && order > 0) 
  {
    index &= ~(1 << order);
    order--;
  }
  merge_block(order, index);
  while (order < max_order_) 
  {
    int buddy_index = index ^ (1 << order);
    if (block_ptrs_[buddy_index] == -1) 
    {
      break;
    }
    merge_block(order, buddy_index);
    index &= ~(1 << order);
    order++;
  }
}
int main()
{
    cout << "-------------------------------------" << endl;
    cout << "Using Auto Function" << endl;

    MemoryManager manager(100);
    // create a memory manager with 100 units of memory

    int pid1 = 1, pid2 = 2;
    int size1 = 20, size2 = 30;

    // allocate memory for process 1 and print the memory blocks

    int start_address1 = manager.allocateMemory(size1);
    if (start_address1 != -1)
    {
        cout << "Allocated " << size1 << " units of memory for process " << pid1 << " starting from address " << start_address1 << endl;
        manager.printMemoryBlocks();
    }
    else
    {
        cout << "Unable to allocate memory for process " << pid1 << endl;
    }
    cout << "-------------------------------------" << endl;

    cout << "-------------------------------------" << endl;
    cout << "Using Dynamic Memory" << endl;
    // allocate memory for process 2 and print the memory blocks

    MemoryAllocator alloc(1024);

    int *int_ptr = static_cast<int *>(alloc.allocate(sizeof(int)));
    if (int_ptr != nullptr)
    {
        *int_ptr = 42;
        std::cout << "Allocated an integer: " << *int_ptr << std::endl;
    }

    double *double_ptr = static_cast<double *>(alloc.allocate(sizeof(double)));
    if (double_ptr != nullptr)
    {
        *double_ptr = 3.14;
        std::cout << "Allocated a double: " << *double_ptr << std::endl;
    }
    cout << "-------------------------------------" << endl;
    alloc.deallocate(int_ptr);
    alloc.deallocate(double_ptr);

    // manager.deallocateMemory(start_address1, size1);
    // manager.deallocateMemory(start_address2, size2);

    cout << "-------------------------------------" << endl;
    cout<<"Using Buddy Allocator"<<endl;
    // Create a BuddyAllocator instance with 1MB of memory
    BuddyAllocator allocator(1024 * 1024);

    // Allocate memory blocks of varying sizes
    void* block1 = alloc.allocate(64);
    void* block2 = alloc.allocate(128);
    void* block3 = alloc.allocate(256);

    // Deallocate one of the memory blocks
    alloc.deallocate(block2);

    // Allocate another memory block
    void* block4 = alloc.allocate(512);

    std::cout << "Trying to allocate too much memory" << std::endl;
    try 
    {
        void* ptr4 = allocator.allocate(1024 * 1024* 2);
        std::cout << "Allocated pointer 4: " << ptr4 << std::endl;
    } 
    catch (std::exception& e) 
    {
        std::cout << "Failed to allocate memory: " << e.what() << std::endl;
    }

    // Deallocate all remaining memory blocks
    alloc.deallocate(block1);
    alloc.deallocate(block3);
    alloc.deallocate(block4);
    cout<<"---------------------------------------"<<endl;

//   int total_memory_bytes = 1024 * 1024; // 1MB
//     BuddyAllocator allocator(total_memory_bytes);

//     std::cout << "Hello" << std::endl;

//     // Allocate 256 bytes
//     std::cout << "Allocating 256 bytes" << std::endl;
//     void * ptr1 = allocator.allocate(256);
//     if (ptr1 == nullptr) {
//         std::cout << "Allocation failed" << std::endl;
//         return 1;
//     }
//     std::cout << "Allocated pointer 1: " << ptr1 << std::endl;

//     // Allocate 512 bytes
//     std::cout << "Allocating 512 bytes" << std::endl;
//     void* ptr2 = allocator.allocate(512);
//     if (ptr2 == nullptr) {

//         std::cout << "Allocation failed" << std::endl;
//         return 1;
//     }
//     std::cout << "Allocated pointer 2: " << ptr2 << std::endl;

//     // Free the first allocation
//     std::cout << "Freeing pointer 1" << std::endl;
//     allocator.deallocate(ptr1);

//     // Allocate 128 bytes
//     std::cout << "Allocating 128 bytes" << std::endl;
//     void* ptr3 = allocator.allocate(128);
//     if (ptr3 == nullptr) {
//         std::cout << "Allocation failed" << std::endl;
//         return 1;
//     }
//     std::cout << "Allocated pointer 3: " << ptr3 << std::endl;

//     // Try to allocate too much memory
//     std::cout << "Trying to allocate too much memory" << std::endl;
//     try 
//     {
//         void* ptr4 = allocator.allocate(1024 * 1024* 2);
//         std::cout << "Allocated pointer 4: " << ptr4 << std::endl;
//     } 
//     catch (std::exception& e) 
//     {
//         std::cout << "Failed to allocate memory: " << e.what() << std::endl;
//     }

    return 0;
}
