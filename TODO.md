# #TODO

---

## Memory
* Mother memory block where dynamic memory is allocated.
  * Mother of allocators.
  * Allocator of allocators.
  * It's a big linear allocator or free-list.
* Memory System
  * At start: allocates big chunk of memory.
    * this block of mem. is where dyn. mem. is allocated by default.


* Global Heap for general purposes
* Special Heap for gameObjects (spawn and destroy)
* Level-Loading Heap
* Stack-allocator for single-frame allocations (cleared every frame)
* Debug memory Heap, only for allocations that will not be needed in final game.

---

## Matrix4
* implement operations

---

## Quaternion
* implement squad
* implement toEuler

---

## List
* Test: put, fill, init(void*), [] operator

---

## Misc.
* use modern keywords: default, override, final method, final class.
* Delete memory inside containers.
* Change asserts by execptions.

---
