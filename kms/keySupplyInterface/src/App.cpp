#include "AppComponent.hpp"
#include "Runner.hpp"

void run() {


  // std::shared_ptr<kms::keypool::manager::KeyPoolManager> keyPoolManager = std::make_shared<kms::keypool::manager::KeyPoolManager>();

  /* Register Components in scope of run() method */
  // kms::keySupply::AppComponent components(
  //   {"localhost", 8002},      // Book Service
  //   {"keySupply.virtualhost", 0},
  //   keyPoolManager  // Book Service Virtual Host
  // );

  // /* run */
  // std::list<std::thread> acceptingThreads;
  
  // kms::keySupply::Runner runner;
  // runner.run(acceptingThreads);

  // for(auto& thread : acceptingThreads) {
  //   thread.join();
  // }

}

int main(int argc, const char * argv[]) {

  oatpp::base::Environment::init();
  run();
  oatpp::base::Environment::destroy();
  
  return 0;
}