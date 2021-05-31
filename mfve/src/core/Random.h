#ifndef MY_FIRST_VULKAN_ENGINE_RANDOM_H
#define MY_FIRST_VULKAN_ENGINE_RANDOM_H

#include <random>

class Random
{
 public:
  /**
   * \brief Generate a random num between a min and max value (range is inclusive)
   * \tparam T Type of num to generate
   * \param min Minimum value
   * \param max Maximum value
   * \return Randomly generated value
   */
  template<typename T>
  static T Range(T min, T max)
  {
    std::random_device random_device;
    using distribution = typename dist_selector<std::is_integral<T>::value, T>::type;
    distribution dist(min, max);

    return dist(random_device);
  }

 private:
  template<bool is_integral, typename T>
  struct dist_selector;

  template<typename T>
  struct dist_selector<true, T>
  {
    using type = std::uniform_int_distribution<T>;
  };

  template<typename T>
  struct dist_selector<false, T>
  {
    using type = std::uniform_real_distribution<T>;
  };
};

#endif // MY_FIRST_VULKAN_ENGINE_RANDOM_H
