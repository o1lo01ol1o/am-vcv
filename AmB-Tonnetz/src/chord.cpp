
#include "chord.hpp"
#include "pitch.hpp"
#include "plugin.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

int &ChordRep::operator[](size_t idx) {
  switch (idx) {
  case 0:
    return extraData;
  default:
    throw std::out_of_range("Index out of range for ChordRep");
  }
}

// Monad
const TNIStructure t1_monad = {
    {0}, {0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 11, 11, 11, 11}, 0};
const std::vector<const TNIStructure *> monad = {nullptr, &t1_monad};

// Diads
const TNIStructure t1_diad = {
    {0, 1}, {1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 9, 9, 8, 8}, 0};
const TNIStructure t2_diad = {
    {0, 2}, {0, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 9, 9, 9, 9}, 0};
const TNIStructure t3_diad = {
    {0, 3}, {0, 0, 1, 0, 0, 0}, {1, 1, 1, 1, 9, 9, 9, 9}, 0};
const TNIStructure t4_diad = {
    {0, 4}, {0, 0, 0, 1, 0, 0}, {1, 1, 1, 1, 9, 9, 9, 9}, 0};
const TNIStructure t5_diad = {
    {0, 5}, {0, 0, 0, 0, 1, 0}, {1, 1, 0, 0, 9, 9, 8, 8}, 0};
const TNIStructure t6_diad = {
    {0, 6}, {0, 0, 0, 0, 0, 1}, {2, 2, 2, 2, 10, 10, 10, 10}, 0};

const std::vector<const TNIStructure *> diad = {
    nullptr, &t1_diad, &t2_diad, &t3_diad, &t4_diad, &t5_diad, &t6_diad};

// Trichords
const TNIStructure t1_trichord = {
    {0, 1, 2}, {2, 1, 0, 0, 0, 0}, {1, 1, 0, 0, 7, 7, 4, 4}, 0};
const TNIStructure t2_trichord = {
    {0, 1, 3}, {1, 1, 1, 0, 0, 0}, {1, 0, 0, 0, 5, 6, 5, 5}, 0};
const TNIStructure t3_trichord = {
    {0, 1, 4}, {1, 0, 1, 1, 0, 0}, {1, 0, 0, 0, 5, 6, 5, 5}, 0};
const TNIStructure t4_trichord = {
    {0, 1, 5}, {1, 0, 0, 1, 1, 0}, {1, 0, 1, 0, 5, 6, 5, 6}, 0};
const TNIStructure t5_trichord = {
    {0, 1, 6}, {1, 0, 0, 0, 1, 1}, {1, 0, 0, 1, 6, 7, 7, 6}, 0};
const TNIStructure t6_trichord = {
    {0, 2, 4}, {0, 2, 0, 1, 0, 0}, {1, 1, 1, 1, 7, 7, 7, 7}, 0};
const TNIStructure t7_trichord = {
    {0, 2, 5}, {0, 1, 1, 0, 1, 0}, {1, 0, 0, 0, 5, 6, 5, 5}, 0};
const TNIStructure t8_trichord = {
    {0, 2, 6}, {0, 1, 0, 1, 0, 1}, {1, 0, 0, 1, 6, 7, 7, 6}, 0};
const TNIStructure t9_trichord = {
    {0, 2, 7}, {0, 1, 0, 0, 2, 0}, {1, 1, 0, 0, 7, 7, 4, 4}, 0};
const TNIStructure t10_trichord = {
    {0, 3, 6}, {0, 0, 2, 0, 0, 1}, {1, 1, 1, 1, 8, 8, 8, 8}, 0};
const TNIStructure t11_trichord = {
    {0, 3, 7}, {0, 0, 1, 1, 1, 0}, {1, 0, 0, 0, 5, 6, 5, 5}, 0};
const TNIStructure t12_trichord = {
    {0, 4, 8}, {0, 0, 0, 3, 0, 0}, {3, 3, 3, 3, 9, 9, 9, 9}, 0};

const std::vector<const TNIStructure *> trichord = {
    nullptr,       &t1_trichord,  &t2_trichord, &t3_trichord, &t4_trichord,
    &t5_trichord,  &t6_trichord,  &t7_trichord, &t8_trichord, &t9_trichord,
    &t10_trichord, &t11_trichord, &t12_trichord};

// Tetrachords
const TNIStructure t1_tetrachord = {
    {0, 1, 2, 3}, {3, 2, 1, 0, 0, 0}, {1, 1, 0, 0, 5, 5, 1, 1}, 0};
const TNIStructure t2_tetrachord = {
    {0, 1, 2, 4}, {2, 2, 1, 1, 0, 0}, {1, 0, 0, 0, 3, 4, 1, 1}, 0};
const TNIStructure t3_tetrachord = {
    {0, 1, 3, 4}, {2, 1, 2, 1, 0, 0}, {1, 1, 0, 0, 3, 3, 2, 2}, 0};
const TNIStructure t4_tetrachord = {
    {0, 1, 2, 5}, {2, 1, 1, 1, 1, 0}, {1, 0, 0, 0, 1, 3, 2, 3}, 0};
const TNIStructure t5_tetrachord = {
    {0, 1, 2, 6}, {2, 1, 0, 1, 1, 1}, {1, 0, 0, 0, 2, 4, 3, 2}, 0};
const TNIStructure t6_tetrachord = {
    {0, 1, 2, 7}, {2, 1, 0, 0, 2, 1}, {1, 1, 1, 1, 4, 4, 4, 4}, 0};
const TNIStructure t7_tetrachord = {
    {0, 1, 4, 5}, {2, 0, 1, 2, 1, 0}, {1, 1, 0, 0, 3, 3, 3, 3}, 0};
const TNIStructure t8_tetrachord = {
    {0, 1, 5, 6}, {2, 0, 0, 1, 2, 1}, {1, 1, 1, 1, 4, 4, 4, 4}, 0};
const TNIStructure t9_tetrachord = {
    {0, 1, 6, 7}, {2, 0, 0, 0, 2, 2}, {2, 2, 2, 2, 6, 6, 6, 6}, 0};
const TNIStructure t10_tetrachord = {
    {0, 2, 3, 5}, {1, 2, 2, 0, 1, 0}, {1, 1, 1, 1, 3, 3, 3, 3}, 0};
const TNIStructure t11_tetrachord = {
    {0, 1, 3, 5}, {1, 2, 1, 1, 1, 0}, {1, 0, 1, 0, 1, 3, 1, 3}, 0};
const TNIStructure t12_tetrachord = {
    {0, 2, 3, 6}, {1, 1, 2, 1, 0, 1}, {1, 0, 0, 0, 2, 4, 3, 2}, 0};
const TNIStructure t13_tetrachord = {
    {0, 1, 3, 6}, {1, 1, 2, 0, 1, 1}, {1, 0, 0, 1, 2, 4, 4, 2}, 0};
const TNIStructure t14_tetrachord = {
    {0, 2, 3, 7}, {1, 1, 1, 1, 2, 0}, {1, 0, 0, 0, 1, 3, 2, 3}, 0};
const TNIStructure t15_tetrachord = {
    {0, 1, 4, 6}, {1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 3, 3, 1}, 29};
const TNIStructure t16_tetrachord = {
    {0, 1, 5, 7}, {1, 1, 0, 1, 2, 1}, {1, 0, 0, 0, 2, 4, 3, 2}, 0};
const TNIStructure t17_tetrachord = {
    {0, 3, 4, 7}, {1, 0, 2, 2, 1, 0}, {1, 1, 1, 1, 3, 3, 3, 3}, 0};
const TNIStructure t18_tetrachord = {
    {0, 1, 4, 7}, {1, 0, 2, 1, 1, 1}, {1, 0, 0, 1, 2, 4, 4, 2}, 0};
const TNIStructure t19_tetrachord = {
    {0, 1, 4, 8}, {1, 0, 1, 3, 1, 0}, {1, 0, 1, 0, 3, 5, 3, 5}, 0};
const TNIStructure t20_tetrachord = {
    {0, 1, 5, 8}, {1, 0, 1, 2, 2, 0}, {1, 1, 0, 0, 3, 3, 3, 3}, 0};
const TNIStructure t21_tetrachord = {
    {0, 2, 4, 6}, {0, 3, 0, 2, 0, 1}, {1, 1, 1, 1, 6, 6, 6, 6}, 0};
const TNIStructure t22_tetrachord = {
    {0, 2, 4, 7}, {0, 2, 1, 1, 2, 0}, {1, 0, 0, 0, 3, 4, 1, 1}, 0};
const TNIStructure t23_tetrachord = {
    {0, 2, 5, 7}, {0, 2, 1, 0, 3, 0}, {1, 1, 0, 0, 5, 5, 1, 1}, 0};
const TNIStructure t24_tetrachord = {
    {0, 2, 4, 8}, {0, 2, 0, 3, 0, 1}, {1, 1, 1, 1, 6, 6, 6, 6}, 0};
const TNIStructure t25_tetrachord = {
    {0, 2, 6, 8}, {0, 2, 0, 2, 0, 2}, {2, 2, 2, 2, 6, 6, 6, 6}, 0};
const TNIStructure t26_tetrachord = {
    {0, 3, 5, 8}, {0, 1, 2, 1, 2, 0}, {1, 1, 0, 0, 3, 3, 2, 2}, 0};
const TNIStructure t27_tetrachord = {
    {0, 2, 5, 8}, {0, 1, 2, 1, 1, 1}, {1, 0, 0, 0, 2, 4, 3, 2}, 0};
const TNIStructure t28_tetrachord = {
    {0, 3, 6, 9}, {0, 0, 4, 0, 0, 2}, {4, 4, 4, 4, 8, 8, 8, 8}, 0};
const TNIStructure t29_tetrachord = {
    {0, 1, 3, 7}, {1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 3, 3, 1}, 15};

const std::vector<const TNIStructure *> tetrachord = {
    nullptr,         &t1_tetrachord,  &t2_tetrachord,  &t3_tetrachord,
    &t4_tetrachord,  &t5_tetrachord,  &t6_tetrachord,  &t7_tetrachord,
    &t8_tetrachord,  &t9_tetrachord,  &t10_tetrachord, &t11_tetrachord,
    &t12_tetrachord, &t13_tetrachord, &t14_tetrachord, &t15_tetrachord,
    &t16_tetrachord, &t17_tetrachord, &t18_tetrachord, &t19_tetrachord,
    &t20_tetrachord, &t21_tetrachord, &t22_tetrachord, &t23_tetrachord,
    &t24_tetrachord, &t25_tetrachord, &t26_tetrachord, &t27_tetrachord,
    &t28_tetrachord, &t29_tetrachord};

const TNIStructure t1_pentachord = {
    {0, 1, 2, 3, 4}, {4, 3, 2, 1, 0, 0}, {1, 1, 0, 0, 3, 3, 0, 0}, 0};
const TNIStructure t2_pentachord = {
    {0, 1, 2, 3, 5}, {3, 3, 2, 1, 1, 0}, {1, 0, 0, 0, 1, 2, 1, 1}, 0};
const TNIStructure t3_pentachord = {
    {0, 1, 2, 4, 5}, {3, 2, 2, 2, 1, 0}, {1, 0, 0, 0, 1, 1, 1, 0}, 0};
const TNIStructure t4_pentachord = {
    {0, 1, 2, 3, 6}, {3, 2, 2, 1, 1, 1}, {1, 0, 0, 0, 0, 2, 0, 0}, 0};
const TNIStructure t5_pentachord = {
    {0, 1, 2, 3, 7}, {3, 2, 1, 1, 2, 1}, {1, 0, 0, 0, 0, 1, 1, 1}, 0};
const TNIStructure t6_pentachord = {
    {0, 1, 2, 5, 6}, {3, 1, 1, 2, 2, 1}, {1, 0, 0, 0, 0, 1, 1, 1}, 0};
const TNIStructure t7_pentachord = {
    {0, 1, 2, 6, 7}, {3, 1, 0, 1, 3, 2}, {1, 0, 0, 1, 2, 3, 3, 2}, 0};
const TNIStructure t8_pentachord = {
    {0, 2, 3, 4, 6}, {2, 3, 2, 2, 0, 1}, {1, 1, 0, 0, 2, 2, 0, 0}, 0};
const TNIStructure t9_pentachord = {
    {0, 1, 2, 4, 6}, {2, 3, 1, 2, 1, 1}, {1, 0, 0, 0, 0, 2, 0, 1}, 0};
const TNIStructure t10_pentachord = {
    {0, 1, 3, 4, 6}, {2, 2, 3, 1, 1, 1}, {1, 0, 0, 0, 0, 1, 1, 0}, 0};
const TNIStructure t11_pentachord = {
    {0, 2, 3, 4, 7}, {2, 2, 2, 2, 2, 0}, {1, 0, 1, 0, 1, 1, 1, 1}, 0};
const TNIStructure t12_pentachord = {
    {0, 1, 3, 5, 6}, {2, 2, 2, 1, 2, 1}, {1, 1, 1, 1, 0, 0, 0, 0}, 36};
const TNIStructure t13_pentachord = {
    {0, 1, 2, 4, 8}, {2, 2, 1, 3, 1, 1}, {1, 0, 0, 0, 0, 2, 0, 1}, 0};
const TNIStructure t14_pentachord = {
    {0, 1, 2, 5, 7}, {2, 2, 1, 1, 3, 1}, {1, 0, 0, 0, 0, 1, 1, 1}, 0};
const TNIStructure t15_pentachord = {
    {0, 1, 2, 6, 8}, {2, 2, 0, 2, 2, 2}, {1, 1, 1, 1, 2, 2, 2, 2}, 0};
const TNIStructure t16_pentachord = {
    {0, 1, 3, 4, 7}, {2, 1, 3, 2, 1, 1}, {1, 0, 0, 0, 0, 1, 1, 0}, 0};
const TNIStructure t17_pentachord = {
    {0, 1, 3, 4, 8}, {2, 1, 2, 3, 2, 0}, {1, 1, 0, 0, 1, 1, 2, 2}, 37};
const TNIStructure t18_pentachord = {
    {0, 1, 4, 5, 7}, {2, 1, 2, 2, 2, 1}, {1, 0, 0, 0, 0, 1, 1, 0}, 38};
const TNIStructure t19_pentachord = {
    {0, 1, 3, 6, 7}, {2, 1, 2, 1, 2, 2}, {1, 0, 0, 1, 0, 2, 2, 0}, 0};
const TNIStructure t20_pentachord = {
    {0, 1, 3, 7, 8}, {2, 1, 1, 2, 3, 1}, {1, 0, 0, 0, 0, 1, 1, 1}, 0};
const TNIStructure t21_pentachord = {
    {0, 1, 4, 5, 8}, {2, 0, 2, 4, 2, 0}, {1, 0, 1, 0, 3, 3, 3, 3}, 0};
const TNIStructure t22_pentachord = {
    {0, 1, 4, 7, 8}, {2, 0, 2, 3, 2, 1}, {1, 1, 1, 1, 2, 2, 2, 2}, 0};
const TNIStructure t23_pentachord = {
    {0, 2, 3, 5, 7}, {1, 3, 2, 1, 3, 0}, {1, 0, 0, 0, 1, 2, 1, 1}, 0};
const TNIStructure t24_pentachord = {
    {0, 1, 3, 5, 7}, {1, 3, 1, 2, 2, 1}, {1, 0, 0, 0, 0, 2, 0, 1}, 0};
const TNIStructure t25_pentachord = {
    {0, 2, 3, 5, 8}, {1, 2, 3, 1, 2, 1}, {1, 0, 0, 0, 0, 1, 1, 0}, 0};
const TNIStructure t26_pentachord = {
    {0, 2, 4, 5, 8}, {1, 2, 2, 3, 1, 1}, {1, 0, 1, 0, 0, 2, 0, 2}, 0};
const TNIStructure t27_pentachord = {
    {0, 1, 3, 5, 8}, {1, 2, 2, 2, 3, 0}, {1, 0, 0, 0, 1, 1, 1, 0}, 0};
const TNIStructure t28_pentachord = {
    {0, 2, 3, 6, 8}, {1, 2, 2, 2, 1, 2}, {1, 0, 0, 1, 0, 2, 2, 0}, 0};
const TNIStructure t29_pentachord = {
    {0, 1, 3, 6, 8}, {1, 2, 2, 1, 3, 1}, {1, 0, 0, 0, 0, 2, 0, 0}, 0};
const TNIStructure t30_pentachord = {
    {0, 1, 4, 6, 8}, {1, 2, 1, 3, 2, 1}, {1, 0, 0, 0, 0, 2, 0, 1}, 0};
const TNIStructure t31_pentachord = {
    {0, 1, 3, 6, 9}, {1, 1, 4, 1, 1, 2}, {1, 0, 0, 1, 0, 3, 3, 0}, 0};
const TNIStructure t32_pentachord = {
    {0, 1, 4, 6, 9}, {1, 1, 3, 2, 2, 1}, {1, 0, 0, 0, 0, 1, 1, 0}, 0};
const TNIStructure t33_pentachord = {
    {0, 2, 4, 6, 8}, {0, 4, 0, 4, 0, 2}, {1, 1, 1, 1, 6, 6, 6, 6}, 0};
const TNIStructure t34_pentachord = {
    {0, 2, 4, 6, 9}, {0, 3, 2, 2, 2, 1}, {1, 1, 0, 0, 2, 2, 0, 0}, 0};
const TNIStructure t35_pentachord = {
    {0, 2, 4, 7, 9}, {0, 3, 2, 1, 4, 0}, {1, 1, 0, 0, 3, 3, 0, 0}, 0};
const TNIStructure t36_pentachord = {
    {0, 1, 2, 4, 7}, {2, 2, 2, 1, 2, 1}, {1, 0, 0, 1, 0, 1, 1, 0}, 12};

const TNIStructure t37_pentachord = {
    {0, 3, 4, 5, 8}, {2, 1, 2, 3, 2, 0}, {1, 1, 0, 0, 1, 1, 2, 2}, 17};
const TNIStructure t38_pentachord = {
    {0, 1, 2, 5, 8}, {2, 1, 2, 2, 2, 1}, {1, 0, 0, 0, 0, 1, 1, 0}, 18};

const std::vector<const TNIStructure *> pentachord = {
    nullptr,         &t1_pentachord,  &t2_pentachord,  &t3_pentachord,
    &t4_pentachord,  &t5_pentachord,  &t6_pentachord,  &t7_pentachord,
    &t8_pentachord,  &t9_pentachord,  &t10_pentachord, &t11_pentachord,
    &t12_pentachord, &t13_pentachord, &t14_pentachord, &t15_pentachord,
    &t16_pentachord, &t17_pentachord, &t18_pentachord, &t19_pentachord,
    &t20_pentachord, &t21_pentachord, &t22_pentachord, &t23_pentachord,
    &t24_pentachord, &t25_pentachord, &t26_pentachord, &t27_pentachord,
    &t28_pentachord, &t29_pentachord, &t30_pentachord, &t31_pentachord,
    &t32_pentachord, &t33_pentachord, &t34_pentachord, &t35_pentachord,
    &t36_pentachord, &t37_pentachord, &t38_pentachord

};

const TNIStructure t1_hexachord = {
    {0, 1, 2, 3, 4, 5}, {5, 4, 3, 2, 1, 0}, {1, 1, 0, 0, 1, 1, 0, 0}, 0};
const TNIStructure t2_hexachord = {
    {0, 1, 2, 3, 4, 6}, {4, 4, 3, 2, 1, 1}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t3_hexachord = {
    {0, 1, 2, 3, 5, 6}, {4, 3, 3, 2, 2, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 36};
const TNIStructure t4_hexachord = {
    {0, 1, 2, 4, 5, 6}, {4, 3, 2, 3, 2, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 37};
const TNIStructure t5_hexachord = {
    {0, 1, 2, 3, 6, 7}, {4, 2, 2, 2, 3, 2}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t6_hexachord = {
    {0, 1, 2, 5, 6, 7}, {4, 2, 1, 2, 4, 2}, {1, 1, 0, 0, 0, 0, 1, 1}, 38};
const TNIStructure t7_hexachord = {
    {0, 1, 2, 6, 7, 8}, {4, 2, 0, 2, 4, 3}, {2, 2, 2, 2, 2, 2, 2, 2}, 0};
const TNIStructure t8_hexachord = {
    {0, 2, 3, 4, 5, 7}, {3, 4, 3, 2, 3, 0}, {1, 1, 1, 1, 1, 1, 1, 1}, 0};
const TNIStructure t9_hexachord = {
    {0, 1, 2, 3, 5, 7}, {3, 4, 2, 2, 3, 1}, {1, 0, 1, 0, 0, 1, 0, 1}, 0};
const TNIStructure t10_hexachord = {
    {0, 1, 3, 4, 5, 7}, {3, 3, 3, 3, 2, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 39};
const TNIStructure t11_hexachord = {
    {0, 1, 2, 4, 5, 7}, {3, 3, 3, 2, 3, 1}, {1, 0, 0, 0, 0, 0, 1, 0}, 40};
const TNIStructure t12_hexachord = {
    {0, 1, 2, 4, 6, 7}, {3, 3, 2, 2, 3, 2}, {1, 0, 0, 1, 0, 0, 0, 0}, 41};
const TNIStructure t13_hexachord = {
    {0, 1, 3, 4, 6, 7}, {3, 2, 4, 2, 2, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 42};
const TNIStructure t14_hexachord = {
    {0, 1, 3, 4, 5, 8}, {3, 2, 3, 4, 3, 0}, {1, 0, 1, 0, 1, 0, 1, 0}, 0};
const TNIStructure t15_hexachord = {
    {0, 1, 2, 4, 5, 8}, {3, 2, 3, 4, 2, 1}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t16_hexachord = {
    {0, 1, 4, 5, 6, 8}, {3, 2, 2, 4, 3, 1}, {1, 0, 1, 0, 0, 1, 0, 1}, 0};
const TNIStructure t17_hexachord = {
    {0, 1, 2, 4, 7, 8}, {3, 2, 2, 3, 3, 2}, {1, 0, 0, 1, 0, 0, 0, 0}, 43};
const TNIStructure t18_hexachord = {
    {0, 1, 2, 5, 7, 8}, {3, 2, 2, 2, 4, 2}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t19_hexachord = {
    {0, 1, 3, 4, 7, 8}, {3, 1, 3, 4, 3, 1}, {1, 0, 0, 0, 0, 0, 1, 0}, 44};
const TNIStructure t20_hexachord = {
    {0, 1, 4, 5, 8, 9}, {3, 0, 3, 6, 3, 0}, {3, 3, 3, 3, 3, 3, 3, 3}, 0};
const TNIStructure t21_hexachord = {
    {0, 2, 3, 4, 6, 8}, {2, 4, 2, 4, 1, 2}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t22_hexachord = {
    {0, 1, 2, 4, 6, 8}, {2, 4, 1, 4, 2, 2}, {1, 0, 1, 0, 0, 1, 0, 1}, 0};
const TNIStructure t23_hexachord = {
    {0, 2, 3, 5, 6, 8}, {2, 3, 4, 2, 2, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 45};
const TNIStructure t24_hexachord = {
    {0, 1, 3, 4, 6, 8}, {2, 3, 3, 3, 3, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 46};
const TNIStructure t25_hexachord = {
    {0, 1, 3, 5, 6, 8}, {2, 3, 3, 2, 4, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 47};
const TNIStructure t26_hexachord = {
    {0, 1, 3, 5, 7, 8}, {2, 3, 2, 3, 4, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 48};
const TNIStructure t27_hexachord = {
    {0, 1, 3, 4, 6, 9}, {2, 2, 5, 2, 2, 2}, {1, 0, 0, 1, 0, 1, 1, 0}, 0};
const TNIStructure t28_hexachord = {
    {0, 1, 3, 5, 6, 9}, {2, 2, 4, 3, 2, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 49};
const TNIStructure t29_hexachord = {
    {0, 1, 3, 6, 8, 9}, {2, 2, 4, 2, 3, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 50};
const TNIStructure t30_hexachord = {
    {0, 1, 3, 6, 7, 9}, {2, 2, 4, 2, 2, 3}, {2, 0, 0, 2, 0, 2, 2, 0}, 0};
const TNIStructure t31_hexachord = {
    {0, 1, 3, 5, 8, 9}, {2, 2, 3, 4, 3, 1}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t32_hexachord = {
    {0, 2, 4, 5, 7, 9}, {1, 4, 3, 2, 5, 0}, {1, 1, 0, 0, 1, 1, 0, 0}, 0};
const TNIStructure t33_hexachord = {
    {0, 2, 3, 5, 7, 9}, {1, 4, 3, 2, 4, 1}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t34_hexachord = {
    {0, 1, 3, 5, 7, 9}, {1, 4, 2, 4, 2, 2}, {1, 0, 0, 0, 0, 1, 0, 0}, 0};
const TNIStructure t35_hexachord = {
    {0, 2, 4, 6, 8, 10}, {0, 6, 0, 6, 0, 3}, {6, 6, 6, 6, 6, 6, 6, 6}, 0};
const TNIStructure t36_hexachord = {
    {0, 1, 2, 3, 4, 7}, {4, 3, 3, 2, 2, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 3};
const TNIStructure t37_hexachord = {
    {0, 1, 2, 3, 4, 8}, {4, 3, 2, 3, 2, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 4};
const TNIStructure t38_hexachord = {
    {0, 1, 2, 3, 7, 8}, {4, 2, 1, 2, 4, 2}, {1, 1, 0, 0, 0, 0, 1, 1}, 6};
const TNIStructure t39_hexachord = {
    {0, 2, 3, 4, 5, 8}, {3, 3, 3, 3, 2, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 10};
const TNIStructure t40_hexachord = {
    {0, 1, 2, 3, 5, 8}, {3, 3, 3, 2, 3, 1}, {1, 0, 0, 0, 0, 0, 1, 0}, 11};
const TNIStructure t41_hexachord = {
    {0, 1, 2, 3, 6, 8}, {3, 3, 2, 2, 3, 2}, {1, 0, 0, 1, 0, 0, 0, 0}, 12};
const TNIStructure t42_hexachord = {
    {0, 1, 2, 3, 6, 9}, {3, 2, 4, 2, 2, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 13};
const TNIStructure t43_hexachord = {
    {0, 1, 2, 5, 6, 8}, {3, 2, 2, 3, 3, 2}, {1, 0, 0, 1, 0, 0, 0, 0}, 17};
const TNIStructure t44_hexachord = {
    {0, 1, 2, 5, 6, 9}, {3, 1, 3, 4, 3, 1}, {1, 0, 0, 0, 0, 0, 1, 0}, 19};
const TNIStructure t45_hexachord = {
    {0, 2, 3, 4, 6, 9}, {2, 3, 4, 2, 2, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 23};
const TNIStructure t46_hexachord = {
    {0, 1, 2, 4, 6, 9}, {2, 3, 3, 3, 3, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 24};
const TNIStructure t47_hexachord = {
    {0, 1, 2, 4, 7, 9}, {2, 3, 3, 2, 4, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 25};
const TNIStructure t48_hexachord = {
    {0, 1, 2, 5, 7, 9}, {2, 3, 2, 3, 4, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 26};
const TNIStructure t49_hexachord = {
    {0, 1, 3, 4, 7, 9}, {2, 2, 4, 3, 2, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 28};
const TNIStructure t50_hexachord = {
    {0, 1, 4, 6, 7, 9}, {2, 2, 4, 2, 3, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 29};
const std::vector<const TNIStructure *> hexachord = {
    nullptr,        &t1_hexachord,  &t2_hexachord,  &t3_hexachord,
    &t4_hexachord,  &t5_hexachord,  &t6_hexachord,  &t7_hexachord,
    &t8_hexachord,  &t9_hexachord,  &t10_hexachord, &t11_hexachord,
    &t12_hexachord, &t13_hexachord, &t14_hexachord, &t15_hexachord,
    &t16_hexachord, &t17_hexachord, &t18_hexachord, &t19_hexachord,
    &t20_hexachord, &t21_hexachord, &t22_hexachord, &t23_hexachord,
    &t24_hexachord, &t25_hexachord, &t26_hexachord, &t27_hexachord,
    &t28_hexachord, &t29_hexachord, &t30_hexachord, &t31_hexachord,
    &t32_hexachord, &t33_hexachord, &t34_hexachord, &t35_hexachord,
    &t36_hexachord, &t37_hexachord, &t38_hexachord, &t39_hexachord,
    &t40_hexachord, &t41_hexachord, &t42_hexachord, &t43_hexachord,
    &t44_hexachord, &t45_hexachord, &t46_hexachord, &t47_hexachord,
    &t48_hexachord, &t49_hexachord, &t50_hexachord,
};

const TNIStructure t1_septachord = {
    {0, 1, 2, 3, 4, 5, 6}, {6, 5, 4, 3, 2, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t2_septachord = {
    {0, 1, 2, 3, 4, 5, 7}, {5, 5, 4, 3, 3, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t3_septachord = {
    {0, 1, 2, 3, 4, 5, 8}, {5, 4, 4, 4, 3, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t4_septachord = {
    {0, 1, 2, 3, 4, 6, 7}, {5, 4, 4, 3, 3, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t5_septachord = {
    {0, 1, 2, 3, 5, 6, 7}, {5, 4, 3, 3, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t6_septachord = {
    {0, 1, 2, 3, 4, 7, 8}, {5, 3, 3, 4, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t7_septachord = {
    {0, 1, 2, 3, 6, 7, 8}, {5, 3, 2, 3, 5, 3}, {1, 0, 0, 1, 0, 0, 0, 0}, 0};
const TNIStructure t8_septachord = {
    {0, 2, 3, 4, 5, 6, 8}, {4, 5, 4, 4, 2, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t9_septachord = {
    {0, 1, 2, 3, 4, 6, 8}, {4, 5, 3, 4, 3, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t10_septachord = {
    {0, 1, 2, 3, 4, 6, 9}, {4, 4, 5, 3, 3, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t11_septachord = {
    {0, 1, 3, 4, 5, 6, 8}, {4, 4, 4, 4, 4, 1}, {1, 0, 1, 0, 0, 0, 0, 0}, 0};
const TNIStructure t12_septachord = {
    {0, 1, 2, 3, 4, 7, 9}, {4, 4, 4, 3, 4, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 36};
const TNIStructure t13_septachord = {
    {0, 1, 2, 4, 5, 6, 8}, {4, 4, 3, 5, 3, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t14_septachord = {
    {0, 1, 2, 3, 5, 7, 8}, {4, 4, 3, 3, 5, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t15_septachord = {
    {0, 1, 2, 4, 6, 7, 8}, {4, 4, 2, 4, 4, 3}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t16_septachord = {
    {0, 1, 2, 3, 5, 6, 9}, {4, 3, 5, 4, 3, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t17_septachord = {
    {0, 1, 2, 4, 5, 6, 9}, {4, 3, 4, 5, 4, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 37};
const TNIStructure t18_septachord = {
    {0, 1, 2, 3, 5, 8, 9}, {4, 3, 4, 4, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 38};
const TNIStructure t19_septachord = {
    {0, 1, 2, 3, 6, 7, 9}, {4, 3, 4, 3, 4, 3}, {1, 0, 0, 1, 0, 0, 0, 0}, 0};
const TNIStructure t20_septachord = {
    {0, 1, 2, 4, 7, 8, 9}, {4, 3, 3, 4, 5, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t21_septachord = {
    {0, 1, 2, 4, 5, 8, 9}, {4, 2, 4, 6, 4, 1}, {1, 0, 1, 0, 0, 0, 0, 0}, 0};
const TNIStructure t22_septachord = {
    {0, 1, 2, 5, 6, 8, 9}, {4, 2, 4, 5, 4, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t23_septachord = {
    {0, 2, 3, 4, 5, 7, 9}, {3, 5, 4, 3, 5, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t24_septachord = {
    {0, 1, 2, 3, 5, 7, 9}, {3, 5, 3, 4, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t25_septachord = {
    {0, 2, 3, 4, 6, 7, 9}, {3, 4, 5, 3, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t26_septachord = {
    {0, 1, 3, 4, 5, 7, 9}, {3, 4, 4, 5, 3, 2}, {1, 0, 1, 0, 0, 0, 0, 0}, 0};
const TNIStructure t27_septachord = {
    {0, 1, 2, 4, 5, 7, 9}, {3, 4, 4, 4, 5, 1}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t28_septachord = {
    {0, 1, 3, 5, 6, 7, 9}, {3, 4, 4, 4, 3, 3}, {1, 0, 0, 1, 0, 0, 0, 0}, 0};
const TNIStructure t29_septachord = {
    {0, 1, 2, 4, 6, 7, 9}, {3, 4, 4, 3, 5, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t30_septachord = {
    {0, 1, 2, 4, 6, 8, 9}, {3, 4, 3, 5, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t31_septachord = {
    {0, 1, 3, 4, 6, 7, 9}, {3, 3, 6, 3, 3, 3}, {1, 0, 0, 1, 0, 0, 0, 0}, 0};
const TNIStructure t32_septachord = {
    {0, 1, 3, 4, 6, 8, 9}, {3, 3, 5, 4, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t33_septachord = {
    {0, 1, 2, 4, 6, 8, 10}, {2, 6, 2, 6, 2, 3}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t34_septachord = {
    {0, 1, 3, 4, 6, 8, 10}, {2, 5, 4, 4, 4, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t35_septachord = {
    {0, 1, 3, 5, 6, 8, 10}, {2, 5, 4, 3, 6, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t36_septachord = {
    {0, 1, 2, 3, 5, 6, 8}, {4, 4, 4, 3, 4, 2}, {1, 0, 0, 1, 0, 0, 0, 0}, 12};
const TNIStructure t37_septachord = {
    {0, 1, 3, 4, 5, 7, 8}, {4, 3, 4, 5, 4, 1}, {1, 1, 0, 0, 0, 0, 0, 0}, 17};
const TNIStructure t38_septachord = {
    {0, 1, 2, 4, 5, 7, 8}, {4, 3, 4, 4, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 18};
const std::vector<const TNIStructure *> septachord = {
    nullptr,         &t1_septachord,  &t2_septachord,  &t3_septachord,
    &t4_septachord,  &t5_septachord,  &t6_septachord,  &t7_septachord,
    &t8_septachord,  &t9_septachord,  &t10_septachord, &t11_septachord,
    &t12_septachord, &t13_septachord, &t14_septachord, &t15_septachord,
    &t16_septachord, &t17_septachord, &t18_septachord, &t19_septachord,
    &t20_septachord, &t21_septachord, &t22_septachord, &t23_septachord,
    &t24_septachord, &t25_septachord, &t26_septachord, &t27_septachord,
    &t28_septachord, &t29_septachord, &t30_septachord, &t31_septachord,
    &t32_septachord, &t33_septachord, &t34_septachord, &t35_septachord,
    &t36_septachord, &t37_septachord, &t38_septachord};

const TNIStructure t1_octachord = {
    {0, 1, 2, 3, 4, 5, 6, 7}, {7, 6, 5, 4, 4, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t2_octachord = {
    {0, 1, 2, 3, 4, 5, 6, 8}, {6, 6, 5, 5, 4, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t3_octachord = {
    {0, 1, 2, 3, 4, 5, 6, 9}, {6, 5, 6, 5, 4, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t4_octachord = {
    {0, 1, 2, 3, 4, 5, 7, 8}, {6, 5, 5, 5, 5, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t5_octachord = {
    {0, 1, 2, 3, 4, 6, 7, 8}, {6, 5, 4, 5, 5, 3}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t6_octachord = {
    {0, 1, 2, 3, 5, 6, 7, 8}, {6, 5, 4, 4, 6, 3}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t7_octachord = {
    {0, 1, 2, 3, 4, 5, 8, 9}, {6, 4, 5, 6, 5, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t8_octachord = {
    {0, 1, 2, 3, 4, 7, 8, 9}, {6, 4, 4, 5, 6, 3}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t9_octachord = {
    {0, 1, 2, 3, 6, 7, 8, 9}, {6, 4, 4, 4, 6, 4}, {2, 2, 2, 2, 0, 0, 0, 0}, 0};
const TNIStructure t10_octachord = {
    {0, 2, 3, 4, 5, 6, 7, 9}, {5, 6, 6, 4, 5, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t11_octachord = {
    {0, 1, 2, 3, 4, 5, 7, 9}, {5, 6, 5, 5, 5, 2}, {1, 0, 1, 0, 0, 0, 0, 0}, 0};
const TNIStructure t12_octachord = {
    {0, 1, 3, 4, 5, 6, 7, 9}, {5, 5, 6, 5, 4, 3}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t13_octachord = {
    {0, 1, 2, 3, 4, 6, 7, 9}, {5, 5, 6, 4, 5, 3}, {1, 0, 0, 1, 0, 0, 0, 0}, 0};
const TNIStructure t14_octachord = {
    {0, 1, 2, 4, 5, 6, 7, 9}, {5, 5, 5, 5, 6, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t15_octachord = {
    {0, 1, 2, 3, 4, 6, 8, 9}, {5, 5, 5, 5, 5, 3}, {1, 0, 0, 0, 0, 0, 0, 0}, 29};
const TNIStructure t16_octachord = {
    {0, 1, 2, 3, 5, 7, 8, 9}, {5, 5, 4, 5, 6, 3}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t17_octachord = {
    {0, 1, 3, 4, 5, 6, 8, 9}, {5, 4, 6, 6, 5, 2}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t18_octachord = {
    {0, 1, 2, 3, 5, 6, 8, 9}, {5, 4, 6, 5, 5, 3}, {1, 0, 0, 1, 0, 0, 0, 0}, 0};
const TNIStructure t19_octachord = {
    {0, 1, 2, 4, 5, 6, 8, 9}, {5, 4, 5, 7, 5, 2}, {1, 0, 1, 0, 0, 0, 0, 0}, 0};
const TNIStructure t20_octachord = {
    {0, 1, 2, 4, 5, 7, 8, 9}, {5, 4, 5, 6, 6, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t21_octachord = {
    {0, 1, 2, 3, 4, 6, 8, 10}, {4, 7, 4, 6, 4, 3}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t22_octachord = {
    {0, 1, 2, 3, 5, 6, 8, 10}, {4, 6, 5, 5, 6, 2}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t23_octachord = {
    {0, 1, 2, 3, 5, 7, 8, 10}, {4, 6, 5, 4, 7, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t24_octachord = {
    {0, 1, 2, 4, 5, 6, 8, 10}, {4, 6, 4, 7, 4, 3}, {1, 1, 1, 1, 0, 0, 0, 0}, 0};
const TNIStructure t25_octachord = {
    {0, 1, 2, 4, 6, 7, 8, 10}, {4, 6, 4, 6, 4, 4}, {2, 2, 2, 2, 0, 0, 0, 0}, 0};
const TNIStructure t26_octachord = {
    {0, 1, 2, 4, 5, 7, 9, 10}, {4, 5, 6, 5, 6, 2}, {1, 1, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t27_octachord = {
    {0, 1, 2, 4, 5, 7, 8, 10}, {4, 5, 6, 5, 5, 3}, {1, 0, 0, 0, 0, 0, 0, 0}, 0};
const TNIStructure t28_octachord = {
    {0, 1, 3, 4, 6, 7, 9, 10}, {4, 4, 8, 4, 4, 4}, {4, 4, 4, 4, 0, 0, 0, 0}, 0};
const TNIStructure t29_octachord = {
    {0, 1, 2, 3, 5, 6, 7, 9}, {5, 5, 5, 5, 5, 3}, {1, 0, 0, 0, 0, 0, 0, 0}, 15};
const std::vector<const TNIStructure *> octachord = {
    nullptr,        &t1_octachord,  &t2_octachord,  &t3_octachord,
    &t4_octachord,  &t5_octachord,  &t6_octachord,  &t7_octachord,
    &t8_octachord,  &t9_octachord,  &t10_octachord, &t11_octachord,
    &t12_octachord, &t13_octachord, &t14_octachord, &t15_octachord,
    &t16_octachord, &t17_octachord, &t18_octachord, &t19_octachord,
    &t20_octachord, &t21_octachord, &t22_octachord, &t23_octachord,
    &t24_octachord, &t25_octachord, &t26_octachord, &t27_octachord,
    &t28_octachord, &t29_octachord};

const TNIStructure t1_nonachord = {{0, 1, 2, 3, 4, 5, 6, 7, 8},
                                   {8, 7, 6, 6, 6, 3},
                                   {1, 1, 0, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t2_nonachord = {{0, 1, 2, 3, 4, 5, 6, 7, 9},
                                   {7, 7, 7, 6, 6, 3},
                                   {1, 0, 0, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t3_nonachord = {{0, 1, 2, 3, 4, 5, 6, 8, 9},
                                   {7, 6, 7, 7, 6, 3},
                                   {1, 0, 0, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t4_nonachord = {{0, 1, 2, 3, 4, 5, 7, 8, 9},
                                   {7, 6, 6, 7, 7, 3},
                                   {1, 0, 1, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t5_nonachord = {{0, 1, 2, 3, 4, 6, 7, 8, 9},
                                   {7, 6, 6, 6, 7, 4},
                                   {1, 0, 0, 1, 0, 0, 0, 0},
                                   0};
const TNIStructure t6_nonachord = {{0, 1, 2, 3, 4, 5, 6, 8, 10},
                                   {6, 8, 6, 7, 6, 3},
                                   {1, 1, 1, 1, 0, 0, 0, 0},
                                   0};
const TNIStructure t7_nonachord = {{0, 1, 2, 3, 4, 5, 7, 8, 10},
                                   {6, 7, 7, 6, 7, 3},
                                   {1, 0, 0, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t8_nonachord = {{0, 1, 2, 3, 4, 6, 7, 8, 10},
                                   {6, 7, 6, 7, 6, 4},
                                   {1, 0, 0, 1, 0, 0, 0, 0},
                                   0};
const TNIStructure t9_nonachord = {{0, 1, 2, 3, 5, 6, 7, 8, 10},
                                   {6, 7, 6, 6, 8, 3},
                                   {1, 1, 0, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t10_nonachord = {{0, 1, 2, 3, 4, 6, 7, 9, 10},
                                    {6, 6, 8, 6, 6, 4},
                                    {1, 1, 1, 1, 0, 0, 0, 0},
                                    0};
const TNIStructure t11_nonachord = {{0, 1, 2, 3, 5, 6, 7, 9, 10},
                                    {6, 6, 7, 7, 7, 3},
                                    {1, 0, 0, 0, 0, 0, 0, 0},
                                    0};
const TNIStructure t12_nonachord = {{0, 1, 2, 4, 5, 6, 8, 9, 10},
                                    {6, 6, 6, 9, 6, 3},
                                    {3, 3, 3, 3, 0, 0, 0, 0},
                                    0};
const std::vector<const TNIStructure *> nonachord = {
    nullptr,        &t1_nonachord,  &t2_nonachord, &t3_nonachord, &t4_nonachord,
    &t5_nonachord,  &t6_nonachord,  &t7_nonachord, &t8_nonachord, &t9_nonachord,
    &t10_nonachord, &t11_nonachord, &t12_nonachord};

const TNIStructure t1_decachord = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                                   {9, 8, 8, 8, 8, 4},
                                   {1, 1, 0, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t2_decachord = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 10},
                                   {8, 9, 8, 8, 8, 4},
                                   {1, 1, 1, 1, 0, 0, 0, 0},
                                   0};
const TNIStructure t3_decachord = {{0, 1, 2, 3, 4, 5, 6, 7, 9, 10},
                                   {8, 8, 9, 8, 8, 4},
                                   {1, 1, 1, 1, 0, 0, 0, 0},
                                   0};
const TNIStructure t4_decachord = {{0, 1, 2, 3, 4, 5, 6, 8, 9, 10},
                                   {8, 8, 8, 9, 8, 4},
                                   {1, 1, 1, 1, 0, 0, 0, 0},
                                   0};
const TNIStructure t5_decachord = {{0, 1, 2, 3, 4, 5, 7, 8, 9, 10},
                                   {8, 8, 8, 8, 9, 4},
                                   {1, 1, 0, 0, 0, 0, 0, 0},
                                   0};
const TNIStructure t6_decachord = {{0, 1, 2, 3, 4, 6, 7, 8, 9, 10},
                                   {8, 8, 8, 8, 8, 5},
                                   {2, 2, 2, 2, 0, 0, 0, 0},
                                   0};
const std::vector<const TNIStructure *> decachord = {
    nullptr,       &t1_decachord, &t2_decachord, &t3_decachord,
    &t4_decachord, &t5_decachord, &t6_decachord};

const TNIStructure t1_undecachord = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                                     {10, 10, 10, 10, 10, 5},
                                     {1, 1, 1, 1, 0, 0, 0, 0},
                                     0};
const std::vector<const TNIStructure *> undecachord = {nullptr,
                                                       &t1_undecachord};

const TNIStructure t1_dodecachord = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
                                     {12, 12, 12, 12, 12, 6},
                                     {12, 12, 12, 12, 0, 0, 0, 0},
                                     0};
const std::vector<const TNIStructure *> dodecachord = {nullptr,
                                                       &t1_dodecachord};

// This dictionary defines the pitch classes to return for the inversion of a
// given forte number.For instance(3, 11) : (0, 4, 7)indicates that for the
// inverted form of Forte class 3 - 11(minor / major triad) return 0, 2, 3
// (the zero could be assumed, but it makes my brain easier to have it there).
// It is faster to store this than to recompute it every time.
std::map<std::pair<int, int>, std::vector<int>> inversionDefaultPitchClasses = {
    {{3, 2}, {0, 2, 3}},
    {{3, 3}, {0, 3, 4}},
    {{3, 4}, {0, 4, 5}},
    {{3, 5}, {0, 5, 6}},
    {{3, 7}, {0, 3, 5}},
    {{3, 8}, {0, 4, 6}},
    {{3, 11}, {0, 4, 7}},
    {{4, 2}, {0, 2, 3, 4}},
    {{4, 4}, {0, 3, 4, 5}},
    {{4, 5}, {0, 4, 5, 6}},
    {{4, 11}, {0, 2, 4, 5}},
    {{4, 12}, {0, 3, 4, 6}},
    {{4, 13}, {0, 3, 5, 6}},
    {{4, 14}, {0, 4, 5, 7}},
    {{4, 15}, {0, 2, 5, 6}},
    {{4, 16}, {0, 2, 6, 7}},
    {{4, 18}, {0, 3, 6, 7}},
    {{4, 19}, {0, 4, 7, 8}},
    {{4, 22}, {0, 3, 5, 7}},
    {{4, 27}, {0, 3, 6, 8}},
    {{4, 29}, {0, 4, 6, 7}},
    {{5, 2}, {0, 2, 3, 4, 5}},
    {{5, 3}, {0, 1, 3, 4, 5}},
    {{5, 4}, {0, 3, 4, 5, 6}},
    {{5, 5}, {0, 4, 5, 6, 7}},
    {{5, 6}, {0, 1, 4, 5, 6}},
    {{5, 7}, {0, 1, 5, 6, 7}},
    {{5, 9}, {0, 2, 4, 5, 6}},
    {{5, 10}, {0, 2, 3, 5, 6}},
    {{5, 11}, {0, 3, 4, 5, 7}},
    {{5, 13}, {0, 4, 6, 7, 8}},
    {{5, 14}, {0, 2, 5, 6, 7}},
    {{5, 16}, {0, 3, 4, 6, 7}},
    {{5, 18}, {0, 2, 3, 6, 7}},
    {{5, 19}, {0, 1, 4, 6, 7}},
    {{5, 20}, {0, 1, 5, 7, 8}},
    {{5, 21}, {0, 3, 4, 7, 8}},
    {{5, 23}, {0, 2, 4, 5, 7}},
    {{5, 24}, {0, 2, 4, 6, 7}},
    {{5, 25}, {0, 3, 5, 6, 8}},
    {{5, 26}, {0, 3, 4, 6, 8}},
    {{5, 27}, {0, 3, 5, 7, 8}},
    {{5, 28}, {0, 2, 5, 6, 8}},
    {{5, 29}, {0, 2, 5, 7, 8}},
    {{5, 30}, {0, 2, 4, 7, 8}},
    {{5, 31}, {0, 3, 6, 8, 9}},
    {{5, 32}, {0, 3, 5, 8, 9}},
    {{5, 36}, {0, 3, 5, 6, 7}},
    {{5, 38}, {0, 3, 6, 7, 8}},
    {{6, 2}, {0, 2, 3, 4, 5, 6}},
    {{6, 3}, {0, 1, 3, 4, 5, 6}},
    {{6, 5}, {0, 1, 4, 5, 6, 7}},
    {{6, 9}, {0, 2, 4, 5, 6, 7}},
    {{6, 10}, {0, 2, 3, 4, 6, 7}},
    {{6, 11}, {0, 2, 3, 5, 6, 7}},
    {{6, 12}, {0, 1, 3, 5, 6, 7}},
    {{6, 14}, {0, 3, 4, 5, 7, 8}},
    {{6, 15}, {0, 3, 4, 6, 7, 8}},
    {{6, 16}, {0, 2, 3, 4, 7, 8}},
    {{6, 17}, {0, 1, 4, 6, 7, 8}},
    {{6, 18}, {0, 1, 3, 6, 7, 8}},
    {{6, 19}, {0, 1, 4, 5, 7, 8}},
    {{6, 21}, {0, 2, 4, 5, 6, 8}},
    {{6, 22}, {0, 2, 4, 6, 7, 8}},
    {{6, 24}, {0, 2, 4, 5, 7, 8}},
    {{6, 25}, {0, 2, 3, 5, 7, 8}},
    {{6, 27}, {0, 3, 5, 6, 8, 9}},
    {{6, 30}, {0, 2, 3, 6, 8, 9}},
    {{6, 31}, {0, 1, 4, 6, 8, 9}},
    {{6, 33}, {0, 2, 4, 6, 7, 9}},
    {{6, 34}, {0, 2, 4, 6, 8, 9}},
    {{6, 36}, {0, 3, 4, 5, 6, 7}},
    {{6, 39}, {0, 3, 4, 5, 6, 8}},
    {{6, 40}, {0, 3, 5, 6, 7, 8}},
    {{6, 41}, {0, 2, 5, 6, 7, 8}},
    {{6, 43}, {0, 2, 3, 6, 7, 8}},
    {{6, 44}, {0, 3, 4, 7, 8, 9}},
    {{6, 46}, {0, 3, 5, 7, 8, 9}},
    {{6, 47}, {0, 2, 5, 7, 8, 9}},
    {{7, 2}, {0, 2, 3, 4, 5, 6, 7}},
    {{7, 3}, {0, 3, 4, 5, 6, 7, 8}},
    {{7, 4}, {0, 1, 3, 4, 5, 6, 7}},
    {{7, 5}, {0, 1, 2, 4, 5, 6, 7}},
    {{7, 6}, {0, 1, 4, 5, 6, 7, 8}},
    {{7, 7}, {0, 1, 2, 5, 6, 7, 8}},
    {{7, 9}, {0, 2, 4, 5, 6, 7, 8}},
    {{7, 10}, {0, 3, 5, 6, 7, 8, 9}},
    {{7, 11}, {0, 2, 3, 4, 5, 7, 8}},
    {{7, 13}, {0, 2, 3, 4, 6, 7, 8}},
    {{7, 14}, {0, 1, 3, 5, 6, 7, 8}},
    {{7, 16}, {0, 3, 4, 6, 7, 8, 9}},
    {{7, 18}, {0, 1, 4, 6, 7, 8, 9}},
    {{7, 19}, {0, 2, 3, 6, 7, 8, 9}},
    {{7, 20}, {0, 1, 2, 5, 7, 8, 9}},
    {{7, 21}, {0, 1, 4, 5, 7, 8, 9}},
    {{7, 23}, {0, 2, 4, 5, 6, 7, 9}},
    {{7, 24}, {0, 2, 4, 6, 7, 8, 9}},
    {{7, 25}, {0, 2, 3, 5, 6, 7, 9}},
    {{7, 26}, {0, 2, 4, 5, 6, 8, 9}},
    {{7, 27}, {0, 2, 4, 5, 7, 8, 9}},
    {{7, 28}, {0, 2, 3, 4, 6, 8, 9}},
    {{7, 29}, {0, 2, 3, 5, 7, 8, 9}},
    {{7, 30}, {0, 1, 3, 5, 7, 8, 9}},
    {{7, 31}, {0, 2, 3, 5, 6, 8, 9}},
    {{7, 32}, {0, 1, 3, 5, 6, 8, 9}},
    {{7, 36}, {0, 2, 3, 5, 6, 7, 8}},
    {{7, 38}, {0, 1, 3, 4, 6, 7, 8}},
    {{8, 2}, {0, 2, 3, 4, 5, 6, 7, 8}},
    {{8, 4}, {0, 1, 3, 4, 5, 6, 7, 8}},
    {{8, 5}, {0, 1, 2, 4, 5, 6, 7, 8}},
    {{8, 11}, {0, 2, 4, 5, 6, 7, 8, 9}},
    {{8, 12}, {0, 2, 3, 4, 5, 6, 8, 9}},
    {{8, 13}, {0, 2, 3, 5, 6, 7, 8, 9}},
    {{8, 14}, {0, 2, 3, 4, 5, 7, 8, 9}},
    {{8, 15}, {0, 1, 3, 5, 6, 7, 8, 9}},
    {{8, 16}, {0, 1, 2, 4, 6, 7, 8, 9}},
    {{8, 18}, {0, 1, 3, 4, 6, 7, 8, 9}},
    {{8, 19}, {0, 1, 3, 4, 5, 7, 8, 9}},
    {{8, 22}, {0, 2, 4, 5, 7, 8, 9, 10}},
    {{8, 27}, {0, 2, 3, 5, 6, 8, 9, 10}},
    {{8, 29}, {0, 2, 3, 4, 6, 7, 8, 9}},
    {{9, 2}, {0, 2, 3, 4, 5, 6, 7, 8, 9}},
    {{9, 3}, {0, 1, 3, 4, 5, 6, 7, 8, 9}},
    {{9, 4}, {0, 1, 2, 4, 5, 6, 7, 8, 9}},
    {{9, 5}, {0, 1, 2, 3, 5, 6, 7, 8, 9}},
    {{9, 7}, {0, 2, 3, 5, 6, 7, 8, 9, 10}},
    {{9, 8}, {0, 2, 3, 4, 6, 7, 8, 9, 10}},
    {{9, 11}, {0, 1, 3, 4, 5, 7, 8, 9, 10}},
};

const std::vector<const std::vector<const TNIStructure *> *> FORTE = {
    nullptr,
    &monad, // monad placeholder
    &diad,  // diad placeholder
    &trichord,    &tetrachord, &pentachord, &hexachord,
    &septachord,  &octachord,  &nonachord,
    &decachord,   // decachord placeholder
    &undecachord, // undecachord placeholder
    &dodecachord, // dodecachord placeholder
};

std::map<int, int> maximumIndexNumberWithoutInversionEquivalence = {
    {0, 1},  {1, 1},  {2, 6},  {3, 19}, {4, 43}, {5, 66}, {6, 80},
    {7, 66}, {8, 43}, {9, 19}, {10, 6}, {11, 1}, {12, 1},
};

std::map<std::tuple<int, int, int>,
         std::map<std::string, std::vector<std::string>>>
    tnIndexToChordInfo = {
        // Cardinality 1
        {{1, 1, 0}, {{"name", {"unison", "monad", "singleton"}}}},
        // Cardinality 2
        {{2, 1, 0},
         {{"name",
           {"interval class 1", "minor second", "m2", "half step",
            "semitone"}}}},
        {{2, 2, 0},
         {{"name",
           {"interval class 2", "major second", "M2", "whole step",
            "whole tone"}}}},
        {{2, 3, 0}, {{"name", {"interval class 3", "minor third", "m3"}}}},
        {{2, 4, 0}, {{"name", {"interval class 4", "major third", "M3"}}}},
        {{2, 5, 0}, {{"name", {"interval class 5", "perfect fourth", "P4"}}}},
        {{2, 6, 0},
         {{"name", {"tritone", "diminished fifth", "augmented fourth"}}}},
        // cardnality 3
        {{3, 1, 0}, {{"name", {"chromatic trimirror"}}}},
        {{3, 2, 1}, {{"name", {"phrygian trichord"}}}},
        {{3, 2, -1}, {{"name", {"minor trichord"}}}},
        {{3, 3, 1}, {{"name", {"major-minor trichord"}}}},
        {{3, 3, -1}, {{"name", {"major-minor trichord"}}}},
        {{3, 4, 1}, {{"name", {"incomplete major-seventh chord"}}}},
        {{3, 4, -1}, {{"name", {"incomplete major-seventh chord"}}}},
        {{3, 5, 1}, {{"name", {"tritone-fourth"}}}},
        {{3, 5, -1}, {{"name", {"tritone-fourth"}}}},
        {{3, 6, 0}, {{"name", {"whole-tone trichord"}}}},
        {{3, 7, 1}, {{"name", {"incomplete minor-seventh chord"}}}},
        {{3, 7, -1}, {{"name", {"incomplete dominant-seventh chord"}}}},
        {{3, 8, 1},
         {{"name",
           {"incomplete dominant-seventh chord",
            "Italian augmented sixth chord"}}}},
        {{3, 8, -1}, {{"name", {"incomplete half-diminished seventh chord"}}}},
        {{3, 9, 0}, {{"name", {"quartal trichord"}}}},
        {{3, 10, 0}, {{"name", {"diminished triad"}}}},
        {{3, 11, 1}, {{"name", {"minor triad"}}}},
        {{3, 11, -1}, {{"name", {"major triad"}}}},
        {{3, 12, 0},
         {{"name", {"augmented triad", "equal 3-part octave division"}}}},
        // Cardinality 4
        {{4, 1, 0}, {{"name", {"chromatic tetramirror", "BACH"}}}},
        {{4, 2, 1}, {{"name", {"major-second tetracluster"}}}},
        {{4, 2, -1}, {{"name", {"major-second tetracluster"}}}},
        {{4, 3, 0}, {{"name", {"alternating tetramirror"}}}},
        {{4, 4, 1}, {{"name", {"minor third tetracluster"}}}},
        {{4, 4, -1}, {{"name", {"minor third tetracluster"}}}},
        {{4, 5, 1}, {{"name", {"major third tetracluster"}}}},
        {{4, 5, -1}, {{"name", {"major third tetracluster"}}}},
        {{4, 6, 0}, {{"name", {"perfect fourth tetramirror"}}}},
        {{4, 7, 0}, {{"name", {"Arabian tetramirror"}}}},
        {{4, 8, 0}, {{"name", {"double-fourth tetramirror"}}}},
        {{4, 9, 0}, {{"name", {"double tritone tetramirror"}}}},
        {{4, 10, 0}, {{"name", {"minor tetramirror"}}}},
        {{4, 11, 1}, {{"name", {"phrygian tetrachord"}}}},
        {{4, 11, -1}, {{"name", {"lydian tetrachord", "major tetrachord"}}}},
        {{4, 12, 1}, {{"name", {"harmonic minor tetrachord"}}}},
        {{4, 12, -1}, {{"name", {"major-third diminished tetrachord"}}}},
        {{4, 13, 1}, {{"name", {"minor-second diminished tetrachord"}}}},
        {{4, 13, -1}, {{"name", {"perfect-fourth diminished tetrachord"}}}},
        {{4, 14, 1}, {{"name", {"major-second minor tetrachord"}}}},
        {{4, 14, -1}, {{"name", {"perfect-fourth major tetrachord"}}}},
        {{4, 15, 1}, {{"name", {"all-interval tetrachord"}}}},
        {{4, 15, -1}, {{"name", {"all-interval tetrachord"}}}},
        {{4, 16, 1}, {{"name", {"minor-second quartal tetrachord"}}}},
        {{4, 16, -1}, {{"name", {"tritone quartal tetrachord"}}}},
        {{4, 17, 0}, {{"name", {"major-minor tetramirror"}}}},
        {{4, 18, 1}, {{"name", {"major-diminished tetrachord"}}}},
        {{4, 18, -1}, {{"name", {"minor-diminished tetrachord"}}}},
        {{4, 19, 1}, {{"name", {"minor-augmented tetrachord"}}}},
        {{4, 19, -1}, {{"name", {"augmented major tetrachord"}}}},
        {{4, 20, 0}, {{"name", {"major seventh chord"}}}},
        {{4, 21, 0}, {{"name", {"whole-tone tetramirror"}}}},
        {{4, 22, 1}, {{"name", {"major-second major tetrachord"}}}},
        {{4, 22, -1}, {{"name", {"perfect-fourth minor tetrachord"}}}},
        {{4, 23, 0}, {{"name", {"quartal tetramirror"}}}},
        {{4, 24, 0}, {{"name", {"augmented seventh chord"}}}},
        {{4, 25, 0},
         {{"name",
           {"Messiaen's truncated mode 6", "French augmented sixth chord"}}}},
        {{4, 26, 0}, {{"name", {"minor seventh chord"}}}},
        {{4, 27, 1}, {{"name", {"half-diminished seventh chord"}}}},
        {{4, 27, -1},
         {{"name",
           {"dominant seventh chord", "major minor seventh chord",
            "German augmented sixth chord", "Swiss augmented sixth chord"}}}},
        {{4, 28, 0},
         {{"name",
           {"diminished seventh chord", "equal 4-part octave division"}}}},
        {{4, 29, 1}, {{"name", {"all-interval tetrachord"}}}},
        {{4, 29, -1}, {{"name", {"all-interval tetrachord"}}}},

        {{5, 1, 0}, {{"name", {"chromatic pentamirror"}}}},
        {{5, 2, 1}, {{"name", {"major-second pentacluster"}}}},
        {{5, 2, -1}, {{"name", {"major-second pentacluster"}}}},
        {{5, 3, 1}, {{"name", {"minor-second major pentachord"}}}},
        {{5, 3, -1}, {{"name", {"Spanish pentacluster"}}}},
        {{5, 4, 1}, {{"name", {"blues pentacluster"}}}},
        {{5, 4, -1}, {{"name", {"minor-third pentacluster"}}}},
        {{5, 5, 1}, {{"name", {"major-third pentacluster"}}}},
        {{5, 5, -1}, {{"name", {"major-third pentacluster"}}}},
        {{5, 6, 1},
         {{"name", {"Asian pentacluster", "quasi raga Megharanji"}}}},
        {{5, 6, -1}, {{"name", {"Asian pentacluster"}}}},
        {{5, 7, 1},
         {{"name", {"double pentacluster", "quasi raga Nabhomani"}}}},
        {{5, 7, -1}, {{"name", {"double pentacluster"}}}},
        {{5, 8, 0}, {{"name", {"tritone-symmetric pentamirror"}}}},
        {{5, 9, 1}, {{"name", {"tritone-expanding pentachord"}}}},
        {{5, 9, -1}, {{"name", {"tritone-contracting pentachord"}}}},
        {{5, 10, 1}, {{"name", {"alternating pentachord"}}}},
        {{5, 10, -1}, {{"name", {"alternating pentachord"}}}},
        {{5, 11, 1}, {{"name", {"center-cluster pentachord"}}}},
        {{5, 11, -1}, {{"name", {"center-cluster pentachord"}}}},
        {{5, 12, 0}, {{"name", {"locrian pentachord"}}}},
        {{5, 13, 1}, {{"name", {"augmented pentacluster"}}}},
        {{5, 13, -1}, {{"name", {"augmented pentacluster"}}}},
        {{5, 14, 1}, {{"name", {"double-seconds triple-fourth pentachord"}}}},
        {{5, 14, -1}, {{"name", {"double-seconds triple-fourth pentachord"}}}},
        {{5, 15, 0}, {{"name", {"asymmetric pentamirror"}}}},
        {{5, 16, 1}, {{"name", {"major-minor-diminished pentachord"}}}},
        {{5, 16, -1}, {{"name", {"major-minor diminished pentachord"}}}},
        {{5, 17, 0}, {{"name", {"minor-major ninth chord"}}}},
        {{5, 18, 1}, {{"name", {"Roma (Gypsy) pentachord"}}}},
        {{5, 18, -1}, {{"name", {"Roma (Gypsy) pentachord"}}}},
        {{5, 19, 1}, {{"name", {"Javanese pentachord"}}}},
        {{5, 19, -1}, {{"name", {"Balinese pentachord"}}}},
        {{5, 20, 1},
         {{"name",
           {"Balinese Pelog pentatonic", "quasi raga Bhupala",
            "quasi raga Bibhas"}}}},
        {{5, 20, -1},
         {{"name",
           {"Hirajoshi pentatonic", "Iwato", "Sakura", "quasi raga Saveri"}}}},
        {{5, 21, 1},
         {{"name",
           {"major-augmented ninth chord", "Syrian pentatonic",
            "quasi raga Megharanji"}}}},
        {{5, 21, -1},
         {{"name", {"Lebanese pentachord", "augmented-minor chord"}}}},
        {{5, 22, 0}, {{"name", {"Persian pentamirror", "quasi raga Ramkali"}}}},
        {{5, 23, 1}, {{"name", {"dorian pentachord", "minor pentachord"}}}},
        {{5, 23, -1}, {{"name", {"major pentachord"}}}},
        {{5, 24, 1}, {{"name", {"phrygian pentachord"}}}},
        {{5, 24, -1}, {{"name", {"lydian pentachord"}}}},
        {{5, 25, 1}, {{"name", {"diminished-major ninth chord"}}}},
        {{5, 25, -1}, {{"name", {"minor-diminished ninth chord"}}}},
        {{5, 26, 1}, {{"name", {"diminished-augmented ninth chord"}}}},
        {{5, 26, -1}, {{"name", {"augmented-diminished ninth chord"}}}},
        {{5, 27, 1}, {{"name", {"major-ninth chord"}}}},
        {{5, 27, -1}, {{"name", {"minor-ninth chord"}}}},
        {{5, 28, 1}, {{"name", {"augmented-sixth pentachord"}}}},
        {{5, 28, -1},
         {{"name", {"Javanese pentatonic", "augmented-sixth pentachord"}}}},
        {{5, 29, 1}, {{"name", {"Kumoi pentachord"}}}},
        {{5, 29, -1}, {{"name", {"Kumoi pentachord"}}}},
        {{5, 30, 1}, {{"name", {"enigmatic pentachord"}}}},
        {{5, 30, -1},
         {{"name", {"enigmatic pentachord", "altered pentatonic"}}}},
        {{5, 31, 1}, {{"name", {"diminished minor-ninth chord"}}}},
        {{5, 31, -1},
         {{"name", {"flat-ninth pentachord", "quasi raga Ranjaniraga"}}}},
        {{5, 32, 1}, {{"name", {"Neapolitan pentachord"}}}},
        {{5, 32, -1}, {{"name", {"Neapolitan pentachord"}}}},
        {{5, 33, 0}, {{"name", {"whole-tone pentachord"}}}},
        {{5, 34, 0},
         {{"name",
           {"dominant-ninth", "major-minor", "Prometheus pentamirror",
            "dominant pentatonic"}}}},
        {{5, 35, 0},
         {{"name",
           {"major pentatonic", "black-key scale", "blues pentatonic",
            "slendro", "quartal pentamirror"}}}},
        {{5, 36, 1}, {{"name", {"major-seventh pentacluster"}}}},
        {{5, 36, -1}, {{"name", {"minor-seventh pentacluster"}}}},
        {{5, 37, 0}, {{"name", {"center-cluster pentamirror"}}}},
        {{5, 38, 1}, {{"name", {"diminished pentacluster"}}}},
        {{5, 38, -1}, {{"name", {"diminished pentacluster"}}}},
        {{6, 1, 0},
         {{"name",
           {"A all combinatorial (P6, I11, RI5, RI11)", "chromatic hexamirror",
            "first-order all-combinatorial"}}}},
        {{6, 2, 1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 2, -1}, {{"name", {"combinatorial I (I1)"}}}},
        {{6, 3, 1}, {{"name", {}}}},
        {{6, 3, -1}, {{"name", {}}}},
        {{6, 4, 0}, {{"name", {"combinatorial RI (RI6)"}}}},
        {{6, 5, 1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 5, -1}, {{"name", {"combinatorial I (I3)"}}}},
        {{6, 6, 0}, {{"name", {"double cluster hexamirror"}}}},
        {{6, 7, 0},
         {{"name",
           {"B all combinatorial (P3, P9, I5, R6, R12, R8)",
            "Messiaen's mode 5", "second-order all combinatorial"}}}},
        {{6, 8, 0}, {{"name", {"D all combinatorial (P6, I1, RI7)"}}}},
        {{6, 9, 1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 9, -1}, {{"name", {"combinatorial I (I3)"}}}},
        {{6, 10, 1}, {{"name", {}}}},
        {{6, 10, -1}, {{"name", {}}}},
        {{6, 11, 1}, {{"name", {}}}},
        {{6, 11, -1}, {{"name", {}}}},
        {{6, 12, 1}, {{"name", {}}}},
        {{6, 12, -1}, {{"name", {}}}},
        {{6, 13, 0},
         {{"name", {"alternating hexamirror", "combinatorial I (I7)"}}}},
        {{6, 14, 1}, {{"name", {"combinatorial P (P6)"}}}},
        {{6, 14, -1}, {{"name", {"combinatorial P (P6)"}}}},
        {{6, 15, 1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 15, -1}, {{"name", {"combinatorial I (I5)"}}}},
        {{6, 16, 1}, {{"name", {"combinatorial I (I3)"}}}},
        {{6, 16, -1}, {{"name", {"combinatorial I (I1)", "quasi raga Megha"}}}},
        {{6, 17, 1}, {{"name", {"all tri-chord hexachord"}}}},
        {{6, 17, -1}, {{"name", {"all tri-chord hexachord (inverted form)"}}}},
        {{6, 18, 1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 18, -1}, {{"name", {"combinatorial I (I5)"}}}},
        {{6, 19, 1}, {{"name", {}}}},
        {{6, 19, -1}, {{"name", {}}}},
        {{6, 20, 0},
         {{"name",
           {"E all combinatorial (P2, P6, P10, I3, I7, R4, R8, RI1, RI5, RI9)",
            "Messiaen's truncated mode 3", "Genus tertium",
            "third-order all combinatorial"}}}},
        {{6, 21, 1}, {{"name", {"combinatorial I (I1)"}}}},
        {{6, 21, -1}, {{"name", {"combinatorial I (I3)"}}}},
        {{6, 22, 1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 22, -1}, {{"name", {"combinatorial I (I5)"}}}},
        {{6, 23, 0},
         {{"name", {"combinatorial RI (RI8)", "super-locrian hexamirror"}}}},
        {{6, 24, 1}, {{"name", {}}}},
        {{6, 24, -1}, {{"name", {"melodic-minor hexachord"}}}},
        {{6, 25, 1}, {{"name", {"locrian hexachord"}}}},
        {{6, 25, -1}, {{"name", {"minor hexachord"}}}},
        {{6, 26, 0},
         {{"name", {"phrygian hexamirror", "combinatorial RI (RI8)"}}}},
        {{6, 27, 1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 27, -1},
         {{"name", {"combinatorial I (I1)", "pyramid hexachord"}}}},
        {{6, 28, 0},
         {{"name", {"double-phrygian hexachord", "combinatorial RI (RI6)"}}}},
        {{6, 29, 0}, {{"name", {"combinatorial RI (RI9)"}}}},
        {{6, 30, 1},
         {{"name",
           {"Messiaen's truncated mode 2", "minor-bitonal hexachord",
            "combinatorial R (R6)", "combinatorial I (I1, I7)"}}}},
        {{6, 30, -1},
         {{"name",
           {"Stravinsky's Petrushka-chord", "Messiaen's truncated mode 2",
            "major-bitonal hexachord", "combinatorial R (R6)",
            "combinatorial I (I1, I7)"}}}},
        {{6, 31, 1}, {{"name", {"combinatorial I (I7)"}}}},
        {{6, 31, -1}, {{"name", {"combinatorial I (I11)"}}}},
        {{6, 32, 0},
         {{"name",
           {"Guidonian hexachord", "C all combinatorial (P6, I3, RI9)",
            "major hexamirror", "quartal hexamirror",
            "first-order all combinatorial"}}}},
        {{6, 33, 1}, {{"name", {"dorian hexachord", "combinatorial I (I6)"}}}},
        {{6, 33, -1},
         {{"name",
           {"dominant-eleventh", "lydian hexachord", "combinatorial I (I1)"}}}},
        {{6, 34, 1},
         {{"name",
           {"Scriabin's Mystic-chord", "Prometheus hexachord",
            "combinatorial I (I11)"}}}},
        {{6, 34, -1},
         {{"name",
           {"augmented-eleventh", "harmonic hexachord",
            "combinatorial I (I7)"}}}},
        {{6, 35, 0},
         {{"name",
           {"whole tone scale", "6 equal part division",
            "F all-combinatorial (P1, P3, P5, P7, P9, P11, I1, I3, I5, I7, I9, "
            "I11, R2, R4, R6, R8, R10, RI2, RI4, RI6, RI8, RI10)",
            "Messiaen's mode 1", "sixth-order all combinatorial"}}}},
        {{6, 36, 1}, {{"name", {}}}},
        {{6, 36, -1}, {{"name", {}}}},
        {{6, 37, 0}, {{"name", {"combinatorial RI (RI4)"}}}},
        {{6, 38, 0}, {{"name", {"combinatorial RI (RI3)"}}}},
        {{6, 39, 1}, {{"name", {}}}},
        {{6, 39, -1}, {{"name", {}}}},
        {{6, 40, 1}, {{"name", {}}}},
        {{6, 40, -1}, {{"name", {}}}},
        {{6, 41, 1}, {{"name", {}}}},
        {{6, 41, -1}, {{"name", {}}}},
        {{6, 42, 0}, {{"name", {"combinatorial RI (RI3)"}}}},
        {{6, 43, 1}, {{"name", {"complement of all tri-chord hexachord"}}}},
        {{6, 43, -1},
         {{"name", {"complement of all-tri-chord hexachord (inverted form)"}}}},
        {{6, 44, 1}, {{"name", {"Schoenberg Anagram hexachord"}}}},
        {{6, 44, -1}, {{"name", {"quasi raga Bauli"}}}},
        {{6, 45, 0}, {{"name", {"combinatorial RI (RI6)"}}}},
        {{6, 46, 1}, {{"name", {}}}},
        {{6, 46, -1}, {{"name", {}}}},
        {{6, 47, 1}, {{"name", {}}}},
        {{6, 47, -1}, {{"name", {"blues scale"}}}},
        {{6, 48, 0}, {{"name", {"combinatorial RI (RI2)"}}}},
        {{6, 49, 0},
         {{"name", {"combinatorial RI (RI4)", "Prometheus Neapolitan mode"}}}},
        {{6, 50, 0}, {{"name", {"combinatorial RI (RI1)"}}}},
        {{7, 1, 0}, {{"name", {"chromatic heptamirror"}}}},
        {{7, 2, 1}, {{"name", {}}}},
        {{7, 2, -1}, {{"name", {}}}},
        {{7, 3, 1}, {{"name", {}}}},
        {{7, 3, -1}, {{"name", {}}}},
        {{7, 4, 1}, {{"name", {}}}},
        {{7, 4, -1}, {{"name", {}}}},
        {{7, 5, 1}, {{"name", {}}}},
        {{7, 5, -1}, {{"name", {}}}},
        {{7, 6, 1}, {{"name", {}}}},
        {{7, 6, -1}, {{"name", {}}}},
        {{7, 7, 1}, {{"name", {}}}},
        {{7, 7, -1}, {{"name", {}}}},
        {{7, 8, 0}, {{"name", {}}}},
        {{7, 9, 1}, {{"name", {}}}},
        {{7, 9, -1}, {{"name", {}}}},
        {{7, 10, 1}, {{"name", {}}}},
        {{7, 10, -1}, {{"name", {}}}},
        {{7, 11, 1}, {{"name", {}}}},
        {{7, 11, -1}, {{"name", {}}}},
        {{7, 12, 0}, {{"name", {}}}},
        {{7, 13, 1}, {{"name", {}}}},
        {{7, 13, -1}, {{"name", {}}}},
        {{7, 14, 1}, {{"name", {}}}},
        {{7, 14, -1}, {{"name", {}}}},
        {{7, 15, 0}, {{"name", {}}}},
        {{7, 16, 1}, {{"name", {"Debussy's heptatonic"}}}},
        {{7, 16, -1}, {{"name", {}}}},
        {{7, 17, 0}, {{"name", {}}}},
        {{7, 18, 1}, {{"name", {}}}},
        {{7, 18, -1}, {{"name", {}}}},
        {{7, 19, 1}, {{"name", {}}}},
        {{7, 19, -1}, {{"name", {}}}},
        {{7, 20, 1}, {{"name", {"chromatic phrygian inverse"}}}},
        {{7, 20, -1},
         {{"name",
           {"Greek chromatic", "chromatic mixolydian", "chromatic dorian",
            "quasi raga Pantuvarali", "mela Kanakangi"}}}},
        {{7, 21, 1}, {{"name", {}}}},
        {{7, 21, -1}, {{"name", {"Roma (Gypsy) hepatonic"}}}},
        {{7, 22, 0},
         {{"name",
           {"double harmonic scale", "major Roma (Gypsy)", "Hungarian minor",
            "double harmonic scale", "quasi raga Mayamdavagaula"}}}},
        {{7, 23, 1}, {{"name", {}}}},
        {{7, 23, -1}, {{"name", {"tritone major heptachord"}}}},
        {{7, 24, 1}, {{"name", {}}}},
        {{7, 24, -1},
         {{"name", {"mystic heptachord", "Enigmatic heptatonic"}}}},
        {{7, 25, 1}, {{"name", {}}}},
        {{7, 25, -1}, {{"name", {}}}},
        {{7, 26, 1}, {{"name", {}}}},
        {{7, 26, -1}, {{"name", {}}}},
        {{7, 27, 1}, {{"name", {}}}},
        {{7, 27, -1}, {{"name", {"modified blues"}}}},
        {{7, 28, 1}, {{"name", {}}}},
        {{7, 28, -1}, {{"name", {}}}},
        {{7, 29, 1}, {{"name", {}}}},
        {{7, 29, -1}, {{"name", {}}}},
        {{7, 30, 1}, {{"name", {"Neapolitan-minor mode"}}}},
        {{7, 30, -1}, {{"name", {}}}},
        {{7, 31, 1},
         {{"name", {"alternating heptachord", "Hungarian major mode"}}}},
        {{7, 31, -1},
         {{"name", {"diminished scale", "alternating heptachord"}}}},
        {{7, 32, 1},
         {{"name",
           {"harmonic minor scale", "harmonic minor collection",
            "Spanish Roma (Gypsy)", "mela Kiravani"}}}},
        {{7, 32, -1},
         {{"name",
           {"harmonic major scale (inverted)",
            "harmonic minor collection (inverted)", "mela Cakravana",
            "quasi raga Ahir Bhairav"}}}},
        {{7, 33, 0},
         {{"name", {"Neapolitan-major mode", "leading-whole-tone mode"}}}},
        {{7, 34, 0},
         {{"name",
           {"melodic minor ascending scale", "jazz minor",
            "augmented thirteenth heptamirror", "harmonic/super-locrian"}}}},
        {{7, 35, 0},
         {{"name",
           {"major scale", "major diatonic heptachord", "natural minor scale",
            "dominant thirteenth", "locrian", "phrygian", "major inverse"}}}},
        {{7, 36, 1}, {{"name", {}}}},
        {{7, 36, -1}, {{"name", {}}}},
        {{7, 37, 0}, {{"name", {}}}},
        {{7, 38, 1}, {{"name", {}}}},
        {{7, 38, -1}, {{"name", {}}}},
        {{8, 1, 0}, {{"name", {"chromatic octamirror"}}}},
        {{8, 2, 1}, {{"name", {}}}},
        {{8, 2, -1}, {{"name", {}}}},
        {{8, 3, 0}, {{"name", {}}}},
        {{8, 4, 1}, {{"name", {}}}},
        {{8, 4, -1}, {{"name", {}}}},
        {{8, 5, 1}, {{"name", {}}}},
        {{8, 5, -1}, {{"name", {}}}},
        {{8, 6, 0}, {{"name", {}}}},
        {{8, 7, 0}, {{"name", {}}}},
        {{8, 8, 0}, {{"name", {}}}},
        {{8, 9, 0}, {{"name", {"Messiaen's mode 4"}}}},
        {{8, 10, 0}, {{"name", {}}}},
        {{8, 11, 1}, {{"name", {}}}},
        {{8, 11, -1}, {{"name", {"blues octatonic"}}}},
        {{8, 12, 1}, {{"name", {}}}},
        {{8, 12, -1}, {{"name", {}}}},
        {{8, 13, 1}, {{"name", {"blues octatonic"}}}},
        {{8, 13, -1}, {{"name", {}}}},
        {{8, 14, 1}, {{"name", {}}}},
        {{8, 14, -1}, {{"name", {}}}},
        {{8, 15, 1}, {{"name", {}}}},
        {{8, 15, -1}, {{"name", {}}}},
        {{8, 16, 1}, {{"name", {}}}},
        {{8, 16, -1}, {{"name", {"enigmatic octachord"}}}},
        {{8, 17, 0}, {{"name", {}}}},
        {{8, 18, 1}, {{"name", {}}}},
        {{8, 18, -1}, {{"name", {}}}},
        {{8, 19, 1}, {{"name", {}}}},
        {{8, 19, -1}, {{"name", {}}}},
        {{8, 20, 0}, {{"name", {}}}},
        {{8, 21, 0}, {{"name", {}}}},
        {{8, 22, 1}, {{"name", {}}}},
        {{8, 22, -1}, {{"name", {"Spanish octatonic scale"}}}},
        {{8, 23, 0},
         {{"name", {"Greek", "quartal octachord", "diatonic octad"}}}},
        {{8, 24, 0}, {{"name", {}}}},
        {{8, 25, 0}, {{"name", {"Messiaen's mode 6"}}}},
        {{8, 26, 0}, {{"name", {"Spanish phrygian", "blues"}}}},
        {{8, 27, 1}, {{"name", {}}}},
        {{8, 27, -1}, {{"name", {}}}},
        {{8, 28, 0},
         {{"name",
           {"octatonic scale", "Messiaen's mode 2",
            "alternating octatonic scale", "diminished scale"}}}},
        {{8, 29, 1}, {{"name", {}}}},
        {{8, 29, -1}, {{"name", {}}}},
        {{9, 1, 0}, {{"name", {"chromatic nonamirror"}}}},
        {{9, 2, 1}, {{"name", {}}}},
        {{9, 2, -1}, {{"name", {}}}},
        {{9, 3, 1}, {{"name", {}}}},
        {{9, 3, -1}, {{"name", {}}}},
        {{9, 4, 1}, {{"name", {}}}},
        {{9, 4, -1}, {{"name", {}}}},
        {{9, 5, 1}, {{"name", {}}}},
        {{9, 5, -1}, {{"name", {}}}},
        {{9, 6, 0}, {{"name", {}}}},
        {{9, 7, 1}, {{"name", {"nonatonic blues"}}}},
        {{9, 7, -1}, {{"name", {}}}},
        {{9, 8, 1}, {{"name", {}}}},
        {{9, 8, -1}, {{"name", {}}}},
        {{9, 9, 0}, {{"name", {}}}},
        {{9, 10, 0}, {{"name", {}}}},
        {{9, 11, 1}, {{"name", {}}}},
        {{9, 11, -1}, {{"name", {"diminishing nonachord"}}}},
        {{9, 12, 0}, {{"name", {"Messiaen's mode 3", "Tsjerepnin"}}}},
        {{10, 1, 0}, {{"name", {"chromatic decamirror"}}}},
        {{10, 2, 0}, {{"name", {}}}},
        {{10, 3, 0}, {{"name", {}}}},
        {{10, 4, 0}, {{"name", {}}}},
        {{10, 5, 0}, {{"name", {"major-minor mixed"}}}},
        {{10, 6, 0}, {{"name", {"Messiaen's mode 7"}}}},
        {{11, 1, 0}, {{"name", {"chromatic undecamirror"}}}},
        {{12, 1, 0},
         {{"name",
           {"aggregate", "dodecachord", "twelve-tone chromatic",
            "chromatic scale", "dodecamirror"}}}}

};

std::vector<int> forteIndexToInversionsAvailable(int card, int index) {
  if (card < 1 || card > 12) {
    throw ChordTablesException("cardinality " + std::to_string(card) +
                               " not valid");
  }
  if (index < 1 ||
      index > maximumIndexNumberWithoutInversionEquivalence[card]) {
    throw ChordTablesException("index " + std::to_string(index) + " not valid");
  }
  // get morris invariance vector
  const TNIStructure *tni = (*FORTE[card])[index];
  auto morris = tni->invarianceVector;
  if (std::get<1>(morris) > 0) // second value stored inversion status
  {
    return {0};
  } else {
    return {-1, 1};
  }
}

// Validate the given address and return a valid address with cardinality,
// index, and inversion Throws ChordTablesException if the address is not valid
std::tuple<int, int, int> _validateAddress(std::tuple<int, int, int> address) {
  int card = std::get<0>(address);
  int index = std::get<1>(address);
  int inversion = std::get<2>(address);

  if (card < 1 || card > 12) {
    throw ChordTablesException("cardinality " + std::to_string(card) +
                               " not valid");
  }

  if (index < 1 ||
      index > maximumIndexNumberWithoutInversionEquivalence[card]) {
    throw ChordTablesException("index " + std::to_string(index) + " not valid");
  }

  std::vector<int> inversionsAvailable =
      forteIndexToInversionsAvailable(card, index);
  if (std::find(inversionsAvailable.begin(), inversionsAvailable.end(),
                inversion) == inversionsAvailable.end()) {
    throw ChordTablesException("inversion " + std::to_string(inversion) +
                               " not valid");
  }

  return std::make_tuple(card, index, inversion);
}

std::vector<int> getPitchClassVector(std::vector<ZZ_12> pitchClasses) {
  std::vector<int> intervalCounts(6, 0);

  for (size_t i = 0; i < pitchClasses.size(); ++i) {
    for (size_t j = i + 1; j < pitchClasses.size(); ++j) {
      int interval = ZZ_12::getInterval(pitchClasses[i], pitchClasses[j]);
      if (interval > 6) {
        interval = 12 - interval; // Consider interval inversions
      }
      if (interval > 0 && interval <= 6) {
        intervalCounts[interval - 1]++;
      }
    }
  }

  return intervalCounts;
}

template <size_t... Is> struct index_sequence {};

template <size_t N, size_t... Is>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};

template <size_t... Is>
struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

template <typename T, size_t... Is>
std::vector<T> tupleToVectorImpl(const std::tuple<T, T, T, T, T, T, T, T> &t,
                                 index_sequence<Is...>) {
  return {std::get<Is>(t)...};
}

template <typename T>
std::vector<T> tupleToVector(const std::tuple<T, T, T, T, T, T, T, T> &t) {
  return tupleToVectorImpl(t, make_index_sequence<8>{});
}

template <typename InputType, typename OutputType, typename Func>
std::vector<OutputType> mapVector(const std::vector<InputType> &vec,
                                  Func func) {
  std::vector<OutputType> result;
  result.reserve(vec.size());
  for (const auto &item : vec) {
    result.push_back(func(item));
  }
  return result;
}

template <typename InputType>
std::string mapVectorToString(const std::vector<InputType> &vec) {
  std::string result;
  for (const auto &item : vec) {
    result += std::to_string(item);
  }
  return result;
}

std::tuple<int, int, int, int>
seekChordTablesAddress(const std::vector<ZZ_12> &pcSetZZ) {
  int index = 0;
  int inversion = 0;
  auto unModLambda = [](const ZZ_12 &pitchClass) { return pitchClass.unMod(); };

  std::vector<int> pcSet = mapVector<ZZ_12, int>(pcSetZZ, unModLambda);

  if (pcSet.size() == 0)

  {
    throw ChordTablesException(
        "cannot access chord tables address for Chord with 0 pitches");
  }

  int card = pcSet.size();
  if (card == 1) { // it is a singleton: return it
    return std::make_tuple(1, 1, 0, pcSet[0]);
  } else if (card == 12) { // it is the aggregate
    return std::make_tuple(12, 1, 0, 0);
  }

  // go through each rotation of pcSet
  std::vector<std::tuple<std::vector<int>, std::vector<int>, ZZ_12>> candidates;
  for (int rot = 0; rot < card; ++rot) {
    std::vector<int> testSet(pcSet.begin() + rot, pcSet.end());
    testSet.insert(testSet.end(), pcSet.begin(), pcSet.begin() + rot);
    // transpose to lead with zero
    int testSetOriginalPC = testSet[0];
    for (auto &x : testSet) {
      x = (x - testSetOriginalPC);
    }
    // create inversion; first take difference from 12 mod 12
    std::vector<int> testSetInvert(testSet.size());
    std::transform(testSet.begin(), testSet.end(), testSetInvert.begin(),
                   [](int x) { return (12 - x) % 12; });
    std::reverse(testSetInvert.begin(), testSetInvert.end());
    // transpose all steps (were last) to zero, mod 12
    int transposition = (12 - testSetInvert[0]) % 12;
    for (auto &x : testSetInvert) {
      x = (x + transposition) % 12;
    }

    std::tuple<std::vector<int>, std::vector<int>, ZZ_12> candidateTuple(
        testSet, testSetInvert, testSetOriginalPC);

    candidates.push_back(candidateTuple);
  }

  // compare sets to those in table
  bool match = false;
  ZZ_12 matchedPCOriginal;

  for (int indexCandidate = 1; indexCandidate < FORTE[card]->size();
       ++indexCandidate) { // first entry is nullptr
    const TNIStructure *dataLine = (*FORTE[card])[indexCandidate];
    const std::vector<int> &dataLinePcs = dataLine->pitchClasses;
    std::vector<int> inversionsAvailable =
        forteIndexToInversionsAvailable(card, indexCandidate);

    for (const auto &elem : candidates) {
      const auto &candidate = std::get<0>(elem);
      const auto &candidateInversion = std::get<1>(elem);
      const auto &candidateOriginalPC = std::get<2>(elem);
      ///  what are we supposed to compare here?
      if (dataLinePcs == candidate) {
        index = indexCandidate;
        if (std::find(inversionsAvailable.begin(), inversionsAvailable.end(),
                      0) != inversionsAvailable.end()) {
          inversion = 0;
        } else {
          inversion = 1;
        }
        matchedPCOriginal = candidateOriginalPC;
        match = true;
        break;
      } else if (dataLinePcs == candidateInversion) {
        index = indexCandidate;
        if (std::find(inversionsAvailable.begin(), inversionsAvailable.end(),
                      0) != inversionsAvailable.end()) {
          inversion = 0; // should never reach this line?
        } else {
          inversion = -1;
        }
        matchedPCOriginal = candidateOriginalPC;
        match = true;
        break;
      }
    }
    if (match) {
      break;
    }
  }
  if (!match) {
    throw ChordTablesException("cannot find a chord table address for " +
                               mapVectorToString(pcSet));
  }
  return std::make_tuple(card, index, inversion, matchedPCOriginal.unMod());
}

std::vector<std::string>
addressToCommonNames(const std::tuple<int, int, int> &address) {
  auto validatedAddress = _validateAddress(address);
  auto refDict = tnIndexToChordInfo[validatedAddress];
  if (refDict.find("name") != refDict.end()) {
    return std::vector<std::string>(refDict["name"].begin(),
                                    refDict["name"].end());
  } else {
    return std::vector<std::string>();
  }
}

std::vector<std::string> getChordNames_prime(const ZZ_12 &root,
                                             const std::set<ZZ_12> &chord) {

  std::vector<ZZ_12> chordVector(chord.begin(), chord.end());
  chordVector.insert(chordVector.begin(), root);

  std::tuple<int, int, int, int> address = seekChordTablesAddress(chordVector);
  auto chordTuple = std::make_tuple(std::get<0>(address), std::get<1>(address),
                                    std::get<2>(address));
  return addressToCommonNames(chordTuple);
}

std::vector<std::string> getChordNames(const Chromatic &root,
                                       const std::set<Chromatic> &chord) {

  std::set<ZZ_12> chord_prime;
  for (const auto &note : chord) {
    chord_prime.insert(toZZ_12(note));
  }
  ZZ_12 root_prime = toZZ_12(root);
  return getChordNames_prime(root_prime, chord_prime);
}

#ifdef TEST
void testCMinorTriad() {
  ZZ_12 root = toZZ_12(Chromatic::C);
  std::set<ZZ_12> chord = {toZZ_12(Chromatic::Eb), toZZ_12(Chromatic::G)};
  std::vector<std::string> chordNames = getChordNames_prime(root, chord);
  for (const auto &name : chordNames)
    assert(chordNames[0] == "minor triad");
}

void testCMajTriad() {
  ZZ_12 root = toZZ_12(Chromatic::C);
  std::set<ZZ_12> chord = {toZZ_12(Chromatic::E), toZZ_12(Chromatic::G)};
  std::vector<std::string> chordNames = getChordNames_prime(root, chord);
  for (const auto &name : chordNames)
    assert(chordNames[0] == "major triad");
}

void testPFifth() {
  ZZ_12 root = toZZ_12(Chromatic::F);
  std::set<ZZ_12> chord = {toZZ_12(Chromatic::C)};
  std::vector<std::string> chordNames = getChordNames_prime(root, chord);
  for (const auto &name : chordNames)
    assert(chordNames[0] == "perfect fifth");
}

void testMaj6() {
  ZZ_12 root = toZZ_12(Chromatic::Eb);
  std::set<ZZ_12> chord = {toZZ_12(Chromatic::C)};
  std::vector<std::string> chordNames = getChordNames_prime(root, chord);
  assert(chordNames[0] == "major sixth");
}

void addressShouldBe(std::vector<Chromatic> chord,
                     std::tuple<int, int, int, int> shouldBe) {
  std::vector<ZZ_12> chord_prime;
  for (const auto &note : chord) {
    chord_prime.push_back(toZZ_12(note));
  }
  std::tuple<int, int, int, int> address = seekChordTablesAddress(chord_prime);

  if (address != shouldBe) {
    // Debug print address
    std::cout << "address: (" << std::get<0>(address) << ", "
              << std::get<1>(address) << ", " << std::get<2>(address) << ", "
              << std::get<3>(address) << ")" << std::endl;

    // Debug print shouldBe
    std::cout << "shouldBe: (" << std::get<0>(shouldBe) << ", "
              << std::get<1>(shouldBe) << ", " << std::get<2>(shouldBe) << ", "
              << std::get<3>(shouldBe) << ")" << std::endl;
  }
  assert(address == shouldBe);
}

void addressShouldBe1() {
  std::vector<Chromatic> chord;
  // Convert Chromatic enum values to int before comparison
  int chromaticC = static_cast<int>(Chromatic::C);
  int chromaticBb = static_cast<int>(Chromatic::Bb);
  for (int i = chromaticC; i <= 11; ++i) {
    if (i != chromaticBb) {
      chord.push_back(static_cast<Chromatic>(i));
    }
  }
  auto sb = std::make_tuple(11, 1, 0, 11);
  addressShouldBe(chord, sb);
}

void addressShouldBe2() {
  std::vector<Chromatic> chord = {Chromatic::G, Chromatic::B, Chromatic::D};
  auto sb = std::make_tuple(3, 11, -1, 7);
  addressShouldBe(chord, sb);
}

void addressShouldBe3() {
  std::vector<Chromatic> chord = {Chromatic::C, Chromatic::Eb, Chromatic::G};
  auto sb = std::make_tuple(3, 11, 1, 0);
  addressShouldBe(chord, sb);
}

void addressShouldBe4() {
  std::vector<Chromatic> chord = {Chromatic::C, Chromatic::Cs, Chromatic::Eb,
                                  Chromatic::E, Chromatic::Fs, Chromatic::Gs,
                                  Chromatic::Bb};
  auto sb = std::make_tuple(7, 34, 0, 0);
  addressShouldBe(chord, sb);
}

int main() {
  addressShouldBe1();
  addressShouldBe2();
  addressShouldBe3();
  addressShouldBe4();
  testCMinorTriad();
  testCMajTriad();
  testMaj6();
  return 0;
}
#endif