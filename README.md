# Slotmap implementation in C++20

A slot map is an associative container that stores data contiguously in memory and provides an indirect access layer to maintain stable references to the data. It facilitates efficient insertion, removal, and retrieval of elements, ensuring that the data is stored in an array-like structure with no gaps.

![](scheme.png)

note: in the actual implementation the generation number is also incremented when removing an element.

note: `slotmap_old.hpp` is just an old implementation, check `slotmap.hpp` for the most recent one.

## Used resources

- https://youtu.be/ZfWF9tqQOh0?list=PLmxqg54iaXrjamduSxTicxzLYC9tQeaFl
- https://www.youtube.com/watch?v=SHaAR7XPtNU
- unit test using the boost ut library: https://github.com/boost-ext/ut

