
#ifndef kms_keySupply_Runner_hpp
#define kms_keySupply_Runner_hpp

#include "oatpp/web/server/api/ApiController.hpp"

#include <list>
#include <thread>

namespace kms { namespace keySupply {

class Runner {
public:

  Runner();

  void run(std::list<std::thread>& acceptingThreads);

};

}}

#endif // kms_keySupply_Runner_hpp