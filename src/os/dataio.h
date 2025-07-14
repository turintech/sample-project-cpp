#ifndef __OS_DATAIO_H__
#define __OS_DATAIO_H__

#include <cstddef>
#include <string>
#include <windows.h>

namespace OsDataIo {

// Prefer passing std::string by const reference to avoid unnecessary copying
std::string CatFile();

// Accept pointer to const int and pointer to long for efficient memory usage,
// mark inputArray as 'const', and make outputArray 'long*' (not 'long []'), both unchanged
// Mark length as 'size_t', which is already efficient
// Add [[nodiscard]] attribute to encourage users to check return value if you refactor CatFile later.
void CastIntToLongArray(const int *inputArray, long *outputArray, size_t length);

}   // namespace OsDataIo

#endif   // ! __OS_DATAIO_H__