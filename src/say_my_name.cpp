#include <fmt/format.h>

#include <algorithm>
#include <string_view>

struct Foo
{
   int arg;
};


template <typename T>
class NameLength
{

   static constexpr size_t getLength()
   {
      const auto* name =
#ifdef _MSC_VER
         __FUNCSIG__;
#else
         __PRETTY_FUNCTION__;
#endif

      return std::char_traits<char>::length(name) - 90;
   }

public:

   static constexpr size_t theLength = getLength();
};

template <typename T>
struct Namer
{
   static constexpr auto getName()
   {
      const auto* name =
#ifdef _MSC_VER
         __FUNCSIG__;
#else
         __PRETTY_FUNCTION__;
#endif

      std::array<char, NameLength<T>::theLength> localName;

      std::fill_n(localName.data(), localName.size(), 0);
      std::copy_n(std::next(name, 52), NameLength<T>::theLength, localName.data());

      return localName;
   }

   static constexpr auto realName = getName();
};

template <typename T>
constexpr std::string_view getMyName()
{
#ifdef _MSC_VER
   return __FUNCSIG__;
#else
   return __PRETTY_FUNCTION__;
#endif
}

int main()
{
#if 0
   static constexpr auto fooName = getMyName<Foo>();
   fmt::print("foo: {}\n", fooName);

   static constexpr auto floatName = getMyName<float>();
   fmt::print("float: {}\n", floatName);
#endif

   static constexpr auto blip = Namer<Foo>();
   fmt::print("namer: {}\n", std::string_view{blip.realName.data(), blip.realName.size()});

   static constexpr auto nl = NameLength<Foo>::theLength;

   fmt::print("Estimated length: {}\n", nl);

   return 0;
}
