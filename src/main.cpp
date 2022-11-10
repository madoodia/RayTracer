#include <iostream>

int main()
{

  // Image

  const int image_width = 256;
  const int image_height = 256;

  // Render

  std::cout << "P3\n"
            << image_width
            << " "
            << image_height
            << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j)
  {
    for (int i = 0; i < image_width; ++i)
    {
      auto r = double(i) / (image_width - 1);
      auto g = double(j) / (image_height - 1);
      auto b = 0.0f;

      int ir = static_cast<int>(255.999f * r);
      int ig = static_cast<int>(255.999f * g);
      int ib = static_cast<int>(255.999f * b);

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
}