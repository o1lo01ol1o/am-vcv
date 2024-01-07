
// #include "tps.hpp"
#include "../../sdk/Rack-SDK/dep/include/nanovg.h"
#include "../../sdk/Rack-SDK/dep/include/pffft.h"
#include "pitch.hpp"
#include "plugin.hpp"
// #include "tps.hpp"
#include "rack.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

// bool prop_euclidean_to_origin_is_l2_norm(ZZ_12 v)
// {
//     std::vector<std::complex<float>> cv =
//         cs((FixedVector<ZZ_12, 1>({v})).to_vector()).to_vector();
//     std::vector<std::complex<float>> zeroVec(cv.size(),
//                                              std::complex<float>(0, 0));

//     std::complex<float> euclideanDist = euclideanDistance(cv, zeroVec);
//     std::complex<float> l2Norm = complexNorm(cv);

//     // Comparing the magnitudes as the result is a complex number
//     return euclideanDist == l2Norm;
// }

// // Function to generate all combinations recursively
// void generateCombinations(std::vector<std::vector<ZZ_12>> &result,
//                           std::vector<ZZ_12> &current, int maxLength,
//                           int start = 0)
// {
//     if (current.size() > 0 && current.size() <= maxLength)
//     {
//         result.push_back(current);
//     }
//     if (current.size() == maxLength)
//     {
//         return;
//     }

//     for (int i = start; i < 12; ++i)
//     {
//         current.push_back(ZZ_12(i));
//         generateCombinations(result, current, maxLength, i);
//         current.pop_back();
//     }
// }

// std::vector<std::vector<ZZ_12>> getAllCombinations(int maxLength)
// {
//     std::vector<std::vector<ZZ_12>> combinations;
//     std::vector<ZZ_12> current;
//     generateCombinations(combinations, current, maxLength);
//     return combinations;
// }

// // Metric function type: takes two vectors of std::complex<float> and returns
// a
// // std::complex<float>
// using MetricFunction = std::function<std::complex<float>(
//     const std::vector<std::complex<float>> &,
//     const std::vector<std::complex<float>> &)>;

// void findMinMaxMetric(MetricFunction metric)
// {

//     std::vector<std::vector<ZZ_12>> allVectors = getAllCombinations(12);
//     float minDistance = std::numeric_limits<float>::max();
//     float maxDistance = 0;

//     for (size_t i = 0; i < allVectors.size(); ++i)
//     {
//         for (size_t j = i + 1; j < allVectors.size(); ++j)
//         {
//             auto dft1 = tk6(allVectors[i]).to_vector();
//             auto dft2 = tk6(allVectors[j]).to_vector();
//             float distance = std::abs(metric(dft1, dft2));

//             if (distance < minDistance)
//             {
//                 minDistance = distance;
//             }
//             if (distance > maxDistance)
//             {
//                 maxDistance = distance;
//             }
//         }
//     }

//     std::cout << "Minimum Distance: " << minDistance << std::endl;
//     std::cout << "Maximum Distance: " << maxDistance << std::endl;
// }
// #ifdef TEST

// void testConsonance()
// {
//     // Representing the pitches [C, F, Bb] - adjust these values to your
//     actual
//     // pitch representation.
//     std::vector<ZZ_12> pitches =
//         FixedVector<Chromatic, 3>({Chromatic::C, Chromatic::F,
//         Chromatic::Bb})
//             .map(toZZ_12)
//             .to_vector();
//     // Print the value of pitches
//     for (const auto &pitch : pitches)
//     {
//         std::cout << pitch.to_string() << " ";
//     }
//     std::cout << cs(pitches).to_string() << " ";
//     float expectedValue = 0.13600764220861314f;
//     float tolerance = 0.001f; // Adjust tolerance as needed.

//     float actualValue = consonance(pitches);

//     std::cout << "Expected value: " << expectedValue
//               << ", Actual value: " << actualValue << std::endl;

//     if (std::abs(expectedValue - actualValue) < tolerance)
//     {
//         std::cout << "Test passed." << std::endl;
//     }
//     else
//     {
//         std::cout << "Test failed." << std::endl;
//     }
// }

// void testMaxClassConsonance()
// {
//     float expectedValue = 32.86335345030997f;
//     float tolerance = 0.001f; // Adjust tolerance as needed

//     float actualValue = maxClassConsonance();

//     std::cout << "Expected value: " << expectedValue
//               << ", Actual value: " << actualValue << std::endl;

//     if (std::abs(expectedValue - actualValue) < tolerance)
//     {
//         std::cout << "Test passed." << std::endl;
//     }
//     else
//     {
//         std::cout << "Test failed." << std::endl;
//     }
// }
// void testCsFunction()
// {
//     // Initialize the vector with chromatic values
//     FixedVectorTwelveChromatic chromaticValues = chromaticVector();

//     // Convert chromatic values to uint using the map function
//     auto toUint = [](Chromatic c)
//     { return static_cast<uint>(c); };
//     auto toRealPart = [](const std::complex<float> &c)
//     { return c.real(); };
//     auto uintValues = chromaticValues.map(toZZ_12).to_vector();

//     // Apply the cs function
//     FixedVectorTwelveFloat csResult = (cs(uintValues)).map(toRealPart);

//     // Expected result
//     FixedVectorTwelveFloat expectedCs(1.0f);

//     // Check the result
//     assert(csResult == expectedCs);

//     std::cout << "Test passed: cs function works as expected." << std::endl;
// }

// int main()
// {
//     testCsFunction();
//     testMaxClassConsonance();
//     testConsonance();
//     assert(prop_euclidean_to_origin_is_l2_norm(toZZ_12(Chromatic::C)));
//     findMinMaxMetric(euclideanDistance);
//     findMinMaxMetric(cosineDistance);
//     return 0;
// }
// #endif