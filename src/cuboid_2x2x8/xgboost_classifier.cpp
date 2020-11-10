#include <trifinger_object_tracking/xgboost_classifier.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

std::array<float, XGB_NUM_CLASSES> xgb_classify(std::array<float, XGB_NUM_FEATURES> &sample) {

  std::array<float, XGB_NUM_CLASSES> sum;
  sum.fill(0.0);

  if (sample[4] <87) {
    if (sample[4] <76) {
      if (sample[4] <65) {
        if (sample[4] <56) {
          sum[0] += 3.49960518;
        } else {
          if (sample[3] <88) {
            sum[0] += 3.4946177;
          } else {
            if (sample[2] <73) {
              sum[0] += 3.36867166;
            } else {
              sum[0] += 2.98359132;
            }
          }
        }
      } else {
        if (sample[0] <98) {
          if (sample[3] <133) {
            if (sample[3] <98) {
              sum[0] += 3.49235201;
            } else {
              sum[0] += 3.16361213;
            }
          } else {
            if (sample[0] <73) {
              sum[0] += 3.19290471;
            } else {
              sum[0] += 0.12201497;
            }
          }
        } else {
          if (sample[3] <129) {
            if (sample[2] <117) {
              sum[0] += 1.92846179;
            } else {
              sum[0] += 3.39742565;
            }
          } else {
            if (sample[3] <165) {
              sum[0] += -0.28985998;
            } else {
              sum[0] += 2.59026885;
            }
          }
        }
      }
    } else {
      if (sample[3] <133) {
        if (sample[0] <98) {
          if (sample[3] <100) {
            if (sample[0] <88) {
              sum[0] += 3.4882431;
            } else {
              sum[0] += 2.73996687;
            }
          } else {
            if (sample[2] <69) {
              sum[0] += 2.89159036;
            } else {
              sum[0] += 0.227436796;
            }
          }
        } else {
          if (sample[2] <130) {
            if (sample[3] <98) {
              sum[0] += 0.125714749;
            } else {
              sum[0] += 2.3853991;
            }
          } else {
            sum[0] += 3.44691038;
          }
        }
      } else {
        if (sample[2] <59) {
          if (sample[5] <65) {
            sum[0] += 3.44502592;
          } else {
            sum[0] += 1.46829259;
          }
        } else {
          if (sample[3] <165) {
            if (sample[2] <69) {
              sum[0] += 0.311740845;
            } else {
              sum[0] += -0.55820626;
            }
          } else {
            if (sample[0] <88) {
              sum[0] += 2.26382971;
            } else {
              sum[0] += 1.18719995;
            }
          }
        }
      }
    }
  } else {
    if (sample[5] <74) {
      if (sample[3] <90) {
        if (sample[3] <83) {
          if (sample[5] <67) {
            if (sample[5] <59) {
              sum[0] += 3.49793243;
            } else {
              sum[0] += 3.36833644;
            }
          } else {
            if (sample[1] <30.9999981) {
              sum[0] += 0.331879765;
            } else {
              sum[0] += 3.16800284;
            }
          }
        } else {
          if (sample[4] <135) {
            if (sample[1] <71) {
              sum[0] += 3.34684277;
            } else {
              sum[0] += 1.76967418;
            }
          } else {
            if (sample[1] <30.9999981) {
              sum[0] += -0.0270627886;
            } else {
              sum[0] += 1.89570153;
            }
          }
        }
      } else {
        if (sample[3] <113) {
          if (sample[4] <135) {
            if (sample[3] <103) {
              sum[0] += 2.85408282;
            } else {
              sum[0] += 0.602881789;
            }
          } else {
            if (sample[0] <13.999999) {
              sum[0] += 3.35610151;
            } else {
              sum[0] += -0.239553198;
            }
          }
        } else {
          if (sample[5] <62) {
            if (sample[3] <115) {
              sum[0] += 2.63773155;
            } else {
              sum[0] += 3.43586302;
            }
          } else {
            if (sample[2] <55) {
              sum[0] += 1.21843946;
            } else {
              sum[0] += -0.47428453;
            }
          }
        }
      }
    } else {
      if (sample[5] <83) {
        if (sample[2] <42) {
          if (sample[4] <135) {
            if (sample[3] <90) {
              sum[0] += -0.248613819;
            } else {
              sum[0] += 1.36069107;
            }
          } else {
            if (sample[3] <105) {
              sum[0] += -0.453344673;
            } else {
              sum[0] += -0.581121206;
            }
          }
        } else {
          if (sample[3] <133) {
            if (sample[1] <38) {
              sum[0] += 0.257681757;
            } else {
              sum[0] += 2.65763927;
            }
          } else {
            if (sample[2] <64) {
              sum[0] += 0.768141568;
            } else {
              sum[0] += -0.527168453;
            }
          }
        }
      } else {
        if (sample[3] <23.9999981) {
          if (sample[3] <14.999999) {
            if (sample[1] <73) {
              sum[0] += -0.425415725;
            } else {
              sum[0] += 2.39538908;
            }
          } else {
            if (sample[2] <124) {
              sum[0] += 3.14795923;
            } else {
              sum[0] += 1.61182332;
            }
          }
        } else {
          if (sample[4] <101) {
            if (sample[3] <75) {
              sum[0] += 3.0669899;
            } else {
              sum[0] += -0.366616428;
            }
          } else {
            if (sample[4] <135) {
              sum[0] += -0.447959363;
            } else {
              sum[0] += -0.569400132;
            }
          }
        }
      }
    }
  }


  if (sample[2] <14.999999) {
    if (sample[0] <23.9999981) {
      if (sample[3] <80) {
        if (sample[3] <75) {
          if (sample[3] <73) {
            if (sample[3] <68) {
              sum[1] += -0.583222628;
            } else {
              sum[1] += -0.566565692;
            }
          } else {
            if (sample[4] <255) {
              sum[1] += -0.509780049;
            } else {
              sum[1] += -0.141057938;
            }
          }
        } else {
          if (sample[5] <33) {
            if (sample[4] <163) {
              sum[1] += -0.213472918;
            } else {
              sum[1] += 0.0536464565;
            }
          } else {
            if (sample[4] <206) {
              sum[1] += -0.408470571;
            } else {
              sum[1] += -0.539673567;
            }
          }
        }
      } else {
        if (sample[3] <103) {
          if (sample[0] <13.999999) {
            if (sample[4] <255) {
              sum[1] += -0.56620729;
            } else {
              sum[1] += 0.418099523;
            }
          } else {
            if (sample[4] <135) {
              sum[1] += -0.535773695;
            } else {
              sum[1] += 3.10227656;
            }
          }
        } else {
          if (sample[3] <109) {
            if (sample[4] <255) {
              sum[1] += -0.541644275;
            } else {
              sum[1] += 1.29359698;
            }
          } else {
            if (sample[3] <110) {
              sum[1] += -0.407221675;
            } else {
              sum[1] += -0.581855774;
            }
          }
        }
      }
    } else {
      if (sample[4] <185) {
        if (sample[5] <45) {
          if (sample[3] <83) {
            if (sample[1] <30.9999981) {
              sum[1] += 0.45161286;
            } else {
              sum[1] += -0.462699234;
            }
          } else {
            if (sample[3] <108) {
              sum[1] += 1.47465432;
            } else {
              sum[1] += -0.398937464;
            }
          }
        } else {
          if (sample[1] <42) {
            sum[1] += 3.30960512;
          } else {
            sum[1] += -0.41420117;
          }
        }
      } else {
        if (sample[0] <36) {
          if (sample[3] <110) {
            if (sample[1] <30.9999981) {
              sum[1] += 3.14378643;
            } else {
              sum[1] += 1.09868085;
            }
          } else {
            if (sample[4] <255) {
              sum[1] += -0.47603327;
            } else {
              sum[1] += 2.92698216;
            }
          }
        } else {
          if (sample[3] <98) {
            if (sample[1] <34) {
              sum[1] += 3.1301024;
            } else {
              sum[1] += 1.24180472;
            }
          } else {
            if (sample[0] <46) {
              sum[1] += 3.27959919;
            } else {
              sum[1] += 3.49345684;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <26.9999981) {
      if (sample[0] <46) {
        if (sample[0] <36) {
          if (sample[3] <80) {
            if (sample[3] <75) {
              sum[1] += -0.582821965;
            } else {
              sum[1] += -0.52973783;
            }
          } else {
            if (sample[3] <129) {
              sum[1] += -0.35729444;
            } else {
              sum[1] += -0.561429083;
            }
          }
        } else {
          if (sample[2] <21.9999981) {
            if (sample[0] <42) {
              sum[1] += 0.168820828;
            } else {
              sum[1] += 1.35013485;
            }
          } else {
            if (sample[3] <118) {
              sum[1] += -0.135125965;
            } else {
              sum[1] += -0.558102429;
            }
          }
        }
      } else {
        if (sample[3] <98) {
          if (sample[2] <21.9999981) {
            if (sample[0] <60) {
              sum[1] += 0.170945495;
            } else {
              sum[1] += 2.07479215;
            }
          } else {
            if (sample[5] <47) {
              sum[1] += 0.969773233;
            } else {
              sum[1] += -0.275251895;
            }
          }
        } else {
          if (sample[3] <117) {
            if (sample[4] <135) {
              sum[1] += 2.00501513;
            } else {
              sum[1] += 3.41910863;
            }
          } else {
            if (sample[0] <60) {
              sum[1] += -0.505243659;
            } else {
              sum[1] += 3.11460662;
            }
          }
        }
      }
    } else {
      if (sample[4] <87) {
        if (sample[4] <65) {
          if (sample[4] <56) {
            sum[1] += -0.583213985;
          } else {
            if (sample[3] <90) {
              sum[1] += -0.5825333;
            } else {
              sum[1] += -0.505929947;
            }
          }
        } else {
          if (sample[3] <98) {
            if (sample[3] <90) {
              sum[1] += -0.582335353;
            } else {
              sum[1] += -0.534906209;
            }
          } else {
            if (sample[3] <123) {
              sum[1] += -0.140253529;
            } else {
              sum[1] += -0.571430743;
            }
          }
        }
      } else {
        if (sample[3] <98) {
          if (sample[3] <90) {
            sum[1] += -0.582866549;
          } else {
            if (sample[1] <45) {
              sum[1] += 0.0381754898;
            } else {
              sum[1] += -0.571344376;
            }
          }
        } else {
          if (sample[3] <120) {
            if (sample[4] <113) {
              sum[1] += 1.23122501;
            } else {
              sum[1] += 3.02702689;
            }
          } else {
            if (sample[3] <129) {
              sum[1] += 0.0637358353;
            } else {
              sum[1] += -0.581015646;
            }
          }
        }
      }
    }
  }


  if (sample[4] <87) {
    if (sample[4] <65) {
      if (sample[4] <57) {
        sum[2] += -0.583229303;
      } else {
        if (sample[0] <105) {
          if (sample[0] <98) {
            if (sample[0] <75) {
              sum[2] += -0.582885683;
            } else {
              sum[2] += -0.56324029;
            }
          } else {
            if (sample[2] <96) {
              sum[2] += -0.406702489;
            } else {
              sum[2] += -0.58022815;
            }
          }
        } else {
          if (sample[2] <99) {
            if (sample[1] <122) {
              sum[2] += -0.367339313;
            } else {
              sum[2] += 2.27674747;
            }
          } else {
            if (sample[2] <127) {
              sum[2] += -0.389588058;
            } else {
              sum[2] += -0.570611954;
            }
          }
        }
      }
    } else {
      if (sample[1] <106) {
        if (sample[1] <100) {
          if (sample[1] <75) {
            sum[2] += -0.582797647;
          } else {
            if (sample[2] <69) {
              sum[2] += -0.327292144;
            } else {
              sum[2] += -0.567442596;
            }
          }
        } else {
          if (sample[2] <79) {
            if (sample[3] <83) {
              sum[2] += -0.304051876;
            } else {
              sum[2] += 1.57380891;
            }
          } else {
            if (sample[2] <82) {
              sum[2] += 0.0806657895;
            } else {
              sum[2] += -0.577080727;
            }
          }
        }
      } else {
        if (sample[3] <65) {
          if (sample[3] <60) {
            sum[2] += -0.582381845;
          } else {
            if (sample[1] <118) {
              sum[2] += 0.0153172705;
            } else {
              sum[2] += -0.351387054;
            }
          }
        } else {
          if (sample[3] <98) {
            if (sample[3] <83) {
              sum[2] += 0.995370626;
            } else {
              sum[2] += 2.72839451;
            }
          } else {
            if (sample[2] <108) {
              sum[2] += -0.140446126;
            } else {
              sum[2] += -0.561724961;
            }
          }
        }
      }
    }
  } else {
    if (sample[0] <102) {
      if (sample[3] <75) {
        if (sample[3] <73) {
          if (sample[3] <70) {
            if (sample[0] <40) {
              sum[2] += -0.583319247;
            } else {
              sum[2] += -0.578630269;
            }
          } else {
            if (sample[5] <78) {
              sum[2] += -0.580449104;
            } else {
              sum[2] += 0.619129181;
            }
          }
        } else {
          if (sample[1] <79) {
            if (sample[1] <75) {
              sum[2] += -0.581365228;
            } else {
              sum[2] += 0.0830860361;
            }
          } else {
            if (sample[2] <48) {
              sum[2] += 3.07726908;
            } else {
              sum[2] += 0.714893579;
            }
          }
        }
      } else {
        if (sample[1] <73) {
          if (sample[1] <69) {
            if (sample[1] <67) {
              sum[2] += -0.583054781;
            } else {
              sum[2] += -0.526421309;
            }
          } else {
            if (sample[2] <37) {
              sum[2] += 2.70408463;
            } else {
              sum[2] += -0.397123635;
            }
          }
        } else {
          if (sample[3] <98) {
            if (sample[4] <101) {
              sum[2] += 2.31015611;
            } else {
              sum[2] += 3.3009212;
            }
          } else {
            if (sample[3] <106) {
              sum[2] += -0.565672636;
            } else {
              sum[2] += -0.338361323;
            }
          }
        }
      }
    } else {
      if (sample[3] <98) {
        if (sample[3] <70) {
          if (sample[2] <106) {
            if (sample[0] <105) {
              sum[2] += 0.11602208;
            } else {
              sum[2] += 2.82153821;
            }
          } else {
            sum[2] += -0.583022296;
          }
        } else {
          if (sample[1] <100) {
            if (sample[0] <105) {
              sum[2] += 1.67417669;
            } else {
              sum[2] += 0.150134027;
            }
          } else {
            if (sample[2] <131) {
              sum[2] += 3.47340751;
            } else {
              sum[2] += -0.425414354;
            }
          }
        }
      } else {
        if (sample[3] <103) {
          if (sample[0] <130) {
            if (sample[1] <106) {
              sum[2] += 0.0137704741;
            } else {
              sum[2] += 2.10679603;
            }
          } else {
            if (sample[1] <134) {
              sum[2] += -0.498357058;
            } else {
              sum[2] += -1.55683768e-08;
            }
          }
        } else {
          if (sample[3] <106) {
            if (sample[2] <77) {
              sum[2] += -0.553213894;
            } else {
              sum[2] += -0.334073246;
            }
          } else {
            sum[2] += -0.583227336;
          }
        }
      }
    }
  }


  if (sample[4] <135) {
    if (sample[4] <113) {
      if (sample[4] <87) {
        sum[3] += -0.583275557;
      } else {
        if (sample[1] <106) {
          if (sample[1] <90) {
            sum[3] += -0.582903147;
          } else {
            if (sample[0] <60) {
              sum[3] += -0.175398648;
            } else {
              sum[3] += -0.579218209;
            }
          }
        } else {
          if (sample[3] <60) {
            if (sample[3] <40) {
              sum[3] += -0.492429912;
            } else {
              sum[3] += 0.781108439;
            }
          } else {
            sum[3] += -0.583279312;
          }
        }
      }
    } else {
      if (sample[1] <188) {
        if (sample[1] <130) {
          if (sample[1] <96) {
            if (sample[1] <90) {
              sum[3] += -0.58106792;
            } else {
              sum[3] += -0.533440113;
            }
          } else {
            if (sample[0] <63) {
              sum[3] += 0.932928741;
            } else {
              sum[3] += -0.549618959;
            }
          }
        } else {
          if (sample[3] <55) {
            if (sample[3] <38) {
              sum[3] += -0.316463858;
            } else {
              sum[3] += 3.09067583;
            }
          } else {
            sum[3] += -0.583069742;
          }
        }
      } else {
        if (sample[0] <109) {
          if (sample[2] <166) {
            if (sample[2] <121) {
              sum[3] += -0.464730293;
            } else {
              sum[3] += 3.44838405;
            }
          } else {
            if (sample[3] <38) {
              sum[3] += -0.573770463;
            } else {
              sum[3] += 1.06970501;
            }
          }
        } else {
          if (sample[0] <112) {
            if (sample[2] <155) {
              sum[3] += 2.40764332;
            } else {
              sum[3] += -0.532743335;
            }
          } else {
            sum[3] += -0.574163795;
          }
        }
      }
    }
  } else {
    if (sample[1] <87) {
      if (sample[1] <71) {
        if (sample[1] <69) {
          if (sample[1] <67) {
            if (sample[1] <59) {
              sum[3] += -0.583314776;
            } else {
              sum[3] += -0.554334283;
            }
          } else {
            if (sample[2] <45) {
              sum[3] += 0.938071012;
            } else {
              sum[3] += -0.56323278;
            }
          }
        } else {
          if (sample[2] <52) {
            if (sample[3] <53) {
              sum[3] += 3.29312396;
            } else {
              sum[3] += -0.507345736;
            }
          } else {
            if (sample[2] <62) {
              sum[3] += 0.22580643;
            } else {
              sum[3] += -0.579612017;
            }
          }
        }
      } else {
        if (sample[0] <23.9999981) {
          if (sample[3] <38) {
            if (sample[2] <67) {
              sum[3] += -1.09798028e-08;
            } else {
              sum[3] += -0.572505236;
            }
          } else {
            sum[3] += 3.45901728;
          }
        } else {
          if (sample[0] <36) {
            if (sample[3] <38) {
              sum[3] += -0.559645593;
            } else {
              sum[3] += 2.47967863;
            }
          } else {
            if (sample[0] <40) {
              sum[3] += -0.405641794;
            } else {
              sum[3] += -0.576909542;
            }
          }
        }
      }
    } else {
      if (sample[3] <38) {
        if (sample[2] <93) {
          if (sample[1] <93) {
            if (sample[2] <79) {
              sum[3] += 2.21846151;
            } else {
              sum[3] += 0.271624863;
            }
          } else {
            if (sample[2] <89) {
              sum[3] += 2.95508051;
            } else {
              sum[3] += 2.10196424;
            }
          }
        } else {
          if (sample[3] <30.9999981) {
            sum[3] += -0.583104491;
          } else {
            if (sample[2] <166) {
              sum[3] += 0.469813943;
            } else {
              sum[3] += -0.570932746;
            }
          }
        }
      } else {
        if (sample[3] <62) {
          sum[3] += 3.4956584;
        } else {
          sum[3] += -0.582187116;
        }
      }
    }
  }


  if (sample[3] <133) {
    if (sample[3] <129) {
      sum[4] += -0.583271563;
    } else {
      if (sample[4] <56) {
        if (sample[4] <54) {
          sum[4] += -0.582799256;
        } else {
          if (sample[0] <88) {
            sum[4] += -0.422037423;
          } else {
            sum[4] += -0.0482758731;
          }
        }
      } else {
        if (sample[5] <83) {
          if (sample[0] <68) {
            if (sample[2] <55) {
              sum[4] += -0.574881494;
            } else {
              sum[4] += 0.462809891;
            }
          } else {
            if (sample[4] <76) {
              sum[4] += -0.128648654;
            } else {
              sum[4] += 0.688524544;
            }
          }
        } else {
          if (sample[4] <113) {
            if (sample[1] <87) {
              sum[4] += 2.61227775;
            } else {
              sum[4] += 0.744221807;
            }
          } else {
            if (sample[4] <135) {
              sum[4] += 0.45161286;
            } else {
              sum[4] += -0.5090909;
            }
          }
        }
      }
    }
  } else {
    if (sample[4] <65) {
      if (sample[4] <52) {
        if (sample[4] <42) {
          if (sample[4] <31.9999981) {
            if (sample[2] <52) {
              sum[4] += -0.527801692;
            } else {
              sum[4] += -0.583230734;
            }
          } else {
            if (sample[3] <165) {
              sum[4] += -0.531747639;
            } else {
              sum[4] += -0.575711966;
            }
          }
        } else {
          if (sample[0] <68) {
            if (sample[2] <55) {
              sum[4] += -0.57378459;
            } else {
              sum[4] += -0.469694376;
            }
          } else {
            if (sample[3] <165) {
              sum[4] += -0.182325587;
            } else {
              sum[4] += -0.522026062;
            }
          }
        }
      } else {
        if (sample[0] <75) {
          if (sample[0] <53) {
            sum[4] += -0.561291575;
          } else {
            if (sample[0] <66) {
              sum[4] += -0.360977471;
            } else {
              sum[4] += -0.115054175;
            }
          }
        } else {
          if (sample[3] <165) {
            if (sample[4] <57) {
              sum[4] += 0.838557243;
            } else {
              sum[4] += 1.7834394;
            }
          } else {
            if (sample[1] <100) {
              sum[4] += -0.280180752;
            } else {
              sum[4] += -0.563661397;
            }
          }
        }
      }
    } else {
      if (sample[0] <60) {
        if (sample[2] <59) {
          if (sample[5] <56) {
            sum[4] += -0.582974494;
          } else {
            if (sample[0] <49) {
              sum[4] += -0.519101143;
            } else {
              sum[4] += -0.289013296;
            }
          }
        } else {
          if (sample[0] <53) {
            if (sample[0] <46) {
              sum[4] += -0.526732683;
            } else {
              sum[4] += 0.0175328571;
            }
          } else {
            if (sample[1] <45) {
              sum[4] += 2.81982994;
            } else {
              sum[4] += 0.299465209;
            }
          }
        }
      } else {
        if (sample[3] <165) {
          if (sample[2] <59) {
            if (sample[4] <101) {
              sum[4] += 0.341116726;
            } else {
              sum[4] += 2.25078034;
            }
          } else {
            if (sample[4] <76) {
              sum[4] += 3.01301241;
            } else {
              sum[4] += 3.49235749;
            }
          }
        } else {
          if (sample[4] <87) {
            if (sample[5] <164) {
              sum[4] += 0.661346912;
            } else {
              sum[4] += -0.532743335;
            }
          } else {
            if (sample[5] <150) {
              sum[4] += 3.14577413;
            } else {
              sum[4] += -0.488372087;
            }
          }
        }
      }
    }
  }


  if (sample[4] <234) {
    if (sample[4] <206) {
      if (sample[3] <14.999999) {
        if (sample[4] <113) {
          if (sample[4] <101) {
            if (sample[4] <76) {
              sum[5] += -0.582812548;
            } else {
              sum[5] += -0.353380293;
            }
          } else {
            if (sample[1] <53) {
              sum[5] += -0.486404836;
            } else {
              sum[5] += 0.604008138;
            }
          }
        } else {
          if (sample[5] <83) {
            if (sample[5] <72) {
              sum[5] += -0.408901751;
            } else {
              sum[5] += 0.762076378;
            }
          } else {
            if (sample[3] <10.999999) {
              sum[5] += 3.05182648;
            } else {
              sum[5] += 1.8198756;
            }
          }
        }
      } else {
        if (sample[3] <16.9999981) {
          if (sample[4] <113) {
            if (sample[0] <53) {
              sum[5] += -0.533765435;
            } else {
              sum[5] += -0.583249032;
            }
          } else {
            if (sample[1] <45) {
              sum[5] += -0.508949757;
            } else {
              sum[5] += -0.287618935;
            }
          }
        } else {
          if (sample[3] <179) {
            sum[5] += -0.583266616;
          } else {
            if (sample[0] <63) {
              sum[5] += -0.224000007;
            } else {
              sum[5] += -0.570393801;
            }
          }
        }
      }
    } else {
      if (sample[3] <14.999999) {
        if (sample[2] <89) {
          if (sample[5] <74) {
            if (sample[2] <62) {
              sum[5] += -0.521536648;
            } else {
              sum[5] += -0.190717578;
            }
          } else {
            if (sample[0] <13.999999) {
              sum[5] += 1.22536767;
            } else {
              sum[5] += 0.428389251;
            }
          }
        } else {
          if (sample[2] <96) {
            if (sample[1] <51) {
              sum[5] += 2.28172278;
            } else {
              sum[5] += 0.626865625;
            }
          } else {
            if (sample[3] <10.999999) {
              sum[5] += 3.42045593;
            } else {
              sum[5] += 3.13285375;
            }
          }
        }
      } else {
        if (sample[3] <16.9999981) {
          if (sample[5] <76) {
            if (sample[0] <13.999999) {
              sum[5] += -0.518803358;
            } else {
              sum[5] += 0.0725388452;
            }
          } else {
            if (sample[1] <47) {
              sum[5] += 0.286746949;
            } else {
              sum[5] += -0.192511916;
            }
          }
        } else {
          if (sample[3] <19.9999981) {
            if (sample[2] <123) {
              sum[5] += -0.561051846;
            } else {
              sum[5] += -0.178913742;
            }
          } else {
            sum[5] += -0.583289087;
          }
        }
      }
    }
  } else {
    if (sample[2] <79) {
      if (sample[2] <69) {
        if (sample[2] <62) {
          if (sample[2] <59) {
            if (sample[2] <52) {
              sum[5] += -0.583200932;
            } else {
              sum[5] += -0.560000002;
            }
          } else {
            if (sample[3] <16.9999981) {
              sum[5] += -0.248509735;
            } else {
              sum[5] += -0.526623547;
            }
          }
        } else {
          if (sample[3] <14.999999) {
            if (sample[2] <67) {
              sum[5] += 0.397533298;
            } else {
              sum[5] += 1.28133297;
            }
          } else {
            if (sample[1] <34) {
              sum[5] += -0.170109361;
            } else {
              sum[5] += -0.5321787;
            }
          }
        }
      } else {
        if (sample[1] <34) {
          if (sample[1] <30.9999981) {
            if (sample[4] <255) {
              sum[5] += 1.4622823;
            } else {
              sum[5] += 3.06122637;
            }
          } else {
            if (sample[5] <72) {
              sum[5] += 0.715034008;
            } else {
              sum[5] += 2.55538821;
            }
          }
        } else {
          if (sample[3] <14.999999) {
            if (sample[5] <74) {
              sum[5] += -0.117564738;
            } else {
              sum[5] += 1.0519377;
            }
          } else {
            if (sample[3] <16.9999981) {
              sum[5] += -0.320602804;
            } else {
              sum[5] += -0.568302214;
            }
          }
        }
      }
    } else {
      if (sample[3] <14.999999) {
        if (sample[5] <85) {
          if (sample[1] <34) {
            sum[5] += 3.25763154;
          } else {
            if (sample[1] <38) {
              sum[5] += 2.57512403;
            } else {
              sum[5] += 1.70242608;
            }
          }
        } else {
          if (sample[3] <10.999999) {
            sum[5] += 3.49372983;
          } else {
            if (sample[2] <96) {
              sum[5] += 3.1608417;
            } else {
              sum[5] += 3.3866384;
            }
          }
        }
      } else {
        if (sample[3] <16.9999981) {
          if (sample[2] <93) {
            if (sample[1] <42) {
              sum[5] += 0.453896463;
            } else {
              sum[5] += -0.0642885789;
            }
          } else {
            if (sample[1] <56) {
              sum[5] += 1.00122869;
            } else {
              sum[5] += -0.300873071;
            }
          }
        } else {
          if (sample[1] <62) {
            if (sample[2] <99) {
              sum[5] += -0.528531432;
            } else {
              sum[5] += -0.128349796;
            }
          } else {
            sum[5] += -0.579762638;
          }
        }
      }
    }
  }


  if (sample[4] <135) {
    if (sample[4] <113) {
      if (sample[4] <87) {
        sum[6] += -0.583294272;
      } else {
        if (sample[2] <178) {
          if (sample[3] <38) {
            if (sample[1] <129) {
              sum[6] += -0.570717812;
            } else {
              sum[6] += -0.236017823;
            }
          } else {
            sum[6] += -0.582696557;
          }
        } else {
          if (sample[3] <26.9999981) {
            if (sample[4] <101) {
              sum[6] += -0.575551331;
            } else {
              sum[6] += -0.353736639;
            }
          } else {
            if (sample[0] <117) {
              sum[6] += 1.33546722;
            } else {
              sum[6] += -0.220433876;
            }
          }
        }
      }
    } else {
      if (sample[2] <178) {
        if (sample[3] <30.9999981) {
          if (sample[1] <119) {
            if (sample[1] <103) {
              sum[6] += -0.562450111;
            } else {
              sum[6] += 0.178410769;
            }
          } else {
            if (sample[0] <82) {
              sum[6] += 2.48043609;
            } else {
              sum[6] += 0.256010801;
            }
          }
        } else {
          if (sample[3] <38) {
            if (sample[1] <106) {
              sum[6] += -0.57860148;
            } else {
              sum[6] += 0.388362616;
            }
          } else {
            sum[6] += -0.582873106;
          }
        }
      } else {
        if (sample[3] <23.9999981) {
          if (sample[3] <19.9999981) {
            sum[6] += -0.517320991;
          } else {
            if (sample[1] <176) {
              sum[6] += 1.59517419;
            } else {
              sum[6] += 0.145228192;
            }
          }
        } else {
          if (sample[1] <176) {
            if (sample[0] <95) {
              sum[6] += 1.46511614;
            } else {
              sum[6] += 0.348547697;
            }
          } else {
            if (sample[0] <127) {
              sum[6] += 3.22198176;
            } else {
              sum[6] += -0.181347162;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <191) {
      if (sample[1] <96) {
        if (sample[1] <77) {
          if (sample[1] <69) {
            if (sample[1] <56) {
              sum[6] += -0.583304226;
            } else {
              sum[6] += -0.570908487;
            }
          } else {
            if (sample[0] <13.999999) {
              sum[6] += -0.281359375;
            } else {
              sum[6] += -0.560980916;
            }
          }
        } else {
          if (sample[3] <38) {
            if (sample[3] <23.9999981) {
              sum[6] += -0.294041544;
            } else {
              sum[6] += 1.9837687;
            }
          } else {
            if (sample[2] <71) {
              sum[6] += -0.579488754;
            } else {
              sum[6] += -0.423487544;
            }
          }
        }
      } else {
        if (sample[3] <38) {
          if (sample[2] <96) {
            if (sample[2] <89) {
              sum[6] += -0.302866429;
            } else {
              sum[6] += 0.574402869;
            }
          } else {
            if (sample[3] <19.9999981) {
              sum[6] += 0.372541428;
            } else {
              sum[6] += 3.43702388;
            }
          }
        } else {
          if (sample[3] <40) {
            sum[6] += -0.553723097;
          } else {
            sum[6] += -0.583137155;
          }
        }
      }
    } else {
      if (sample[3] <19.9999981) {
        sum[6] += -0.581377804;
      } else {
        sum[6] += 3.49965024;
      }
    }
  }


  return sum;
}

