#include <fmt/format.h>
#include <frozen/string.h>

#include <cstdint>
#include <string_view>

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

template <typename Kind>
struct Field
{
   using Type = typename Kind::Type;

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

   Type value;
};

struct Width
{
   using Type = uint32_t;

   static constexpr std::string_view Name = "width";

   static constexpr uint64_t Hash = fnv1(Name);
};

struct Height
{
   using Type = uint32_t;

   static constexpr std::string_view Name = "height";

   static constexpr uint64_t Hash = fnv1(Name);
};

struct Top
{
   using Type = uint32_t;

   static constexpr std::string_view Name = "top";

   static constexpr uint64_t Hash = fnv1(Name);
};

struct Left
{
   using Type = uint32_t;

   static constexpr std::string_view Name = "left";

   static constexpr uint64_t Hash = fnv1(Name);
};

struct RectangleSize
{
   void set(std::string_view name, uint32_t value)
   {
      const auto inputHash = fnv1(name);

      switch (inputHash)
      {
      case Width::Hash:
         width.value = value;
         break;

      case Height::Hash:
         height.value = value;
         break;

      default:
         throw std::invalid_argument(fmt::format("Invalid field name {}", name));
      }
   }

   Field<Width>  width{0};
   Field<Height> height{0};
};

struct Target
{
   using Type = RectangleSize;

   static constexpr std::string_view Name = "target";

   static constexpr uint64_t Hash = fnv1(Name);
};

struct Param
{
   Field<Target> target;

   Field<Top>  top;
   Field<Left> left;
};

// https://hackernoon.com/variadic-template-in-c-implementing-unsophisticated-tuple-w8153ump

template <typename... T>
struct Tuple
{
};

template <typename T,
          typename... Rest // Template parameter pack
          >
struct Tuple<T, Rest...> // Class parameter pack
{
   Field<T>       first;
   Tuple<Rest...> rest; // Parameter pack expansion

   Tuple() = default;

   Tuple(const T& f, const Rest&... r) : first(f), rest(r...)
   {
   }
};

int main()
{
   Field<Width>  f1{5};
   Field<Height> f2{7};

   fmt::print("{} -> {}\n", f1.getName(), f1.value);
   fmt::print("{} -> {}\n", f2.getName(), f2.value);

   RectangleSize rs;

   fmt::print("Before: {}\n", rs.width.value);
   rs.set(Width::Name, 5);
   fmt::print("After:  {}\n", rs.width.value);

   Tuple<Width, Height> sample;

   sample.first.value = 5;
   sample.rest.first.value = 7;

   return 0;
}
