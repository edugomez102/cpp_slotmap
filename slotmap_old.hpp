#pragma once

#include <array>
#include <cstdint>
#include <cwchar>
#include <iostream>

template <typename TData, std::size_t SIZE>
struct slotmap_old {

  using value_type = TData;
  using size_t = std::size_t;

public:

  struct Key {
    size_t slot_ind {0};
    size_t gen {0};
  };

  using key_t = Key;

  // fill indices with empty slots
  slotmap_old() {

    for (size_t i = 0; i < SIZE; i++) indices_[i].id = i + 1;

  }

  Key push_back(const TData& element) {
    if (is_full()) throw "slotmap_old is full";

    Slot oldslot = indices_[freelist_];
    Slot newslot = { .id = last_, .gen = generation_ };
    indices_[freelist_] = newslot;

    data_[newslot.id] = element;
    erase_[freelist_] = newslot.id;

    generation_++;
    size_t inserted_slot_index = freelist_;
    freelist_ = oldslot.id;
    last_++;

    return { .slot_ind = inserted_slot_index, .gen = newslot.gen };

  }

  void remove(Key key) {
    Slot slot = indices_[key.slot_ind];
    if(slot.gen == key.gen and last_ != 0) {

      indices_[key.slot_ind].id = last_;
      indices_[key.slot_ind].gen = generation_;
      freelist_ = key.slot_ind;
      data_ [slot.id] = {};
      erase_[slot.id] = {};
      if(last_ - 1 != slot.id) {
        data_ [slot.id] = data_ [last_ - 1];
        erase_[slot.id] = erase_[last_ - 1];
        data_ [last_ - 1] = {};
        erase_[last_ - 1] = {};
        indices_[erase_[slot.id]].id = slot.id;
      }
      last_--;
      ++generation_;
    }
    else 
    {
      throw "Error, different key gens or empty";
    }

  }

  const TData& operator[](const Key rhs) {
    Slot slot = indices_[rhs.slot_ind];
    if( slot.gen == rhs.gen )
      return data_[slot.id];
    else
      throw "Error, different key gens";
  }

  const TData* data() const {
    return data_.data();
  }

  const auto begin() const {
    return data_.begin();
  }

  const auto end() const {
    return data_.begin() + last_ ;
  }

  constexpr size_t capacity() const { return SIZE; };
  size_t size() const { return last_; };

private:

  struct Slot {
    size_t id  {0};
    size_t gen {0};
  };

  uint64_t generation_ {0};
  size_t freelist_ {0};

  size_t last_ {0};

  std::array<Slot, SIZE>   indices_ {};
  std::array<TData, SIZE>  data_ {};
  std::array<size_t, SIZE> erase_ {};


  bool is_full() {
    return freelist_ >= SIZE;
  }

};

