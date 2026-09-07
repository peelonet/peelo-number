#include <iostream>

#include "peelo/number.hpp"

int main()
{
  using peelo::number;

  const auto left = number::parse("1.25");
  const auto right = number::parse("2.75");
  const auto sum = left.add(right);

  std::cout << "1.25 + 2.75 = " << sum.to_string() << std::endl;

  const auto distance = number::parse("1km");
  const auto offset = number::parse("500m");
  const auto total = distance.add(offset);

  std::cout << "1km + 500m = " << total.to_string() << std::endl;
  return 0;
}
