#include "AppComponent.hpp"
#include "Runner.hpp"

void run() {


}

int main(int argc, const char * argv[]) {

  oatpp::base::Environment::init();
  run();
  oatpp::base::Environment::destroy();
  
  return 0;
}