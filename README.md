# Custom-Containers
This repository contains a few container implementations. It is created mostly as a learning experience. Appreciate suggestions to improve them.

**Current containers:**
* TArray (vector implementation)
* TSharedPtr (With TWeakPtr)
* TUniquePtr
* TFunction (function implementation)

**Limitations/Improvements to come:**
* **TFunction** uses a fixed size, should be able to allocate all sized and be able to use an allocator
* **TArray** does not support allocators
* **TSharedPtr** and **TUniquePtr** should support custom deleters
