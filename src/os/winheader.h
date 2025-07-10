#ifndef __OS_WINHEADER_H__
#define __OS_WINHEADER_H__

#include <windows.h>

namespace OsWinHeader {

// Accept message as const char* to avoid string copy. 
// Mark function as inline to give compiler opportunity to optimize out-of-header usage for this trivial wrapper.
// Add noexcept for better codegen and possible optimization.
inline void ShowMessage(const char* message) noexcept {
    MessageBoxA(nullptr, message, "Info", MB_OK | MB_ICONINFORMATION);
}

}

#endif // ! __OS_WINHEADER_H__