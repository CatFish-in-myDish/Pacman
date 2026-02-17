#include "../include/SortUtils.h"
#include <algorithm> // for std::swap

namespace SortUtils {

// consistent Helper function for partitioning
static int partition(std::vector<Monster *> &items, int low, int high,
                     std::function<bool(Monster *, Monster *)> comp) {
  Monster *pivot = items[high];
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++) {
    // If current element is "smaller" than the pivot
    if (comp(items[j], pivot)) {
      i++;
      std::swap(items[i], items[j]);
    }
  }
  std::swap(items[i + 1], items[high]);
  return (i + 1);
}

static void quickSortRecursive(std::vector<Monster *> &items, int low, int high,
                               std::function<bool(Monster *, Monster *)> comp) {
  if (low < high) {
    int pi = partition(items, low, high, comp);

    quickSortRecursive(items, low, pi - 1, comp);
    quickSortRecursive(items, pi + 1, high, comp);
  }
}

void quickSort(std::vector<Monster *> &items,
               std::function<bool(Monster *, Monster *)> comp) {
  if (items.empty())
    return;
  quickSortRecursive(items, 0, items.size() - 1, comp);
}

} // namespace SortUtils
