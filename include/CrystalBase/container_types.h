#ifndef CRYSTAL_BASE_CONTAINER_H_
#define CRYSTAL_BASE_CONTAINER_H_

#include <array>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

namespace crystal {

/* Arrays */
template <typename T, std::size_t N>
using Array = std::array<T, N>;

/* Dynamic Arrays */
template <typename T>
using Vector = std::vector<T>;

/* Lists */
template <typename T>
using List = std::list<T>;

/* Deques */
template <typename T>
using Deque = std::deque<T>;

/* Maps */
template <typename Key, typename Value>
using Map = std::map<Key, Value>;

template <typename Key, typename Value>
using UnorderedMap = std::unordered_map<Key, Value>;

/* Sets */
template <typename Key>
using Set = std::set<Key>;

template <typename Key>
using UnorderedSet = std::unordered_set<Key>;

/* Queues */
template <typename T>
using Queue = std::queue<T>;

template <typename T>
using PriorityQueue = std::priority_queue<T>;

/* Stacks */
template <typename T>
using Stack = std::stack<T>;

} // namespace crystal

#endif // CRYSTAL_BASE_CONTAINER_H_
