#include <iostream>
#include <fstream>

#include <trifinger_object_tracking/color_segmenter.hpp>

namespace trifinger_object_tracking {
std::array<float, XGB_NUM_CLASSES> xgb_classify_cube_v2(
        std::array<float, XGB_NUM_FEATURES> &sample) {

  std::array<float, XGB_NUM_CLASSES> sum;
  sum.fill(0.0);

  if (sample[4] <78) {
    if (sample[4] <56) {
      if (sample[4] <47) {
        if (sample[4] <39) {
          sum[0] += 3.49921942;
        } else {
          if (sample[3] <98) {
            if (sample[3] <90) {
              sum[0] += 3.49249029;
            } else {
              sum[0] += 3.38680482;
            }
          } else {
            if (sample[1] <81) {
              sum[0] += 2.62602401;
            } else {
              sum[0] += 3.47406745;
            }
          }
        }
      } else {
        if (sample[3] <100) {
          if (sample[1] <93) {
            if (sample[3] <91) {
              sum[0] += 3.49608135;
            } else {
              sum[0] += 2.96746278;
            }
          } else {
            if (sample[2] <89) {
              sum[0] += 2.32025337;
            } else {
              sum[0] += 3.34029818;
            }
          }
        } else {
          if (sample[1] <73) {
            if (sample[1] <53) {
              sum[0] += 3.07400298;
            } else {
              sum[0] += 0.7576285;
            }
          } else {
            if (sample[3] <130) {
              sum[0] += 3.43166447;
            } else {
              sum[0] += 1.87913048;
            }
          }
        }
      }
    } else {
      if (sample[0] <70) {
        if (sample[3] <98) {
          if (sample[1] <87) {
            if (sample[3] <8.99999905) {
              sum[0] += 2.48268437;
            } else {
              sum[0] += 3.49261522;
            }
          } else {
            if (sample[3] <45) {
              sum[0] += 3.27984357;
            } else {
              sum[0] += 0.167358533;
            }
          }
        } else {
          if (sample[3] <113) {
            if (sample[1] <45) {
              sum[0] += 3.19563913;
            } else {
              sum[0] += 1.06137753;
            }
          } else {
            if (sample[0] <63) {
              sum[0] += 3.41188002;
            } else {
              sum[0] += 2.8171525;
            }
          }
        }
      } else {
        if (sample[3] <130) {
          if (sample[1] <97) {
            if (sample[1] <68) {
              sum[0] += 1.78415203;
            } else {
              sum[0] += 3.00617409;
            }
          } else {
            if (sample[3] <45) {
              sum[0] += 2.53264499;
            } else {
              sum[0] += 0.90244621;
            }
          }
        } else {
          if (sample[3] <163) {
            if (sample[2] <72) {
              sum[0] += 1.7706548;
            } else {
              sum[0] += -0.483549863;
            }
          } else {
            if (sample[4] <67) {
              sum[0] += 3.23955488;
            } else {
              sum[0] += 2.48472142;
            }
          }
        }
      }
    }
  } else {
    if (sample[5] <75) {
      if (sample[3] <98) {
        if (sample[3] <80) {
          if (sample[1] <68) {
            if (sample[5] <64) {
              sum[0] += 3.49826622;
            } else {
              sum[0] += 3.30189228;
            }
          } else {
            if (sample[4] <178) {
              sum[0] += 3.40314913;
            } else {
              sum[0] += -0.324050456;
            }
          }
        } else {
          if (sample[4] <178) {
            if (sample[1] <64) {
              sum[0] += 3.14834261;
            } else {
              sum[0] += 2.2804842;
            }
          } else {
            if (sample[0] <15.999999) {
              sum[0] += 3.29173732;
            } else {
              sum[0] += 0.563766718;
            }
          }
        }
      } else {
        if (sample[3] <113) {
          if (sample[4] <120) {
            if (sample[3] <103) {
              sum[0] += 2.41179514;
            } else {
              sum[0] += 1.02725232;
            }
          } else {
            if (sample[0] <35) {
              sum[0] += 1.56779027;
            } else {
              sum[0] += -0.403097808;
            }
          }
        } else {
          if (sample[5] <61) {
            if (sample[2] <57) {
              sum[0] += 3.42846107;
            } else {
              sum[0] += 1.88141143;
            }
          } else {
            if (sample[4] <89) {
              sum[0] += 2.27202368;
            } else {
              sum[0] += 0.211807564;
            }
          }
        }
      }
    } else {
      if (sample[5] <89) {
        if (sample[3] <40) {
          if (sample[3] <14.999999) {
            if (sample[3] <11.999999) {
              sum[0] += 0.246853918;
            } else {
              sum[0] += 1.64220035;
            }
          } else {
            if (sample[2] <68) {
              sum[0] += 0.219411567;
            } else {
              sum[0] += 3.32785749;
            }
          }
        } else {
          if (sample[4] <104) {
            if (sample[2] <63) {
              sum[0] += 2.44842172;
            } else {
              sum[0] += 0.0334044769;
            }
          } else {
            if (sample[4] <120) {
              sum[0] += 0.305971086;
            } else {
              sum[0] += -0.522308111;
            }
          }
        }
      } else {
        if (sample[4] <104) {
          if (sample[3] <45) {
            if (sample[1] <172) {
              sum[0] += 1.79192376;
            } else {
              sum[0] += -0.0846736431;
            }
          } else {
            if (sample[2] <96) {
              sum[0] += -0.101765171;
            } else {
              sum[0] += -0.509298623;
            }
          }
        } else {
          if (sample[3] <23.9999981) {
            if (sample[3] <14.999999) {
              sum[0] += -0.461576581;
            } else {
              sum[0] += 1.2102319;
            }
          } else {
            if (sample[4] <151) {
              sum[0] += -0.461130142;
            } else {
              sum[0] += -0.575227618;
            }
          }
        }
      }
    }
  }


  if (sample[2] <15.999999) {
    if (sample[0] <29.9999981) {
      if (sample[3] <80) {
        if (sample[3] <75) {
          if (sample[3] <70) {
            sum[1] += -0.583036959;
          } else {
            if (sample[1] <49) {
              sum[1] += -0.556138039;
            } else {
              sum[1] += -0.0482758731;
            }
          }
        } else {
          if (sample[5] <33) {
            if (sample[4] <151) {
              sum[1] += -0.476217926;
            } else {
              sum[1] += -0.106760912;
            }
          } else {
            if (sample[1] <42) {
              sum[1] += -0.429439813;
            } else {
              sum[1] += 0.577981651;
            }
          }
        }
      } else {
        if (sample[3] <105) {
          if (sample[4] <201) {
            if (sample[4] <151) {
              sum[1] += -0.261410803;
            } else {
              sum[1] += 0.935402095;
            }
          } else {
            if (sample[0] <15.999999) {
              sum[1] += -0.467928231;
            } else {
              sum[1] += 2.51887918;
            }
          }
        } else {
          if (sample[3] <110) {
            if (sample[4] <255) {
              sum[1] += -0.53365159;
            } else {
              sum[1] += 0.813623667;
            }
          } else {
            if (sample[5] <27.9999981) {
              sum[1] += -0.581164539;
            } else {
              sum[1] += -0.463338077;
            }
          }
        }
      }
    } else {
      if (sample[4] <178) {
        if (sample[5] <44) {
          if (sample[3] <111) {
            if (sample[3] <91) {
              sum[1] += -0.143902823;
            } else {
              sum[1] += 1.2767868;
            }
          } else {
            if (sample[3] <113) {
              sum[1] += -0.0776340216;
            } else {
              sum[1] += -0.56344527;
            }
          }
        } else {
          if (sample[1] <39) {
            if (sample[3] <114) {
              sum[1] += 3.06308198;
            } else {
              sum[1] += -0.204878047;
            }
          } else {
            if (sample[1] <42) {
              sum[1] += 0.22292991;
            } else {
              sum[1] += -0.335463256;
            }
          }
        }
      } else {
        if (sample[0] <35) {
          if (sample[3] <107) {
            if (sample[3] <90) {
              sum[1] += 0.449213713;
            } else {
              sum[1] += 3.09540892;
            }
          } else {
            if (sample[4] <255) {
              sum[1] += -0.440049589;
            } else {
              sum[1] += 2.31724119;
            }
          }
        } else {
          if (sample[3] <115) {
            if (sample[3] <98) {
              sum[1] += 2.4925139;
            } else {
              sum[1] += 3.43898273;
            }
          } else {
            if (sample[0] <49) {
              sum[1] += -0.525650561;
            } else {
              sum[1] += 2.85283017;
            }
          }
        }
      }
    }
  } else {
    if (sample[4] <120) {
      if (sample[4] <44) {
        if (sample[4] <35) {
          if (sample[1] <71) {
            if (sample[3] <100) {
              sum[1] += -0.581363738;
            } else {
              sum[1] += -0.546907604;
            }
          } else {
            sum[1] += -0.583286166;
          }
        } else {
          if (sample[3] <98) {
            if (sample[3] <90) {
              sum[1] += -0.582273364;
            } else {
              sum[1] += -0.531285107;
            }
          } else {
            if (sample[2] <68) {
              sum[1] += 0.000482641364;
            } else {
              sum[1] += -0.548511982;
            }
          }
        }
      } else {
        if (sample[3] <98) {
          if (sample[3] <90) {
            if (sample[2] <23.9999981) {
              sum[1] += -0.559421122;
            } else {
              sum[1] += -0.582221091;
            }
          } else {
            if (sample[1] <58) {
              sum[1] += -0.20927988;
            } else {
              sum[1] += -0.570853412;
            }
          }
        } else {
          if (sample[2] <66) {
            if (sample[3] <115) {
              sum[1] += 1.36193788;
            } else {
              sum[1] += -0.419321984;
            }
          } else {
            if (sample[2] <76) {
              sum[1] += -0.296085954;
            } else {
              sum[1] += -0.574688017;
            }
          }
        }
      }
    } else {
      if (sample[3] <98) {
        if (sample[3] <91) {
          if (sample[3] <75) {
            sum[1] += -0.583046377;
          } else {
            if (sample[2] <23.9999981) {
              sum[1] += -0.43317166;
            } else {
              sum[1] += -0.576484025;
            }
          }
        } else {
          if (sample[2] <28.9999981) {
            if (sample[4] <178) {
              sum[1] += 0.242060363;
            } else {
              sum[1] += 2.03225803;
            }
          } else {
            if (sample[2] <34) {
              sum[1] += -0.290299326;
            } else {
              sum[1] += -0.578169167;
            }
          }
        }
      } else {
        if (sample[3] <118) {
          if (sample[3] <103) {
            if (sample[2] <34) {
              sum[1] += 2.00883579;
            } else {
              sum[1] += -0.0720411763;
            }
          } else {
            if (sample[0] <46) {
              sum[1] += 0.693330348;
            } else {
              sum[1] += 3.43098617;
            }
          }
        } else {
          if (sample[3] <120) {
            if (sample[0] <68) {
              sum[1] += -0.521216989;
            } else {
              sum[1] += 2.54162478;
            }
          } else {
            if (sample[3] <130) {
              sum[1] += -0.278045624;
            } else {
              sum[1] += -0.579603314;
            }
          }
        }
      }
    }
  }


  if (sample[4] <67) {
    if (sample[4] <58) {
      if (sample[4] <48) {
        if (sample[4] <40) {
          sum[2] += -0.583249331;
        } else {
          if (sample[0] <85) {
            sum[2] += -0.582985342;
          } else {
            if (sample[2] <89) {
              sum[2] += -0.505681753;
            } else {
              sum[2] += -0.579533875;
            }
          }
        }
      } else {
        if (sample[0] <88) {
          if (sample[0] <75) {
            sum[2] += -0.582892597;
          } else {
            if (sample[2] <79) {
              sum[2] += -0.532946229;
            } else {
              sum[2] += -0.579532087;
            }
          }
        } else {
          if (sample[2] <86) {
            if (sample[1] <97) {
              sum[2] += -0.493501812;
            } else {
              sum[2] += 0.51483655;
            }
          } else {
            if (sample[2] <100) {
              sum[2] += -0.471400648;
            } else {
              sum[2] += -0.576533914;
            }
          }
        }
      }
    } else {
      if (sample[0] <92) {
        if (sample[0] <79) {
          if (sample[0] <70) {
            sum[2] += -0.58310616;
          } else {
            if (sample[2] <71) {
              sum[2] += -0.514282346;
            } else {
              sum[2] += -0.580617905;
            }
          }
        } else {
          if (sample[2] <76) {
            if (sample[1] <81) {
              sum[2] += -0.566925406;
            } else {
              sum[2] += -0.0619901121;
            }
          } else {
            if (sample[2] <79) {
              sum[2] += -0.495296746;
            } else {
              sum[2] += -0.574341893;
            }
          }
        }
      } else {
        if (sample[2] <100) {
          if (sample[3] <100) {
            if (sample[3] <84) {
              sum[2] += 0.255729198;
            } else {
              sum[2] += 1.85796261;
            }
          } else {
            if (sample[3] <103) {
              sum[2] += 0.127764627;
            } else {
              sum[2] += -0.570735633;
            }
          }
        } else {
          if (sample[2] <121) {
            if (sample[1] <143) {
              sum[2] += -0.523711562;
            } else {
              sum[2] += 0.755939484;
            }
          } else {
            if (sample[0] <158) {
              sum[2] += -0.580229044;
            } else {
              sum[2] += -0.517157674;
            }
          }
        }
      }
    }
  } else {
    if (sample[0] <92) {
      if (sample[3] <75) {
        if (sample[3] <70) {
          if (sample[0] <77) {
            sum[2] += -0.583117008;
          } else {
            if (sample[3] <65) {
              sum[2] += -0.582031071;
            } else {
              sum[2] += 0.665916085;
            }
          }
        } else {
          if (sample[1] <81) {
            if (sample[1] <78) {
              sum[2] += -0.582192481;
            } else {
              sum[2] += -0.116612688;
            }
          } else {
            if (sample[2] <48) {
              sum[2] += 2.84452271;
            } else {
              sum[2] += 0.762806118;
            }
          }
        }
      } else {
        if (sample[1] <81) {
          if (sample[1] <64) {
            if (sample[1] <61) {
              sum[2] += -0.582454145;
            } else {
              sum[2] += -0.453024805;
            }
          } else {
            if (sample[2] <36) {
              sum[2] += 2.32465696;
            } else {
              sum[2] += -0.129732981;
            }
          }
        } else {
          if (sample[4] <89) {
            if (sample[1] <100) {
              sum[2] += 0.479237407;
            } else {
              sum[2] += 2.75573444;
            }
          } else {
            if (sample[4] <104) {
              sum[2] += 2.31067944;
            } else {
              sum[2] += 3.40367174;
            }
          }
        }
      }
    } else {
      if (sample[3] <103) {
        if (sample[3] <75) {
          if (sample[3] <68) {
            if (sample[3] <65) {
              sum[2] += -0.583063364;
            } else {
              sum[2] += -0.063189581;
            }
          } else {
            if (sample[4] <78) {
              sum[2] += 0.200832918;
            } else {
              sum[2] += 1.19096351;
            }
          }
        } else {
          if (sample[3] <100) {
            if (sample[4] <78) {
              sum[2] += 2.65477777;
            } else {
              sum[2] += 3.4238565;
            }
          } else {
            if (sample[0] <121) {
              sum[2] += 1.95740211;
            } else {
              sum[2] += -0.0463261642;
            }
          }
        }
      } else {
        if (sample[3] <107) {
          if (sample[0] <102) {
            if (sample[1] <83) {
              sum[2] += -0.496372551;
            } else {
              sum[2] += -0.0197183266;
            }
          } else {
            if (sample[1] <103) {
              sum[2] += -0.571992397;
            } else {
              sum[2] += -0.440413773;
            }
          }
        } else {
          if (sample[1] <106) {
            if (sample[3] <108) {
              sum[2] += -0.531042278;
            } else {
              sum[2] += -0.583149493;
            }
          } else {
            if (sample[2] <109) {
              sum[2] += -0.167886898;
            } else {
              sum[2] += -0.581256449;
            }
          }
        }
      }
    }
  }


  if (sample[4] <120) {
    if (sample[4] <58) {
      if (sample[4] <52) {
        if (sample[4] <44) {
          sum[3] += -0.583303154;
        } else {
          if (sample[1] <90) {
            sum[3] += -0.583159924;
          } else {
            if (sample[0] <82) {
              sum[3] += -0.278599948;
            } else {
              sum[3] += -0.565216482;
            }
          }
        }
      } else {
        if (sample[1] <93) {
          if (sample[1] <87) {
            if (sample[1] <81) {
              sum[3] += -0.583259523;
            } else {
              sum[3] += -0.559101403;
            }
          } else {
            if (sample[0] <73) {
              sum[3] += -0.325014502;
            } else {
              sum[3] += -0.571847856;
            }
          }
        } else {
          if (sample[0] <82) {
            if (sample[3] <45) {
              sum[3] += -0.52706778;
            } else {
              sum[3] += 2.18878412;
            }
          } else {
            if (sample[1] <147) {
              sum[3] += -0.5663715;
            } else {
              sum[3] += -0.204942808;
            }
          }
        }
      }
    } else {
      if (sample[1] <83) {
        if (sample[1] <81) {
          if (sample[1] <78) {
            sum[3] += -0.583121717;
          } else {
            if (sample[0] <59) {
              sum[3] += -0.529007435;
            } else {
              sum[3] += -0.58167845;
            }
          }
        } else {
          if (sample[0] <59) {
            if (sample[3] <40) {
              sum[3] += -0.574748218;
            } else {
              sum[3] += 0.934664667;
            }
          } else {
            if (sample[0] <65) {
              sum[3] += -0.501897931;
            } else {
              sum[3] += -0.582865894;
            }
          }
        }
      } else {
        if (sample[3] <60) {
          if (sample[3] <43) {
            if (sample[3] <38) {
              sum[3] += -0.562148809;
            } else {
              sum[3] += 0.0552829765;
            }
          } else {
            if (sample[3] <56) {
              sum[3] += 3.02623844;
            } else {
              sum[3] += 0.895228207;
            }
          }
        } else {
          if (sample[3] <65) {
            if (sample[1] <97) {
              sum[3] += -0.122653328;
            } else {
              sum[3] += -0.560293913;
            }
          } else {
            if (sample[3] <68) {
              sum[3] += -0.525962114;
            } else {
              sum[3] += -0.583280265;
            }
          }
        }
      }
    }
  } else {
    if (sample[1] <90) {
      if (sample[1] <68) {
        if (sample[1] <64) {
          if (sample[1] <61) {
            sum[3] += -0.583239555;
          } else {
            if (sample[5] <64) {
              sum[3] += -0.323589385;
            } else {
              sum[3] += -0.583136559;
            }
          }
        } else {
          if (sample[5] <69) {
            if (sample[0] <15.999999) {
              sum[3] += 2.97872329;
            } else {
              sum[3] += -0.484135628;
            }
          } else {
            sum[3] += -0.583147407;
          }
        }
      } else {
        if (sample[5] <92) {
          if (sample[0] <26.9999981) {
            if (sample[3] <30.9999981) {
              sum[3] += -0.579095602;
            } else {
              sum[3] += 3.39296293;
            }
          } else {
            if (sample[0] <35) {
              sum[3] += 0.154303461;
            } else {
              sum[3] += -0.52688992;
            }
          }
        } else {
          sum[3] += -0.58285749;
        }
      }
    } else {
      if (sample[3] <30.9999981) {
        if (sample[2] <106) {
          if (sample[1] <100) {
            if (sample[1] <97) {
              sum[3] += -0.495293021;
            } else {
              sum[3] += -0.0299074445;
            }
          } else {
            if (sample[0] <35) {
              sum[3] += 1.91126263;
            } else {
              sum[3] += 0.553406179;
            }
          }
        } else {
          if (sample[5] <121) {
            if (sample[1] <117) {
              sum[3] += -0.530116379;
            } else {
              sum[3] += 0.62236774;
            }
          } else {
            if (sample[3] <26.9999981) {
              sum[3] += -0.583309591;
            } else {
              sum[3] += -0.571256459;
            }
          }
        }
      } else {
        if (sample[3] <56) {
          if (sample[3] <38) {
            if (sample[4] <151) {
              sum[3] += 3.05234981;
            } else {
              sum[3] += 3.35734868;
            }
          } else {
            if (sample[3] <53) {
              sum[3] += 3.48624039;
            } else {
              sum[3] += 2.81440949;
            }
          }
        } else {
          if (sample[0] <56) {
            if (sample[1] <97) {
              sum[3] += -0.45161289;
            } else {
              sum[3] += 1.94936693;
            }
          } else {
            if (sample[3] <60) {
              sum[3] += -0.335463256;
            } else {
              sum[3] += -0.58300823;
            }
          }
        }
      }
    }
  }


  if (sample[3] <130) {
    sum[4] += -0.583275437;
  } else {
    if (sample[4] <52) {
      if (sample[4] <44) {
        if (sample[4] <35) {
          if (sample[4] <27.9999981) {
            if (sample[4] <22.9999981) {
              sum[4] += -0.583289981;
            } else {
              sum[4] += -0.570532918;
            }
          } else {
            if (sample[0] <109) {
              sum[4] += -0.490373284;
            } else {
              sum[4] += -0.56681639;
            }
          }
        } else {
          if (sample[0] <73) {
            if (sample[5] <69) {
              sum[4] += -0.579121649;
            } else {
              sum[4] += -0.483317256;
            }
          } else {
            if (sample[1] <71) {
              sum[4] += 0.585546911;
            } else {
              sum[4] += -0.401624084;
            }
          }
        }
      } else {
        if (sample[0] <73) {
          if (sample[0] <68) {
            if (sample[0] <65) {
              sum[4] += -0.569616437;
            } else {
              sum[4] += -0.422110558;
            }
          } else {
            if (sample[3] <163) {
              sum[4] += 0.0137704741;
            } else {
              sum[4] += -0.555663109;
            }
          }
        } else {
          if (sample[2] <89) {
            if (sample[3] <163) {
              sum[4] += 2.50617695;
            } else {
              sum[4] += -0.430639327;
            }
          } else {
            if (sample[3] <163) {
              sum[4] += 0.626567185;
            } else {
              sum[4] += -0.569550514;
            }
          }
        }
      }
    } else {
      if (sample[0] <59) {
        if (sample[2] <57) {
          if (sample[2] <54) {
            sum[4] += -0.581475914;
          } else {
            if (sample[0] <40) {
              sum[4] += 0.758122683;
            } else {
              sum[4] += -0.446665049;
            }
          }
        } else {
          if (sample[4] <78) {
            if (sample[3] <163) {
              sum[4] += 0.168510064;
            } else {
              sum[4] += -0.466450334;
            }
          } else {
            if (sample[0] <52) {
              sum[4] += 0.457186908;
            } else {
              sum[4] += 2.22837353;
            }
          }
        }
      } else {
        if (sample[2] <68) {
          if (sample[4] <89) {
            if (sample[2] <63) {
              sum[4] += -0.230691239;
            } else {
              sum[4] += 0.844274759;
            }
          } else {
            if (sample[2] <57) {
              sum[4] += 0.379903227;
            } else {
              sum[4] += 2.88092613;
            }
          }
        } else {
          if (sample[1] <124) {
            if (sample[0] <73) {
              sum[4] += 2.59152818;
            } else {
              sum[4] += 3.45460439;
            }
          } else {
            if (sample[5] <170) {
              sum[4] += -0.220630378;
            } else {
              sum[4] += -0.567773521;
            }
          }
        }
      }
    }
  }


  if (sample[4] <242) {
    if (sample[3] <14.999999) {
      if (sample[4] <89) {
        if (sample[4] <78) {
          if (sample[4] <67) {
            if (sample[4] <56) {
              sum[5] += -0.583058476;
            } else {
              sum[5] += -0.46777156;
            }
          } else {
            if (sample[3] <8.99999905) {
              sum[5] += 0.503651261;
            } else {
              sum[5] += -0.517135739;
            }
          }
        } else {
          if (sample[3] <8.99999905) {
            if (sample[2] <79) {
              sum[5] += -0.488765985;
            } else {
              sum[5] += 2.32175112;
            }
          } else {
            if (sample[3] <11.999999) {
              sum[5] += -0.143946111;
            } else {
              sum[5] += -0.513373613;
            }
          }
        }
      } else {
        if (sample[2] <86) {
          if (sample[5] <77) {
            if (sample[2] <71) {
              sum[5] += -0.555521607;
            } else {
              sum[5] += -0.0568019226;
            }
          } else {
            if (sample[1] <51) {
              sum[5] += 0.32473591;
            } else {
              sum[5] += 1.75312269;
            }
          }
        } else {
          if (sample[3] <11.999999) {
            if (sample[1] <90) {
              sum[5] += 3.24370718;
            } else {
              sum[5] += 1.19586504;
            }
          } else {
            if (sample[0] <46) {
              sum[5] += 2.56638837;
            } else {
              sum[5] += -0.146667749;
            }
          }
        }
      }
    } else {
      if (sample[3] <17.9999981) {
        if (sample[4] <178) {
          if (sample[4] <151) {
            if (sample[4] <120) {
              sum[5] += -0.58320564;
            } else {
              sum[5] += -0.536326885;
            }
          } else {
            if (sample[2] <79) {
              sum[5] += -0.546857238;
            } else {
              sum[5] += -0.267570913;
            }
          }
        } else {
          if (sample[2] <96) {
            if (sample[2] <86) {
              sum[5] += -0.502208173;
            } else {
              sum[5] += -0.0217739809;
            }
          } else {
            if (sample[1] <68) {
              sum[5] += 2.21495867;
            } else {
              sum[5] += 0.299863786;
            }
          }
        }
      } else {
        if (sample[3] <179) {
          sum[5] += -0.583313704;
        } else {
          if (sample[4] <78) {
            if (sample[5] <85) {
              sum[5] += -0.261410803;
            } else {
              sum[5] += -0.574859142;
            }
          } else {
            if (sample[0] <73) {
              sum[5] += -0.539376736;
            } else {
              sum[5] += 1.62790692;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <82) {
      if (sample[2] <72) {
        if (sample[2] <68) {
          if (sample[2] <61) {
            if (sample[2] <57) {
              sum[5] += -0.583318353;
            } else {
              sum[5] += -0.561275423;
            }
          } else {
            if (sample[1] <35) {
              sum[5] += -0.325697392;
            } else {
              sum[5] += -0.575127542;
            }
          }
        } else {
          if (sample[1] <30.9999981) {
            if (sample[1] <24.9999981) {
              sum[5] += -0.201438859;
            } else {
              sum[5] += 1.86233652;
            }
          } else {
            if (sample[3] <14.999999) {
              sum[5] += -0.00379920285;
            } else {
              sum[5] += -0.55668515;
            }
          }
        }
      } else {
        if (sample[1] <35) {
          if (sample[3] <11.999999) {
            if (sample[1] <24.9999981) {
              sum[5] += 2.161273;
            } else {
              sum[5] += 2.93534231;
            }
          } else {
            if (sample[2] <74) {
              sum[5] += 1.01428139;
            } else {
              sum[5] += 2.33708763;
            }
          }
        } else {
          if (sample[3] <14.999999) {
            if (sample[5] <77) {
              sum[5] += -1.68920042e-08;
            } else {
              sum[5] += 1.13500106;
            }
          } else {
            if (sample[1] <42) {
              sum[5] += -0.327377826;
            } else {
              sum[5] += -0.55212009;
            }
          }
        }
      }
    } else {
      if (sample[3] <17.9999981) {
        if (sample[3] <14.999999) {
          if (sample[2] <89) {
            if (sample[1] <35) {
              sum[5] += 3.27339411;
            } else {
              sum[5] += 2.16883922;
            }
          } else {
            if (sample[3] <11.999999) {
              sum[5] += 3.4936626;
            } else {
              sum[5] += 3.39841628;
            }
          }
        } else {
          if (sample[2] <100) {
            if (sample[2] <93) {
              sum[5] += 0.137435392;
            } else {
              sum[5] += 1.15068483;
            }
          } else {
            if (sample[1] <56) {
              sum[5] += 3.24739742;
            } else {
              sum[5] += 2.77639771;
            }
          }
        }
      } else {
        if (sample[3] <19.9999981) {
          if (sample[1] <53) {
            sum[5] += -0.545543075;
          } else {
            if (sample[1] <58) {
              sum[5] += -0.175750166;
            } else {
              sum[5] += -0.492462307;
            }
          }
        } else {
          sum[5] += -0.582245708;
        }
      }
    }
  }


  if (sample[4] <120) {
    if (sample[4] <67) {
      if (sample[4] <56) {
        if (sample[4] <45) {
          sum[6] += -0.583275437;
        } else {
          if (sample[1] <97) {
            if (sample[1] <93) {
              sum[6] += -0.583309233;
            } else {
              sum[6] += -0.558432758;
            }
          } else {
            if (sample[0] <85) {
              sum[6] += -0.0919606015;
            } else {
              sum[6] += -0.566559076;
            }
          }
        }
      } else {
        if (sample[2] <96) {
          if (sample[2] <93) {
            if (sample[2] <89) {
              sum[6] += -0.583271623;
            } else {
              sum[6] += -0.56125617;
            }
          } else {
            if (sample[1] <93) {
              sum[6] += -0.581406295;
            } else {
              sum[6] += -0.385582238;
            }
          }
        } else {
          if (sample[5] <109) {
            if (sample[1] <100) {
              sum[6] += -0.416214198;
            } else {
              sum[6] += 1.6208024;
            }
          } else {
            if (sample[2] <163) {
              sum[6] += -0.537023425;
            } else {
              sum[6] += 0.202042818;
            }
          }
        }
      }
    } else {
      if (sample[2] <173) {
        if (sample[3] <30.9999981) {
          if (sample[1] <100) {
            if (sample[1] <97) {
              sum[6] += -0.569518924;
            } else {
              sum[6] += 0.273920357;
            }
          } else {
            if (sample[3] <23.9999981) {
              sum[6] += 0.296511382;
            } else {
              sum[6] += 2.1474967;
            }
          }
        } else {
          if (sample[3] <38) {
            if (sample[2] <93) {
              sum[6] += -0.581083417;
            } else {
              sum[6] += -0.256281197;
            }
          } else {
            sum[6] += -0.583078206;
          }
        }
      } else {
        if (sample[0] <137) {
          if (sample[3] <19.9999981) {
            sum[6] += -0.562893569;
          } else {
            if (sample[0] <114) {
              sum[6] += 0.170761004;
            } else {
              sum[6] += 3.04589081;
            }
          }
        } else {
          if (sample[0] <138) {
            if (sample[1] <184) {
              sum[6] += 0.698544621;
            } else {
              sum[6] += -0.504155099;
            }
          } else {
            if (sample[0] <140) {
              sum[6] += -0.375080705;
            } else {
              sum[6] += -0.581585824;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <204) {
      if (sample[1] <93) {
        if (sample[1] <87) {
          if (sample[1] <76) {
            if (sample[1] <73) {
              sum[6] += -0.583083928;
            } else {
              sum[6] += -0.534182787;
            }
          } else {
            if (sample[5] <111) {
              sum[6] += -0.216438368;
            } else {
              sum[6] += -0.582926273;
            }
          }
        } else {
          if (sample[3] <30.9999981) {
            if (sample[3] <23.9999981) {
              sum[6] += -0.518572092;
            } else {
              sum[6] += 2.85540009;
            }
          } else {
            if (sample[3] <38) {
              sum[6] += -0.0998573601;
            } else {
              sum[6] += -0.582511008;
            }
          }
        }
      } else {
        if (sample[3] <30.9999981) {
          if (sample[3] <19.9999981) {
            if (sample[3] <17.9999981) {
              sum[6] += -0.520190835;
            } else {
              sum[6] += 1.26825583;
            }
          } else {
            if (sample[4] <151) {
              sum[6] += 2.77647138;
            } else {
              sum[6] += 3.45796657;
            }
          }
        } else {
          if (sample[3] <38) {
            if (sample[2] <100) {
              sum[6] += -0.533854127;
            } else {
              sum[6] += -0.364399284;
            }
          } else {
            sum[6] += -0.582524955;
          }
        }
      }
    } else {
      if (sample[3] <19.9999981) {
        sum[6] += -0.582321048;
      } else {
        if (sample[0] <124) {
          if (sample[3] <23.9999981) {
            if (sample[0] <106) {
              sum[6] += 3.34907579;
            } else {
              sum[6] += 0.25454542;
            }
          } else {
            sum[6] += 3.49650502;
          }
        } else {
          if (sample[0] <127) {
            sum[6] += 1.10526311;
          } else {
            sum[6] += -0.32110092;
          }
        }
      }
    }
  }


  return sum;
}
}
