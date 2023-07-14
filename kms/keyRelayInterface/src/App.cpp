#include "AppComponent.hpp"
#include "Runner.hpp"

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