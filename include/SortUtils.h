#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include <vector>
#include <functional>
#include "Monster.h"

namespace SortUtils {

/**
 * Custom Quick Sort for Monster pointers.
 * Sorts the vector using the provided comparator.
 */
void quickSort(std::vector<Monster *> &items,
               std::function<bool(Monster *, Monster *)> comp);

} // namespace SortUtils

#endif
