#include <trifinger_object_tracking/xgboost_classifier.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

std::array<float, XGB_NUM_CLASSES> xgb_classify(std::array<float, XGB_NUM_FEATURES> &sample) {

  std::array<float, XGB_NUM_CLASSES> sum;
  sum.fill(0.0);

  if (sample[4] <89) {
    if (sample[4] <77) {
      if (sample[4] <65) {
        if (sample[4] <56) {
          sum[0] += 3.49960184;
        } else {
          if (sample[3] <86) {
            sum[0] += 3.4959147;
          } else {
            if (sample[1] <107) {
              sum[0] += 3.32815909;
            } else {
              sum[0] += 2.69475603;
            }
          }
        }
      } else {
        if (sample[0] <100) {
          if (sample[3] <131) {
            if (sample[3] <100) {
              sum[0] += 3.4926405;
            } else {
              sum[0] += 3.22867203;
            }
          } else {
            if (sample[0] <73) {
              sum[0] += 3.33794546;
            } else {
              sum[0] += 0.389211565;
            }
          }
        } else {
          if (sample[3] <81) {
            if (sample[2] <94) {
              sum[0] += 2.40142345;
            } else {
              sum[0] += 3.46214771;
            }
          } else {
            if (sample[3] <131) {
              sum[0] += 1.86348915;
            } else {
              sum[0] += -0.140684754;
            }
          }
        }
      }
    } else {
      if (sample[0] <86) {
        if (sample[3] <103) {
          if (sample[0] <71) {
            if (sample[3] <98) {
              sum[0] += 3.49614906;
            } else {
              sum[0] += 3.04971099;
            }
          } else {
            if (sample[3] <35) {
              sum[0] += 3.43287659;
            } else {
              sum[0] += 3.16379547;
            }
          }
        } else {
          if (sample[2] <63) {
            if (sample[1] <42) {
              sum[0] += 3.37774253;
            } else {
              sum[0] += 2.55345678;
            }
          } else {
            if (sample[3] <165) {
              sum[0] += -0.361031502;
            } else {
              sum[0] += 2.56575871;
            }
          }
        }
      } else {
        if (sample[3] <68) {
          if (sample[5] <121) {
            sum[0] += 0.867768586;
          } else {
            sum[0] += 3.42946339;
          }
        } else {
          if (sample[3] <131) {
            if (sample[3] <98) {
              sum[0] += 0.225463316;
            } else {
              sum[0] += 2.68123388;
            }
          } else {
            if (sample[3] <165) {
              sum[0] += -0.563750505;
            } else {
              sum[0] += 1.28096116;
            }
          }
        }
      }
    }
  } else {
    if (sample[5] <73) {
      if (sample[3] <91) {
        if (sample[3] <81) {
          if (sample[5] <68) {
            if (sample[3] <77) {
              sum[0] += 3.49779344;
            } else {
              sum[0] += 3.28236723;
            }
          } else {
            if (sample[1] <30.9999981) {
              sum[0] += 0.715658247;
            } else {
              sum[0] += 3.29014397;
            }
          }
        } else {
          if (sample[1] <30.9999981) {
            if (sample[4] <166) {
              sum[0] += 2.19953489;
            } else {
              sum[0] += -0.0987451971;
            }
          } else {
            if (sample[5] <71) {
              sum[0] += 3.1298697;
            } else {
              sum[0] += 1.53734279;
            }
          }
        }
      } else {
        if (sample[3] <113) {
          if (sample[4] <139) {
            if (sample[3] <103) {
              sum[0] += 2.82586432;
            } else {
              sum[0] += 0.426091373;
            }
          } else {
            if (sample[0] <21.9999981) {
              sum[0] += 3.05988002;
            } else {
              sum[0] += -0.318359137;
            }
          }
        } else {
          if (sample[5] <62) {
            if (sample[3] <115) {
              sum[0] += 2.54308724;
            } else {
              sum[0] += 3.44986153;
            }
          } else {
            if (sample[2] <56) {
              sum[0] += 1.11481011;
            } else {
              sum[0] += -0.451112747;
            }
          }
        }
      }
    } else {
      if (sample[5] <83) {
        if (sample[3] <38) {
          if (sample[1] <33) {
            if (sample[4] <255) {
              sum[0] += 1.71912479;
            } else {
              sum[0] += 0.0900175124;
            }
          } else {
            if (sample[1] <38) {
              sum[0] += 2.02720094;
            } else {
              sum[0] += 3.29135895;
            }
          }
        } else {
          if (sample[4] <115) {
            if (sample[3] <131) {
              sum[0] += 2.60763144;
            } else {
              sum[0] += -0.445486993;
            }
          } else {
            if (sample[4] <139) {
              sum[0] += 0.225672752;
            } else {
              sum[0] += -0.563145697;
            }
          }
        }
      } else {
        if (sample[3] <24.9999981) {
          if (sample[3] <12.999999) {
            if (sample[4] <139) {
              sum[0] += 2.25513983;
            } else {
              sum[0] += -0.493417442;
            }
          } else {
            if (sample[2] <123) {
              sum[0] += 2.42387414;
            } else {
              sum[0] += 0.325648218;
            }
          }
        } else {
          if (sample[4] <103) {
            if (sample[3] <75) {
              sum[0] += 3.10674024;
            } else {
              sum[0] += -0.401397794;
            }
          } else {
            if (sample[4] <139) {
              sum[0] += -0.46095112;
            } else {
              sum[0] += -0.571830988;
            }
          }
        }
      }
    }
  }


  if (sample[2] <14.999999) {
    if (sample[0] <21.9999981) {
      if (sample[3] <77) {
        if (sample[3] <75) {
          if (sample[3] <68) {
            sum[1] += -0.583202362;
          } else {
            if (sample[5] <27.9999981) {
              sum[1] += -0.497278422;
            } else {
              sum[1] += -0.567822158;
            }
          }
        } else {
          if (sample[1] <33) {
            if (sample[1] <30.9999981) {
              sum[1] += -0.383664697;
            } else {
              sum[1] += -0.116424128;
            }
          } else {
            sum[1] += -0.514614701;
          }
        }
      } else {
        if (sample[3] <98) {
          if (sample[3] <81) {
            if (sample[5] <27.9999981) {
              sum[1] += 0.427795351;
            } else {
              sum[1] += -0.145720899;
            }
          } else {
            if (sample[0] <12.999999) {
              sum[1] += -0.552026272;
            } else {
              sum[1] += 2.50800776;
            }
          }
        } else {
          if (sample[3] <105) {
            if (sample[4] <255) {
              sum[1] += -0.432818055;
            } else {
              sum[1] += 1.28096116;
            }
          } else {
            if (sample[3] <109) {
              sum[1] += -0.327807695;
            } else {
              sum[1] += -0.578149021;
            }
          }
        }
      }
    } else {
      if (sample[4] <186) {
        if (sample[5] <45) {
          if (sample[3] <86) {
            if (sample[5] <27.9999981) {
              sum[1] += 1.5164113;
            } else {
              sum[1] += -0.266642243;
            }
          } else {
            if (sample[3] <108) {
              sum[1] += 1.84640062;
            } else {
              sum[1] += -0.40794301;
            }
          }
        } else {
          if (sample[1] <42) {
            if (sample[1] <25.9999981) {
              sum[1] += -0.0482758731;
            } else {
              sum[1] += 3.35381198;
            }
          } else {
            sum[1] += -0.45161289;
          }
        }
      } else {
        if (sample[0] <35) {
          if (sample[3] <110) {
            if (sample[1] <33) {
              sum[1] += 3.1766336;
            } else {
              sum[1] += 0.459326565;
            }
          } else {
            if (sample[4] <255) {
              sum[1] += -0.51557833;
            } else {
              sum[1] += 2.29891515;
            }
          }
        } else {
          if (sample[3] <115) {
            if (sample[0] <46) {
              sum[1] += 3.28707266;
            } else {
              sum[1] += 3.49147773;
            }
          } else {
            if (sample[0] <46) {
              sum[1] += -0.520787716;
            } else {
              sum[1] += 2.75109148;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <26.9999981) {
      if (sample[0] <46) {
        if (sample[0] <35) {
          if (sample[3] <75) {
            sum[1] += -0.58279413;
          } else {
            if (sample[3] <123) {
              sum[1] += -0.461922735;
            } else {
              sum[1] += -0.571644187;
            }
          }
        } else {
          if (sample[2] <21.9999981) {
            if (sample[1] <38) {
              sum[1] += 0.858431339;
            } else {
              sum[1] += -0.101057187;
            }
          } else {
            if (sample[3] <120) {
              sum[1] += -0.25098142;
            } else {
              sum[1] += -0.567869246;
            }
          }
        }
      } else {
        if (sample[3] <100) {
          if (sample[3] <98) {
            if (sample[3] <91) {
              sum[1] += -0.334851921;
            } else {
              sum[1] += 0.391521901;
            }
          } else {
            if (sample[2] <21.9999981) {
              sum[1] += 2.41726613;
            } else {
              sum[1] += 1.1404779;
            }
          }
        } else {
          if (sample[1] <25.9999981) {
            if (sample[0] <56) {
              sum[1] += -0.250671446;
            } else {
              sum[1] += 2.55248618;
            }
          } else {
            if (sample[4] <139) {
              sum[1] += 1.99238729;
            } else {
              sum[1] += 3.44516134;
            }
          }
        }
      }
    } else {
      if (sample[4] <89) {
        if (sample[4] <65) {
          if (sample[4] <56) {
            if (sample[5] <27.9999981) {
              sum[1] += 0.12844035;
            } else {
              sum[1] += -0.58319211;
            }
          } else {
            if (sample[3] <100) {
              sum[1] += -0.582205355;
            } else {
              sum[1] += -0.490535945;
            }
          }
        } else {
          if (sample[3] <98) {
            if (sample[3] <81) {
              sum[1] += -0.582670331;
            } else {
              sum[1] += -0.561660469;
            }
          } else {
            if (sample[3] <124) {
              sum[1] += -0.121900283;
            } else {
              sum[1] += -0.567767024;
            }
          }
        }
      } else {
        if (sample[3] <100) {
          if (sample[3] <98) {
            if (sample[3] <91) {
              sum[1] += -0.582525969;
            } else {
              sum[1] += -0.556647301;
            }
          } else {
            if (sample[2] <36) {
              sum[1] += 0.0186115038;
            } else {
              sum[1] += -0.51610595;
            }
          }
        } else {
          if (sample[3] <120) {
            if (sample[4] <115) {
              sum[1] += 1.59235919;
            } else {
              sum[1] += 3.18345952;
            }
          } else {
            if (sample[3] <126) {
              sum[1] += 0.456211776;
            } else {
              sum[1] += -0.579357624;
            }
          }
        }
      }
    }
  }


  if (sample[4] <89) {
    if (sample[4] <65) {
      if (sample[4] <57) {
        sum[2] += -0.583164394;
      } else {
        if (sample[0] <100) {
          if (sample[0] <75) {
            sum[2] += -0.583244026;
          } else {
            if (sample[2] <87) {
              sum[2] += -0.520251036;
            } else {
              sum[2] += -0.575747669;
            }
          }
        } else {
          if (sample[2] <98) {
            if (sample[1] <122) {
              sum[2] += -0.308209151;
            } else {
              sum[2] += 2.49983382;
            }
          } else {
            if (sample[3] <86) {
              sum[2] += -0.577789843;
            } else {
              sum[2] += -0.406711608;
            }
          }
        }
      }
    } else {
      if (sample[1] <104) {
        if (sample[1] <94) {
          if (sample[1] <79) {
            if (sample[1] <74) {
              sum[2] += -0.583247304;
            } else {
              sum[2] += -0.5630669;
            }
          } else {
            if (sample[2] <56) {
              sum[2] += 0.326530576;
            } else {
              sum[2] += -0.537932873;
            }
          }
        } else {
          if (sample[2] <70) {
            if (sample[0] <92) {
              sum[2] += -0.127319038;
            } else {
              sum[2] += 1.84019434;
            }
          } else {
            if (sample[2] <83) {
              sum[2] += -0.115298919;
            } else {
              sum[2] += -0.58255744;
            }
          }
        }
      } else {
        if (sample[2] <98) {
          if (sample[3] <81) {
            if (sample[3] <65) {
              sum[2] += -0.45874235;
            } else {
              sum[2] += 0.976999402;
            }
          } else {
            if (sample[3] <98) {
              sum[2] += 2.8763988;
            } else {
              sum[2] += -0.0469595455;
            }
          }
        } else {
          if (sample[3] <70) {
            if (sample[3] <65) {
              sum[2] += -0.582465649;
            } else {
              sum[2] += -0.264483631;
            }
          } else {
            if (sample[3] <98) {
              sum[2] += 1.90963483;
            } else {
              sum[2] += -0.496384561;
            }
          }
        }
      }
    }
  } else {
    if (sample[0] <92) {
      if (sample[0] <63) {
        if (sample[0] <61) {
          if (sample[0] <56) {
            sum[2] += -0.583165348;
          } else {
            if (sample[2] <53) {
              sum[2] += -0.517482698;
            } else {
              sum[2] += -0.583080411;
            }
          }
        } else {
          if (sample[2] <94) {
            if (sample[1] <68) {
              sum[2] += -0.577924013;
            } else {
              sum[2] += 1.24900126;
            }
          } else {
            sum[2] += -0.580608368;
          }
        }
      } else {
        if (sample[4] <139) {
          if (sample[2] <63) {
            if (sample[1] <76) {
              sum[2] += -0.360133737;
            } else {
              sum[2] += 3.08629942;
            }
          } else {
            if (sample[2] <68) {
              sum[2] += -0.223485678;
            } else {
              sum[2] += -0.574868739;
            }
          }
        } else {
          if (sample[2] <53) {
            if (sample[1] <68) {
              sum[2] += -0.583260298;
            } else {
              sum[2] += 3.43608737;
            }
          } else {
            sum[2] += -0.583281517;
          }
        }
      }
    } else {
      if (sample[3] <98) {
        if (sample[3] <70) {
          if (sample[3] <65) {
            if (sample[3] <61) {
              sum[2] += -0.583202839;
            } else {
              sum[2] += 0.11602208;
            }
          } else {
            if (sample[0] <96) {
              sum[2] += -0.368421048;
            } else {
              sum[2] += 1.69968045;
            }
          }
        } else {
          if (sample[1] <91) {
            if (sample[5] <93) {
              sum[2] += 2.19402981;
            } else {
              sum[2] += 0.615384579;
            }
          } else {
            if (sample[3] <77) {
              sum[2] += 1.7251184;
            } else {
              sum[2] += 3.46813011;
            }
          }
        }
      } else {
        if (sample[3] <103) {
          if (sample[0] <106) {
            sum[2] += -0.538770795;
          } else {
            if (sample[0] <132) {
              sum[2] += 1.07318604;
            } else {
              sum[2] += -0.523515821;
            }
          }
        } else {
          sum[2] += -0.583033085;
        }
      }
    }
  }


  if (sample[4] <115) {
    if (sample[4] <103) {
      if (sample[4] <77) {
        sum[3] += -0.58330977;
      } else {
        if (sample[1] <110) {
          sum[3] += -0.5827474;
        } else {
          if (sample[0] <83) {
            if (sample[3] <35) {
              sum[3] += -0.47369054;
            } else {
              sum[3] += -0.0884090587;
            }
          } else {
            if (sample[3] <51) {
              sum[3] += -0.516673028;
            } else {
              sum[3] += -0.583197653;
            }
          }
        }
      }
    } else {
      if (sample[1] <104) {
        sum[3] += -0.581771314;
      } else {
        if (sample[3] <60) {
          if (sample[3] <38) {
            if (sample[3] <35) {
              sum[3] += -0.553703845;
            } else {
              sum[3] += 0.269086987;
            }
          } else {
            if (sample[0] <103) {
              sum[3] += 0.855934322;
            } else {
              sum[3] += 2.38528538;
            }
          }
        } else {
          sum[3] += -0.583250701;
        }
      }
    }
  } else {
    if (sample[1] <87) {
      if (sample[1] <72) {
        if (sample[1] <68) {
          if (sample[1] <57) {
            sum[3] += -0.583320916;
          } else {
            if (sample[2] <43) {
              sum[3] += -0.365806669;
            } else {
              sum[3] += -0.582179368;
            }
          }
        } else {
          if (sample[2] <53) {
            if (sample[0] <21.9999981) {
              sum[3] += 3.37674665;
            } else {
              sum[3] += -0.448394656;
            }
          } else {
            sum[3] += -0.582288027;
          }
        }
      } else {
        if (sample[0] <21.9999981) {
          if (sample[3] <35) {
            sum[3] += -0.579916298;
          } else {
            if (sample[3] <38) {
              sum[3] += 2.27790952;
            } else {
              sum[3] += 3.47171974;
            }
          }
        } else {
          if (sample[0] <29.9999981) {
            if (sample[3] <38) {
              sum[3] += -0.568473458;
            } else {
              sum[3] += 3.1182158;
            }
          } else {
            if (sample[0] <35) {
              sum[3] += -0.175223827;
            } else {
              sum[3] += -0.574759841;
            }
          }
        }
      }
    } else {
      if (sample[3] <35) {
        if (sample[3] <29.9999981) {
          if (sample[5] <93) {
            if (sample[0] <35) {
              sum[3] += -0.495384604;
            } else {
              sum[3] += 0.310249269;
            }
          } else {
            if (sample[4] <139) {
              sum[3] += -0.535905898;
            } else {
              sum[3] += -0.580519497;
            }
          }
        } else {
          if (sample[5] <107) {
            if (sample[1] <101) {
              sum[3] += -0.0279131737;
            } else {
              sum[3] += 1.20394969;
            }
          } else {
            if (sample[0] <49) {
              sum[3] += -0.520078182;
            } else {
              sum[3] += -0.0760808438;
            }
          }
        }
      } else {
        if (sample[3] <60) {
          if (sample[3] <38) {
            if (sample[4] <139) {
              sum[3] += 1.42733181;
            } else {
              sum[3] += 2.8387742;
            }
          } else {
            if (sample[3] <56) {
              sum[3] += 3.48289037;
            } else {
              sum[3] += 1.42818785;
            }
          }
        } else {
          if (sample[0] <61) {
            if (sample[3] <68) {
              sum[3] += -0.470072985;
            } else {
              sum[3] += 0.462809891;
            }
          } else {
            sum[3] += -0.583248496;
          }
        }
      }
    }
  }


  if (sample[3] <131) {
    sum[4] += -0.583283484;
  } else {
    if (sample[4] <65) {
      if (sample[4] <52) {
        if (sample[4] <42) {
          if (sample[4] <28.9999981) {
            sum[4] += -0.583257616;
          } else {
            sum[4] += -0.565537095;
          }
        } else {
          if (sample[0] <73) {
            sum[4] += -0.563451469;
          } else {
            if (sample[3] <165) {
              sum[4] += -0.26946941;
            } else {
              sum[4] += -0.542298555;
            }
          }
        }
      } else {
        if (sample[0] <86) {
          if (sample[0] <71) {
            if (sample[0] <67) {
              sum[4] += -0.541443765;
            } else {
              sum[4] += -0.373454779;
            }
          } else {
            if (sample[2] <94) {
              sum[4] += -0.0716555044;
            } else {
              sum[4] += -0.515439391;
            }
          }
        } else {
          if (sample[3] <165) {
            if (sample[4] <57) {
              sum[4] += 0.603377938;
            } else {
              sum[4] += 1.52173901;
            }
          } else {
            if (sample[2] <132) {
              sum[4] += -0.355329961;
            } else {
              sum[4] += -0.564136982;
            }
          }
        }
      }
    } else {
      if (sample[0] <61) {
        if (sample[2] <60) {
          if (sample[2] <56) {
            if (sample[2] <53) {
              sum[4] += -0.582983077;
            } else {
              sum[4] += -0.405076146;
            }
          } else {
            if (sample[3] <150) {
              sum[4] += 1.02752292;
            } else {
              sum[4] += -0.24000001;
            }
          }
        } else {
          if (sample[3] <165) {
            if (sample[1] <46) {
              sum[4] += 2.86761022;
            } else {
              sum[4] += 0.165680453;
            }
          } else {
            if (sample[4] <77) {
              sum[4] += -0.453942508;
            } else {
              sum[4] += 0.363890409;
            }
          }
        }
      } else {
        if (sample[3] <165) {
          if (sample[2] <60) {
            if (sample[4] <103) {
              sum[4] += -0.482950032;
            } else {
              sum[4] += 1.9427036;
            }
          } else {
            if (sample[4] <77) {
              sum[4] += 2.90617418;
            } else {
              sum[4] += 3.4910984;
            }
          }
        } else {
          if (sample[4] <89) {
            if (sample[4] <77) {
              sum[4] += -0.152389973;
            } else {
              sum[4] += 0.657218754;
            }
          } else {
            if (sample[2] <151) {
              sum[4] += 1.9856863;
            } else {
              sum[4] += -0.178913742;
            }
          }
        }
      }
    }
  }


  if (sample[4] <238) {
    if (sample[4] <209) {
      if (sample[4] <139) {
        if (sample[4] <115) {
          sum[5] += -0.583284318;
        } else {
          if (sample[3] <12.999999) {
            if (sample[1] <57) {
              sum[5] += -0.31049189;
            } else {
              sum[5] += 1.55891025;
            }
          } else {
            if (sample[3] <15.999999) {
              sum[5] += -0.347932369;
            } else {
              sum[5] += -0.583117008;
            }
          }
        }
      } else {
        if (sample[3] <12.999999) {
          if (sample[2] <94) {
            if (sample[5] <75) {
              sum[5] += -0.480864257;
            } else {
              sum[5] += 0.645782769;
            }
          } else {
            if (sample[1] <82) {
              sum[5] += 2.87951851;
            } else {
              sum[5] += 0.668341637;
            }
          }
        } else {
          if (sample[3] <15.999999) {
            if (sample[5] <75) {
              sum[5] += -0.511247873;
            } else {
              sum[5] += -0.00954677351;
            }
          } else {
            if (sample[3] <17.9999981) {
              sum[5] += -0.551531374;
            } else {
              sum[5] += -0.583208978;
            }
          }
        }
      }
    } else {
      if (sample[3] <12.999999) {
        if (sample[5] <93) {
          if (sample[2] <70) {
            if (sample[1] <33) {
              sum[5] += -0.567133009;
            } else {
              sum[5] += -0.181347162;
            }
          } else {
            if (sample[0] <8.99999905) {
              sum[5] += 0.876357913;
            } else {
              sum[5] += 0.415206224;
            }
          }
        } else {
          if (sample[5] <100) {
            if (sample[1] <46) {
              sum[5] += 1.97208881;
            } else {
              sum[5] += 2.8709228;
            }
          } else {
            if (sample[5] <121) {
              sum[5] += 3.18052864;
            } else {
              sum[5] += 3.44952035;
            }
          }
        }
      } else {
        if (sample[3] <15.999999) {
          if (sample[2] <90) {
            if (sample[2] <76) {
              sum[5] += -0.490974724;
            } else {
              sum[5] += 0.178285182;
            }
          } else {
            if (sample[1] <49) {
              sum[5] += 2.10204077;
            } else {
              sum[5] += 1.30091751;
            }
          }
        } else {
          if (sample[3] <17.9999981) {
            if (sample[1] <82) {
              sum[5] += -0.534561396;
            } else {
              sum[5] += 0.12844035;
            }
          } else {
            sum[5] += -0.583289444;
          }
        }
      }
    }
  } else {
    if (sample[2] <83) {
      if (sample[2] <72) {
        if (sample[2] <68) {
          if (sample[2] <60) {
            sum[5] += -0.582992256;
          } else {
            if (sample[1] <33) {
              sum[5] += -0.243354753;
            } else {
              sum[5] += -0.558632433;
            }
          }
        } else {
          if (sample[1] <30.9999981) {
            if (sample[3] <9.99999905) {
              sum[5] += -0.443902433;
            } else {
              sum[5] += 2.21742034;
            }
          } else {
            if (sample[1] <33) {
              sum[5] += 0.172163188;
            } else {
              sum[5] += -0.550011635;
            }
          }
        }
      } else {
        if (sample[1] <33) {
          if (sample[3] <12.999999) {
            if (sample[4] <255) {
              sum[5] += 1.60792065;
            } else {
              sum[5] += 2.89019012;
            }
          } else {
            if (sample[2] <74) {
              sum[5] += 1.39630115;
            } else {
              sum[5] += 2.36108088;
            }
          }
        } else {
          if (sample[1] <38) {
            if (sample[2] <78) {
              sum[5] += 0.0493597873;
            } else {
              sum[5] += 1.90554559;
            }
          } else {
            if (sample[3] <15.999999) {
              sum[5] += 0.00456544803;
            } else {
              sum[5] += -0.537104726;
            }
          }
        }
      }
    } else {
      if (sample[3] <15.999999) {
        if (sample[3] <12.999999) {
          if (sample[2] <87) {
            if (sample[4] <255) {
              sum[5] += 2.20272303;
            } else {
              sum[5] += 3.20792198;
            }
          } else {
            sum[5] += 3.48471045;
          }
        } else {
          if (sample[5] <93) {
            if (sample[1] <42) {
              sum[5] += 1.94418216;
            } else {
              sum[5] += 0.39546755;
            }
          } else {
            if (sample[1] <52) {
              sum[5] += 2.80464888;
            } else {
              sum[5] += 2.09465647;
            }
          }
        }
      } else {
        if (sample[1] <57) {
          if (sample[2] <90) {
            sum[5] += -0.537925303;
          } else {
            if (sample[4] <255) {
              sum[5] += -1.65787739e-08;
            } else {
              sum[5] += -0.462654471;
            }
          }
        } else {
          sum[5] += -0.572535515;
        }
      }
    }
  }


  if (sample[4] <139) {
    if (sample[4] <115) {
      if (sample[4] <89) {
        sum[6] += -0.583312511;
      } else {
        if (sample[2] <180) {
          if (sample[3] <38) {
            if (sample[1] <104) {
              sum[6] += -0.577600062;
            } else {
              sum[6] += -0.351390749;
            }
          } else {
            if (sample[3] <41) {
              sum[6] += -0.569332719;
            } else {
              sum[6] += -0.583019257;
            }
          }
        } else {
          if (sample[1] <177) {
            if (sample[0] <110) {
              sum[6] += -0.177215204;
            } else {
              sum[6] += -0.564322472;
            }
          } else {
            if (sample[0] <115) {
              sum[6] += 1.80198014;
            } else {
              sum[6] += -0.170895174;
            }
          }
        }
      }
    } else {
      if (sample[2] <180) {
        if (sample[3] <29.9999981) {
          if (sample[1] <114) {
            if (sample[1] <94) {
              sum[6] += -0.559915245;
            } else {
              sum[6] += 0.420158505;
            }
          } else {
            if (sample[0] <83) {
              sum[6] += 2.7745831;
            } else {
              sum[6] += 0.0103499088;
            }
          }
        } else {
          if (sample[3] <35) {
            if (sample[5] <83) {
              sum[6] += -0.582675517;
            } else {
              sum[6] += 1.03240216;
            }
          } else {
            if (sample[3] <38) {
              sum[6] += -0.556400657;
            } else {
              sum[6] += -0.58298403;
            }
          }
        }
      } else {
        if (sample[1] <177) {
          if (sample[1] <167) {
            if (sample[0] <92) {
              sum[6] += 0.12844035;
            } else {
              sum[6] += -0.522388041;
            }
          } else {
            if (sample[0] <96) {
              sum[6] += 1.84210515;
            } else {
              sum[6] += -1.34591129e-08;
            }
          }
        } else {
          if (sample[1] <189) {
            if (sample[0] <96) {
              sum[6] += 2.48063111;
            } else {
              sum[6] += 0.162790671;
            }
          } else {
            if (sample[0] <127) {
              sum[6] += 3.30962753;
            } else {
              sum[6] += -0.0482758731;
            }
          }
        }
      }
    }
  } else {
    if (sample[1] <97) {
      if (sample[1] <77) {
        if (sample[1] <68) {
          if (sample[1] <57) {
            sum[6] += -0.583264947;
          } else {
            if (sample[5] <90) {
              sum[6] += -0.507383347;
            } else {
              sum[6] += -0.582673371;
            }
          }
        } else {
          if (sample[3] <20.9999981) {
            sum[6] += -0.58049655;
          } else {
            if (sample[2] <63) {
              sum[6] += -0.578597128;
            } else {
              sum[6] += 0.0908314586;
            }
          }
        }
      } else {
        if (sample[3] <35) {
          if (sample[3] <24.9999981) {
            if (sample[1] <94) {
              sum[6] += -0.352426112;
            } else {
              sum[6] += 1.47412241;
            }
          } else {
            if (sample[4] <166) {
              sum[6] += 1.76635504;
            } else {
              sum[6] += 3.1570251;
            }
          }
        } else {
          if (sample[3] <38) {
            if (sample[1] <85) {
              sum[6] += 1.3034482;
            } else {
              sum[6] += -0.208510637;
            }
          } else {
            sum[6] += -0.580355704;
          }
        }
      }
    } else {
      if (sample[3] <35) {
        if (sample[3] <17.9999981) {
          if (sample[1] <133) {
            if (sample[2] <140) {
              sum[6] += 0.368421018;
            } else {
              sum[6] += -0.562989295;
            }
          } else {
            if (sample[0] <78) {
              sum[6] += 1.67768586;
            } else {
              sum[6] += -0.34710744;
            }
          }
        } else {
          if (sample[2] <101) {
            if (sample[1] <104) {
              sum[6] += 2.29879737;
            } else {
              sum[6] += 0.349676192;
            }
          } else {
            if (sample[3] <20.9999981) {
              sum[6] += 2.74458766;
            } else {
              sum[6] += 3.48367977;
            }
          }
        }
      } else {
        if (sample[3] <38) {
          if (sample[2] <133) {
            if (sample[0] <42) {
              sum[6] += -0.341931075;
            } else {
              sum[6] += 0.0950656161;
            }
          } else {
            if (sample[0] <69) {
              sum[6] += 2.67076898;
            } else {
              sum[6] += 0.0314606577;
            }
          }
        } else {
          sum[6] += -0.582375526;
        }
      }
    }
  }


  return sum;
}

