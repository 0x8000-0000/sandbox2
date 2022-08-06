#include <iostream>

struct Reporter
{
public:
   explicit Reporter(size_t size) : m_size{size}
   {
      std::cout << "Collected size " << m_size << '\n';
   }

   size_t m_size;
};

template <typename Bar>
class TypeCollector
{
public:
   TypeCollector()
   {
      std::cout << "Constructed collector size " << m_helper.m_size << '\n';
   }

private:
   static Reporter m_helper;
};

template <typename Bar>
Reporter TypeCollector<Bar>::m_helper{sizeof(Bar)};

template <typename Foo>
void doWithFoo(Foo foo)
{
   static const TypeCollector<Foo> collector;
   (void)collector;

   std::cout << "Foo with " << sizeof(foo) << '\n';
}

int main()
{
   std::cout << "Main begins\n";

   doWithFoo(char{1});
   doWithFoo(short{3});
   doWithFoo(float{5});

   std::cout << "Main ends\n";

   return 0;
}
