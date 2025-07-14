#include "genstring.h"

/** @brief Generates a random string of length n
 *
 *  @param n Length of the string to generate
 *  @return A random string of length n
 */
std::string
GenString::RandomString(int n) {
  // Reserve memory to avoid reallocation
  std::string ret;
  ret.reserve(n);

  // Use a static local variable to avoid reseeding every call (causes performance and repetition issues)
  static bool seeded = false;
  if (!seeded) {
    srand(0);
    seeded = true;
  }

  for (int i = 0; i < n; ++i) {
    ret += static_cast<char>('a' + rand() % 26);
  }
  return ret;
}