# Slotmap implementation in C++20

A slot map is an associative container that stores data contiguously in memory and provides an indirect access layer to maintain stable references to the data. It facilitates efficient insertion, removal, and retrieval of elements, ensuring that the data is stored in an array-like structure with no gaps.

![](scheme.png)

## Used resources

- https://youtu.be/ZfWF9tqQOh0?list=PLmxqg54iaXrjamduSxTicxzLYC9tQeaFl
- https://youtu.be/-8UZhDjgeZU?si=iaBnd99Y9P9OXLXp
- unit test using the boost ut library: https://github.com/boost-ext/ut

