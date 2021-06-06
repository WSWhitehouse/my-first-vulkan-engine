#include <pch.h>

#include "Sandbox.h"

int main(int argc, char** argv)
{
  MFVE::Logger::CreateLogger();

  Sandbox app;

  try
  {
    app.Init({});
    app.Run();
    app.CleanUp();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  MFVE::Logger::DestroyLogger();
  return EXIT_SUCCESS;
}