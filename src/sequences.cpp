#include <fmt/core.h>
#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <fmt/format.h>

#include <array>
#include <span>
#include <string>

static constexpr auto setOne   = std::to_array({'A'});
static constexpr auto setTwo   = std::to_array({'B', 'C'});
static constexpr auto setThree = std::to_array({'D', 'E', 'F'});

static constexpr std::pair<frozen::string, std::span<const char>> tableData[] = {
   {"one", setOne},
   {"two", setTwo},
   {"three", setThree},
};

static constexpr auto lookupTable = frozen::make_unordered_map(tableData);

int main(int /* argc */, char* argv[])
{
   const auto iter = lookupTable.find(frozen::string{argv[1]});
   if (iter != lookupTable.end())
   {
      fmt::print("{} has {} elements, first is {}\n", argv[1], iter->second.size(), iter->second[0]);
   }

   return 0;
}
