#include <iostream>
#include <fstream>
#include <vector>

#include <trifinger_object_tracking/color_segmenter.hpp>

namespace trifinger_object_tracking {
std::array<float, XGB_NUM_CLASSES> xgb_classify_cube_v2(
        std::array<float, XGB_NUM_FEATURES> &sample) {

  std::array<float, XGB_NUM_CLASSES> sum;
  sum.fill(0.0);

  if (sample[4] <85) {
    if (sample[4] <73) {
      if (sample[4] <63) {
        if (sample[4] <56) {
          sum[0] += 3.49971128;
        } else {
          if (sample[3] <84) {
            sum[0] += 3.49603295;
          } else {
            if (sample[2] <74) {
              sum[0] += 3.41082644;
            } else {
              sum[0] += 3.02998114;
            }
          }
        }
      } else {
        if (sample[0] <93) {
          if (sample[3] <100) {
            if (sample[0] <90) {
              sum[0] += 3.49513483;
            } else {
              sum[0] += 3.02015162;
            }
          } else {
            if (sample[2] <70) {
              sum[0] += 3.24593997;
            } else {
              sum[0] += 1.55148768;
            }
          }
        } else {
          if (sample[3] <70) {
            sum[0] += 3.47853351;
          } else {
            if (sample[3] <95) {
              sum[0] += 0.596619666;
            } else {
              sum[0] += 2.72099733;
            }
          }
        }
      }
    } else {
      if (sample[0] <86) {
        if (sample[3] <133) {
          if (sample[3] <98) {
            if (sample[0] <78) {
              sum[0] += 3.49565125;
            } else {
              sum[0] += 2.87385678;
            }
          } else {
            if (sample[1] <43) {
              sum[0] += 3.315727;
            } else {
              sum[0] += 2.90229321;
            }
          }
        } else {
          if (sample[0] <70) {
            if (sample[2] <60) {
              sum[0] += 3.43414736;
            } else {
              sum[0] += 1.90469253;
            }
          } else {
            if (sample[3] <163) {
              sum[0] += -0.291080534;
            } else {
              sum[0] += 2.19942594;
            }
          }
        }
      } else {
        if (sample[3] <65) {
          if (sample[1] <95) {
            sum[0] += 1.02752292;
          } else {
            sum[0] += 3.45945621;
          }
        } else {
          if (sample[2] <68) {
            if (sample[1] <98) {
              sum[0] += 3.0273931;
            } else {
              sum[0] += 0.187221378;
            }
          } else {
            if (sample[3] <95) {
              sum[0] += -0.285285175;
            } else {
              sum[0] += 0.455409735;
            }
          }
        }
      }
    }
  } else {
    if (sample[5] <73) {
      if (sample[3] <95) {
        if (sample[3] <84) {
          if (sample[5] <66) {
            if (sample[3] <80) {
              sum[0] += 3.49814153;
            } else {
              sum[0] += 3.14161468;
            }
          } else {
            if (sample[1] <31.9999981) {
              sum[0] += 1.30080271;
            } else {
              sum[0] += 3.36031175;
            }
          }
        } else {
          if (sample[1] <31.9999981) {
            if (sample[4] <162) {
              sum[0] += 2.24457693;
            } else {
              sum[0] += -0.270013392;
            }
          } else {
            if (sample[2] <14.999999) {
              sum[0] += 1.88007677;
            } else {
              sum[0] += 3.3082757;
            }
          }
        }
      } else {
        if (sample[3] <113) {
          if (sample[4] <134) {
            if (sample[3] <103) {
              sum[0] += 2.79399824;
            } else {
              sum[0] += 0.723330557;
            }
          } else {
            if (sample[0] <13.999999) {
              sum[0] += 3.37925339;
            } else {
              sum[0] += -0.230451748;
            }
          }
        } else {
          if (sample[5] <60) {
            if (sample[3] <115) {
              sum[0] += 2.81401086;
            } else {
              sum[0] += 3.47262907;
            }
          } else {
            if (sample[2] <57) {
              sum[0] += 1.45607865;
            } else {
              sum[0] += -0.325177521;
            }
          }
        }
      }
    } else {
      if (sample[5] <84) {
        if (sample[3] <105) {
          if (sample[1] <34) {
            if (sample[4] <255) {
              sum[0] += 1.36229789;
            } else {
              sum[0] += -0.126468167;
            }
          } else {
            if (sample[1] <82) {
              sum[0] += 2.82626677;
            } else {
              sum[0] += 0.159378365;
            }
          }
        } else {
          if (sample[1] <54) {
            if (sample[4] <112) {
              sum[0] += -0.0896220356;
            } else {
              sum[0] += -0.572483122;
            }
          } else {
            if (sample[0] <83) {
              sum[0] += 2.04743075;
            } else {
              sum[0] += -0.0554675274;
            }
          }
        }
      } else {
        if (sample[4] <100) {
          if (sample[3] <75) {
            if (sample[3] <36) {
              sum[0] += 3.42316818;
            } else {
              sum[0] += 2.7879467;
            }
          } else {
            if (sample[2] <60) {
              sum[0] += 1.91580677;
            } else {
              sum[0] += -0.395401388;
            }
          }
        } else {
          if (sample[3] <23.9999981) {
            if (sample[3] <14.999999) {
              sum[0] += -0.428513139;
            } else {
              sum[0] += 1.78236389;
            }
          } else {
            if (sample[4] <134) {
              sum[0] += -0.418182343;
            } else {
              sum[0] += -0.569675803;
            }
          }
        }
      }
    }
  }


  if (sample[2] <14.999999) {
    if (sample[3] <84) {
      if (sample[3] <80) {
        if (sample[3] <75) {
          if (sample[3] <73) {
            if (sample[1] <54) {
              sum[1] += -0.582891583;
            } else {
              sum[1] += -0.0482758731;
            }
          } else {
            if (sample[1] <34) {
              sum[1] += -0.543201447;
            } else {
              sum[1] += -0.44265154;
            }
          }
        } else {
          if (sample[1] <31.9999981) {
            if (sample[1] <26.9999981) {
              sum[1] += -0.378651291;
            } else {
              sum[1] += -0.0385272503;
            }
          } else {
            if (sample[1] <43) {
              sum[1] += -0.47812739;
            } else {
              sum[1] += 0.834196866;
            }
          }
        }
      } else {
        if (sample[1] <31.9999981) {
          if (sample[4] <203) {
            if (sample[4] <162) {
              sum[1] += 0.0277502295;
            } else {
              sum[1] += 1.02293956;
            }
          } else {
            if (sample[1] <26.9999981) {
              sum[1] += 0.956033587;
            } else {
              sum[1] += 2.9586401;
            }
          }
        } else {
          if (sample[4] <255) {
            if (sample[4] <182) {
              sum[1] += -0.44262293;
            } else {
              sum[1] += -0.110399224;
            }
          } else {
            if (sample[1] <34) {
              sum[1] += -0.368421048;
            } else {
              sum[1] += 1.84210515;
            }
          }
        }
      }
    } else {
      if (sample[3] <115) {
        if (sample[0] <37) {
          if (sample[3] <107) {
            if (sample[4] <203) {
              sum[1] += 1.48357129;
            } else {
              sum[1] += 3.1757338;
            }
          } else {
            if (sample[4] <255) {
              sum[1] += -0.376814574;
            } else {
              sum[1] += 2.10606694;
            }
          }
        } else {
          if (sample[3] <98) {
            if (sample[1] <38) {
              sum[1] += 2.43033552;
            } else {
              sum[1] += 0.496369034;
            }
          } else {
            if (sample[5] <46) {
              sum[1] += 3.23186994;
            } else {
              sum[1] += 3.49156141;
            }
          }
        }
      } else {
        if (sample[0] <50) {
          if (sample[0] <47) {
            sum[1] += -0.576118529;
          } else {
            sum[1] += 0.12844035;
          }
        } else {
          sum[1] += 3.11460662;
        }
      }
    }
  } else {
    if (sample[4] <112) {
      if (sample[4] <85) {
        if (sample[4] <63) {
          if (sample[4] <49) {
            sum[1] += -0.583266556;
          } else {
            if (sample[3] <90) {
              sum[1] += -0.582780659;
            } else {
              sum[1] += -0.543683827;
            }
          }
        } else {
          if (sample[3] <98) {
            if (sample[3] <90) {
              sum[1] += -0.582401872;
            } else {
              sum[1] += -0.549822986;
            }
          } else {
            if (sample[3] <124) {
              sum[1] += -0.229287729;
            } else {
              sum[1] += -0.569238245;
            }
          }
        }
      } else {
        if (sample[3] <98) {
          if (sample[3] <95) {
            if (sample[3] <90) {
              sum[1] += -0.582405329;
            } else {
              sum[1] += -0.559824705;
            }
          } else {
            if (sample[1] <43) {
              sum[1] += 0.112952441;
            } else {
              sum[1] += -0.556291401;
            }
          }
        } else {
          if (sample[3] <120) {
            if (sample[3] <103) {
              sum[1] += -0.0611638464;
            } else {
              sum[1] += 1.44986749;
            }
          } else {
            if (sample[3] <123) {
              sum[1] += 0.151783139;
            } else {
              sum[1] += -0.580324113;
            }
          }
        }
      }
    } else {
      if (sample[3] <95) {
        if (sample[3] <90) {
          if (sample[3] <73) {
            sum[1] += -0.583161294;
          } else {
            if (sample[5] <27.9999981) {
              sum[1] += 1.4672488;
            } else {
              sum[1] += -0.566138387;
            }
          }
        } else {
          if (sample[2] <22.9999981) {
            if (sample[1] <54) {
              sum[1] += 0.0761130601;
            } else {
              sum[1] += 1.92830181;
            }
          } else {
            if (sample[2] <36) {
              sum[1] += -0.3547602;
            } else {
              sum[1] += -0.581660271;
            }
          }
        }
      } else {
        if (sample[3] <120) {
          if (sample[3] <103) {
            if (sample[2] <31.9999981) {
              sum[1] += 1.46276832;
            } else {
              sum[1] += -0.25512287;
            }
          } else {
            if (sample[5] <46) {
              sum[1] += 0.378785133;
            } else {
              sum[1] += 3.37826061;
            }
          }
        } else {
          if (sample[3] <128) {
            if (sample[5] <57) {
              sum[1] += -0.556939721;
            } else {
              sum[1] += 1.71244621;
            }
          } else {
            if (sample[3] <133) {
              sum[1] += -0.471035689;
            } else {
              sum[1] += -0.582321107;
            }
          }
        }
      }
    }
  }


  if (sample[4] <73) {
    if (sample[4] <63) {
      if (sample[4] <56) {
        if (sample[4] <48) {
          sum[2] += -0.583300352;
        } else {
          if (sample[1] <95) {
            sum[2] += -0.58313781;
          } else {
            if (sample[2] <94) {
              sum[2] += -0.438481987;
            } else {
              sum[2] += -0.571941376;
            }
          }
        }
      } else {
        if (sample[0] <96) {
          if (sample[0] <86) {
            sum[2] += -0.583018363;
          } else {
            if (sample[2] <84) {
              sum[2] += -0.416640699;
            } else {
              sum[2] += -0.58202678;
            }
          }
        } else {
          if (sample[2] <101) {
            if (sample[3] <95) {
              sum[2] += 0.667311966;
            } else {
              sum[2] += -0.559476495;
            }
          } else {
            if (sample[3] <75) {
              sum[2] += -0.581754506;
            } else {
              sum[2] += -0.443820596;
            }
          }
        }
      }
    } else {
      if (sample[1] <102) {
        if (sample[1] <98) {
          if (sample[1] <95) {
            if (sample[0] <76) {
              sum[2] += -0.583189845;
            } else {
              sum[2] += -0.559362471;
            }
          } else {
            if (sample[2] <74) {
              sum[2] += 0.0157598313;
            } else {
              sum[2] += -0.566120982;
            }
          }
        } else {
          if (sample[2] <78) {
            if (sample[0] <90) {
              sum[2] += -0.265768528;
            } else {
              sum[2] += 1.81870937;
            }
          } else {
            if (sample[2] <84) {
              sum[2] += 0.00669854786;
            } else {
              sum[2] += -0.580287695;
            }
          }
        }
      } else {
        if (sample[2] <98) {
          if (sample[3] <75) {
            if (sample[3] <65) {
              sum[2] += -0.542301416;
            } else {
              sum[2] += 0.4138394;
            }
          } else {
            if (sample[3] <95) {
              sum[2] += 2.67875338;
            } else {
              sum[2] += -0.217012212;
            }
          }
        } else {
          if (sample[3] <70) {
            if (sample[3] <62) {
              sum[2] += -0.582886457;
            } else {
              sum[2] += -0.230769232;
            }
          } else {
            if (sample[3] <95) {
              sum[2] += 1.1774677;
            } else {
              sum[2] += -0.347144544;
            }
          }
        }
      }
    }
  } else {
    if (sample[0] <100) {
      if (sample[0] <78) {
        if (sample[0] <65) {
          if (sample[0] <62) {
            if (sample[0] <55) {
              sum[2] += -0.583283305;
            } else {
              sum[2] += -0.572862744;
            }
          } else {
            if (sample[2] <53) {
              sum[2] += -0.405780554;
            } else {
              sum[2] += -0.583098114;
            }
          }
        } else {
          if (sample[2] <60) {
            if (sample[5] <79) {
              sum[2] += -0.573939085;
            } else {
              sum[2] += 2.58894467;
            }
          } else {
            if (sample[2] <72) {
              sum[2] += -0.541576564;
            } else {
              sum[2] += -0.582781136;
            }
          }
        }
      } else {
        if (sample[4] <134) {
          if (sample[3] <95) {
            if (sample[3] <68) {
              sum[2] += -0.550890148;
            } else {
              sum[2] += 2.91659522;
            }
          } else {
            if (sample[1] <88) {
              sum[2] += -0.582928061;
            } else {
              sum[2] += -0.0452708304;
            }
          }
        } else {
          if (sample[0] <80) {
            if (sample[2] <47) {
              sum[2] += 0.103668161;
            } else {
              sum[2] += -0.582979679;
            }
          } else {
            if (sample[2] <60) {
              sum[2] += -0.513019621;
            } else {
              sum[2] += -0.583266258;
            }
          }
        }
      }
    } else {
      if (sample[3] <98) {
        if (sample[3] <73) {
          if (sample[3] <65) {
            if (sample[2] <101) {
              sum[2] += -1.34591129e-08;
            } else {
              sum[2] += -0.58311522;
            }
          } else {
            if (sample[1] <156) {
              sum[2] += 0.457106233;
            } else {
              sum[2] += 2.05251622;
            }
          }
        } else {
          if (sample[3] <95) {
            if (sample[2] <132) {
              sum[2] += 3.4544208;
            } else {
              sum[2] += -0.372712135;
            }
          } else {
            if (sample[1] <115) {
              sum[2] += 2.09363532;
            } else {
              sum[2] += 0.553051353;
            }
          }
        }
      } else {
        if (sample[3] <103) {
          if (sample[1] <115) {
            if (sample[1] <104) {
              sum[2] += -0.308330685;
            } else {
              sum[2] += 0.723258913;
            }
          } else {
            sum[2] += -0.502511084;
          }
        } else {
          if (sample[1] <107) {
            sum[2] += -0.582892179;
          } else {
            if (sample[5] <149) {
              sum[2] += -0.0856470689;
            } else {
              sum[2] += -0.562900007;
            }
          }
        }
      }
    }
  }


  if (sample[4] <134) {
    if (sample[4] <112) {
      if (sample[4] <85) {
        sum[3] += -0.583292127;
      } else {
        if (sample[1] <104) {
          sum[3] += -0.58263427;
        } else {
          if (sample[0] <80) {
            if (sample[3] <36) {
              sum[3] += -0.384087145;
            } else {
              sum[3] += 0.451800317;
            }
          } else {
            if (sample[3] <55) {
              sum[3] += -0.416946441;
            } else {
              sum[3] += -0.583206892;
            }
          }
        }
      }
    } else {
      if (sample[1] <176) {
        if (sample[1] <98) {
          if (sample[1] <91) {
            sum[3] += -0.581473112;
          } else {
            if (sample[0] <50) {
              sum[3] += 0.944497526;
            } else {
              sum[3] += -0.568979561;
            }
          }
        } else {
          if (sample[3] <55) {
            if (sample[3] <36) {
              sum[3] += -0.390878201;
            } else {
              sum[3] += 2.57849431;
            }
          } else {
            if (sample[0] <55) {
              sum[3] += 0.449056566;
            } else {
              sum[3] += -0.582855344;
            }
          }
        }
      } else {
        if (sample[0] <110) {
          if (sample[3] <36) {
            if (sample[2] <177) {
              sum[3] += -0.290566027;
            } else {
              sum[3] += -0.576267004;
            }
          } else {
            if (sample[3] <55) {
              sum[3] += 3.31238461;
            } else {
              sum[3] += -0.523908496;
            }
          }
        } else {
          if (sample[0] <113) {
            if (sample[2] <155) {
              sum[3] += 2.55248618;
            } else {
              sum[3] += -0.543689311;
            }
          } else {
            sum[3] += -0.58182776;
          }
        }
      }
    }
  } else {
    if (sample[1] <88) {
      if (sample[1] <71) {
        if (sample[1] <68) {
          if (sample[1] <57) {
            sum[3] += -0.583321869;
          } else {
            if (sample[5] <71) {
              sum[3] += -0.370935351;
            } else {
              sum[3] += -0.583208263;
            }
          }
        } else {
          if (sample[5] <71) {
            if (sample[2] <50) {
              sum[3] += 2.20185018;
            } else {
              sum[3] += -0.463576138;
            }
          } else {
            sum[3] += -0.582693219;
          }
        }
      } else {
        if (sample[5] <90) {
          if (sample[0] <27.9999981) {
            if (sample[3] <36) {
              sum[3] += -0.572559357;
            } else {
              sum[3] += 3.42852569;
            }
          } else {
            if (sample[0] <50) {
              sum[3] += 0.166033119;
            } else {
              sum[3] += -0.581634462;
            }
          }
        } else {
          sum[3] += -0.583219469;
        }
      }
    } else {
      if (sample[3] <29.9999981) {
        if (sample[2] <110) {
          if (sample[1] <102) {
            if (sample[2] <101) {
              sum[3] += -0.0297492724;
            } else {
              sum[3] += -0.524623573;
            }
          } else {
            if (sample[0] <31.9999981) {
              sum[3] += 0.773540795;
            } else {
              sum[3] += -0.558542907;
            }
          }
        } else {
          sum[3] += -0.58281821;
        }
      } else {
        if (sample[3] <62) {
          if (sample[3] <36) {
            if (sample[2] <104) {
              sum[3] += 3.28993011;
            } else {
              sum[3] += 1.03884542;
            }
          } else {
            sum[3] += 3.49319363;
          }
        } else {
          sum[3] += -0.582342088;
        }
      }
    }
  }


  if (sample[3] <133) {
    if (sample[3] <128) {
      sum[4] += -0.583307385;
    } else {
      if (sample[4] <63) {
        if (sample[4] <56) {
          sum[4] += -0.583042264;
        } else {
          if (sample[2] <70) {
            sum[4] += -0.543689311;
          } else {
            if (sample[0] <100) {
              sum[4] += 0.0709938928;
            } else {
              sum[4] += -0.381818175;
            }
          }
        }
      } else {
        if (sample[2] <68) {
          if (sample[2] <63) {
            if (sample[0] <68) {
              sum[4] += -0.581939638;
            } else {
              sum[4] += -0.378258109;
            }
          } else {
            if (sample[4] <73) {
              sum[4] += -0.41420117;
            } else {
              sum[4] += 0.373887211;
            }
          }
        } else {
          if (sample[0] <123) {
            if (sample[5] <97) {
              sum[4] += 1.00719416;
            } else {
              sum[4] += 2.6891036;
            }
          } else {
            if (sample[1] <68) {
              sum[4] += -0.470355719;
            } else {
              sum[4] += 0.855656624;
            }
          }
        }
      }
    }
  } else {
    if (sample[4] <73) {
      if (sample[4] <63) {
        if (sample[4] <52) {
          if (sample[4] <42) {
            if (sample[4] <31.9999981) {
              sum[4] += -0.583263099;
            } else {
              sum[4] += -0.560654759;
            }
          } else {
            if (sample[0] <74) {
              sum[4] += -0.560123622;
            } else {
              sum[4] += -0.355043232;
            }
          }
        } else {
          if (sample[0] <74) {
            if (sample[0] <62) {
              sum[4] += -0.54674381;
            } else {
              sum[4] += -0.272633493;
            }
          } else {
            if (sample[3] <163) {
              sum[4] += 1.12296724;
            } else {
              sum[4] += -0.387281388;
            }
          }
        }
      } else {
        if (sample[0] <80) {
          if (sample[0] <70) {
            if (sample[2] <60) {
              sum[4] += -0.563455224;
            } else {
              sum[4] += -0.152038097;
            }
          } else {
            if (sample[1] <68) {
              sum[4] += 1.02927768;
            } else {
              sum[4] += -0.220630378;
            }
          }
        } else {
          if (sample[3] <163) {
            if (sample[1] <124) {
              sum[4] += 2.93545914;
            } else {
              sum[4] += -0.386206895;
            }
          } else {
            if (sample[1] <119) {
              sum[4] += 0.306681246;
            } else {
              sum[4] += -0.522388041;
            }
          }
        }
      }
    } else {
      if (sample[2] <60) {
        if (sample[0] <62) {
          if (sample[5] <57) {
            sum[4] += -0.582411826;
          } else {
            if (sample[2] <53) {
              sum[4] += -0.530499041;
            } else {
              sum[4] += 0.194199219;
            }
          }
        } else {
          if (sample[4] <100) {
            sum[4] += -0.533795476;
          } else {
            if (sample[4] <134) {
              sum[4] += 2.74572396;
            } else {
              sum[4] += 0.0341463275;
            }
          }
        }
      } else {
        if (sample[0] <62) {
          if (sample[0] <50) {
            if (sample[1] <43) {
              sum[4] += -0.532743335;
            } else {
              sum[4] += 0.228486627;
            }
          } else {
            if (sample[4] <85) {
              sum[4] += 0.760986865;
            } else {
              sum[4] += 2.66427279;
            }
          }
        } else {
          if (sample[1] <119) {
            if (sample[3] <179) {
              sum[4] += 3.48608303;
            } else {
              sum[4] += -0.41420117;
            }
          } else {
            sum[4] += -0.484429061;
          }
        }
      }
    }
  }


  if (sample[4] <234) {
    if (sample[4] <203) {
      if (sample[4] <162) {
        if (sample[4] <134) {
          if (sample[4] <112) {
            sum[5] += -0.58326304;
          } else {
            if (sample[3] <11.999999) {
              sum[5] += 0.87732023;
            } else {
              sum[5] += -0.581513405;
            }
          }
        } else {
          if (sample[3] <14.999999) {
            if (sample[2] <84) {
              sum[5] += -0.126015618;
            } else {
              sum[5] += 2.64332032;
            }
          } else {
            if (sample[3] <17.9999981) {
              sum[5] += -0.368050575;
            } else {
              sum[5] += -0.583074152;
            }
          }
        }
      } else {
        if (sample[3] <14.999999) {
          if (sample[2] <91) {
            if (sample[2] <72) {
              sum[5] += -0.504693687;
            } else {
              sum[5] += 0.792486191;
            }
          } else {
            if (sample[1] <71) {
              sum[5] += 3.0647428;
            } else {
              sum[5] += 1.92945087;
            }
          }
        } else {
          if (sample[3] <17.9999981) {
            if (sample[2] <81) {
              sum[5] += -0.440590084;
            } else {
              sum[5] += 0.0760536045;
            }
          } else {
            if (sample[3] <19.9999981) {
              sum[5] += -0.544321656;
            } else {
              sum[5] += -0.583272994;
            }
          }
        }
      }
    } else {
      if (sample[3] <14.999999) {
        if (sample[2] <91) {
          if (sample[5] <75) {
            if (sample[2] <70) {
              sum[5] += -0.537769914;
            } else {
              sum[5] += -0.165465564;
            }
          } else {
            if (sample[0] <9.99999905) {
              sum[5] += 1.39791083;
            } else {
              sum[5] += 0.744396389;
            }
          }
        } else {
          if (sample[5] <97) {
            if (sample[1] <50) {
              sum[5] += 2.46054888;
            } else {
              sum[5] += 0.659068346;
            }
          } else {
            if (sample[3] <11.999999) {
              sum[5] += 3.38984609;
            } else {
              sum[5] += 3.10830545;
            }
          }
        }
      } else {
        if (sample[3] <17.9999981) {
          if (sample[2] <104) {
            if (sample[2] <76) {
              sum[5] += -0.541759014;
            } else {
              sum[5] += -0.212038234;
            }
          } else {
            if (sample[1] <68) {
              sum[5] += 2.89958501;
            } else {
              sum[5] += 0.174148157;
            }
          }
        } else {
          sum[5] += -0.58295089;
        }
      }
    }
  } else {
    if (sample[2] <81) {
      if (sample[2] <72) {
        if (sample[2] <68) {
          if (sample[2] <60) {
            if (sample[2] <57) {
              sum[5] += -0.583295703;
            } else {
              sum[5] += -0.555466533;
            }
          } else {
            if (sample[1] <31.9999981) {
              sum[5] += -0.202866942;
            } else {
              sum[5] += -0.527261794;
            }
          }
        } else {
          if (sample[1] <31.9999981) {
            if (sample[4] <255) {
              sum[5] += 0.0709938928;
            } else {
              sum[5] += 2.088938;
            }
          } else {
            if (sample[1] <34) {
              sum[5] += -0.14936398;
            } else {
              sum[5] += -0.551821947;
            }
          }
        }
      } else {
        if (sample[1] <34) {
          if (sample[1] <31.9999981) {
            if (sample[4] <255) {
              sum[5] += 1.41587889;
            } else {
              sum[5] += 3.03639698;
            }
          } else {
            if (sample[5] <75) {
              sum[5] += 0.916722596;
            } else {
              sum[5] += 2.4750092;
            }
          }
        } else {
          if (sample[3] <14.999999) {
            if (sample[2] <76) {
              sum[5] += 0.115463898;
            } else {
              sum[5] += 1.29860222;
            }
          } else {
            if (sample[1] <43) {
              sum[5] += -0.259819627;
            } else {
              sum[5] += -0.572058499;
            }
          }
        }
      }
    } else {
      if (sample[3] <14.999999) {
        if (sample[2] <91) {
          if (sample[1] <34) {
            sum[5] += 3.41845202;
          } else {
            if (sample[2] <84) {
              sum[5] += 2.29685473;
            } else {
              sum[5] += 2.9767158;
            }
          }
        } else {
          if (sample[2] <113) {
            if (sample[3] <11.999999) {
              sum[5] += 3.47709656;
            } else {
              sum[5] += 3.31400156;
            }
          } else {
            sum[5] += 3.4922626;
          }
        }
      } else {
        if (sample[2] <101) {
          if (sample[3] <17.9999981) {
            if (sample[5] <90) {
              sum[5] += -0.280177414;
            } else {
              sum[5] += 0.73481369;
            }
          } else {
            if (sample[1] <60) {
              sum[5] += -0.407230675;
            } else {
              sum[5] += -0.581734776;
            }
          }
        } else {
          if (sample[3] <17.9999981) {
            if (sample[1] <63) {
              sum[5] += 2.99466467;
            } else {
              sum[5] += 1.63574958;
            }
          } else {
            sum[5] += -0.578233719;
          }
        }
      }
    }
  }


  if (sample[4] <134) {
    if (sample[4] <112) {
      if (sample[4] <85) {
        sum[6] += -0.583321154;
      } else {
        if (sample[2] <177) {
          if (sample[2] <121) {
            sum[6] += -0.582667232;
          } else {
            if (sample[3] <40) {
              sum[6] += -0.414632767;
            } else {
              sum[6] += -0.583195686;
            }
          }
        } else {
          if (sample[3] <29.9999981) {
            if (sample[4] <100) {
              sum[6] += -0.569250286;
            } else {
              sum[6] += -0.302685469;
            }
          } else {
            if (sample[0] <121) {
              sum[6] += 1.41587889;
            } else {
              sum[6] += -0.419083238;
            }
          }
        }
      }
    } else {
      if (sample[2] <192) {
        if (sample[3] <29.9999981) {
          if (sample[1] <120) {
            if (sample[1] <104) {
              sum[6] += -0.568522632;
            } else {
              sum[6] += 0.0608350448;
            }
          } else {
            if (sample[0] <68) {
              sum[6] += 3.1029284;
            } else {
              sum[6] += 1.1717118;
            }
          }
        } else {
          if (sample[2] <177) {
            if (sample[3] <36) {
              sum[6] += -0.508698225;
            } else {
              sum[6] += -0.582893074;
            }
          } else {
            if (sample[3] <36) {
              sum[6] += 2.88822818;
            } else {
              sum[6] += -0.534752667;
            }
          }
        }
      } else {
        if (sample[3] <23.9999981) {
          if (sample[0] <103) {
            sum[6] += 0.462809891;
          } else {
            if (sample[1] <188) {
              sum[6] += -0.506702423;
            } else {
              sum[6] += 0.0725388452;
            }
          }
        } else {
          if (sample[0] <128) {
            if (sample[3] <29.9999981) {
              sum[6] += 3.17018533;
            } else {
              sum[6] += 1.72573185;
            }
          } else {
            if (sample[0] <129) {
              sum[6] += -0.386206895;
            } else {
              sum[6] += -0.124260366;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <192) {
      if (sample[1] <98) {
        if (sample[1] <78) {
          if (sample[1] <73) {
            if (sample[1] <57) {
              sum[6] += -0.583295226;
            } else {
              sum[6] += -0.574758768;
            }
          } else {
            if (sample[0] <13.999999) {
              sum[6] += -0.217516601;
            } else {
              sum[6] += -0.575798094;
            }
          }
        } else {
          if (sample[3] <36) {
            if (sample[3] <19.9999981) {
              sum[6] += -0.570397377;
            } else {
              sum[6] += 1.18559039;
            }
          } else {
            if (sample[3] <40) {
              sum[6] += -0.433548391;
            } else {
              sum[6] += -0.582000911;
            }
          }
        }
      } else {
        if (sample[3] <29.9999981) {
          if (sample[3] <19.9999981) {
            if (sample[3] <17.9999981) {
              sum[6] += -0.495088398;
            } else {
              sum[6] += 1.22143483;
            }
          } else {
            if (sample[4] <162) {
              sum[6] += 3.28091407;
            } else {
              sum[6] += 3.48846292;
            }
          }
        } else {
          if (sample[3] <36) {
            if (sample[2] <101) {
              sum[6] += -0.493927211;
            } else {
              sum[6] += 1.53802478;
            }
          } else {
            if (sample[3] <40) {
              sum[6] += -0.566823244;
            } else {
              sum[6] += -0.583136916;
            }
          }
        }
      }
    } else {
      if (sample[1] <123) {
        sum[6] += -0.583126128;
      } else {
        if (sample[3] <19.9999981) {
          sum[6] += -0.401273876;
        } else {
          sum[6] += 3.49943638;
        }
      }
    }
  }



  return sum;
}
}
