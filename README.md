# Tiny Custom Template Library
This repository contains a few template library implementations. It is created mostly as a learning experience. Appreciate suggestions to improve them.

**Current implementations:**
* **TArray** - (Similar to std::vector)
* **TStaticArray** - (Similar to std::array)
* **TArrayView** - (Similar to std::span)
* **TSharedPtr** and **TWeakPtr** - (Similar to std::shared_ptr and std::weak_ptr)
* **TUniquePtr** - (Similar to std::unique_ptr)
* **TFunction** - (Similar to std::function)

**Limitations/Improvements to come:**
* **TArray** does not support allocators
* **TSharedPtr** and **TUniquePtr** does not support custom deleters

**Planned:**
* **String**
* **THashTable**
* **TList**
