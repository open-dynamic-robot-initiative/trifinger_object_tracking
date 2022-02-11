#include <trifinger_object_tracking/xgboost_classifier.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

namespace trifinger_object_tracking {
std::array<float, XGB_NUM_CLASSES> xgb_classify_cuboid_v2(std::array<float, XGB_NUM_FEATURES> &sample) {

  std::array<float, XGB_NUM_CLASSES> sum;
  sum.fill(0.0);

  if (sample[4] <95) {
    if (sample[4] <72) {
      if (sample[4] <62) {
        if (sample[4] <44) {
          sum[0] += 3.49958777;
        } else {
          if (sample[3] <123) {
            if (sample[3] <83) {
              sum[0] += 3.49473572;
            } else {
              sum[0] += 3.43639994;
            }
          } else {
            if (sample[2] <76) {
              sum[0] += 3.31641483;
            } else {
              sum[0] += 2.82041311;
            }
          }
        }
      } else {
        if (sample[0] <97) {
          if (sample[3] <96) {
            if (sample[1] <82) {
              sum[0] += 3.49544168;
            } else {
              sum[0] += 3.37871146;
            }
          } else {
            if (sample[2] <74) {
              sum[0] += 3.18455529;
            } else {
              sum[0] += 1.041237;
            }
          }
        } else {
          if (sample[3] <129) {
            if (sample[2] <112) {
              sum[0] += 2.35607219;
            } else {
              sum[0] += 3.42651439;
            }
          } else {
            if (sample[3] <165) {
              sum[0] += -0.19732821;
            } else {
              sum[0] += 2.70521855;
            }
          }
        }
      }
    } else {
      if (sample[0] <86) {
        if (sample[3] <133) {
          if (sample[3] <100) {
            if (sample[1] <76) {
              sum[0] += 3.49272919;
            } else {
              sum[0] += 3.16762924;
            }
          } else {
            if (sample[3] <111) {
              sum[0] += 2.4120481;
            } else {
              sum[0] += 3.08397245;
            }
          }
        } else {
          if (sample[0] <65) {
            if (sample[5] <61) {
              sum[0] += 3.41192746;
            } else {
              sum[0] += 1.90644014;
            }
          } else {
            if (sample[3] <165) {
              sum[0] += -0.429553807;
            } else {
              sum[0] += 2.1328609;
            }
          }
        }
      } else {
        if (sample[3] <65) {
          if (sample[3] <43) {
            sum[0] += 3.43602228;
          } else {
            if (sample[2] <91) {
              sum[0] += 1.60458446;
            } else {
              sum[0] += 2.70330548;
            }
          }
        } else {
          if (sample[3] <129) {
            if (sample[3] <96) {
              sum[0] += -0.0928667784;
            } else {
              sum[0] += 3.02109408;
            }
          } else {
            if (sample[3] <165) {
              sum[0] += -0.555294275;
            } else {
              sum[0] += 1.45976198;
            }
          }
        }
      }
    }
  } else {
    if (sample[5] <73) {
      if (sample[3] <86) {
        if (sample[5] <68) {
          if (sample[3] <78) {
            if (sample[5] <58) {
              sum[0] += 3.49710608;
            } else {
              sum[0] += 3.25699115;
            }
          } else {
            if (sample[1] <31.9999981) {
              sum[0] += 1.02514875;
            } else {
              sum[0] += 3.27683139;
            }
          }
        } else {
          if (sample[1] <31.9999981) {
            if (sample[4] <255) {
              sum[0] += 1.84551334;
            } else {
              sum[0] += 0.246768475;
            }
          } else {
            if (sample[2] <54) {
              sum[0] += 1.29989672;
            } else {
              sum[0] += 3.19437337;
            }
          }
        }
      } else {
        if (sample[3] <113) {
          if (sample[4] <156) {
            if (sample[3] <100) {
              sum[0] += 2.64092159;
            } else {
              sum[0] += 0.434197783;
            }
          } else {
            if (sample[0] <9.99999905) {
              sum[0] += 3.44688749;
            } else {
              sum[0] += -0.348675758;
            }
          }
        } else {
          if (sample[5] <64) {
            if (sample[3] <115) {
              sum[0] += 2.29967809;
            } else {
              sum[0] += 3.39522314;
            }
          } else {
            if (sample[5] <68) {
              sum[0] += 0.859945595;
            } else {
              sum[0] += -0.381161481;
            }
          }
        }
      }
    } else {
      if (sample[5] <84) {
        if (sample[3] <38) {
          if (sample[1] <35) {
            if (sample[1] <31.9999981) {
              sum[0] += -0.213556334;
            } else {
              sum[0] += 0.576160789;
            }
          } else {
            if (sample[3] <14.999999) {
              sum[0] += 1.92416632;
            } else {
              sum[0] += 3.154706;
            }
          }
        } else {
          if (sample[4] <124) {
            if (sample[3] <133) {
              sum[0] += 1.41617453;
            } else {
              sum[0] += -0.513225019;
            }
          } else {
            if (sample[4] <156) {
              sum[0] += -0.333623827;
            } else {
              sum[0] += -0.579836071;
            }
          }
        }
      } else {
        if (sample[3] <24.9999981) {
          if (sample[4] <124) {
            if (sample[1] <68) {
              sum[0] += 1.36136901;
            } else {
              sum[0] += 3.17197108;
            }
          } else {
            if (sample[3] <14.999999) {
              sum[0] += -0.455350935;
            } else {
              sum[0] += 0.565160215;
            }
          }
        } else {
          if (sample[4] <108) {
            if (sample[3] <75) {
              sum[0] += 2.43448043;
            } else {
              sum[0] += -0.508681893;
            }
          } else {
            if (sample[4] <156) {
              sum[0] += -0.515387893;
            } else {
              sum[0] += -0.578871667;
            }
          }
        }
      }
    }
  }


  if (sample[2] <5.99999905) {
    if (sample[3] <78) {
      if (sample[3] <75) {
        if (sample[3] <68) {
          sum[1] += -0.58317101;
        } else {
          if (sample[1] <31.9999981) {
            sum[1] += -0.502488077;
          } else {
            if (sample[1] <35) {
              sum[1] += -0.58079797;
            } else {
              sum[1] += -0.413422823;
            }
          }
        }
      } else {
        if (sample[5] <33) {
          if (sample[1] <26.9999981) {
            if (sample[0] <9.99999905) {
              sum[1] += -0.536704719;
            } else {
              sum[1] += 0.152475223;
            }
          } else {
            if (sample[4] <232) {
              sum[1] += 1.62818325;
            } else {
              sum[1] += 0.325581372;
            }
          }
        } else {
          sum[1] += -0.466234952;
        }
      }
    } else {
      if (sample[0] <9.99999905) {
        sum[1] += -0.580034614;
      } else {
        if (sample[0] <28.9999981) {
          if (sample[3] <110) {
            if (sample[4] <255) {
              sum[1] += 2.44917655;
            } else {
              sum[1] += 3.36515307;
            }
          } else {
            if (sample[4] <255) {
              sum[1] += -0.554937422;
            } else {
              sum[1] += 0.99999994;
            }
          }
        } else {
          if (sample[0] <37) {
            if (sample[3] <108) {
              sum[1] += 3.31525993;
            } else {
              sum[1] += 0.21620588;
            }
          } else {
            if (sample[3] <96) {
              sum[1] += 1.61077106;
            } else {
              sum[1] += 3.4809339;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <23.9999981) {
      if (sample[0] <45) {
        if (sample[3] <83) {
          if (sample[3] <78) {
            if (sample[3] <72) {
              sum[1] += -0.582524538;
            } else {
              sum[1] += -0.523729622;
            }
          } else {
            if (sample[1] <31.9999981) {
              sum[1] += 0.556515038;
            } else {
              sum[1] += -0.376981616;
            }
          }
        } else {
          if (sample[3] <111) {
            if (sample[4] <156) {
              sum[1] += 0.41028595;
            } else {
              sum[1] += 2.22988296;
            }
          } else {
            if (sample[3] <116) {
              sum[1] += -0.0589251183;
            } else {
              sum[1] += -0.536012113;
            }
          }
        }
      } else {
        if (sample[3] <96) {
          if (sample[1] <43) {
            if (sample[4] <156) {
              sum[1] += 2.12474012;
            } else {
              sum[1] += 0.165680453;
            }
          } else {
            if (sample[5] <58) {
              sum[1] += -0.136333123;
            } else {
              sum[1] += 0.979190707;
            }
          }
        } else {
          if (sample[3] <120) {
            if (sample[4] <156) {
              sum[1] += 2.94187212;
            } else {
              sum[1] += 3.4675703;
            }
          } else {
            if (sample[0] <56) {
              sum[1] += -0.562989295;
            } else {
              sum[1] += 1.92660546;
            }
          }
        }
      }
    } else {
      if (sample[4] <95) {
        if (sample[4] <62) {
          if (sample[1] <35) {
            if (sample[4] <57) {
              sum[1] += -0.536548018;
            } else {
              sum[1] += -0.286191821;
            }
          } else {
            if (sample[4] <43) {
              sum[1] += -0.583254635;
            } else {
              sum[1] += -0.580551326;
            }
          }
        } else {
          if (sample[3] <96) {
            if (sample[3] <83) {
              sum[1] += -0.582241356;
            } else {
              sum[1] += -0.555650949;
            }
          } else {
            if (sample[2] <45) {
              sum[1] += 0.0750043839;
            } else {
              sum[1] += -0.49993059;
            }
          }
        }
      } else {
        if (sample[3] <96) {
          if (sample[3] <90) {
            sum[1] += -0.582483768;
          } else {
            if (sample[1] <47) {
              sum[1] += 0.304347783;
            } else {
              sum[1] += -0.571457446;
            }
          }
        } else {
          if (sample[3] <120) {
            if (sample[3] <102) {
              sum[1] += 0.118610188;
            } else {
              sum[1] += 2.72903395;
            }
          } else {
            if (sample[3] <129) {
              sum[1] += 0.0361235291;
            } else {
              sum[1] += -0.57951051;
            }
          }
        }
      }
    }
  }


  if (sample[4] <83) {
    if (sample[4] <62) {
      if (sample[4] <41) {
        sum[2] += -0.583159089;
      } else {
        if (sample[1] <76) {
          if (sample[5] <68) {
            sum[2] += -0.583278835;
          } else {
            sum[2] += -0.575871646;
          }
        } else {
          if (sample[3] <53) {
            if (sample[3] <47) {
              sum[2] += -0.579845786;
            } else {
              sum[2] += -0.546450377;
            }
          } else {
            if (sample[3] <96) {
              sum[2] += -0.431144893;
            } else {
              sum[2] += -0.569521666;
            }
          }
        }
      }
    } else {
      if (sample[1] <101) {
        if (sample[1] <82) {
          if (sample[1] <76) {
            if (sample[1] <68) {
              sum[2] += -0.583006501;
            } else {
              sum[2] += -0.560041189;
            }
          } else {
            if (sample[2] <74) {
              sum[2] += -0.421712607;
            } else {
              sum[2] += -0.572580814;
            }
          }
        } else {
          if (sample[2] <74) {
            if (sample[1] <98) {
              sum[2] += -0.122916736;
            } else {
              sum[2] += 0.893291652;
            }
          } else {
            if (sample[2] <78) {
              sum[2] += -0.402501762;
            } else {
              sum[2] += -0.572332084;
            }
          }
        }
      } else {
        if (sample[2] <98) {
          if (sample[3] <96) {
            if (sample[3] <86) {
              sum[2] += 0.503334045;
            } else {
              sum[2] += 2.99210835;
            }
          } else {
            if (sample[3] <100) {
              sum[2] += 0.0603231378;
            } else {
              sum[2] += -0.427371293;
            }
          }
        } else {
          if (sample[3] <62) {
            sum[2] += -0.581042051;
          } else {
            if (sample[3] <96) {
              sum[2] += 1.64250124;
            } else {
              sum[2] += -0.488223255;
            }
          }
        }
      }
    }
  } else {
    if (sample[0] <100) {
      if (sample[3] <75) {
        if (sample[3] <72) {
          if (sample[0] <40) {
            sum[2] += -0.583105862;
          } else {
            if (sample[3] <53) {
              sum[2] += -0.574401796;
            } else {
              sum[2] += 0.0264292117;
            }
          }
        } else {
          if (sample[1] <80) {
            if (sample[5] <73) {
              sum[2] += -0.576012969;
            } else {
              sum[2] += 0.294601977;
            }
          } else {
            if (sample[0] <72) {
              sum[2] += 2.6519587;
            } else {
              sum[2] += 0.547231197;
            }
          }
        }
      } else {
        if (sample[1] <74) {
          if (sample[1] <70) {
            if (sample[1] <68) {
              sum[2] += -0.582602441;
            } else {
              sum[2] += -0.276679844;
            }
          } else {
            if (sample[2] <37) {
              sum[2] += 3.06830525;
            } else {
              sum[2] += -0.23341006;
            }
          }
        } else {
          if (sample[3] <96) {
            if (sample[4] <95) {
              sum[2] += 1.93951583;
            } else {
              sum[2] += 3.37526083;
            }
          } else {
            if (sample[3] <105) {
              sum[2] += -0.469172359;
            } else {
              sum[2] += 0.0111066857;
            }
          }
        }
      }
    } else {
      if (sample[3] <96) {
        if (sample[3] <70) {
          if (sample[2] <104) {
            if (sample[0] <104) {
              sum[2] += 0.11602208;
            } else {
              sum[2] += 2.38415837;
            }
          } else {
            if (sample[2] <109) {
              sum[2] += 0.14558056;
            } else {
              sum[2] += -0.583145678;
            }
          }
        } else {
          if (sample[5] <206) {
            if (sample[1] <98) {
              sum[2] += 1.78638935;
            } else {
              sum[2] += 3.4747231;
            }
          } else {
            sum[2] += -0.34710744;
          }
        }
      } else {
        if (sample[3] <100) {
          if (sample[1] <104) {
            if (sample[0] <107) {
              sum[2] += -0.00856794696;
            } else {
              sum[2] += 0.531386793;
            }
          } else {
            if (sample[1] <114) {
              sum[2] += 2.22299814;
            } else {
              sum[2] += 0.432294577;
            }
          }
        } else {
          if (sample[3] <102) {
            if (sample[0] <122) {
              sum[2] += -0.295774639;
            } else {
              sum[2] += 0.330708623;
            }
          } else {
            if (sample[3] <106) {
              sum[2] += -0.369961172;
            } else {
              sum[2] += -0.582876682;
            }
          }
        }
      }
    }
  }


  if (sample[4] <124) {
    if (sample[4] <108) {
      if (sample[4] <83) {
        sum[3] += -0.583256006;
      } else {
        if (sample[1] <112) {
          if (sample[1] <76) {
            sum[3] += -0.582918942;
          } else {
            if (sample[0] <65) {
              sum[3] += -0.484598696;
            } else {
              sum[3] += -0.580708861;
            }
          }
        } else {
          if (sample[0] <81) {
            if (sample[3] <38) {
              sum[3] += -0.279237568;
            } else {
              sum[3] += 1.25685;
            }
          } else {
            if (sample[3] <60) {
              sum[3] += -0.371106416;
            } else {
              sum[3] += -0.583256423;
            }
          }
        }
      }
    } else {
      if (sample[5] <190) {
        if (sample[1] <101) {
          if (sample[1] <91) {
            if (sample[1] <58) {
              sum[3] += -0.582927644;
            } else {
              sum[3] += -0.574911714;
            }
          } else {
            if (sample[0] <56) {
              sum[3] += 0.73597157;
            } else {
              sum[3] += -0.577039063;
            }
          }
        } else {
          if (sample[3] <60) {
            if (sample[3] <38) {
              sum[3] += -0.339910835;
            } else {
              sum[3] += 2.25053406;
            }
          } else {
            sum[3] += -0.583236635;
          }
        }
      } else {
        if (sample[0] <113) {
          if (sample[3] <38) {
            sum[3] += -0.558542907;
          } else {
            if (sample[2] <121) {
              sum[3] += -0.34710744;
            } else {
              sum[3] += 3.30230784;
            }
          }
        } else {
          if (sample[0] <116) {
            if (sample[2] <174) {
              sum[3] += 1.47706413;
            } else {
              sum[3] += -0.475471705;
            }
          } else {
            sum[3] += -0.582572103;
          }
        }
      }
    }
  } else {
    if (sample[1] <74) {
      if (sample[1] <70) {
        if (sample[1] <61) {
          if (sample[1] <58) {
            sum[3] += -0.583021581;
          } else {
            if (sample[5] <61) {
              sum[3] += -0.229357809;
            } else {
              sum[3] += -0.579545617;
            }
          }
        } else {
          if (sample[2] <51) {
            if (sample[0] <25.9999981) {
              sum[3] += 3.02749252;
            } else {
              sum[3] += -0.457718313;
            }
          } else {
            if (sample[2] <63) {
              sum[3] += -0.268847793;
            } else {
              sum[3] += -0.579244077;
            }
          }
        }
      } else {
        if (sample[2] <58) {
          if (sample[0] <28.9999981) {
            if (sample[0] <25.9999981) {
              sum[3] += 3.40352321;
            } else {
              sum[3] += 1.10526311;
            }
          } else {
            if (sample[3] <78) {
              sum[3] += 1.03675663;
            } else {
              sum[3] += -0.578029275;
            }
          }
        } else {
          if (sample[2] <74) {
            if (sample[0] <28.9999981) {
              sum[3] += 0.804400921;
            } else {
              sum[3] += -0.364928901;
            }
          } else {
            sum[3] += -0.577740431;
          }
        }
      }
    } else {
      if (sample[3] <33) {
        if (sample[3] <28.9999981) {
          if (sample[5] <118) {
            if (sample[0] <56) {
              sum[3] += -0.569753349;
            } else {
              sum[3] += -0.331420362;
            }
          } else {
            sum[3] += -0.582656622;
          }
        } else {
          if (sample[5] <107) {
            if (sample[1] <101) {
              sum[3] += -0.258939594;
            } else {
              sum[3] += 0.257482827;
            }
          } else {
            if (sample[0] <49) {
              sum[3] += -0.570566416;
            } else {
              sum[3] += -0.390814215;
            }
          }
        }
      } else {
        if (sample[3] <60) {
          if (sample[3] <38) {
            if (sample[2] <163) {
              sum[3] += 2.22934723;
            } else {
              sum[3] += -0.371531963;
            }
          } else {
            if (sample[2] <174) {
              sum[3] += 3.48989677;
            } else {
              sum[3] += -0.34710744;
            }
          }
        } else {
          if (sample[0] <49) {
            if (sample[1] <78) {
              sum[3] += 1.27272725;
            } else {
              sum[3] += -0.290993065;
            }
          } else {
            sum[3] += -0.582580686;
          }
        }
      }
    }
  }


  if (sample[3] <133) {
    if (sample[3] <129) {
      if (sample[3] <125) {
        sum[4] += -0.583254576;
      } else {
        if (sample[4] <49) {
          if (sample[4] <44) {
            sum[4] += -0.583299994;
          } else {
            if (sample[2] <81) {
              sum[4] += -0.450847447;
            } else {
              sum[4] += -0.566801608;
            }
          }
        } else {
          if (sample[0] <90) {
            if (sample[0] <67) {
              sum[4] += -0.558165491;
            } else {
              sum[4] += -0.159423053;
            }
          } else {
            if (sample[1] <85) {
              sum[4] += 0.861047745;
            } else {
              sum[4] += -0.5090909;
            }
          }
        }
      }
    } else {
      if (sample[4] <56) {
        if (sample[4] <53) {
          if (sample[4] <50) {
            sum[4] += -0.58290112;
          } else {
            if (sample[0] <62) {
              sum[4] += -0.264483631;
            } else {
              sum[4] += -0.50359714;
            }
          }
        } else {
          if (sample[1] <47) {
            sum[4] += -0.522388041;
          } else {
            if (sample[0] <97) {
              sum[4] += 0.0200860668;
            } else {
              sum[4] += -0.475471705;
            }
          }
        }
      } else {
        if (sample[2] <58) {
          if (sample[2] <54) {
            if (sample[5] <61) {
              sum[4] += -0.578774154;
            } else {
              sum[4] += -0.322135299;
            }
          } else {
            if (sample[4] <57) {
              sum[4] += 1.84210515;
            } else {
              sum[4] += 0.0920043588;
            }
          }
        } else {
          if (sample[0] <93) {
            if (sample[4] <72) {
              sum[4] += 0.429772884;
            } else {
              sum[4] += 1.96971595;
            }
          } else {
            if (sample[0] <122) {
              sum[4] += 2.97576189;
            } else {
              sum[4] += 0.928881109;
            }
          }
        }
      }
    }
  } else {
    if (sample[4] <72) {
      if (sample[4] <62) {
        if (sample[4] <52) {
          if (sample[4] <42) {
            if (sample[4] <34) {
              sum[4] += -0.583053648;
            } else {
              sum[4] += -0.53736347;
            }
          } else {
            if (sample[0] <69) {
              sum[4] += -0.550915778;
            } else {
              sum[4] += -0.305205315;
            }
          }
        } else {
          if (sample[0] <69) {
            if (sample[0] <56) {
              sum[4] += -0.54607439;
            } else {
              sum[4] += -0.216123506;
            }
          } else {
            if (sample[3] <165) {
              sum[4] += 1.28310335;
            } else {
              sum[4] += -0.354373604;
            }
          }
        }
      } else {
        if (sample[0] <72) {
          if (sample[0] <56) {
            if (sample[2] <61) {
              sum[4] += -0.579876661;
            } else {
              sum[4] += -0.413359135;
            }
          } else {
            if (sample[3] <165) {
              sum[4] += 0.122448958;
            } else {
              sum[4] += -0.413359135;
            }
          }
        } else {
          if (sample[3] <165) {
            if (sample[2] <74) {
              sum[4] += 1.98852217;
            } else {
              sum[4] += 3.1080575;
            }
          } else {
            if (sample[0] <93) {
              sum[4] += -0.398540139;
            } else {
              sum[4] += 0.0179257188;
            }
          }
        }
      }
    } else {
      if (sample[0] <62) {
        if (sample[2] <58) {
          if (sample[5] <58) {
            sum[4] += -0.579295099;
          } else {
            if (sample[2] <54) {
              sum[4] += -0.380800009;
            } else {
              sum[4] += -0.0305676982;
            }
          }
        } else {
          if (sample[0] <56) {
            if (sample[0] <45) {
              sum[4] += -0.547826052;
            } else {
              sum[4] += 0.257537007;
            }
          } else {
            if (sample[1] <47) {
              sum[4] += 2.51999974;
            } else {
              sum[4] += 0.279534072;
            }
          }
        }
      } else {
        if (sample[2] <58) {
          if (sample[2] <54) {
            if (sample[4] <108) {
              sum[4] += 0.265822768;
            } else {
              sum[4] += -0.492012769;
            }
          } else {
            if (sample[0] <67) {
              sum[4] += 0.662316442;
            } else {
              sum[4] += 2.29090905;
            }
          }
        } else {
          if (sample[3] <165) {
            if (sample[2] <65) {
              sum[4] += 2.78734803;
            } else {
              sum[4] += 3.49299192;
            }
          } else {
            if (sample[4] <95) {
              sum[4] += 0.948817432;
            } else {
              sum[4] += 3.1836803;
            }
          }
        }
      }
    }
  }


  if (sample[4] <255) {
    if (sample[4] <201) {
      if (sample[4] <124) {
        if (sample[3] <14.999999) {
          if (sample[4] <108) {
            if (sample[4] <72) {
              sum[5] += -0.582745314;
            } else {
              sum[5] += -0.313566118;
            }
          } else {
            if (sample[5] <64) {
              sum[5] += -0.42685771;
            } else {
              sum[5] += 0.781245947;
            }
          }
        } else {
          sum[5] += -0.583258867;
        }
      } else {
        if (sample[3] <14.999999) {
          if (sample[1] <43) {
            if (sample[2] <65) {
              sum[5] += -0.47634533;
            } else {
              sum[5] += 0.589584827;
            }
          } else {
            if (sample[3] <10.999999) {
              sum[5] += 3.01449442;
            } else {
              sum[5] += 1.7316606;
            }
          }
        } else {
          if (sample[3] <19.9999981) {
            if (sample[1] <43) {
              sum[5] += -0.5035097;
            } else {
              sum[5] += -0.210034147;
            }
          } else {
            if (sample[3] <165) {
              sum[5] += -0.582685173;
            } else {
              sum[5] += -0.47432214;
            }
          }
        }
      }
    } else {
      if (sample[3] <14.999999) {
        if (sample[2] <81) {
          if (sample[5] <71) {
            if (sample[2] <58) {
              sum[5] += -0.551257789;
            } else {
              sum[5] += -0.119841672;
            }
          } else {
            if (sample[1] <35) {
              sum[5] += 1.6996274;
            } else {
              sum[5] += 0.432766587;
            }
          }
        } else {
          if (sample[2] <101) {
            if (sample[4] <232) {
              sum[5] += 1.71760786;
            } else {
              sum[5] += 2.81335378;
            }
          } else {
            if (sample[2] <119) {
              sum[5] += 3.13498855;
            } else {
              sum[5] += 3.42642903;
            }
          }
        }
      } else {
        if (sample[3] <16.9999981) {
          if (sample[2] <120) {
            if (sample[2] <78) {
              sum[5] += -0.427453429;
            } else {
              sum[5] += 0.0384844467;
            }
          } else {
            if (sample[1] <74) {
              sum[5] += 2.58629656;
            } else {
              sum[5] += 0.101326875;
            }
          }
        } else {
          if (sample[3] <19.9999981) {
            if (sample[5] <90) {
              sum[5] += -0.547287941;
            } else {
              sum[5] += -0.321253181;
            }
          } else {
            if (sample[3] <24.9999981) {
              sum[5] += -0.568026721;
            } else {
              sum[5] += -0.583026588;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <74) {
      if (sample[2] <67) {
        if (sample[2] <63) {
          if (sample[2] <58) {
            if (sample[2] <48) {
              sum[5] += -0.582431972;
            } else {
              sum[5] += -0.521429658;
            }
          } else {
            if (sample[3] <14.999999) {
              sum[5] += 0.367445171;
            } else {
              sum[5] += -0.519831002;
            }
          }
        } else {
          if (sample[1] <31.9999981) {
            if (sample[3] <10.999999) {
              sum[5] += 2.14960623;
            } else {
              sum[5] += 0.829150736;
            }
          } else {
            if (sample[1] <35) {
              sum[5] += -0.275873125;
            } else {
              sum[5] += -0.546122074;
            }
          }
        }
      } else {
        if (sample[1] <31.9999981) {
          if (sample[5] <68) {
            sum[5] += 1.47560167;
          } else {
            if (sample[1] <26.9999981) {
              sum[5] += 2.97080445;
            } else {
              sum[5] += 2.4632833;
            }
          }
        } else {
          if (sample[3] <14.999999) {
            if (sample[2] <72) {
              sum[5] += 0.190314338;
            } else {
              sum[5] += 0.674197316;
            }
          } else {
            if (sample[1] <35) {
              sum[5] += -0.124494754;
            } else {
              sum[5] += -0.52788496;
            }
          }
        }
      }
    } else {
      if (sample[3] <14.999999) {
        if (sample[2] <87) {
          if (sample[1] <35) {
            if (sample[1] <31.9999981) {
              sum[5] += 3.28206253;
            } else {
              sum[5] += 2.73249125;
            }
          } else {
            if (sample[2] <81) {
              sum[5] += 0.51155287;
            } else {
              sum[5] += 1.92607796;
            }
          }
        } else {
          if (sample[3] <10.999999) {
            sum[5] += 3.49776292;
          } else {
            if (sample[2] <94) {
              sum[5] += 3.11193871;
            } else {
              sum[5] += 3.38145661;
            }
          }
        }
      } else {
        if (sample[3] <16.9999981) {
          if (sample[2] <87) {
            if (sample[2] <78) {
              sum[5] += -0.39133653;
            } else {
              sum[5] += -0.0975202098;
            }
          } else {
            if (sample[1] <46) {
              sum[5] += 1.43001246;
            } else {
              sum[5] += 0.0126765491;
            }
          }
        } else {
          if (sample[2] <81) {
            sum[5] += -0.570032537;
          } else {
            if (sample[1] <61) {
              sum[5] += -0.397223055;
            } else {
              sum[5] += -0.573846638;
            }
          }
        }
      }
    }
  }


  if (sample[4] <124) {
    if (sample[4] <108) {
      if (sample[4] <95) {
        sum[6] += -0.583244801;
      } else {
        if (sample[3] <38) {
          if (sample[1] <95) {
            if (sample[1] <76) {
              sum[6] += -0.581204176;
            } else {
              sum[6] += -0.520576894;
            }
          } else {
            if (sample[3] <28.9999981) {
              sum[6] += -0.416980475;
            } else {
              sum[6] += -0.0839962363;
            }
          }
        } else {
          if (sample[3] <41) {
            if (sample[1] <128) {
              sum[6] += -0.581268668;
            } else {
              sum[6] += 0.624874592;
            }
          } else {
            sum[6] += -0.582699418;
          }
        }
      }
    } else {
      if (sample[2] <174) {
        if (sample[3] <33) {
          if (sample[1] <104) {
            if (sample[1] <85) {
              sum[6] += -0.572090685;
            } else {
              sum[6] += -0.194003835;
            }
          } else {
            if (sample[0] <69) {
              sum[6] += 0.948625326;
            } else {
              sum[6] += 0.199559495;
            }
          }
        } else {
          if (sample[3] <41) {
            if (sample[5] <94) {
              sum[6] += -0.581165671;
            } else {
              sum[6] += -0.0242479648;
            }
          } else {
            sum[6] += -0.583088279;
          }
        }
      } else {
        if (sample[3] <24.9999981) {
          if (sample[3] <19.9999981) {
            sum[6] += -0.513447404;
          } else {
            if (sample[1] <173) {
              sum[6] += 1.02831852;
            } else {
              sum[6] += -0.475471705;
            }
          }
        } else {
          if (sample[1] <173) {
            if (sample[0] <100) {
              sum[6] += -0.0830039605;
            } else {
              sum[6] += -0.32110092;
            }
          } else {
            if (sample[3] <33) {
              sum[6] += 2.75046062;
            } else {
              sum[6] += 0.200573042;
            }
          }
        }
      }
    }
  } else {
    if (sample[2] <203) {
      if (sample[1] <88) {
        if (sample[1] <78) {
          if (sample[1] <70) {
            if (sample[1] <58) {
              sum[6] += -0.583092809;
            } else {
              sum[6] += -0.56698209;
            }
          } else {
            if (sample[3] <38) {
              sum[6] += -0.353575051;
            } else {
              sum[6] += -0.581590712;
            }
          }
        } else {
          if (sample[3] <38) {
            if (sample[3] <24.9999981) {
              sum[6] += 0.0433002636;
            } else {
              sum[6] += 2.05079627;
            }
          } else {
            sum[6] += -0.579933643;
          }
        }
      } else {
        if (sample[3] <33) {
          if (sample[3] <19.9999981) {
            if (sample[0] <28.9999981) {
              sum[6] += 2.9566288;
            } else {
              sum[6] += 0.0779912621;
            }
          } else {
            if (sample[4] <156) {
              sum[6] += 3.07844925;
            } else {
              sum[6] += 3.47887301;
            }
          }
        } else {
          if (sample[3] <38) {
            if (sample[2] <163) {
              sum[6] += 0.450614214;
            } else {
              sum[6] += 2.98260856;
            }
          } else {
            sum[6] += -0.582063615;
          }
        }
      }
    } else {
      if (sample[3] <19.9999981) {
        sum[6] += -0.571368754;
      } else {
        sum[6] += 3.49790883;
      }
    }
  }


  return sum;
}
}
