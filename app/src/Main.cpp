#include <pch.h>

#include "Sandbox.h"

int main(int argc, char** argv)
{
  MFVE::Logger::CreateLogger();

  MFVE::AppProperties appProperties {};
  appProperties.name = "MFVE Sandbox";

  MFVE::Application* app = new Sandbox(appProperties);

  try
  {
    app->Run();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  delete app;

  MFVE::Logger::DestroyLogger();
  return EXIT_SUCCESS;
}