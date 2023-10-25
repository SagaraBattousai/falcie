
#include <tensorflow/lite/c/c_api_types.h>
#include <valhalla/status_flag.h>

namespace valhalla {
StatusFlag FromTfLiteStatus(int tfLiteStatus) noexcept {
  switch (static_cast<TfLiteStatus>(tfLiteStatus)) {
    /// Success
    case (kTfLiteOk):
      return StatusFlag::kOk;

    /// Generally referring to an error in the runtime (i.e. interpreter)
    case (kTfLiteError):
      return StatusFlag::kRuntimeError;

    /// Generally referring to an error from a TfLiteDelegate itself.
    case (kTfLiteDelegateError):
      return StatusFlag::kDelegateError;

    /// Generally referring to an error in applying a delegate due to
    /// incompatibility between runtime and delegate, e.g., this error is
    /// returned when trying to apply a TF Lite delegate onto a model graph
    /// that's already immutable.
    case (kTfLiteApplicationError):
      return StatusFlag::kApplicationError;

    /// Generally referring to serialized delegate data not being found.
    /// See tflite::delegates::Serialization.
    case (kTfLiteDelegateDataNotFound):
      return StatusFlag::kDataNotFound;

    /// Generally referring to data-writing issues in delegate
    /// serialization. See tflite::delegates::Serialization.
    case (kTfLiteDelegateDataWriteError):
      return StatusFlag::kDataWriteError;

    /// Generally referring to data-reading issues in delegate
    /// serialization. See tflite::delegates::Serialization.
    case (kTfLiteDelegateDataReadError):
      return StatusFlag::kDataReadError;

    /// Generally referring to issues when the TF Lite model has ops that
    /// cannot be resolved at runtime. This could happen when the specific
    /// op is not registered or built with the TF Lite framework.
    case (kTfLiteUnresolvedOps):
      return StatusFlag::kUnresolvedOps;

    /// Generally referring to invocation cancelled by the user.
    /// See `interpreter::Cancel`.
    // TODO(b/194915839): Implement `interpreter::Cancel`.
    // TODO(b/250636993): Cancellation triggered by
    // `SetCancellationFunction` should also return this status code.
    case (kTfLiteCancelled):
      return StatusFlag::kCancelled;

    default:
      return StatusFlag::kError;
  }
}

std::ostream& operator<<(std::ostream& os, StatusFlag flag) noexcept {
  os << "Status: ";
  switch (flag) {
    case (StatusFlag::kOk):
      os << "OK";
      break;
    case (StatusFlag::kRuntimeError):
      os << "Runtime Error";
      break;
    case (StatusFlag::kDelegateError):
      os << "Delegate Error";
      break;
    case (StatusFlag::kApplicationError):
      os << "Application Error";
      break;
    case (StatusFlag::kDataNotFound):
      os << "Data Not Found";
      break;
    case (StatusFlag::kDataWriteError):
      os << "Data Write Error";
      break;
    case (StatusFlag::kDataReadError):
      os << "Data Read Error";
      break;
    case (StatusFlag::kUnresolvedOps):
      os << "Unresolved Ops";
      break;
    case (StatusFlag::kCancelled):
      os << "Cancelled";
      break;
    case (StatusFlag::kError):
      os << "(General) Error";
      break;
  }
  return os;
}

}  // namespace valhalla
