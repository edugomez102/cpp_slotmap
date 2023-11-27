#pragma once

#include <cstdint>
#include <array>
#include <stdexcept>
#include <cassert>

template <typename TData, std::size_t Capacity>
struct slotmap {

public:
  using data_t   = TData;
  using index_t  = std::size_t;
  using gen_t    = index_t;
  using key_t    = struct { index_t id; gen_t gen; };

  template <typename T>
  using storage_t = std::array<T, Capacity>;

  using iterator = data_t*;

private:
  using size_t = std::size_t;

public:

  explicit slotmap() {
    clear();
  }

  [[nodiscard]] constexpr size_t size()     const noexcept { return last_; };
  [[nodiscard]] constexpr size_t capacity() const noexcept { return Capacity; };

  void clear() noexcept {
    for(size_t i{}; i < Capacity; ++i)
      slots_[i] = {i + 1};
  }

  // returns key_t with index of reserved slot
  [[nodiscard]] key_t push_back(data_t&& newdata) {
    index_t slots_ind = reserve_slot();
    key_t& slot = slots_[slots_ind];

    data_[slot.id] = std::move(newdata);
    erase_[slot.id] = slots_ind;

    return {slots_ind, slot.gen };
  }

  [[nodiscard]] key_t push_back(const data_t& newdata) {
    return push_back( data_t{newdata} );
  }

  bool remove(key_t key) {
    if(is_valid(key)) {
      delete_slot(key);
    }
    else throw std::runtime_error("invalid key");
    return false;
  }

  const data_t& operator[](key_t rhs) const {
    if(is_valid(rhs)) return data_[slots_[rhs.id].id];
    else              throw std::runtime_error("invalid key");
  };

  // Might have garbage data
  [[nodiscard]] const data_t* const data() {
    return data_.data();
  }

  const iterator begin() { return data_.begin(); }
  const iterator end()   { return data_.begin() + last_; }

private:

  bool is_valid(const key_t key) const noexcept {
    return !(key.id >= Capacity || slots_[key.id].gen != key.gen);
  };

  bool is_full() const noexcept { return last_ >= Capacity; }

  [[nodiscard]] index_t reserve_slot() {
    if (is_full()) throw std::runtime_error("Container is full");
    assert(nextfree_ < Capacity);

    index_t slots_ind = nextfree_;
    nextfree_ = slots_[slots_ind].id;

    key_t& slot = slots_[slots_ind];
    slot.id = last_;
    slot.gen = generation_;

    ++last_;
    ++generation_;

    return slots_ind;
  }

  void delete_slot(key_t key) noexcept {
    assert(is_valid(key));
    key_t& slot = slots_[key.id];
    index_t data_ind = slot.id;

    slot.id = nextfree_;
    slot.gen = generation_;
    nextfree_ = key.id;

    if(data_ind != last_ - 1) {
      data_ [data_ind] = data_ [last_ - 1];
      erase_[data_ind] = erase_[last_ - 1];
      slots_[erase_[data_ind]].id = data_ind;
    }
    --last_;
    ++generation_;
  }

private:

  index_t last_{0};

  index_t nextfree_{0};
  gen_t generation_{0};

  storage_t<key_t>   slots_{};
  storage_t<data_t>  data_{};
  storage_t<index_t> erase_{};

};


