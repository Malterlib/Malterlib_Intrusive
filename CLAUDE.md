# CLAUDE.md - Intrusive Module

## Module Overview

The Intrusive module provides foundational intrusive data structures that form the basis for all container classes in Malterlib. Intrusive data structures embed link information directly within the stored objects, enabling zero-allocation container operations and optimal cache performance. This module is critical infrastructure used throughout the framework.

## Key Components

### Core Data Structures
- **AVL Tree** (`TCAVLTreeAggregate`) - Self-balancing binary search tree with O(log n) operations
  - Multiple link types: Aligned, AlignedRight, Unaligned, Custom
  - Template-based with customizable comparison and allocation
  - Stack-based and recursive implementations

- **Doubly Linked List** (`CDLinkAggregateListNoPrevPtr`) - Auto-unlinking doubly linked list
  - List head uses single pointer with bit flags for memory efficiency
  - Nodes still use two pointers (next/prev) for full bidirectional traversal
  - Automatic unlinking on destruction

- **Singly Linked List** - Basic forward-only linked list
  - Minimal memory overhead
  - Simple iteration patterns

## Module-Specific Conventions

### Namespace Organization
- Primary namespace: `NMib::NIntrusive`
- Private implementations: `NMib::NIntrusive::NPrivate`

### Naming Patterns
- Template classes: `TC[Structure]Aggregate` (e.g., `TCAVLTreeAggregate`)
- Link types: `C[Type]Link` (e.g., `CAVLLink`, `CDLinkAggregateListNoPrevPtr`)
- Enums: `E[Type]` (e.g., `EAVLLinkType`)
- Member functions:
  - `fp_*` - Public functions
  - `fpr_*` - Protected/private functions
  - `fsp_*` - Static functions
  - `fs_*` - Static functions (alternative)
  - `fg_*` - Functions that may move/relocate (e.g., `fg_Move`)
- Constants: `mc_*` (e.g., `mc_SafeTreeDepth`)

### Template Parameters Convention
- `t_pLinkMember` - Pointer to member containing intrusive links
- `t_CCompare` - Comparison functor type
- `t_CAllocator` - Memory allocator type
- `t_COverrideNodeType` - Node type override

### Memory Layout
- Links are embedded directly in user objects
- No separate allocations for container nodes
- Bit-packing optimizations for flags and pointers in list heads
- Alignment-aware link structures

## Dependencies

### External
- **Boost** - Used ONLY in tests for performance and correctness comparison
  - Not a runtime dependency
  - Test modules: intrusive, static_assert, assert, move, preprocessor

### Internal Malterlib Modules
- **Core** - Basic types, platform abstractions, and macros
- **Memory** - Allocator interfaces (for non-intrusive operations)
- **Traits** - Type traits and metaprogramming utilities

## Architecture Details

### AVL Tree Implementation
```cpp
// Safe tree depth calculation based on AVL tree properties:
// AVL trees guarantee height ≤ 1.44 * log₂(n)
// For 64-bit address space: max ~2^64 objects
// Safe depth ensures no stack overflow for maximum possible tree
constexpr static umint mc_SafeTreeDepth = ((sizeof(void *) * 12) - gc_HighestBitSet<sizeof(CLink)>;
```

### Doubly Linked List Architecture
```cpp
// List HEAD uses single pointer with bit flag to save memory:
// - Bit 0: Indicates if this is a list link vs regular pointer
// - Remaining bits: Next pointer

// NODES still use full two pointers for bidirectional traversal
struct CNode {
    CNode* m_pNext;
    CNode* m_pPrev;
};
```

### Comparison Semantics
```cpp
// Comparisons use three-way comparison (spaceship) operator semantics:
struct CMyCompare {
    auto operator()(const CMyObject& a, const CMyObject& b) const {
        return a.m_Data <=> b.m_Data;  // Returns std::strong_ordering
    }
};
// Not traditional less-than comparison
```

### Link Types (AVL Tree)
- **EAVLLinkType_Aligned** - Links aligned to natural boundaries
- **EAVLLinkType_AlignedRight** - Right-child optimized alignment
- **EAVLLinkType_Unaligned** - No alignment requirements
- **EAVLLinkType_Custom** - User-defined link structure

## Common Tasks

### Adding Objects to Intrusive Container
```cpp
// Objects must contain link member:
struct CMyObject {
    NIntrusive::CAVLLink m_AVLLink;
    int m_Data;
};

// Container declaration:
using CTree = TCAVLTreeAggregate<&CMyObject::m_AVLLink>;
```

### Custom Comparison with Spaceship Operator
```cpp
struct CMyCompare {
    auto operator()(const CMyObject& a, const CMyObject& b) const {
        return a.m_Data <=> b.m_Data;  // Three-way comparison
    }
};
using CTree = TCAVLTreeAggregate<&CMyObject::m_AVLLink, CMyCompare>;
```

### Moving Doubly Linked Nodes
```cpp
// Nodes can be relocated in memory:
CNode node1;
list.fp_Add(&node1);

// Move to new location - automatically relinks:
CNode node2;
node2.fg_Move(&node1);  // node2 takes node1's place in list
```

### Running Module Tests
```bash
# Build tests
MalterlibBuildShowProgress=false ./mib build Tests

# Run all intrusive tests
/opt/Deploy/Tests/RunAllTests --paths '["Malterlib/Intrusive/*"]'

# Run specific test
/opt/Deploy/Tests/RunAllTests --paths '["Malterlib/Intrusive/AVLTree"]'
```

## Important Files

### Headers (Public API)
- `Include/Mib/Intrusive/AVLTree` - AVL tree public interface
- `Include/Mib/Intrusive/DoublyLinkedList` - Doubly linked list interface
- `Include/Mib/Intrusive/SinglyLinkedList` - Singly linked list interface

### Core Implementation
- `Source/Malterlib_Intrusive_AVLTree.h` - Main AVL tree implementation
- `Source/Malterlib_Intrusive_DoublyLinkedList.h` - Doubly linked list
- `Source/Malterlib_Intrusive_SinglyLinkedList.h` - Singly linked list

### AVL Tree Implementation Details (Private/)
- `Malterlib_Intrusive_AVLTree_Implementation_StaticStack.h` - Default traversal
- `Malterlib_Intrusive_AVLTree_Implementation_LowStack.h` - Memory-efficient traversal
- `Malterlib_Intrusive_AVLTree_Implementation_Recursive.h` - Recursive algorithms
- `Malterlib_Intrusive_AVLTree_Implementation_Iterator.h` - Iterator support
- `Malterlib_Intrusive_AVLTree_Implementation_Search.h` - Search algorithms
- `Malterlib_Intrusive_AVLTree_Implementation_Debug.h` - Debug validation

### Support Files
- `Malterlib_Intrusive_AVLTree_CompareDefaults.h` - Default comparison functors
- `Malterlib_Intrusive_AVLTree_Stream.h` - Stream serialization support
- `Malterlib_Intrusive_AVLTree_Links.h` - Link structure definitions

## Module-Specific Notes

### Performance Characteristics
- **Zero allocation overhead** - No dynamic memory for container operations
- **Cache-friendly** - Data and links co-located in memory
- **Predictable performance** - No allocation failures or fragmentation
- **AVL Tree**: O(log n) insert/delete/search, perfectly balanced
- **Linked Lists**: O(1) insert/delete at known positions

### Design Philosophy
- Intrusive containers own the links, not the objects
- Objects can be in multiple intrusive containers simultaneously
- No object copying during container operations (move semantics supported)
- Direct pointer access to contained objects

### Memory Safety
- **Automatic cleanup**: Doubly linked list nodes automatically unlink on destruction
- **Iterator invalidation**: Iterators become invalid if they point to removed objects
- **Move support**: Nodes can be relocated with `fg_Move()` - automatically relinks
- Use `fp_IsValid()` methods for debug validation
- Links must be properly initialized before use

### Thread Safety
- Intrusive containers are NOT thread-safe by default
- External synchronization required for concurrent access
- Safe to have multiple readers with no writers
- Consider using lock-free variants for concurrent scenarios

### Debugging Support
- Validation methods check tree balance and link integrity
- Test files include comprehensive correctness checks
- Old test files (Test_*Old*.cpp) retained for regression testing
- Boost comparison tests validate correctness against known implementation

### Known Limitations
- Maximum tree depth limited by `mc_SafeTreeDepth` (safe for all possible trees in address space)
- List head optimization requires aligned pointers
- Iterator invalidation on node removal (by design)
- Custom allocators only used for temporary traversal structures

### Integration with Container Module
- Container module builds non-intrusive wrappers on top
- Provides STL-like interfaces using intrusive primitives
- Enables hybrid intrusive/non-intrusive designs
- See Container module for higher-level abstractions
