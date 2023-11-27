#include "ut.hpp"

#include "slotmap.hpp"
#include "slotmap_old.hpp"

namespace ut = boost::ut;

template < template <typename T, std::size_t TS, typename... TA> typename slotmap,
         ut::fixed_string Name>
ut::suite<Name> s_test_slotmap = [] 
{
  using namespace boost::ut;

  constexpr std::size_t S = 8;
  using slotmap_t = slotmap<char, S>;

  "default size and capacity"_test = [] {
    slotmap_t sm {};
    expect(sm.size() == 0 and sm.capacity() == S);
  };

  "basic insert/access temporal"_test = [] {
    slotmap_t sm {};
    std::array<typename slotmap_t::key_t, S> insert_keys {};

    insert_keys[0] = sm.push_back('a');
    insert_keys[1] = sm.push_back('b');

    expect(sm[ insert_keys[0] ] == 'a');
    expect(sm[ insert_keys[1] ] == 'b');
  };

  "basic insert/access references"_test = [] {
    slotmap_t sm {};
    std::array<typename slotmap_t::key_t, S> insert_keys {};

    char a = 'a';
    char b = 'b';
    insert_keys[0] = sm.push_back(a);
    insert_keys[1] = sm.push_back(b);

    expect(sm[ insert_keys[0] ] == 'a');
    expect(sm[ insert_keys[1] ] == 'b');
  };

  "last element to removed position"_test = [] {
    slotmap_t sm {};
    std::array<typename slotmap_t::key_t, S> insert_keys {};

    insert_keys[0] = sm.push_back('a');
    insert_keys[1] = sm.push_back('b'); // gets removed
    insert_keys[2] = sm.push_back('c');

    sm.remove(insert_keys[1]);

    expect(*(sm.begin() + 1) == 'c');
  };

  "Cannot access deleted key after another insert"_test = [] {
    slotmap_t sm {};
    std::array<typename slotmap_t::key_t, S> insert_keys {};

    "delete 1"_test = [&] {
      insert_keys[0] = ( sm.push_back('a') );
      insert_keys[1] = ( sm.push_back('b') );
      insert_keys[2] = ( sm.push_back('c') );

      sm.remove(insert_keys[0]); // a

      insert_keys[3] = ( sm.push_back('d') );

      expect(sm[insert_keys[1]] == 'b');
      expect(sm[insert_keys[2]] == 'c');
      expect(sm[insert_keys[3]] == 'd');
      expect(throws([&] {
        sm[insert_keys[0]];
      }));
    };

    "delete 2"_test = [&] {
      insert_keys[4] = ( sm.push_back('e') );
      insert_keys[5] = ( sm.push_back('f') );

      sm.remove(insert_keys[2]); // c

      expect(sm[insert_keys[4]] == 'e');
      expect(sm[insert_keys[5]] == 'f');
      expect(throws([&] {
        sm[insert_keys[2]];
      }));
    };
  };


  "remove same key twice"_test = [] {
    slotmap_t sm {};
    std::array<typename slotmap_t::key_t, S> insert_keys {};

    insert_keys[0] = sm.push_back('a');
    insert_keys[1] = sm.push_back('b'); 

    auto key = insert_keys[1];
    sm.remove(key);

    expect(throws([&] {
      sm.remove(key);
    })) << "exception on same key deletion";

    expect(throws([&] {
      sm[key];
    })) << "cannot access deleted key of first call";
  };

  "access with an invalid gen key"_test = [] {
    slotmap_t sm {};
    auto k1 = sm.push_back('a');
    auto k2 = sm.push_back('b');
    auto key = typename  slotmap_t::key_t{0, 999};
    expect(throws([&] {
          sm[key];
    }));
  };

  "begin() and end() with 2 inserts"_test = [] {
    slotmap_t sm {};
    auto k1 = sm.push_back('a');
    auto k2 = sm.push_back('b');
    expect(*sm.begin()      == 'a') << "begin has first value";
    expect(*(sm.end() - 1 ) == 'b') << "end - 1 has second value";
  };

  "insert when full should trow exception"_test = [] {
    slotmap<char, 2> sm {};
    auto k1 = sm.push_back('a');
    auto k2 = sm.push_back('b');

    expect(throws([&] {
      auto k3 = sm.push_back('c'); 
    }));
  };

  "for each iteration "_test = [] {
    slotmap_t sm {};
    std::array<typename slotmap_t::key_t, S> insert_keys {};

    insert_keys[0] = sm.push_back('a');
    insert_keys[1] = sm.push_back('b');
    insert_keys[2] = sm.push_back('c');
    insert_keys[3] = sm.push_back('d');

    std::size_t i {0};
    for (const char& e : sm) {
      expect(sm[insert_keys[i]] == e);
      ++i;
    }
    expect(i == sm.size());
  };

};

int main() {
  auto s2 = s_test_slotmap<slotmap_old, "slotmap_old" >;
  auto s1 = s_test_slotmap<slotmap    , "slotmap"     >;
}


