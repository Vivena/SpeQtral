#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"


void run() {

  /* Register Components in scope of run() method */
  example::book::AppComponent components(
    {"localhost", 8002},      // Book Service
    {"book.virtualhost", 0}   // Book Service Virtual Host
  );

  /* run */
  std::list<std::thread> acceptingThreads;

  example::book::Runner runner;
  runner.run(acceptingThreads);

  for(auto& thread : acceptingThreads) {
    thread.join();
  }

}

int main(int argc, const char * argv[]) {


}