#include "21day.h"

#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <unordered_map>

#define HUMAN "humn"

struct monkey_math {
  std::string name;
  long *val;
  monkey_math *monkey1;
  monkey_math *monkey2;
  char sign;

  ~monkey_math() {
    delete this->val;
    delete this->monkey1;
    delete this->monkey2;
  }
};

monkey_math *chain_monkeys(const std::vector<std::string> &puzzle_input) {
  std::unordered_map<std::string, monkey_math *> monkey_ref;
  std::unordered_map<std::string, std::pair<std::string, std::string>>
      dependencies;

  for (std::vector<std::string>::const_iterator monkey = puzzle_input.begin();
       monkey != puzzle_input.end(); ++monkey) {
    std::stringstream ss(*monkey);
    std::string name;
    std::string name_or_val;
    ss >> name >> name_or_val;
    name.pop_back(); // remove colon
    // monkey that simply shouts
    if ('0' <= name_or_val[0] && name_or_val[0] <= '9') {
      monkey_ref[name] =
          new monkey_math{name, new long, nullptr, nullptr, '\0'};
      *(monkey_ref[name]->val) = std::stol(name_or_val);
      // monkey with a job, depends on other monkeys
    } else {
      char sign;
      std::string monkey2;
      ss >> sign >> monkey2;
      monkey_ref[name] = new monkey_math{name, nullptr, nullptr, nullptr, sign};
      dependencies[name] = std::make_pair(name_or_val, monkey2);
    }
  }

  for (std::unordered_map<std::string,
                          std::pair<std::string, std::string>>::const_iterator
           dep = dependencies.begin();
       dep != dependencies.end(); ++dep) {
    monkey_ref[dep->first]->monkey1 = monkey_ref[dep->second.first];
    monkey_ref[dep->first]->monkey2 = monkey_ref[dep->second.second];
  }

  return monkey_ref["root"];
}

long compute_number_from_monkey(const monkey_math *root,
                                std::unordered_map<std::string, long> &values) {
  std::stack<const monkey_math *> next_monkey;
  next_monkey.push(root);
  while (!next_monkey.empty()) {
    const monkey_math *curr = next_monkey.top();
    // leaf or path we've seen before
    if (next_monkey.top()->val != nullptr ||
        values.find(curr->name) != values.end()) {
      curr = next_monkey.top();
      next_monkey.pop();
      values[curr->name] = *curr->val;
      // non-leaf, have children been computed yet?
    } else if (values.find(curr->monkey1->name) != values.end() &&
               values.find(curr->monkey2->name) != values.end()) {
      curr = next_monkey.top();
      next_monkey.pop();
      switch (curr->sign) {
      case '-':
        values[curr->name] =
            values[curr->monkey1->name] - values[curr->monkey2->name];
        break;
      case '*':
        values[curr->name] =
            values[curr->monkey1->name] * values[curr->monkey2->name];
        break;
      case '/':
        values[curr->name] =
            values[curr->monkey1->name] / values[curr->monkey2->name];
        break;
      case '+':
      default:
        values[curr->name] =
            values[curr->monkey1->name] + values[curr->monkey2->name];
      }
      // non-leaf, unique path
    } else {
      if (values.find(curr->monkey2->name) == values.end()) {
        next_monkey.push(curr->monkey2);
      }
      while (curr->val == nullptr &&
             values.find(curr->monkey1->name) == values.end()) {
        next_monkey.push(curr->monkey1);
        curr = curr->monkey1;
      }
    }
  }
  return values[root->name];
}

long compute_number_from_monkey(const monkey_math *root) {
  std::unordered_map<std::string, long> temp;
  return compute_number_from_monkey(root, temp);
}

long compute_value_for_human(const monkey_math *root,
                             std::unordered_map<std::string, long> values) {
  std::unordered_map<std::string, long> human_values;
  human_values[root->monkey1->name] = values[root->monkey2->name];
  human_values[root->monkey2->name] = values[root->monkey1->name];
  std::queue<const monkey_math *> next_monkey;
  next_monkey.push(root->monkey1);
  next_monkey.push(root->monkey2);
  while (!next_monkey.empty() && next_monkey.front()->name != HUMAN) {
    const monkey_math *curr = next_monkey.front();
    next_monkey.pop();

    if (curr->monkey1 != nullptr) {
      switch (curr->sign) {
      case '-':
        human_values[curr->monkey1->name] =
            human_values[curr->name] + values[curr->monkey2->name];
        break;
      case '*':
        human_values[curr->monkey1->name] =
            human_values[curr->name] / values[curr->monkey2->name];
        break;
      case '/':
        human_values[curr->monkey1->name] =
            human_values[curr->name] * values[curr->monkey2->name];
        break;
      case '+':
      default:
        human_values[curr->monkey1->name] =
            human_values[curr->name] - values[curr->monkey2->name];
      }
      next_monkey.push(curr->monkey1);
    }

    if (curr->monkey2 != nullptr) {
      switch (curr->sign) {
      case '-':
        human_values[curr->monkey2->name] =
            -(human_values[curr->name] - values[curr->monkey1->name]);
        break;
      case '*':
        human_values[curr->monkey2->name] =
            human_values[curr->name] / values[curr->monkey1->name];
        break;
      case '/':
        human_values[curr->monkey2->name] =
            values[curr->monkey1->name] / human_values[curr->name];
        break;
      case '+':
      default:
        human_values[curr->monkey2->name] =
            human_values[curr->name] - values[curr->monkey1->name];
      }
      next_monkey.push(curr->monkey2);
    }
  }
  return human_values[HUMAN];
}

long monkey_math_pt1(const std::vector<std::string> &puzzle_input) {
  monkey_math *root = chain_monkeys(puzzle_input);
  return compute_number_from_monkey(root);
}

long monkey_math_pt2(const std::vector<std::string> &puzzle_input) {
  monkey_math *root = chain_monkeys(puzzle_input);
  std::unordered_map<std::string, long> values;
  compute_number_from_monkey(root, values);
  return compute_value_for_human(root, values);
}