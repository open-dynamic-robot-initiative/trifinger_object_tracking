#include <iostream>
#include <fstream>
#include <vector>

#include <trifinger_object_tracking/color_segmenter.hpp>

namespace trifinger_object_tracking {
std::array<float, XGB_NUM_CLASSES> xgb_classify_cube_v1(
        std::array<float, XGB_NUM_FEATURES> &sample) {

  std::array<float, XGB_NUM_CLASSES> sum;
  sum.fill(0.0);

  if (sample[4] <235) {
    if (sample[3] <162) {
      if (sample[4] <174) {
        if (sample[4] <142) {
          if (sample[4] <105) {
            sum[0] += 3.49965096;
          } else {
            if (sample[0] <49) {
              sum[0] += 3.49364305;
            } else {
              sum[0] += 3.41167545;
            }
          }
        } else {
          if (sample[0] <52) {
            if (sample[3] <14.999999) {
              sum[0] += 2.96999526;
            } else {
              sum[0] += 3.48441577;
            }
          } else {
            if (sample[3] <88) {
              sum[0] += 3.1216495;
            } else {
              sum[0] += 2.70985818;
            }
          }
        }
      } else {
        if (sample[5] <65) {
          if (sample[0] <49) {
            if (sample[0] <34) {
              sum[0] += 3.49600673;
            } else {
              sum[0] += 3.23305202;
            }
          } else {
            if (sample[1] <52) {
              sum[0] += 2.69915366;
            } else {
              sum[0] += 0.555397391;
            }
          }
        } else {
          if (sample[1] <107) {
            if (sample[2] <26.9999981) {
              sum[0] += 1.14262915;
            } else {
              sum[0] += 2.69821239;
            }
          } else {
            if (sample[3] <30.9999981) {
              sum[0] += 2.4551394;
            } else {
              sum[0] += -0.0444328077;
            }
          }
        }
      }
    } else {
      if (sample[4] <142) {
        if (sample[4] <105) {
          if (sample[4] <90) {
            sum[0] += 3.49376702;
          } else {
            if (sample[2] <123) {
              sum[0] += 3.09467435;
            } else {
              sum[0] += 2.4704349;
            }
          }
        } else {
          if (sample[2] <120) {
            if (sample[2] <63) {
              sum[0] += 3.32813144;
            } else {
              sum[0] += 2.38624096;
            }
          } else {
            if (sample[1] <96) {
              sum[0] += 0.588173389;
            } else {
              sum[0] += 3.02607226;
            }
          }
        }
      } else {
        if (sample[5] <68) {
          if (sample[5] <60) {
            if (sample[2] <45) {
              sum[0] += 3.47867155;
            } else {
              sum[0] += 2.76842737;
            }
          } else {
            if (sample[0] <25.9999981) {
              sum[0] += 1.0138768;
            } else {
              sum[0] += 1.70368755;
            }
          }
        } else {
          if (sample[2] <76) {
            if (sample[4] <174) {
              sum[0] += 1.1192801;
            } else {
              sum[0] += -0.0809754729;
            }
          } else {
            if (sample[2] <140) {
              sum[0] += -0.475870162;
            } else {
              sum[0] += -0.570258677;
            }
          }
        }
      }
    }
  } else {
    if (sample[5] <63) {
      if (sample[0] <49) {
        if (sample[0] <44) {
          if (sample[0] <34) {
            sum[0] += 3.49532294;
          } else {
            if (sample[3] <108) {
              sum[0] += 3.09685159;
            } else {
              sum[0] += 3.41303158;
            }
          }
        } else {
          if (sample[4] <255) {
            if (sample[1] <57) {
              sum[0] += 3.24041843;
            } else {
              sum[0] += 0.834196866;
            }
          } else {
            if (sample[3] <98) {
              sum[0] += 2.5269773;
            } else {
              sum[0] += 1.4237287;
            }
          }
        }
      } else {
        if (sample[4] <255) {
          if (sample[1] <52) {
            if (sample[3] <110) {
              sum[0] += 2.31758833;
            } else {
              sum[0] += 3.06643057;
            }
          } else {
            if (sample[0] <52) {
              sum[0] += 1.35820889;
            } else {
              sum[0] += 0.560887218;
            }
          }
        } else {
          if (sample[2] <15.999999) {
            if (sample[3] <101) {
              sum[0] += 0.437563419;
            } else {
              sum[0] += -0.23007144;
            }
          } else {
            sum[0] += 3.15622067;
          }
        }
      }
    } else {
      if (sample[5] <95) {
        if (sample[3] <53) {
          if (sample[1] <25.9999981) {
            if (sample[2] <88) {
              sum[0] += 2.81752968;
            } else {
              sum[0] += 0.995222569;
            }
          } else {
            if (sample[3] <50) {
              sum[0] += 3.44812655;
            } else {
              sum[0] += 1.97062409;
            }
          }
        } else {
          if (sample[3] <110) {
            if (sample[3] <90) {
              sum[0] += 0.051456105;
            } else {
              sum[0] += -0.41882062;
            }
          } else {
            if (sample[2] <74) {
              sum[0] += 0.869058073;
            } else {
              sum[0] += -0.277825862;
            }
          }
        }
      } else {
        if (sample[5] <126) {
          if (sample[3] <30.9999981) {
            if (sample[1] <25.9999981) {
              sum[0] += -0.152445823;
            } else {
              sum[0] += 2.59523726;
            }
          } else {
            if (sample[3] <53) {
              sum[0] += -0.119218998;
            } else {
              sum[0] += -0.458424926;
            }
          }
        } else {
          if (sample[3] <110) {
            if (sample[5] <175) {
              sum[0] += -0.514339745;
            } else {
              sum[0] += -0.575347364;
            }
          } else {
            if (sample[3] <162) {
              sum[0] += 2.02335119;
            } else {
              sum[0] += -0.2989209;
            }
          }
        }
      }
    }
  }


  if (sample[2] <7.99999905) {
    if (sample[3] <101) {
      if (sample[3] <98) {
        if (sample[3] <97) {
          if (sample[3] <80) {
            sum[1] += -0.583028257;
          } else {
            if (sample[1] <54) {
              sum[1] += -0.479809195;
            } else {
              sum[1] += -0.575630486;
            }
          }
        } else {
          if (sample[1] <25.9999981) {
            sum[1] += -0.571368754;
          } else {
            if (sample[0] <71) {
              sum[1] += 0.069733724;
            } else {
              sum[1] += -0.413224608;
            }
          }
        }
      } else {
        if (sample[1] <25.9999981) {
          if (sample[5] <35) {
            sum[1] += -0.558419049;
          } else {
            if (sample[0] <38) {
              sum[1] += -0.225305215;
            } else {
              sum[1] += 0.386740297;
            }
          }
        } else {
          if (sample[4] <255) {
            if (sample[0] <38) {
              sum[1] += 1.38761961;
            } else {
              sum[1] += -0.277484566;
            }
          } else {
            if (sample[3] <100) {
              sum[1] += 0.69740057;
            } else {
              sum[1] += 2.02082992;
            }
          }
        }
      }
    } else {
      if (sample[0] <49) {
        if (sample[0] <44) {
          if (sample[1] <15.999999) {
            if (sample[5] <42) {
              sum[1] += -0.563308954;
            } else {
              sum[1] += -0.403332174;
            }
          } else {
            if (sample[4] <255) {
              sum[1] += -0.454940557;
            } else {
              sum[1] += -0.206361532;
            }
          }
        } else {
          if (sample[4] <255) {
            if (sample[3] <111) {
              sum[1] += -0.0240446702;
            } else {
              sum[1] += -0.529157639;
            }
          } else {
            if (sample[3] <108) {
              sum[1] += 2.15808439;
            } else {
              sum[1] += 0.324402571;
            }
          }
        }
      } else {
        if (sample[3] <111) {
          if (sample[4] <235) {
            if (sample[1] <34) {
              sum[1] += 0.302158237;
            } else {
              sum[1] += 0.877837777;
            }
          } else {
            if (sample[3] <103) {
              sum[1] += 2.81947494;
            } else {
              sum[1] += 3.43537307;
            }
          }
        } else {
          if (sample[4] <255) {
            if (sample[5] <95) {
              sum[1] += 0.406122655;
            } else {
              sum[1] += 2.59978127;
            }
          } else {
            if (sample[1] <38) {
              sum[1] += 2.66980958;
            } else {
              sum[1] += -0.0580912977;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <26.9999981) {
      if (sample[3] <100) {
        if (sample[3] <97) {
          if (sample[3] <85) {
            sum[1] += -0.582423627;
          } else {
            if (sample[1] <110) {
              sum[1] += -0.550281823;
            } else {
              sum[1] += -0.582885921;
            }
          }
        } else {
          if (sample[4] <174) {
            if (sample[1] <46) {
              sum[1] += -0.553426743;
            } else {
              sum[1] += -0.395178527;
            }
          } else {
            if (sample[3] <99) {
              sum[1] += -0.350989312;
            } else {
              sum[1] += -0.0736423209;
            }
          }
        }
      } else {
        if (sample[0] <63) {
          if (sample[5] <53) {
            if (sample[5] <46) {
              sum[1] += -0.566625118;
            } else {
              sum[1] += -0.413343191;
            }
          } else {
            if (sample[2] <15.999999) {
              sum[1] += 0.570592165;
            } else {
              sum[1] += -0.097211346;
            }
          }
        } else {
          if (sample[3] <104) {
            if (sample[3] <102) {
              sum[1] += -1.72251937e-08;
            } else {
              sum[1] += 0.427686363;
            }
          } else {
            if (sample[1] <57) {
              sum[1] += 0.79704535;
            } else {
              sum[1] += 2.6449163;
            }
          }
        }
      }
    } else {
      if (sample[4] <120) {
        sum[1] += -0.583249569;
      } else {
        if (sample[0] <79) {
          if (sample[0] <52) {
            sum[1] += -0.583132505;
          } else {
            if (sample[2] <34) {
              sum[1] += -0.501312256;
            } else {
              sum[1] += -0.57755363;
            }
          }
        } else {
          if (sample[2] <45) {
            if (sample[3] <101) {
              sum[1] += -0.484455675;
            } else {
              sum[1] += 0.0863112733;
            }
          } else {
            if (sample[2] <48) {
              sum[1] += -0.447875619;
            } else {
              sum[1] += -0.5802145;
            }
          }
        }
      }
    }
  }


  if (sample[2] <7.99999905) {
    if (sample[0] <52) {
      if (sample[0] <41) {
        if (sample[0] <34) {
          sum[2] += -0.582904458;
        } else {
          if (sample[1] <49) {
            if (sample[1] <34) {
              sum[2] += -0.578565776;
            } else {
              sum[2] += -0.503978372;
            }
          } else {
            if (sample[1] <90) {
              sum[2] += -0.29376626;
            } else {
              sum[2] += -0.572360814;
            }
          }
        }
      } else {
        if (sample[1] <43) {
          if (sample[3] <99) {
            if (sample[5] <51) {
              sum[2] += -0.444169521;
            } else {
              sum[2] += 0.135396495;
            }
          } else {
            sum[2] += -0.57512027;
          }
        } else {
          if (sample[1] <67) {
            if (sample[1] <49) {
              sum[2] += -0.0416611694;
            } else {
              sum[2] += 0.957309604;
            }
          } else {
            if (sample[1] <78) {
              sum[2] += -0.105197258;
            } else {
              sum[2] += -0.535721838;
            }
          }
        }
      }
    } else {
      if (sample[3] <98) {
        if (sample[3] <88) {
          if (sample[1] <69) {
            if (sample[0] <57) {
              sum[2] += 1.95989192;
            } else {
              sum[2] += 3.02786827;
            }
          } else {
            if (sample[3] <85) {
              sum[2] += -0.138768166;
            } else {
              sum[2] += 0.814298272;
            }
          }
        } else {
          if (sample[1] <52) {
            if (sample[3] <94) {
              sum[2] += 1.41559005;
            } else {
              sum[2] += 0.0769772828;
            }
          } else {
            if (sample[3] <97) {
              sum[2] += 3.37971115;
            } else {
              sum[2] += 1.83684635;
            }
          }
        }
      } else {
        if (sample[3] <100) {
          if (sample[1] <46) {
            if (sample[0] <57) {
              sum[2] += -0.476871312;
            } else {
              sum[2] += -0.270187914;
            }
          } else {
            if (sample[0] <144) {
              sum[2] += 0.189018875;
            } else {
              sum[2] += 1.55017292;
            }
          }
        } else {
          if (sample[3] <103) {
            if (sample[1] <40) {
              sum[2] += -0.557806432;
            } else {
              sum[2] += -0.424058855;
            }
          } else {
            sum[2] += -0.580755353;
          }
        }
      }
    }
  } else {
    if (sample[4] <142) {
      if (sample[4] <120) {
        if (sample[4] <90) {
          sum[2] += -0.583276153;
        } else {
          if (sample[3] <85) {
            sum[2] += -0.58216846;
          } else {
            if (sample[3] <109) {
              sum[2] += -0.559930027;
            } else {
              sum[2] += -0.580305815;
            }
          }
        }
      } else {
        if (sample[3] <83) {
          if (sample[3] <78) {
            sum[2] += -0.582258523;
          } else {
            if (sample[0] <49) {
              sum[2] += -0.577616274;
            } else {
              sum[2] += -0.521125019;
            }
          }
        } else {
          if (sample[1] <52) {
            sum[2] += -0.570540428;
          } else {
            if (sample[2] <36) {
              sum[2] += -0.35800609;
            } else {
              sum[2] += -0.520168126;
            }
          }
        }
      }
    } else {
      if (sample[0] <123) {
        if (sample[0] <52) {
          if (sample[0] <49) {
            if (sample[0] <38) {
              sum[2] += -0.583081782;
            } else {
              sum[2] += -0.562027454;
            }
          } else {
            if (sample[2] <15.999999) {
              sum[2] += 0.139829457;
            } else {
              sum[2] += -0.54536581;
            }
          }
        } else {
          if (sample[2] <26.9999981) {
            if (sample[3] <98) {
              sum[2] += 1.88858771;
            } else {
              sum[2] += -0.392981976;
            }
          } else {
            if (sample[2] <52) {
              sum[2] += -0.236407369;
            } else {
              sum[2] += -0.581757188;
            }
          }
        }
      } else {
        if (sample[3] <98) {
          if (sample[2] <52) {
            if (sample[3] <90) {
              sum[2] += 1.39562714;
            } else {
              sum[2] += 3.39506865;
            }
          } else {
            if (sample[2] <63) {
              sum[2] += 0.962764621;
            } else {
              sum[2] += -0.551758528;
            }
          }
        } else {
          if (sample[3] <102) {
            if (sample[2] <61) {
              sum[2] += 0.741721809;
            } else {
              sum[2] += -0.552026272;
            }
          } else {
            if (sample[3] <105) {
              sum[2] += -0.281879187;
            } else {
              sum[2] += -0.56752491;
            }
          }
        }
      }
    }
  }


  if (sample[2] <7.99999905) {
    if (sample[1] <83) {
      if (sample[1] <78) {
        if (sample[1] <76) {
          if (sample[1] <69) {
            if (sample[1] <49) {
              sum[3] += -0.583323061;
            } else {
              sum[3] += -0.571019292;
            }
          } else {
            if (sample[3] <72) {
              sum[3] += 2.7713151;
            } else {
              sum[3] += -0.580906689;
            }
          }
        } else {
          if (sample[0] <25.9999981) {
            if (sample[4] <255) {
              sum[3] += 1.02752292;
            } else {
              sum[3] += 3.25343084;
            }
          } else {
            if (sample[0] <34) {
              sum[3] += 0.736842096;
            } else {
              sum[3] += -0.580666244;
            }
          }
        }
      } else {
        if (sample[0] <34) {
          sum[3] += 3.43496799;
        } else {
          if (sample[0] <44) {
            if (sample[1] <80) {
              sum[3] += -0.0482758731;
            } else {
              sum[3] += 0.818867862;
            }
          } else {
            if (sample[0] <57) {
              sum[3] += -0.413422823;
            } else {
              sum[3] += -0.581726134;
            }
          }
        }
      }
    } else {
      if (sample[3] <75) {
        if (sample[3] <72) {
          sum[3] += 3.49734855;
        } else {
          if (sample[1] <110) {
            if (sample[1] <86) {
              sum[3] += 0.0725388452;
            } else {
              sum[3] += 1.03039992;
            }
          } else {
            sum[3] += 2.74890494;
          }
        }
      } else {
        if (sample[0] <52) {
          if (sample[5] <85) {
            if (sample[0] <49) {
              sum[3] += 0.0725388452;
            } else {
              sum[3] += -0.32110092;
            }
          } else {
            if (sample[1] <93) {
              sum[3] += 0.509090841;
            } else {
              sum[3] += -0.0482758731;
            }
          }
        } else {
          if (sample[3] <78) {
            if (sample[0] <63) {
              sum[3] += -1.58442717e-08;
            } else {
              sum[3] += -0.465960652;
            }
          } else {
            sum[3] += -0.583253145;
          }
        }
      }
    }
  } else {
    if (sample[4] <174) {
      sum[3] += -0.583274901;
    } else {
      if (sample[1] <74) {
        if (sample[1] <69) {
          sum[3] += -0.583043575;
        } else {
          if (sample[5] <75) {
            if (sample[2] <34) {
              sum[3] += -0.29402262;
            } else {
              sum[3] += -0.580232143;
            }
          } else {
            sum[3] += -0.582604229;
          }
        }
      } else {
        if (sample[3] <47) {
          sum[3] += -0.583050609;
        } else {
          if (sample[3] <70) {
            if (sample[3] <50) {
              sum[3] += -0.216718271;
            } else {
              sum[3] += 2.61570215;
            }
          } else {
            if (sample[0] <63) {
              sum[3] += 0.0674698576;
            } else {
              sum[3] += -0.583110809;
            }
          }
        }
      }
    }
  }


  if (sample[3] <162) {
    if (sample[3] <14.999999) {
      if (sample[2] <99) {
        if (sample[4] <105) {
          if (sample[4] <79) {
            if (sample[4] <60) {
              sum[4] += -0.581845701;
            } else {
              sum[4] += -0.55707258;
            }
          } else {
            if (sample[1] <38) {
              sum[4] += -0.565022409;
            } else {
              sum[4] += -0.475270599;
            }
          }
        } else {
          if (sample[2] <65) {
            if (sample[2] <58) {
              sum[4] += -0.561189353;
            } else {
              sum[4] += -0.32302475;
            }
          } else {
            if (sample[1] <34) {
              sum[4] += 0.777689636;
            } else {
              sum[4] += -0.359524727;
            }
          }
        }
      } else {
        if (sample[4] <60) {
          sum[4] += -0.583047748;
        } else {
          if (sample[4] <235) {
            if (sample[1] <43) {
              sum[4] += 0.639188945;
            } else {
              sum[4] += -0.46041733;
            }
          } else {
            if (sample[2] <122) {
              sum[4] += -0.427367061;
            } else {
              sum[4] += -0.58126384;
            }
          }
        }
      }
    } else {
      if (sample[3] <135) {
        sum[4] += -0.58323133;
      } else {
        if (sample[4] <105) {
          if (sample[4] <79) {
            sum[4] += -0.582764387;
          } else {
            if (sample[2] <63) {
              sum[4] += -0.560567677;
            } else {
              sum[4] += -0.467271537;
            }
          }
        } else {
          if (sample[2] <63) {
            if (sample[2] <58) {
              sum[4] += -0.572015941;
            } else {
              sum[4] += -0.32041502;
            }
          } else {
            if (sample[0] <85) {
              sum[4] += 0.307317048;
            } else {
              sum[4] += -0.26087296;
            }
          }
        }
      }
    }
  } else {
    if (sample[4] <142) {
      if (sample[4] <105) {
        if (sample[4] <90) {
          if (sample[4] <70) {
            if (sample[4] <51) {
              sum[4] += -0.582525909;
            } else {
              sum[4] += -0.552538097;
            }
          } else {
            if (sample[2] <121) {
              sum[4] += -0.469915211;
            } else {
              sum[4] += -0.24839291;
            }
          }
        } else {
          if (sample[2] <56) {
            if (sample[3] <179) {
              sum[4] += -0.537486434;
            } else {
              sum[4] += -0.0891719833;
            }
          } else {
            if (sample[2] <170) {
              sum[4] += 0.175947756;
            } else {
              sum[4] += -0.553213894;
            }
          }
        }
      } else {
        if (sample[2] <120) {
          if (sample[2] <56) {
            if (sample[5] <53) {
              sum[4] += -0.568507552;
            } else {
              sum[4] += -0.10918545;
            }
          } else {
            if (sample[1] <49) {
              sum[4] += 0.649709463;
            } else {
              sum[4] += 0.00424112147;
            }
          }
        } else {
          if (sample[1] <96) {
            if (sample[4] <120) {
              sum[4] += 1.38557315;
            } else {
              sum[4] += 2.61398745;
            }
          } else {
            if (sample[5] <175) {
              sum[4] += 0.627586186;
            } else {
              sum[4] += -0.467520565;
            }
          }
        }
      }
    } else {
      if (sample[5] <68) {
        if (sample[5] <60) {
          if (sample[2] <45) {
            sum[4] += -0.582127929;
          } else {
            if (sample[4] <255) {
              sum[4] += 0.133897349;
            } else {
              sum[4] += -0.29594627;
            }
          }
        } else {
          if (sample[4] <255) {
            if (sample[0] <38) {
              sum[4] += 1.49983335;
            } else {
              sum[4] += 0.702241123;
            }
          } else {
            if (sample[0] <11.999999) {
              sum[4] += -0.351387054;
            } else {
              sum[4] += 0.170463443;
            }
          }
        }
      } else {
        if (sample[2] <205) {
          if (sample[4] <255) {
            if (sample[5] <75) {
              sum[4] += 2.69825196;
            } else {
              sum[4] += 3.44270968;
            }
          } else {
            if (sample[2] <118) {
              sum[4] += 2.96554828;
            } else {
              sum[4] += 1.39206886;
            }
          }
        } else {
          if (sample[1] <83) {
            sum[4] += -0.571718812;
          } else {
            sum[4] += -1.09798028e-08;
          }
        }
      }
    }
  }


  if (sample[2] <255) {
    if (sample[1] <15.999999) {
      if (sample[2] <117) {
        if (sample[2] <99) {
          if (sample[2] <88) {
            if (sample[5] <75) {
              sum[5] += -0.583057165;
            } else {
              sum[5] += -0.528164625;
            }
          } else {
            if (sample[0] <6.99999905) {
              sum[5] += 0.62627399;
            } else {
              sum[5] += -0.568279922;
            }
          }
        } else {
          if (sample[3] <14.999999) {
            if (sample[4] <255) {
              sum[5] += 1.11401415;
            } else {
              sum[5] += 2.70367098;
            }
          } else {
            if (sample[0] <19.9999981) {
              sum[5] += -0.171295032;
            } else {
              sum[5] += -0.568281472;
            }
          }
        }
      } else {
        if (sample[0] <29.9999981) {
          if (sample[3] <14.999999) {
            if (sample[2] <134) {
              sum[5] += 3.32189608;
            } else {
              sum[5] += 3.48750615;
            }
          } else {
            if (sample[2] <134) {
              sum[5] += 0.599840522;
            } else {
              sum[5] += 3.07803154;
            }
          }
        } else {
          if (sample[2] <205) {
            if (sample[0] <52) {
              sum[5] += 0.262087733;
            } else {
              sum[5] += -0.553426743;
            }
          } else {
            if (sample[0] <76) {
              sum[5] += 3.19918132;
            } else {
              sum[5] += 0.12844035;
            }
          }
        }
      }
    } else {
      if (sample[3] <14.999999) {
        if (sample[4] <235) {
          if (sample[4] <208) {
            if (sample[4] <142) {
              sum[5] += -0.582763851;
            } else {
              sum[5] += -0.421477467;
            }
          } else {
            if (sample[2] <124) {
              sum[5] += -0.455087185;
            } else {
              sum[5] += 1.71768963;
            }
          }
        } else {
          if (sample[2] <107) {
            if (sample[2] <95) {
              sum[5] += -0.540513992;
            } else {
              sum[5] += 0.539451838;
            }
          } else {
            if (sample[2] <123) {
              sum[5] += 1.88332105;
            } else {
              sum[5] += 3.2983849;
            }
          }
        }
      } else {
        if (sample[3] <179) {
          if (sample[3] <162) {
            sum[5] += -0.583289564;
          } else {
            if (sample[0] <52) {
              sum[5] += -0.546771884;
            } else {
              sum[5] += -0.581627667;
            }
          }
        } else {
          if (sample[4] <208) {
            if (sample[2] <109) {
              sum[5] += -0.552145541;
            } else {
              sum[5] += -0.302647561;
            }
          } else {
            if (sample[2] <138) {
              sum[5] += 0.392931372;
            } else {
              sum[5] += 3.00859594;
            }
          }
        }
      }
    }
  } else {
    if (sample[1] <124) {
      sum[5] += 3.49952722;
    } else {
      sum[5] += -0.58276552;
    }
  }


  if (sample[0] <6.99999905) {
    if (sample[1] <114) {
      if (sample[1] <107) {
        if (sample[1] <100) {
          if (sample[1] <90) {
            if (sample[1] <76) {
              sum[6] += -0.583305895;
            } else {
              sum[6] += -0.554143667;
            }
          } else {
            if (sample[2] <39) {
              sum[6] += -0.582802236;
            } else {
              sum[6] += -0.25685364;
            }
          }
        } else {
          if (sample[2] <58) {
            if (sample[3] <45) {
              sum[6] += -0.0482758731;
            } else {
              sum[6] += -0.58244586;
            }
          } else {
            if (sample[3] <30.9999981) {
              sum[6] += -0.424164057;
            } else {
              sum[6] += 1.60830784;
            }
          }
        }
      } else {
        if (sample[2] <56) {
          if (sample[2] <42) {
            sum[6] += -0.582342923;
          } else {
            sum[6] += 0.22292991;
          }
        } else {
          if (sample[2] <117) {
            if (sample[2] <74) {
              sum[6] += 0.797468305;
            } else {
              sum[6] += 2.80943274;
            }
          } else {
            if (sample[2] <152) {
              sum[6] += 0.0572987534;
            } else {
              sum[6] += -0.575960755;
            }
          }
        }
      }
    } else {
      if (sample[2] <69) {
        if (sample[2] <58) {
          if (sample[3] <53) {
            if (sample[3] <50) {
              sum[6] += 0.0314606577;
            } else {
              sum[6] += -0.353238016;
            }
          } else {
            sum[6] += -0.583139479;
          }
        } else {
          if (sample[1] <199) {
            if (sample[3] <45) {
              sum[6] += -0.0580912977;
            } else {
              sum[6] += 1.55867589;
            }
          } else {
            if (sample[2] <61) {
              sum[6] += 0.0341463275;
            } else {
              sum[6] += -0.443902433;
            }
          }
        }
      } else {
        if (sample[3] <25.9999981) {
          if (sample[5] <148) {
            if (sample[1] <116) {
              sum[6] += 0.867768586;
            } else {
              sum[6] += -0.124260366;
            }
          } else {
            sum[6] += -0.561273813;
          }
        } else {
          if (sample[3] <30.9999981) {
            if (sample[1] <184) {
              sum[6] += 1.85029113;
            } else {
              sum[6] += 3.15997052;
            }
          } else {
            if (sample[2] <88) {
              sum[6] += 2.14621687;
            } else {
              sum[6] += 3.46757841;
            }
          }
        }
      }
    }
  } else {
    if (sample[4] <174) {
      if (sample[4] <142) {
        sum[6] += -0.583255887;
      } else {
        if (sample[1] <100) {
          if (sample[1] <83) {
            sum[6] += -0.583218396;
          } else {
            if (sample[0] <44) {
              sum[6] += -0.458729357;
            } else {
              sum[6] += -0.579729676;
            }
          }
        } else {
          if (sample[3] <50) {
            if (sample[3] <25.9999981) {
              sum[6] += -0.50783515;
            } else {
              sum[6] += 0.0893083885;
            }
          } else {
            if (sample[3] <53) {
              sum[6] += -0.363369256;
            } else {
              sum[6] += -0.580881655;
            }
          }
        }
      }
    } else {
      if (sample[1] <174) {
        if (sample[1] <100) {
          if (sample[1] <86) {
            sum[6] += -0.583236635;
          } else {
            if (sample[2] <69) {
              sum[6] += -0.582888365;
            } else {
              sum[6] += -0.303189248;
            }
          }
        } else {
          if (sample[2] <76) {
            if (sample[2] <69) {
              sum[6] += -0.583149612;
            } else {
              sum[6] += 0.20121026;
            }
          } else {
            if (sample[3] <30.9999981) {
              sum[6] += 0.0646153614;
            } else {
              sum[6] += 2.51127076;
            }
          }
        }
      } else {
        if (sample[3] <50) {
          if (sample[3] <25.9999981) {
            sum[6] += -0.528046429;
          } else {
            if (sample[0] <66) {
              sum[6] += 3.01119328;
            } else {
              sum[6] += 0.615384579;
            }
          }
        } else {
          if (sample[3] <53) {
            if (sample[5] <209) {
              sum[6] += 0.745562077;
            } else {
              sum[6] += -0.443902433;
            }
          } else {
            sum[6] += -0.580626845;
          }
        }
      }
    }
  }


  return sum;
}
}
