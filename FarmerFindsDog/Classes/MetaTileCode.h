#pragma once

// The MetaTileCode enum has the same codes that are used as propeties in
// meta_tiles_64x64_v01.tsx file
// Also there are some additional codes specific for "FarmerFindsDog" project
// to make the program more readable
enum MetaTileCode {
  MTC_RED    = 1,
  MTC_GREEN  = 2,
  MTC_BLUE   = 3,
  MTC_YELLOW = 4,

  MTC_RED_PLUS    = 11,
  MTC_GREEN_PLUS  = 12, // not implemented in tileset
  MTC_BLUE_PLUS   = 13, // not implemented in tileset
  MTC_YELLOW_PLUS = 14,

  MTC_POINT_A = 21,
  MTC_POINT_B = 22,
  MTC_POINT_C = 23,
  MTC_POINT_D = 24,
  MTC_POINT_E = 25,

  // following codes are specially for "FarmerFindsDog"
  MTC_FFD_DOG_START    = MTC_POINT_A,
  MTC_FFD_FARMER_START = MTC_POINT_B,
  MTC_FFD_UNPASSABLE   = MTC_RED,

  // finally
  MTC_MAX_VALUE = 1024
};
