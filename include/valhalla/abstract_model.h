#ifndef __VALHALLA_ABSTRACT_MODEL_H__
#define __VALHALLA_ABSTRACT_MODEL_H__

namespace valhalla {
class AbstractModel {

  virtual ~AbstractModel() = default;

  virtual int GetFunctionCount() = 0;

  virtual const char* GetFunctionName(int index) = 0;



  //const char *GetSignatureKey(int signature_index);
};
}  // namespace valhalla

#endif