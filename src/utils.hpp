
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

}  // namespace utils
