#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "hitableList.h"
#include "object/hitable.h"
#include "tool/common.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vcruntime.h>
#include <vector>
class bvh_node : public hitable {
public:
  bvh_node() {}
  bvh_node(hitable_list &list) : bvh_node(list.obj_list) {}
  bvh_node(std::vector<shared_ptr<object>> &aabb_list)
      : bvh_node(aabb_list, 0, aabb_list.size()) {}
  bvh_node(std::vector<shared_ptr<object>> &aabb_list, size_t start,
           size_t end);
  bool bounding_box(AABB &box_out) const override;
  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;

  shared_ptr<hitable> left;
  shared_ptr<hitable> right;
  AABB box;
};

namespace BVH_CMP {
inline bool comparator(const shared_ptr<hitable> &a,
                       const shared_ptr<hitable> &b, int axis) {
  AABB box_a;
  AABB box_b;
  if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
    std::cerr << "Error : No bounding box in bvh node.\n";
  return box_a.min()[axis] < box_b.min()[axis];
}

inline bool x_compare(const shared_ptr<hitable> &a,
                      const shared_ptr<hitable> &b) {
  return comparator(a, b, 0);
}

inline bool y_compare(const shared_ptr<hitable> &a,
                      const shared_ptr<hitable> &b) {
  return comparator(a, b, 1);
}

inline bool z_compare(const shared_ptr<hitable> &a,
                      const shared_ptr<hitable> &b) {
  return comparator(a, b, 2);
}

}; // namespace BVH_CMP

inline bool bvh_node::hit(const ray &r, double t_min, double t_max,
                          record &rec) const {
  if (!box.hit(r, t_min, t_max, rec))
    return false;
  bool hit_left = left->hit(r, t_min, t_max, rec);
  // if hit left then changing t_max to t to get the closest t.
  bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);
  return hit_left || hit_right;
}
inline bool bvh_node::bounding_box(AABB &box_out) const {
  box_out = box;
  return true;
};

inline bvh_node::bvh_node(std::vector<shared_ptr<object>> &aabb_list,
                          size_t start, size_t end) {
  auto objs = aabb_list;
  int axis = rand_i(0, 2);
  using namespace BVH_CMP;
  auto comparator = axis == 0 ? x_compare : axis == 1 ? y_compare : z_compare;
  size_t mInterval = end - start;

//set up AABB recursively
  if (mInterval == 1) {
    left = objs[start];
    right = objs[start];
  } else if (mInterval == 2) {
    if (comparator(objs[start], objs[start + 1])) {
      left = objs[start];
      right = objs[start + 1];
    } else {
      left = objs[start + 1];
      right = objs[start];
    }
  } else {
    std::sort(objs.begin() + start, objs.begin() + end, comparator);
    int mid = start + mInterval / 2;
    left = std::make_shared<bvh_node>(bvh_node(objs, start, mid));
    right = std::make_shared<bvh_node>(bvh_node(objs, mid, end));
  }
  AABB box_left, box_right;
  if (!left->bounding_box(box_left) || !right->bounding_box(box_right)) {
    std::cerr << "Error : No bounding box in bvh node.\n";
  }
  box = AABB::surrounding_box(box_left, box_right);
}
#endif