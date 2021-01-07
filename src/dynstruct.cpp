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

template <typename Kind>
struct Field
{
   explicit constexpr Field(typename Kind::Type aValue) : value{aValue}
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

   typename Kind::Type value;
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

   return 0;
}
