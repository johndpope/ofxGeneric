#include "ofColor.h"
#include "ofConstants.h"

template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gray(limit() / 2, limit() / 2, limit() / 2);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::white(limit(), limit(), limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::red(limit(), 0, 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::green(0, limit(), 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blue(0, 0, limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cyan(0, limit(), limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::magenta(limit(), 0, limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellow(limit(), limit(), 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::black(0, 0, 0);
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aliceBlue((float)0.941176*limit(),(float)0.972549*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::antiqueWhite(0.980392f*limit(),0.921569f*limit(),0.843137f*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aqua(0*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::aquamarine(0.498039f*limit(),1*limit(),0.831373f*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::azure((float)0.941176*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::beige((float)0.960784*limit(),(float)0.960784*limit(),(float)0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::bisque(1*limit(),(float)0.894118*limit(),(float)0.768627*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blanchedAlmond(1*limit(),(float)0.921569*limit(),(float)0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::blueViolet((float)0.541176*limit(),(float)0.168627*limit(),(float)0.886275*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::brown((float)0.647059*limit(),(float)0.164706*limit(),(float)0.164706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::burlyWood((float)0.870588*limit(),(float)0.721569*limit(),(float)0.529412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cadetBlue((float)0.372549*limit(),(float)0.619608*limit(),(float)0.627451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chartreuse((float)0.498039*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::chocolate((float)0.823529*limit(),(float)0.411765*limit(),(float)0.117647*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::coral(1*limit(),(float)0.498039*limit(),(float)0.313726*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornflowerBlue((float)0.392157*limit(),(float)0.584314*limit(),(float)0.929412*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::cornsilk(1*limit(),(float)0.972549*limit(),(float)0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::crimson((float)0.862745*limit(),(float)0.0784314*limit(),(float)0.235294*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkBlue(0*limit(),0*limit(),(float)0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkCyan(0*limit(),(float)0.545098*limit(),(float)0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGoldenRod((float)0.721569*limit(),(float)0.52549*limit(),(float)0.0431373*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGray((float)0.662745*limit(),(float)0.662745*limit(),(float)0.662745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGrey((float)0.662745*limit(),(float)0.662745*limit(),(float)0.662745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkGreen(0*limit(),(float)0.392157*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkKhaki((float)0.741176*limit(),(float)0.717647*limit(),(float)0.419608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkMagenta((float)0.545098*limit(),0*limit(),(float)0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkOliveGreen((float)0.333333*limit(),(float)0.419608*limit(),(float)0.184314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkorange(1*limit(),(float)0.54902*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkOrchid((float)0.6*limit(),(float)0.196078*limit(),(float)0.8*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkRed((float)0.545098*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSalmon((float)0.913725*limit(),(float)0.588235*limit(),(float)0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSeaGreen((float)0.560784*limit(),(float)0.737255*limit(),(float)0.560784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSlateBlue((float)0.282353*limit(),(float)0.239216*limit(),(float)0.545098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSlateGray((float)0.184314*limit(),(float)0.309804*limit(),(float)0.309804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkSlateGrey((float)0.184314*limit(),(float)0.309804*limit(),(float)0.309804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkTurquoise(0*limit(),(float)0.807843*limit(),(float)0.819608*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::darkViolet((float)0.580392*limit(),0*limit(),(float)0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepPink(1*limit(),(float)0.0784314*limit(),(float)0.576471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::deepSkyBlue(0*limit(),(float)0.74902*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dimGray((float)0.411765*limit(),(float)0.411765*limit(),(float)0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dimGrey((float)0.411765*limit(),(float)0.411765*limit(),(float)0.411765*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::dodgerBlue((float)0.117647*limit(),(float)0.564706*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::fireBrick((float)0.698039*limit(),(float)0.133333*limit(),(float)0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::floralWhite(1*limit(),(float)0.980392*limit(),(float)0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::forestGreen((float)0.133333*limit(),(float)0.545098*limit(),(float)0.133333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::fuchsia(1*limit(),0*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gainsboro((float)0.862745*limit(),(float)0.862745*limit(),(float)0.862745*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ghostWhite((float)0.972549*limit(),(float)0.972549*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::gold(1*limit(),(float)0.843137*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::goldenRod((float)0.854902*limit(),(float)0.647059*limit(),(float)0.12549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::grey((float)0.501961*limit(),(float)0.501961*limit(),(float)0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::greenYellow((float)0.678431*limit(),1*limit(),(float)0.184314*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::honeyDew((float)0.941176*limit(),1*limit(),(float)0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::hotPink(1*limit(),(float)0.411765*limit(),(float)0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indianRed ((float)0.803922*limit(),(float)0.360784*limit(),(float)0.360784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::indigo ((float)0.294118*limit(),0*limit(),(float)0.509804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::ivory(1*limit(),1*limit(),(float)0.941176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::khaki((float)0.941176*limit(),(float)0.901961*limit(),(float)0.54902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavender((float)0.901961*limit(),(float)0.901961*limit(),(float)0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lavenderBlush(1*limit(),(float)0.941176*limit(),(float)0.960784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lawnGreen((float)0.486275*limit(),(float)0.988235*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lemonChiffon(1*limit(),(float)0.980392*limit(),(float)0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightBlue((float)0.678431*limit(),(float)0.847059*limit(),(float)0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightCoral((float)0.941176*limit(),(float)0.501961*limit(),(float)0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightCyan((float)0.878431*limit(),1*limit(),1*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGoldenRodYellow((float)0.980392*limit(),(float)0.980392*limit(),(float)0.823529*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGray((float)0.827451*limit(),(float)0.827451*limit(),(float)0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGrey((float)0.827451*limit(),(float)0.827451*limit(),(float)0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightGreen((float)0.564706*limit(),(float)0.933333*limit(),(float)0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightPink(1*limit(),(float)0.713726*limit(),(float)0.756863*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSalmon(1*limit(),(float)0.627451*limit(),(float)0.478431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSeaGreen((float)0.12549*limit(),(float)0.698039*limit(),(float)0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSkyBlue((float)0.529412*limit(),(float)0.807843*limit(),(float)0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSlateGray((float)0.466667*limit(),(float)0.533333*limit(),(float)0.6*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSlateGrey((float)0.466667*limit(),(float)0.533333*limit(),(float)0.6*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightSteelBlue((float)0.690196*limit(),(float)0.768627*limit(),(float)0.870588*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lightYellow(1*limit(),1*limit(),(float)0.878431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::lime(0*limit(),1*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::limeGreen((float)0.196078*limit(),(float)0.803922*limit(),(float)0.196078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::linen((float)0.980392*limit(),(float)0.941176*limit(),(float)0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::maroon((float)0.501961*limit(),0*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumAquaMarine((float)0.4*limit(),(float)0.803922*limit(),(float)0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumBlue(0*limit(),0*limit(),(float)0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumOrchid((float)0.729412*limit(),(float)0.333333*limit(),(float)0.827451*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumPurple((float)0.576471*limit(),(float)0.439216*limit(),(float)0.858824*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumSeaGreen((float)0.235294*limit(),(float)0.701961*limit(),(float)0.443137*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumSlateBlue((float)0.482353*limit(),(float)0.407843*limit(),(float)0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumSpringGreen(0*limit(),(float)0.980392*limit(),(float)0.603922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumTurquoise((float)0.282353*limit(),(float)0.819608*limit(),(float)0.8*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mediumVioletRed((float)0.780392*limit(),(float)0.0823529*limit(),(float)0.521569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::midnightBlue((float)0.0980392*limit(),(float)0.0980392*limit(),(float)0.439216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mintCream((float)0.960784*limit(),1*limit(),(float)0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::mistyRose(1*limit(),(float)0.894118*limit(),(float)0.882353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::moccasin(1*limit(),(float)0.894118*limit(),(float)0.709804*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navajoWhite(1*limit(),(float)0.870588*limit(),(float)0.678431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::navy(0*limit(),0*limit(),(float)0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::oldLace((float)0.992157*limit(),(float)0.960784*limit(),(float)0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::olive((float)0.501961*limit(),(float)0.501961*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::oliveDrab((float)0.419608*limit(),(float)0.556863*limit(),(float)0.137255*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orange(1*limit(),(float)0.647059*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orangeRed(1*limit(),(float)0.270588*limit(),0*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::orchid((float)0.854902*limit(),(float)0.439216*limit(),(float)0.839216*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleGoldenRod((float)0.933333*limit(),(float)0.909804*limit(),(float)0.666667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleGreen((float)0.596078*limit(),(float)0.984314*limit(),(float)0.596078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleTurquoise((float)0.686275*limit(),(float)0.933333*limit(),(float)0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::paleVioletRed((float)0.858824*limit(),(float)0.439216*limit(),(float)0.576471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::papayaWhip(1*limit(),(float)0.937255*limit(),(float)0.835294*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peachPuff(1*limit(),(float)0.854902*limit(),(float)0.72549*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::peru((float)0.803922*limit(),(float)0.521569*limit(),(float)0.247059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::pink(1*limit(),(float)0.752941*limit(),(float)0.796078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::plum((float)0.866667*limit(),(float)0.627451*limit(),(float)0.866667*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::powderBlue((float)0.690196*limit(),(float)0.878431*limit(),(float)0.901961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::purple((float)0.501961*limit(),0*limit(),(float)0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::rosyBrown((float)0.737255*limit(),(float)0.560784*limit(),(float)0.560784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::royalBlue((float)0.254902*limit(),(float)0.411765*limit(),(float)0.882353*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::saddleBrown((float)0.545098*limit(),(float)0.270588*limit(),(float)0.0745098*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::salmon((float)0.980392*limit(),(float)0.501961*limit(),(float)0.447059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sandyBrown((float)0.956863*limit(),(float)0.643137*limit(),(float)0.376471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seaGreen((float)0.180392*limit(),(float)0.545098*limit(),(float)0.341176*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::seaShell(1*limit(),(float)0.960784*limit(),(float)0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::sienna((float)0.627451*limit(),(float)0.321569*limit(),(float)0.176471*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::silver((float)0.752941*limit(),(float)0.752941*limit(),(float)0.752941*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::skyBlue((float)0.529412*limit(),(float)0.807843*limit(),(float)0.921569*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateBlue((float)0.415686*limit(),(float)0.352941*limit(),(float)0.803922*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateGray((float)0.439216*limit(),(float)0.501961*limit(),(float)0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::slateGrey((float)0.439216*limit(),(float)0.501961*limit(),(float)0.564706*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::snow(1*limit(),(float)0.980392*limit(),(float)0.980392*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::springGreen(0*limit(),1*limit(),(float)0.498039*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::steelBlue((float)0.27451*limit(),(float)0.509804*limit(),(float)0.705882*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tan((float)0.823529*limit(),(float)0.705882*limit(),(float)0.54902*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::teal(0*limit(),(float)0.501961*limit(),(float)0.501961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::thistle((float)0.847059*limit(),(float)0.74902*limit(),(float)0.847059*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::tomato(1*limit(),(float)0.388235*limit(),(float)0.278431*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::turquoise((float)0.25098*limit(),(float)0.878431*limit(),(float)0.815686*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::violet((float)0.933333*limit(),(float)0.509804*limit(),(float)0.933333*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::wheat((float)0.960784*limit(),(float)0.870588*limit(),(float)0.701961*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::whiteSmoke((float)0.960784*limit(),(float)0.960784*limit(),(float)0.960784*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::yellowGreen((float)0.603922*limit(),(float)0.803922*limit(),(float)0.196078*limit());
template<typename PixelType> const ofColor_<PixelType> ofColor_<PixelType>::clearColor( limit(), limit(), limit(), 0);



template<typename PixelType>
float ofColor_<PixelType>::limit() {
	return (float)numeric_limits<PixelType>::max();
}

template<>
float ofColor_<float>::limit() {
	return 1.f;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (){
	r = (char) limit();
	g = (char) limit();
	b = (char) limit();
	a = (char) limit();
}

template<typename PixelType>
ofColor_<PixelType>::~ofColor_ (){}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float _r, float _g, float _b, float _a){
	r = (char)_r;
	g = (char)_g;
	b = (char)_b;
	a = (char)_a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color, float _a){
	r = color.r;
	g = color.g;
	b = color.b;
	a = (char)_a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float grayValue, float _a){
	r = g = b = (char)grayValue;
	a = (char)_a;
}

template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHsb (float hue, float saturation, float brightness, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHsb(hue, saturation, brightness, alpha);
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHex(int hexColor, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHex(hexColor, alpha);
	return cur;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _r, float _g, float _b, float _a){
	r = (char)_r;
	g = (char)_g;
	b = (char)_b;
	a = (char)_a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _gray, float _a){
	r = g = b = (char)_gray;
	a = (char)_a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (ofColor_<PixelType> const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}


template<typename PixelType>
void ofColor_<PixelType>::setHex (int hexColor, float alpha){
	r = (hexColor >> 16) & 0xff;
	g = (hexColor >> 8) & 0xff;
	b = (hexColor >> 0) & 0xff;
	a = (char)alpha;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::clamp (){
	r = (char)CLAMP(r, 0.0f, limit());
	g = (char)CLAMP(g, 0.0f, limit());
	b = (char)CLAMP(b, 0.0f, limit());
	a = (char)CLAMP(a, 0.0f, limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::invert() {
	r = (char)limit() - r;
	g = (char)limit() - g;
	b = (char)limit() - b;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::normalize() {
	float brightness = getBrightness(); 
    // avoid division by 0
    if ( brightness > 0 ) 
    {
        r = (char)(limit() * (r / brightness));
        g = (char)(limit() * (g / brightness));
        b = (char)(limit() * (b / brightness));
    }
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::lerp(const ofColor_<PixelType>& target, float amount) {
	float invAmount = 1.f - amount;
	r = (char)(invAmount * r + amount * target.r);
	g = (char)(invAmount * g + amount * target.g);
	b = (char)(invAmount * b + amount * target.b);
	a = (char)(invAmount * a + amount * target.a);
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getClamped () const {
	ofColor_<PixelType> cur(*this);
	cur.clamp();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getInverted () const {
	ofColor_<PixelType> cur(*this);
	cur.invert();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getNormalized () const {
	ofColor_<PixelType> cur(*this);
	cur.normalize();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getLerped(const ofColor_<PixelType>& target, float amount) const {
	ofColor_<PixelType> cur(*this);
	cur.lerp(target, amount);
	return cur;
}



template<typename PixelType>
float ofColor_<PixelType>::getHue() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return hue;
}


template<typename PixelType>
float ofColor_<PixelType>::getSaturation() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return saturation;
}

/*
	Brightness is simply the maximum of the three color components.
	This is used by Photoshop (HSB) and Processing (HSB).
	Brightness is also called "value".
*/

template<typename PixelType>
float ofColor_<PixelType>::getBrightness() const {
	float max = (float)r;
	if(g > max) {
		max = (float)g;
	}
	if(b > max) {
		max = (float)b;
	}
	return max;
}

/*
	Lightness is the average of the three color components.
	This is used by the Lab and HSL color spaces.
*/

template<typename PixelType>
float ofColor_<PixelType>::getLightness() const {
	return (float)(r + g + b) / 3.f;
}



template<typename PixelType>
void ofColor_<PixelType>::getHsb(float& hue, float& saturation, float& brightness) const {
	float max = getBrightness();
	
	float min = (float)r;
	if(g < min) {
		min = (float)g;
	}
	if(b < min) {
		min = (float)b;
	}
	
	if(max == min) { // grays
		hue = 0.f;
		saturation = 0.f;
		brightness = max;
		return;
	}
	
	float hueSixth;
	if(r == max) {
		hueSixth = (float)(g - b) / (max - min);
		if(hueSixth < 0.f)
			hueSixth += 6.f;
	} else if (g == max) {
		hueSixth = (float)(2.f + (b - r) / (max - min));
	} else {
		hueSixth = (float)(4.f + (r - g) / (max - min));
	}
	hue = limit() * hueSixth / 6.f;
	saturation = limit() * (max - min) / max;
	brightness = max;
}


template<typename PixelType>
void ofColor_<PixelType>::setHue (float hue) {
	float oldHue, saturation, brightness;
	getHsb(oldHue, saturation, brightness);
	setHsb(hue, saturation, brightness, (float)a );
}


template<typename PixelType>
void ofColor_<PixelType>::setSaturation (float saturation) {
	float hue, oldSaturation, brightness;
	getHsb(hue, oldSaturation, brightness);
	setHsb(hue, saturation, brightness, (float)a );
}


template<typename PixelType>
void ofColor_<PixelType>::setBrightness (float brightness) {
	float hue, saturation, oldBrightness;
	getHsb(hue, saturation, oldBrightness);
	setHsb(hue, saturation, brightness, (float)a );
}


template<typename PixelType>
void ofColor_<PixelType>::setHsb(float hue, float saturation, float brightness, float alpha ) {
	saturation = ofClamp(saturation, 0, limit());
	brightness = ofClamp(brightness, 0, limit());
	if(brightness == 0) { // black
		set(0);
	} else if(saturation == 0) { // grays
		set(brightness);
	} else {
		float hueSix = (float)(hue * 6. / limit());
		float saturationNorm = saturation / limit();
		int hueSixCategory = (int) floorf(hueSix);
		float hueSixRemainder = hueSix - hueSixCategory;
		PixelType pv = (PixelType) ((1.f - saturationNorm) * brightness);
		PixelType qv = (PixelType) ((1.f - saturationNorm * hueSixRemainder) * brightness);
		PixelType tv = (PixelType) ((1.f - saturationNorm * (1.f - hueSixRemainder)) * brightness);
		switch(hueSixCategory) {
			case 0: // r
				r = (char)brightness;
				g = tv;
				b = pv;
				break;
			case 1: // g
				r = qv;
				g = (char)brightness;
				b = pv;
				break;
			case 2:
				r = pv;
				g = (char)brightness;
				b = tv;
				break;
			case 3: // b
				r = pv;
				g = qv;
				b = (char)brightness;
				break;
			case 4:
				r = tv;
				g = pv;
				b = (char)brightness;
				break;
			case 5: // back to r
				r = (char)brightness;
				g = pv;
				b = qv;
				break;
		}
	}
    
    // finally assign the alpha
    a = (char)alpha;
}


template<typename PixelType>
int ofColor_<PixelType>::getHex () const {
	return
		((0xff & (unsigned char) r) << 16) |
		((0xff & (unsigned char) g) << 8) |
		((0xff & (unsigned char) b));
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (ofColor_<PixelType> const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (float const & val){
	r = (char)val;
	g = (char)val;
	b = (char)val;
	a = (char)limit();
	return *this;
}


template<typename PixelType>
bool ofColor_<PixelType>::operator == (ofColor_<PixelType> const & color){
	return (r == color.r) && (g == color.g) && (b == color.b);
}


template<typename PixelType>
bool ofColor_<PixelType>::operator != (ofColor_<PixelType> const & color){
	return (r != color.r) || (g != color.g) || (b != color.b);
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( (float)(r+color.r), (float)(g+color.g), (float)(b+color.b) ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (float const & val) const{
	return ofColor_<PixelType>( (float)(r+val), (float)(g+val), (float)(b+val) ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (ofColor_<PixelType> const & color){
	r += color.r;
	g += color.g;
	b += color.b;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (float const & val){
	r += val;
	g += val;
	b += val;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( (float)(r-color.r), (float)(g-color.g), (float)(b-color.b) ).clamp();
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (float const & val) const{
	return ofColor_<PixelType>( (float)(r-val), (float)(g-val), (float)(b-val)).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (ofColor_<PixelType> const & color){
	r -= color.r;
	g -= color.g;
	b -= color.b;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (float const & val){
	r -= val;
	g -= val;
	b -= val;
	this->clamp();
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( (float)(r*(color.r/limit())), (float)(g*(color.g/limit())), (float)(b*(color.b/limit())));
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (float const & val) const{
	float v = CLAMP(val,0.0,1.0);
	return ofColor_<PixelType>( (float)r*v, (float)g*v, (float)b*v).clamp();
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (ofColor_<PixelType> const & color){
	r *= (color.r/limit());
	g *= (color.g/limit());
	b *= (color.b/limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (float const & val){
	float v = CLAMP(val,0.0,1.0);
	r *= v;
	g *= v;
	b *= v;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>(color.r!=0.0f ? (float)(r/(color.r/limit())) : (float)r ,
                               color.g!=0.0f ? (float)(g/(color.g/limit())) : (float)g,
                               color.b!=0.0f ? (float)(b/(color.b/limit())) : (float)b );
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (float const & val) const{
	if( val != 0.0f)
		return ofColor_<PixelType>( (float)r/val, (float)g/val, (float)b/val ).clamp();
	else
		return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (ofColor_<PixelType> const & color){
	if (color.r!=0.0f) r /= (color.r/limit());
	if (color.g!=0.0f) g /= (color.g/limit());
	if (color.b!=0.0f) b /= (color.b/limit());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (float const & val){
	if( val != 0.0f ){
		r /= val;
		g /= val;
		b /= val;
		clamp();
		return *this;
	}
	else
		return *this;
}


template<typename PixelType>
const PixelType & ofColor_<PixelType>::operator [] (int n) const{
	switch( n ){
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3: 
			return a;
		default:
			return r;
			break;
	}
}

template<typename PixelType>
PixelType & ofColor_<PixelType>::operator [] (int n){
	switch( n ){
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		default:
			return r;
			break;
	}
}

template class ofColor_<char>;
template class ofColor_<unsigned char>;
template class ofColor_<short>;
template class ofColor_<unsigned short>;
template class ofColor_<int>;
template class ofColor_<unsigned int>;
template class ofColor_<long>;
template class ofColor_<unsigned long>;
template class ofColor_<float>;
template class ofColor_<double>;