#include "11day.h"

long Monkey::PlayWithItem(long item) {
    if (this->has_multiplier) {
        return this->is_careless ? item * this->multiplier
                                 : item + this->multiplier;
    }
    return this->is_careless ? item * item : item + item;
}

Monkey::Monkey(std::vector<long> worry_levels, long division_by,
               bool careless, long multiply) {
    items = new std::queue<long>{
        std::deque<long>(worry_levels.begin(), worry_levels.end())};
    divisible = division_by;
    is_careless = careless;
    multiplier = multiply;
    has_multiplier = multiply ? true : false;
    neighbors = new std::vector<Monkey>();
}

void Monkey::SetGCD(long GCD) { this->gcd = GCD; }

void Monkey::AddNeighbor(const Monkey& monkey) { neighbors->push_back(monkey); }

bool Monkey::HasItems() { return items->empty(); }

void Monkey::ThrowItem(long item) { items->push(item); }

void Monkey::InspectItem() {
    // deque item
    long item = this->items->front();
    this->items->pop();
    if (this->PlayWithItem(item) < 0) {
        std::cout << item << std::endl;
    }
    // apply monkey specific operation
    item = this->PlayWithItem(item);

    // part 1
    // item = std::floor(item / 3.0);

    // part 2
    item = item % this->gcd;

    // test divisibility
    if (item % this->divisible != 0) {
        // throw item to monkey based on divisibility
        (*this->neighbors)[1].ThrowItem(item);
    } else {
        (*this->neighbors)[0].ThrowItem(item);
    }
}

long long MonkeyInTheMiddle::GetMonkeyBusinessLevelForRounds(
    std::vector<Monkey> monkeys, int rounds) {
    std::vector<long long> count(monkeys.size(), 0);
    for (int i = 0; i < rounds; i++) {
        for (std::vector<Monkey>::size_type j = 0; j < monkeys.size(); j++) {
            while (!monkeys[j].HasItems()) {
                monkeys[j].InspectItem();
                count[j]++;
            }
        }
    }
    std::sort(count.rbegin(), count.rend());
    return count[0] * count[1];
}

std::vector<Monkey> MonkeyInTheMiddle::MakeMonkeys(std::vector<std::string> puzzle_input) {
    std::vector<Monkey> parsed_input;
    std::vector<long> items;
    std::vector<std::vector<int>> neighbors;
    long gcd = 1;
    for (std::vector<std::string>::const_iterator it = puzzle_input.begin();
         it != puzzle_input.end(); ++it) {
        std::string items_string = (++it)->substr(18);
        std::stringstream ss(items_string);
        std::string item;
        while (std::getline(ss, item, ',')) {
            items.push_back(std::stod(item));
        }

        ++it;
        std::string::size_type old = it->find("old ");
        std::string test = it->substr(old + 5);
        long worry_multiplier = 0;
        try {
            worry_multiplier = std::stoi(it->substr(old + 5));
        } catch (std::exception&) {
        }
        char symbol = it->substr(old + 4)[0];

        ++it;
        std::string::size_type divisible = it->find("divisible by ") + 13;
        long divisible_by = std::stod(it->substr(divisible));
        gcd *= divisible_by;

        ++it;
        int n1 = std::stoi(it->substr(it->find("monkey") + 7));

        ++it;
        int n2 = std::stoi(it->substr(it->find("monkey") + 7));

        std::vector<int> monkey_ids = {n1, n2};
        neighbors.push_back(monkey_ids);

        Monkey new_monkey(items, divisible_by, symbol == '*', worry_multiplier);
        parsed_input.push_back(new_monkey);
        items.clear();

        if (it + 1 != puzzle_input.end()) {
            ++it;
        }
    }

    for (std::vector<std::vector<int>>::size_type i = 0; i < neighbors.size();
         i++) {
        parsed_input[i].AddNeighbor(parsed_input[neighbors[i][0]]);
        parsed_input[i].AddNeighbor(parsed_input[neighbors[i][1]]);
        parsed_input[i].SetGCD(gcd);
    }

    return parsed_input;
}

