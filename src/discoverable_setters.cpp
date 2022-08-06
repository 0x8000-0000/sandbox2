/** @file discoverable_setters.cpp
 * @brief Explore the ability to export write-only properties from a class
 */

#include <fmt/format.h>

#include <array>
#include <functional>
#include <span>
#include <string_view>
#include <variant>

#if 0
class AlphaSlot
{
public:
   int computeSum(int val)
   {
      return val + 2;
   }
};

class BetaSlot
{
public:
   int computeProd(int val)
   {
      return val * 2;
   }
};

class Configurable;

template <typename SlotType>
class SlotSetter
{
public:
   constexpr SlotSetter(SlotType*& slot) : m_slot{slot}
   {
   }

   void set(SlotType* actual)
   {
      m_slot = actual;
   }

private:
   SlotType*& m_slot;
};

class Configurable
{
public:

   struct Element
   {
      std::string_view name;
      std::variant<SlotSetter<AlphaSlot>, SlotSetter<BetaSlot>> setter;
   };

   [[nodiscard]] virtual std::span<const Element> enumerateSlots() = 0;
};

class Concrete final : public Configurable
{
public:
   [[nodiscard]] std::span<const Element> enumerateSlots() override
   {
      return k_Elements;
   }

private:

   AlphaSlot* m_alphaOne;
   AlphaSlot* m_alphaTwo;
   BetaSlot*  m_beta;

   static constexpr auto k_Elements = std::to_array<Element>({ {"one", SlotSetter<AlphaSlot>(m_alphaOne)} });
};
#endif

class Foo
{
public:
   using IntField = int Foo::*;

   struct Element
   {
      std::string_view name;
      IntField         field;
   };

   void setFoo(int val, IntField field)
   {
      this->*field = val;
   }

   int getAlpha() const noexcept { return alpha; }
   int getBeta() const noexcept { return beta; }

   static std::span<const Element> getIndices() noexcept
   {
      return k_Elements;
   }

private:
   int alpha = 0;
   int beta  = 0;

   static constexpr auto k_Elements = std::to_array<Element>({
      {"alpha", &Foo::alpha},
      {"beta", &Foo::beta},
   });
};

int main()
{
   Foo foo;

   foo.setFoo(5, Foo::getIndices()[0].field);
   foo.setFoo(3, Foo::getIndices()[1].field);

   const auto field = Foo::getIndices()[0].field;
   foo.setFoo(9, field);

   fmt::print("foo.alpha = {}\n", foo.getAlpha());
   fmt::print("foo.beta = {}\n", foo.getBeta());

   return 0;
}