#ifndef __VALHALLA_STATUS_H__
#define __VALHALLA_STATUS_H__

#include <_falcie_config.h>
#include <thoth/enum_flag.h>

#include <ostream>

namespace valhalla {
enum class FALCIE_EXPORT StatusFlag {
  kOk = 0,  // How will this play with flags? That said can't be or'd
  kRuntimeError = 1,
  kDelegateError = 2,
  kApplicationError = 4,  // 3,
  kDataNotFound = 8,      // kTfLiteDelegateDataNotFound = 4,
  kDataWriteError = 16,   // kTfLiteDelegateDataWriteError = 5,
  kDataReadError = 32,    // kTfLiteDelegateDataReadError = 6,
  kUnresolvedOps = 64,    // kTfLiteUnresolvedOps = 7,
  kCancelled = 128,       // kTfLiteCancelled = 8,
  kError = (kRuntimeError | kDelegateError | kApplicationError | kDataNotFound |
            kDataWriteError | kDataReadError | kUnresolvedOps | kCancelled),
};

FALCIE_LOCAL StatusFlag FromTfLiteStatus(int tfLiteStatus) noexcept;

FALCIE_EXPORT std::ostream& operator<<(std::ostream& os,
                                       StatusFlag flag) noexcept;

}  // namespace valhalla

#endif