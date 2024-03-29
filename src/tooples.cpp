#include <fmt/format.h>

#include <array>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <tuple>
#include <utility>

constexpr uint64_t fnv1(std::string_view input)
{
   constexpr uint64_t FNV_offset_basis = 0xcbf29ce484222325;
   constexpr uint64_t FNV_prime        = 0x100000001b3;

   uint64_t hashValue = FNV_offset_basis;

   const char* ptr = input.data();
   const char* end = ptr + input.size();

   while (ptr != end)
   {
      hashValue *= FNV_prime;
      hashValue ^= *ptr;

      ++ptr;
   }

   return hashValue;
}

struct Width
{
   using Type = uint32_t;

   static constexpr std::string_view Name = "width";

   static constexpr uint64_t Hash = fnv1(Name);

   static void set(std::string_view newValue, uint32_t& value)
   {
      const auto [p, ec] = std::from_chars(newValue.begin(), newValue.end(), value);
      if (ec == std::errc())
      {
         throw std::invalid_argument(fmt::format("Can't parse '{}' as integer", newValue));
      }
   }

   static std::string get(float value)
   {
      return fmt::format("{}", value);
   }
};

struct Height
{
   using Type = uint32_t;

   static constexpr std::string_view Name = "height";

   static constexpr uint64_t Hash = fnv1(Name);

   static void set(std::string_view newValue, uint32_t& value)
   {
      const auto [p, ec] = std::from_chars(newValue.begin(), newValue.end(), value);
      if (ec == std::errc())
      {
         throw std::invalid_argument(fmt::format("Can't parse '{}' as integer", newValue));
      }
   }

   static std::string get(float value)
   {
      return fmt::format("{}", value);
   }
};

struct Ratio
{
   using Type = float;

   static constexpr std::string_view Name = "ratio";

   static constexpr uint64_t Hash = fnv1(Name);

   static void set(std::string_view newValue, float& value)
   {
      std::string buf{newValue};
      value = ::atof(buf.data());
   }

   static std::string get(float value)
   {
      return fmt::format("{}", value);
   }
};

template <typename Kind>
struct Field
{
   using Type = typename Kind::Type;

   static constexpr auto Name = Kind::Name;

   Field() = default;

   explicit constexpr Field(Type aValue) : value{aValue}
   {
   }

   constexpr auto getName() const noexcept
   {
      return Kind::Name;
   }

   constexpr auto getHash() const noexcept
   {
      return Kind::Hash;
   }

   bool setIf(std::string_view aName, std::string_view aValue)
   {
      if (aName == Kind::Name)
      {
         Kind::set(aValue, value);
         return true;
      }

      return false;
   }

   std::string getIf(std::string_view aName)
   {
      if (aName == Kind::Name)
      {
         return Kind::get(value);
      }

      return "";
   }

   std::string getAsString() const
   {
      return Kind::get(value);
   }

   Type value;
};

/*
 * https://devblogs.microsoft.com/oldnewthing/20200629-00/?p=103910
 */
template <typename T, typename Tuple>
struct tuple_element_index_helper;

template <typename T>
struct tuple_element_index_helper<T, std::tuple<>>
{
   static constexpr std::size_t value = 0;
};

template <typename T, typename... Rest>
struct tuple_element_index_helper<T, std::tuple<T, Rest...>>
{
   static constexpr std::size_t value = 0;
   using RestTuple                    = std::tuple<Rest...>;
   static_assert(tuple_element_index_helper<T, RestTuple>::value == std::tuple_size_v<RestTuple>,
                 "type appears more than once in tuple");
};

template <typename T, typename First, typename... Rest>
struct tuple_element_index_helper<T, std::tuple<First, Rest...>>
{
   using RestTuple                    = std::tuple<Rest...>;
   static constexpr std::size_t value = 1 + tuple_element_index_helper<T, RestTuple>::value;
};

template <typename T, typename Tuple>
struct tuple_element_index
{
   static constexpr std::size_t value = tuple_element_index_helper<T, Tuple>::value;
   static_assert(value < std::tuple_size_v<Tuple>, "type does not appear in tuple");
};

template <typename T, typename Tuple>
inline constexpr std::size_t tuple_element_index_v = tuple_element_index<T, Tuple>::value;

struct RectangleSize
{
   using TupleType = std::tuple<Field<Width>, Field<Height>>;

   template <typename T>
   auto get()
   {
      constexpr auto pos = tuple_element_index_v<Field<T>, TupleType>;
      return std::get<pos>(value).value;
   }

   template <typename T>
   auto set(typename T::Type newValue)
   {
      constexpr auto pos         = tuple_element_index_v<Field<T>, TupleType>;
      std::get<pos>(value).value = newValue;
   }

   TupleType value;
};

template <typename T, std::size_t N>
constexpr bool isSorted(const std::array<T, N>& arr)
{
   if (N == 0)
   {
      return true;
   }

   auto last = arr[0];
   for (auto elem : arr)
   {
      if (last > elem)
      {
         return false;
      }
      last = elem;
   }

   return true;
}

template <typename... Args>
struct ParamGroup
{
   using Tuple = std::tuple<Field<Args>...>;

   static constexpr std::array<std::string_view, std::tuple_size<Tuple>::value> Names = {{Args::Name...}};

   static_assert(isSorted(Names), "Type names need to be sorted");

   template <typename T>
   auto get()
   {
      constexpr auto pos = tuple_element_index_v<Field<T>, Tuple>;
      return std::get<pos>(value).value;
   }

   template <typename T>
   auto set(typename T::Type newValue)
   {
      constexpr auto pos         = tuple_element_index_v<Field<T>, Tuple>;
      std::get<pos>(value).value = newValue;
   }

   void set(std::string_view name, std::string_view newValue)
   {
      std::apply([name, newValue](auto&&... args) { (args.setIf(name, newValue) || ...); }, value);
   }

   std::string get(std::string_view name)
   {
      std::string result;
      std::apply([name, &result](auto&&... args) { result = (args.getIf(name) + ...); }, value);
      return result;
   }

private:
   template <size_t N>
   static std::string getString(const Tuple& tup) noexcept
   {
      return std::get<N>(tup).getAsString();
   }

   /*
    * TODO: simplify this
    */
   using GetAsStringType = std::string (*)(const Tuple& tup);

   template <typename TheTuple, typename Indices = std::make_index_sequence<std::tuple_size<TheTuple>::value>>
   struct runtime_get_func_table;

   template <typename TheTuple, size_t... Indices>
   struct runtime_get_func_table<TheTuple, std::index_sequence<Indices...>>
   {
      static constexpr std::array<GetAsStringType, std::tuple_size<TheTuple>::value> GetAsStringHelpers = {
         {&getString<Indices>...}};
   };

   static constexpr runtime_get_func_table<Tuple> getLookup{};

public:
   std::string getFast(std::string_view name)
   {
      const auto iter = std::lower_bound(Names.begin(), Names.end(), name);
      if (iter == Names.end())
      {
         throw std::invalid_argument(fmt::format("{} is not a valid field name", name));
      }

      const auto pos = std::distance(Names.begin(), iter);

      const auto func = getLookup.GetAsStringHelpers[pos];

      return func(value);
   }

   Tuple value;
};

int main()
{
   auto val = std::make_tuple(Field<Width>{}, Field<Height>{7});

   std::get<0>(val).value = 5;
   fmt::print("Value[0] is: {}\n", std::get<0>(val).value);
   fmt::print("Value[1] is: {}\n", std::get<1>(val).value);

   std::apply([](auto&&... args) { (fmt::print("{}\n", args.value), ...); }, val);

   std::apply([](auto&&... args) { (((args.value == 7) ? printf("found 7\n") : 0), ...); }, val);

   std::apply([](auto&&... args) { (((args.getName() == "height") ? printf("found width\n") : 0), ...); }, val);

   std::apply([](auto&&... args) { (((args.getName() == "foo") ? printf("found foo\n") : 0), ...); }, val);

   using TupleType = decltype(val);

   constexpr auto widthPos  = tuple_element_index_v<Field<Width>, TupleType>;
   constexpr auto heightPos = tuple_element_index_v<Field<Height>, TupleType>;

   fmt::print("Width  index: {}\n", widthPos);
   fmt::print("Height index: {}\n", heightPos);

   fmt::print("Width  value: {}\n", std::get<widthPos>(val).value);
   fmt::print("Height value: {}\n", std::get<heightPos>(val).value);

   fmt::print("sizeof(RectangleSize) = {}\n", sizeof(RectangleSize));

   RectangleSize rs;
   rs.set<Width>(5u);
   fmt::print("RS Width: {}\n", rs.get<Width>());

   // using Rs2 = ParamGroup<Width, Height, Ratio>;  // not sorted, fails to compile!
   using Rs2 = ParamGroup<Height, Ratio, Width>;

   fmt::print("sizeof(Rs2) = {}\n", sizeof(Rs2));

   Rs2 rs2;
   rs2.set<Height>(4);
   fmt::print("RS2 Height: {}\n", rs2.get<Height>());

   rs2.set<Ratio>(1.68);
   fmt::print("RS2 ratio: {}\n", rs2.get<Ratio>());

   rs2.set("ratio", "2.71");
   fmt::print("RS2 ratio: {}\n", rs2.get("ratio"));

   fmt::print("Field names:\n");
   for (const auto name : Rs2::Names)
   {
      fmt::print("   {}\n", name);
   }

#if 0
   fmt::print("2nd parameter value: {}\n", Rs2::getString<1>(rs2.value));

   const auto func = Rs2::getLookup.GetAsStringHelpers[1];

   fmt::print("2nd parameter value: {}\n", func(rs2.value));
#endif

   fmt::print("width parameter value: {}\n", rs2.getFast("width"));
   fmt::print("height parameter value: {}\n", rs2.getFast("height"));
   fmt::print("ratio parameter value: {}\n", rs2.getFast("ratio"));

   return 0;
}

