#include <fmt/format.h>

#include <array>
#include <ostream>
#include <streambuf>

class MyStreambuf : public std::streambuf
{
public:
   ~MyStreambuf() override
   {
      if (m_size > 0)
      {
         fmt::print("FIN: {}\n", std::string_view{m_buffer.data(), m_size});
      }
   }

   std::streamsize xsputn(const char_type* s, std::streamsize n) override
   {
      std::streamsize idx = 0;

      while (idx < n)
      {
         if (s[idx] == '\n')
         {
            fmt::print("LOG: {}\n", std::string_view{m_buffer.data(), m_size});
            m_size = 0;
         }
         else if (m_size == BUFSIZE)
         {
            fmt::print("LOG: {}\n", std::string_view{m_buffer.data(), m_size});
            m_size = 0;
         }
         else
         {
            m_buffer[m_size] = s[idx];
            m_size++;
         }

         ++idx;
      }

      return n;
   }

   int_type overflow(int_type ch) override
   {
      if (ch == '\n')
      {
         if (m_size > 0)
         {
            fmt::print("LOG: {}\n", std::string_view{m_buffer.data(), m_size});
            m_size = 0;
         }
      }
      else if (m_size == BUFSIZE)
      {
         fmt::print("LOG: {}\n", std::string_view{m_buffer.data(), m_size});
         m_size = 0;
      }
      else
      {
         m_buffer[m_size] = ch;
         m_size++;
      }

      return 0;
   }

protected:
   int sync() override
   {
      if (m_size > 0)
      {
         fmt::print("SYNC: {}\n", std::string_view{m_buffer.data(), m_size});
         m_size = 0;
      }
      return 0;
   }

private:
   static constexpr size_t BUFSIZE = 256;

   std::array<char, BUFSIZE> m_buffer{};
   size_t                    m_size{0};
};

std::ostream& getLogger()
{
   static MyStreambuf  myBuf;
   static std::ostream myStream{&myBuf};

   return myStream;
}

int main()
{
   getLogger() << "One message" << std::endl;
   getLogger() << "Hello, world!\n";
   getLogger() << "Bye";

   return 0;
}
