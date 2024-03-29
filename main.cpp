#include <complex>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

// Метафункции
template <bool condition, typename Type> struct EnableIf;

template <typename Type> struct EnableIf<true, Type> {
  using type = Type;
};

template <typename T, typename U> struct IsSame {
  static constexpr bool value = false;
};

template <typename T> struct IsSame<T, T> {
  static constexpr bool value = true;
};

/*!
\brief Первый шаблон:
\param Адрес может быть представлен в виде произвольного
целочисленного типа. Выводить побайтово в беззнаковом виде, начиная со старшего
байта, с символом `.` (символ точки) в качестве разделителя. Выводятся все байты
числа.
\return вывод ip адреса
*/
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type print_ip(T ip) {

  int i = 0;
  while (i < sizeof(ip)) {
    if (i != 0)
      std::cout << ".";
    std::cout << (0xFF & (ip >> ((sizeof(ip) - i - 1)) * 8));
    i++;
  }
  std::cout << std::endl;
}
/*!
\brief Второй шаблон:
\param Адрес может быть представлен в виде строки. Выводится как
есть, вне зависимости от содержимого
\return вывод ip адреса
*/
template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
print_ip(const T &ip) {
  std::cout << ip << std::endl;
}

/*!
\brief Третий шаблон:
\param Адрес может быть представлен в виде контейнеров
`std::list`, `std::vector`. Выводится полное содержимое контейнера поэлементно и
разделяется
`.` (символом точка). Элементы выводятся как есть.
\return вывод ip адреса
*/
template <typename T>
typename std::enable_if<
    std::is_same<std::vector<typename T::value_type>, T>::value ||
        std::is_same<std::list<typename T::value_type>, T>::value,
    void>::type
print_ip(const T &ip) {
  std::string resultStr = "";
  for (const auto &al : ip) {
    if (resultStr != "")
      resultStr.append(".");
    resultStr += std::to_string(al);
  }
  std::cout << resultStr << std::endl;
}

/*!
\brief Четвертый шаблон:
\param Опционально адрес может быть представлен в виде
`std::tuple` при условии, что все типы одинаковы. Выводится полное содержимое
поэлементно и разделяется
`.` (одним символом точка). Элементы выводятся как есть. В случае, если типы
кортежа не одинаковы, должна быть выдана ошибка при компиляции кода
\return вывод ip адреса
*/
template <typename T, typename... Args,
          typename std::enable_if<std::is_same<T, Args...>::value, void>::type>
auto print_ip(std::tuple<T, Args...> ip) {

  // std::apply([](auto&&... args) {((std::cout << args << '.'), ...);}, ip);
  // std::cout << std::endl;
}

int main(int argc, char const *argv[]) {

  print_ip(int8_t{-1});                           // 255
  print_ip(int16_t{0});                           // 0.0
  print_ip(int32_t{2130706433});                  // 127.0.0.1
  print_ip(int64_t{8875824491850138409});         // 123.45.67.89.101.112.131.41
  print_ip(std::string{"Hello, World!"});         // Hello, World!
  print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
  print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
  // print_ip(std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0

  return 0;
}