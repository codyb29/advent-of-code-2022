#include "20day.h"

#define FIRST 1000
#define SECOND 2000
#define THIRD 3000

struct node {
  long long val;
  node *next;
  node *prev;

  node(long long val) : val(val), next(nullptr), prev(nullptr) {}
  node(long long val, node *prev) : val(val), next(nullptr), prev(prev) {}
};

node *build_sequence(const std::vector<std::string> &input,
                     long long encryption_key) {
  std::vector<std::string>::const_iterator it = input.begin();
  node *head = new node(std::stoll(*it) * encryption_key);
  node *prev = head;
  node *curr = head->next;
  for (it = it + 1; it != input.end(); ++it) {
    curr = new node(std::stoll(*it) * encryption_key, prev);
    prev->next = curr;
    prev = curr;
    curr = curr->next;
  }

  // create the circle
  prev->next = head;
  head->prev = prev;

  return head;
}

std::vector<node *> sequence_to_vector(node *sequence_head) {
  std::vector<node *> vector_form;
  node *curr = sequence_head;
  vector_form.emplace_back(curr);
  curr = curr->next;
  while (curr != sequence_head) {
    vector_form.emplace_back(curr);
    curr = curr->next;
  }
  return vector_form;
}

std::vector<node *> mix_sequence(node *encrypted_file, unsigned times) {
  std::vector<node *> original_order = sequence_to_vector(encrypted_file);
  std::vector<node *>::size_type cap = original_order.size() - 1;
  for (unsigned i = 0; i < times; i++) {
    for (node *curr : original_order) {
      // pull out
      node *prev = curr->prev;
      prev->next = curr->next;
      curr->next->prev = prev;

      // find next location
      std::vector<node *>::size_type optimized_max = curr->val % cap;
      if (curr->val > 0 && optimized_max > 0) {
        prev = curr;
        for (std::vector<node *>::size_type j = 0; j < optimized_max; j++) {
          prev = prev->next;
        }
      } else if (curr->val < 0) {
        std::vector<node *>::size_type optimized_max = (-curr->val) % cap;
        for (std::vector<node *>::size_type j = 0; j < optimized_max; j++) {
          prev = prev->prev;
        }
      }

      // insert
      node *next = prev->next;
      prev->next = curr;
      curr->prev = prev;
      next->prev = curr;
      curr->next = next;
    }
  }

  std::vector<node *>::const_iterator zero = original_order.begin();
  while (zero != original_order.end() && (*zero)->val != 0) {
    ++zero;
  }

  return sequence_to_vector(*zero);
}

long long grove_positioning_system_pt1(const std::vector<std::string> &input) {
  node *file_sequence = build_sequence(input, 1);
  std::vector<node *> mixed_order = mix_sequence(file_sequence, 1);
  return mixed_order[FIRST % mixed_order.size()]->val +
         mixed_order[SECOND % mixed_order.size()]->val +
         mixed_order[THIRD % mixed_order.size()]->val;
}

long long grove_positioning_system_pt2(const std::vector<std::string> &input) {
  node *file_sequence = build_sequence(input, 811589153);
  std::vector<node *> mixed_order = mix_sequence(file_sequence, 10);
  return mixed_order[FIRST % mixed_order.size()]->val +
         mixed_order[SECOND % mixed_order.size()]->val +
         mixed_order[THIRD % mixed_order.size()]->val;
}