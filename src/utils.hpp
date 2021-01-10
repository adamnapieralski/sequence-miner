
#include <sstream>
#include <string>
#include <memory>

namespace utils {

template <typename Container>
std::string print(const Container &container) {
  if (container.empty()) {
    return "[]";
  }
  std::stringstream ss;
  ss << "[";
  for (const auto &elem : container) {
    ss << elem << " ";
  }
  auto str = ss.str();
  str.pop_back();
  str.push_back(']');
  return str;
}

template <typename MapContainer>
std::string printMap(const MapContainer &container) {
  if (container.empty()) {
    return "[]";
  }
  std::stringstream ss;
  ss << "[";
  for (const auto &elem : container) {
    ss << "(" << elem.first << ": " << elem.second << ") ";
  }
  auto str = ss.str();
  str.pop_back();
  str.push_back(']');
  return str;
}

template<typename D, typename B>
std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}
  
template<typename D, typename B>
std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>&& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}

}  // namespace utils
