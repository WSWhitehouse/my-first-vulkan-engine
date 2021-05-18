#include <iostream>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

int main(int /*argc*/, char ** /*argv*/)
{
    glm::vec2 vector = {0, 1};
    std::cout << "Hello World! " << vector.x << std::endl;
    return EXIT_SUCCESS;
}