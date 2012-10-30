#include "NumericalRecipes.hpp"

#include "QNMs.hpp"

#include "Interpolate.hpp"
#include "Utilities.hpp"
using WaveformUtilities::Matrix;
using WaveformUtilities::Interpolate;
using std::vector;
using std::cerr;
using std::endl;

//// We have the following (l,m) modes at present:
////   [0] = 2,2
////   [1] = 2,1
////   [2] = 3,3
////   [3] = 3,2
////   [4] = 4,4
//// We have the 8 overtones 0 through 7

Matrix<int> WaveformUtilities::QNMLMs() {
  Matrix<int> LMs(10, 2);
  LMs[0][0] = 2; LMs[0][1] = -2;
  LMs[1][0] = 2; LMs[1][1] = -1;
  LMs[2][0] = 2; LMs[2][1] =  1;
  LMs[3][0] = 2; LMs[3][1] =  2;
  LMs[4][0] = 3; LMs[4][1] = -3;
  LMs[5][0] = 3; LMs[5][1] = -2;
  LMs[6][0] = 3; LMs[6][1] =  2;
  LMs[7][0] = 3; LMs[7][1] =  3;
  LMs[8][0] = 4; LMs[8][1] = -4;
  LMs[9][0] = 4; LMs[9][1] =  4;
  return LMs;
}

void Initialize();
bool Initialized=false;
vector<double> EvaluatedSpins(21);
vector<vector<vector<double> > > omegasRe(5, vector<vector<double> >(8, vector<double>(21, 0.0)));
vector<vector<vector<double> > > omegasIm(5, vector<vector<double> >(8, vector<double>(21, 0.0)));

void WaveformUtilities::QNM(const int L, const int M, const int N, const double chi, double& omegaRe, double& omegaIm) {
  if(N>7) {
    std::cerr << "N=" << N << " has not been included in the data." << std::endl;
    throw("Bad overtone N request.");
  }
  
  if(!Initialized) Initialize();
  
  //// Find the right pair of vectors
  unsigned int i=0;
  if(L==2 && abs(M)==2) {
    i=0;
  } else if(L==2 && abs(M)==1) {
    i=1;
  } else if(L==3 && abs(M)==3) {
    i=2;
  } else if(L==3 && abs(M)==2) {
    i=3;
  } else if(L==4 && abs(M)==4) {
    i=4;
  } else {
    cerr << "(L,M)=(" << L << "," << M << ") has not been included in the data." << endl;
    throw("Bad (L,M) request");
  }
  
  //// Interpolate each to the desired spin
  omegaRe = Interpolate(EvaluatedSpins, omegasRe[i][N], chi);
  omegaIm = Interpolate(EvaluatedSpins, omegasIm[i][N], chi);
  if(M>0) omegaRe *= -1.0; // ensures negative frequencies for positive M; spin is s=-2
  
  return;
}



void Initialize() {
  //// Set the values of EvaluatedSpins, omegasRe, and omegasIm
  const double EvaluatedSpinsArray[] = {0., 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 0.998};
  EvaluatedSpins.assign(EvaluatedSpinsArray, EvaluatedSpinsArray+21);
  
  const double omegasReArray[5][8][21] = {
    {
      {0.37367168, 0.38015134875, 0.38701754, 0.39432921125, 0.40214532, 0.41051178374999997, 0.41952668, 0.429253065625, 0.43984192, 0.373671680, 0.373671681,
       0.373671682, 0.373671683, 0.373671684, 0.373671685, 0.373671686, 0.373671687, 0.373671688, 0.373671689, 0.380151348750, 0.380151348751},
      {0.346711, 0.354106795625, 0.36190968, 0.370179564375, 0.37897636, 0.38834649062500004, 0.39839033, 0.40917185562499997, 0.42084668, 0.3467110, 0.3467111,
       0.3467112, 0.3467113, 0.3467114, 0.3467115, 0.3467116, 0.3467117, 0.3467118, 0.3467119, 0.3541067956250, 0.3541067956251},
      {0.30105345, 0.309879426875, 0.31915347, 0.328935425625, 0.33928514, 0.35024866312499997, 0.36192723, 0.37438425875, 0.38777925, 0.301053450, 0.301053451,
       0.301053452, 0.301053453, 0.301053454, 0.301053455, 0.301053456, 0.301053457, 0.301053458, 0.301053459, 0.3098794268750, 0.3098794268751},
      {0.25150496, 0.26135354125, 0.27174872, 0.28274503125, 0.29439701, 0.30674646875, 0.31988611, 0.33387323875, 0.34885586, 0.251504960, 0.251504961, 0.251504962,
       0.251504963, 0.251504964, 0.251504965, 0.251504966, 0.251504967, 0.251504968, 0.251504969, 0.261353541250, 0.261353541251},
      {0.20751458, 0.217598063125, 0.22843281, 0.240057424375, 0.25251051, 0.265820268125, 0.28005651, 0.29526072187499997, 0.31154608, 0.207514580, 0.207514581,
       0.207514582, 0.207514583, 0.207514584, 0.207514585, 0.207514586, 0.207514587, 0.207514588, 0.207514589, 0.2175980631250, 0.2175980631251},
      {0.1692994, 0.17908263562500001, 0.1901402, 0.202439266875, 0.21594701, 0.230612929375, 0.24645722, 0.26345189, 0.281691, 0.16929940, 0.16929941, 0.16929942,
       0.16929943, 0.16929944, 0.16929945, 0.16929946, 0.16929947, 0.16929948, 0.16929949, 0.179082635625000010, 0.179082635625000011},
      {0.13325234, 0.14260010187500002, 0.15484078, 0.169466973125, 0.18597128, 0.203661975, 0.22258463, 0.24249119874999997, 0.26357081, 0.133252340, 0.133252341,
       0.133252342, 0.133252343, 0.133252344, 0.133252345, 0.133252346, 0.133252347, 0.133252348, 0.133252349, 0.142600101875000020, 0.142600101875000021},
      {0.092822398, 0.105018380625, 0.12246772, 0.143418131375, 0.16611733, 0.187832298125, 0.20975295, 0.23204610062499997, 0.25504803, 0.0928223980, 0.0928223981,
       0.0928223982, 0.0928223983, 0.0928223984, 0.0928223985, 0.0928223986, 0.0928223987, 0.0928223988, 0.0928223989, 0.1050183806250, 0.1050183806251}
    },
    {
      {0.3736716844, 0.3769308456, 0.380432255, 0.3841965331, 0.3882478199, 0.3926145398, 0.3973303975, 0.4024356959, 0.4079791087, 0.37367168440, 0.37367168441,
       0.37367168442, 0.37367168443, 0.37367168444, 0.37367168445, 0.37367168446, 0.37367168447, 0.37367168448, 0.37367168449, 0.37693084560, 0.37693084561},
      {0.3467109969, 0.3504481596, 0.3544782503, 0.3588225667, 0.3635061125, 0.3685583784, 0.3740143534, 0.3799158525, 0.38631329, 0.34671099690, 0.34671099691,
       0.34671099692, 0.34671099693, 0.34671099694, 0.34671099695, 0.34671099696, 0.34671099697, 0.34671099698, 0.34671099699, 0.35044815960, 0.35044815961},
      {0.3010534545, 0.3055401353, 0.3104096213, 0.315683854, 0.3213886443, 0.3275544767, 0.3342175299, 0.3414209946, 0.3492168074, 0.30105345450, 0.30105345451,
       0.30105345452, 0.30105345453, 0.30105345454, 0.30105345455, 0.30105345456, 0.30105345457, 0.30105345458, 0.30105345459, 0.30554013530, 0.30554013531},
      {0.2515049622, 0.2565340714, 0.2620584381, 0.2681010611, 0.2746885407, 0.2818519097, 0.2896276416, 0.2980589075, 0.3071971794, 0.25150496220, 0.25150496221,
       0.25150496222, 0.25150496223, 0.25150496224, 0.25150496225, 0.25150496226, 0.25150496227, 0.25150496228, 0.25150496229, 0.25653407140, 0.25653407141},
      {0.2075145798, 0.2126793977, 0.2184795252, 0.2249388234, 0.2320829538, 0.2399402857, 0.2485429494, 0.2579280773, 0.2681392837, 0.20751457980, 0.20751457981,
       0.20751457982, 0.20751457983, 0.20751457984, 0.20751457985, 0.20751457986, 0.20751457987, 0.20751457988, 0.20751457989, 0.21267939770, 0.21267939771},
      {0.1692994034, 0.174305879, 0.1801918011, 0.1869766741, 0.1946714946, 0.2032816807, 0.2128105953, 0.2232631124, 0.2346488155, 0.16929940340, 0.16929940341,
       0.16929940342, 0.16929940343, 0.16929940344, 0.16929940345, 0.16929940346, 0.16929940347, 0.16929940348, 0.16929940349, 0.1743058790, 0.1743058791},
      {0.1332523443, 0.1378626476, 0.144021248, 0.1516719047, 0.1606836045, 0.1709018292, 0.1821862747, 0.1944287198, 0.2075564398, 0.13325234430, 0.13325234431,
       0.13325234432, 0.13325234433, 0.13325234434, 0.13325234435, 0.13325234436, 0.13325234437, 0.13325234438, 0.13325234439, 0.13786264760, 0.13786264761},
      {0.09282239791, 0.09676079056, 0.1052666743, 0.1167483672, 0.1298169054, 0.1437187119, 0.1581240633, 0.1729200898, 0.1880936731, 0.092822397910, 0.092822397911,
       0.092822397912, 0.092822397913, 0.092822397914, 0.092822397915, 0.092822397916, 0.092822397917, 0.092822397918, 0.092822397919, 0.096760790560, 0.096760790561}
    },
    {
      {0.5994432884, 0.6098233498, 0.6207956124, 0.6324248566, 0.6447869769, 0.657971685, 0.6720860938, 0.6872595489, 0.7036502752, 0.59944328840, 0.59944328841,
       0.59944328842, 0.59944328843, 0.59944328844, 0.59944328845, 0.59944328846, 0.59944328847, 0.59944328848, 0.59944328849, 0.60982334980, 0.60982334981},
      {0.582643803, 0.5936417862, 0.6052503876, 0.6175351118, 0.6305726013, 0.6444533398, 0.659285235, 0.6751984503, 0.6923520524, 0.5826438030, 0.5826438031,
       0.5826438032, 0.5826438033, 0.5826438034, 0.5826438035, 0.5826438036, 0.5826438037, 0.5826438038, 0.5826438039, 0.59364178620, 0.59364178621},
      {0.5516849008, 0.5638043493, 0.5765672483, 0.5900405608, 0.6043024574, 0.6194450235, 0.6355778444, 0.6528328401, 0.6713709165, 0.55168490080, 0.55168490081,
       0.55168490082, 0.55168490083, 0.55168490084, 0.55168490085, 0.55168490086, 0.55168490087, 0.55168490088, 0.55168490089, 0.56380434930, 0.56380434931},
      {0.5119619111, 0.5254016557, 0.5395338943, 0.5544276851, 0.5701634339, 0.5868356112, 0.6045563477, 0.6234602778, 0.6437111989, 0.51196191110, 0.51196191111,
       0.51196191112, 0.51196191113, 0.51196191114, 0.51196191115, 0.51196191116, 0.51196191117, 0.51196191118, 0.51196191119, 0.52540165570, 0.52540165571},
      {0.4701740058, 0.4847253606, 0.5000419614, 0.5161953769, 0.5332687341, 0.5513594627, 0.5705829142, 0.5910772269, 0.6130100012, 0.47017400580, 0.47017400581,
       0.47017400582, 0.47017400583, 0.47017400584, 0.47017400585, 0.47017400586, 0.47017400587, 0.47017400588, 0.47017400589, 0.48472536060, 0.48472536061},
      {0.4313864786, 0.446612333, 0.462696355, 0.4797132645, 0.4977495717, 0.5169063802, 0.5373030628, 0.5590821794, 0.5824161957, 0.43138647860, 0.43138647861,
       0.43138647862, 0.43138647863, 0.43138647864, 0.43138647865, 0.43138647866, 0.43138647867, 0.43138647868, 0.43138647869, 0.4466123330, 0.4466123331},
      {0.3976595242, 0.4131523107, 0.42960427, 0.447093921, 0.4657117304, 0.4855630349, 0.5067718402, 0.5294858582, 0.5538833351, 0.39765952420, 0.39765952421,
       0.39765952422, 0.39765952423, 0.39765952424, 0.39765952425, 0.39765952426, 0.39765952427, 0.39765952428, 0.39765952429, 0.41315231070, 0.41315231071},
      {0.3689922759, 0.384474772, 0.4010191846, 0.4187078844, 0.437635084, 0.4579099959, 0.4796608711, 0.5030402699, 0.5282321113, 0.36899227590, 0.36899227591,
       0.36899227592, 0.36899227593, 0.36899227594, 0.36899227595, 0.36899227596, 0.36899227597, 0.36899227598, 0.36899227599, 0.3844747720, 0.3844747721}
    },
    {
      {0.5994432884, 0.6063693467, 0.6137000191, 0.6214745373, 0.6297384965, 0.6385453043, 0.6479580761, 0.6580521531, 0.6689185113, 0.59944328840, 0.59944328841,
       0.59944328842, 0.59944328843, 0.59944328844, 0.59944328845, 0.59944328846, 0.59944328847, 0.59944328848, 0.59944328849, 0.60636934670, 0.60636934671},
      {0.582643803, 0.5899887457, 0.5977647152, 0.6060118669, 0.6147768062, 0.6241140453, 0.6340879005, 0.6447750078, 0.6562677178, 0.5826438030, 0.5826438031,
       0.5826438032, 0.5826438033, 0.5826438034, 0.5826438035, 0.5826438036, 0.5826438037, 0.5826438038, 0.5826438039, 0.58998874570, 0.58998874571},
      {0.5516849008, 0.5597903439, 0.5683747787, 0.5774799382, 0.5871541488, 0.5974538015, 0.6084452605, 0.6202073761, 0.6328348606, 0.55168490080, 0.55168490081,
       0.55168490082, 0.55168490083, 0.55168490084, 0.55168490085, 0.55168490086, 0.55168490087, 0.55168490088, 0.55168490089, 0.55979034390, 0.55979034391},
      {0.5119619111, 0.5209623383, 0.5305045159, 0.5406323222, 0.5513963638, 0.5628554792, 0.5750786705, 0.5881476271, 0.6021600892, 0.51196191110, 0.51196191111,
       0.51196191112, 0.51196191113, 0.51196191114, 0.51196191115, 0.51196191116, 0.51196191117, 0.51196191118, 0.51196191119, 0.52096233830, 0.52096233831},
      {0.4701740058, 0.4799262889, 0.4902912469, 0.501315614, 0.5130528893, 0.5255648961, 0.5389237582, 0.5532144552, 0.5685381976, 0.47017400580, 0.47017400581,
       0.47017400582, 0.47017400583, 0.47017400584, 0.47017400585, 0.47017400586, 0.47017400587, 0.47017400588, 0.47017400589, 0.47992628890, 0.47992628891},
      {0.4313864786, 0.4415924709, 0.4524822139, 0.4641058518, 0.4765201469, 0.4897901165, 0.5039910747, 0.5192112439, 0.5355551752, 0.43138647860, 0.43138647861,
       0.43138647862, 0.43138647863, 0.43138647864, 0.43138647865, 0.43138647866, 0.43138647867, 0.43138647868, 0.43138647869, 0.44159247090, 0.44159247091},
      {0.3976595242, 0.4080419318, 0.4191731868, 0.4311068074, 0.4439025002, 0.4576278862, 0.4723606228, 0.4881910925, 0.5052259062, 0.39765952420, 0.39765952421,
       0.39765952422, 0.39765952423, 0.39765952424, 0.39765952425, 0.39765952426, 0.39765952427, 0.39765952428, 0.39765952429, 0.40804193180, 0.40804193181},
      {0.3689922759, 0.3793624731, 0.3905398333, 0.4025806628, 0.4155466054, 0.4295064651, 0.4445384207, 0.4607328148, 0.4781957892, 0.36899227590, 0.36899227591,
       0.36899227592, 0.36899227593, 0.36899227594, 0.36899227595, 0.36899227596, 0.36899227597, 0.36899227598, 0.36899227599, 0.37936247310, 0.37936247311}
    },
    {
      {0.8091783775, 0.8235173968, 0.8386602441, 0.8546933996, 0.8717179971, 0.8898533556, 0.909241654, 0.9300542239, 0.9525001933, 0.80917837750, 0.80917837751,
       0.80917837752, 0.80917837753, 0.80917837754, 0.80917837755, 0.80917837756, 0.80917837757, 0.80917837758, 0.80917837759, 0.82351739680, 0.82351739681},
      {0.796631532, 0.8114344796, 0.8270552357, 0.8435809775, 0.8611135727, 0.8797731127, 0.8997025841, 0.9210741598, 0.9440978347, 0.7966315320, 0.7966315321,
       0.7966315322, 0.7966315323, 0.7966315324, 0.7966315325, 0.7966315326, 0.7966315327, 0.7966315328, 0.7966315329, 0.81143447960, 0.81143447961},
      {0.7727095326, 0.7883918669, 0.804917568, 0.8223752194, 0.8408681858, 0.8605181485, 0.8814697803, 0.9038970344, 0.9280117764, 0.77270953260, 0.77270953261,
       0.77270953262, 0.77270953263, 0.77270953264, 0.77270953265, 0.77270953266, 0.77270953267, 0.77270953268, 0.77270953269, 0.78839186690, 0.78839186691},
      {0.73983673, 0.7566895724, 0.7744212381, 0.7931224524, 0.8128988806, 0.8338746776, 0.8561971755, 0.8800431839, 0.9056276286, 0.739836730, 0.739836731, 0.739836732,
       0.739836733, 0.739836734, 0.739836735, 0.739836736, 0.739836737, 0.739836738, 0.739836739, 0.75668957240, 0.75668957241},
      {0.7015155093, 0.7196200191, 0.7386509975, 0.7587019582, 0.7798815979, 0.8023173746, 0.8261602268, 0.8515909059, 0.8788286478, 0.70151550930, 0.70151550931,
       0.70151550932, 0.70151550933, 0.70151550934, 0.70151550935, 0.70151550936, 0.70151550937, 0.70151550938, 0.70151550939, 0.71962001910, 0.71962001911},
      {0.6615724994, 0.6807767113, 0.7009708715, 0.7222517911, 0.7447317957, 0.7685423535, 0.7938388505, 0.820806986, 0.8496715121, 0.66157249940, 0.66157249941,
       0.66157249942, 0.66157249943, 0.66157249944, 0.66157249945, 0.66157249946, 0.66157249947, 0.66157249948, 0.66157249949, 0.68077671130, 0.68077671131},
      {0.6231088747, 0.643108728, 0.664176248, 0.6864121129, 0.7099329205, 0.7348748925, 0.7613987368, 0.7896961435, 0.8199986402, 0.62310887470, 0.62310887471,
       0.62310887472, 0.62310887473, 0.62310887474, 0.62310887475, 0.62310887476, 0.62310887477, 0.62310887478, 0.62310887479, 0.6431087280, 0.6431087281},
      {0.587908825, 0.6083713535, 0.6299863337, 0.6528589557, 0.6771107812, 0.7028835542, 0.7303441865, 0.7596914004, 0.7911647557, 0.5879088250, 0.5879088251,
       0.5879088252, 0.5879088253, 0.5879088254, 0.5879088255, 0.5879088256, 0.5879088257, 0.5879088258, 0.5879088259, 0.60837135350, 0.60837135351}
    }
  };
  
  for(unsigned int i=0; i<5; ++i) {
    for(unsigned int j=0; j<8; ++j) {
      omegasRe[i][j].assign(&omegasReArray[i][j][0], &omegasReArray[i][j][20]);
    }
  }
  
  
  const double omegasImArray[5][8][21] = {
    {
      {0.088962316, 0.0888481566875, 0.088705699, 0.0885287623125, 0.088311166, 0.0880485175625, 0.087729272, 0.0873469440625, 0.086881962, 0.0889623160, 0.0889623161,
       0.0889623162, 0.0889623163, 0.0889623164, 0.0889623165, 0.0889623166, 0.0889623167, 0.0889623168, 0.0889623169, 0.08884815668750, 0.08884815668751},
      {0.27391488, 0.27322978875000004, 0.27245211, 0.27156349625000004, 0.2705456, 0.269385123125, 0.26804857, 0.26651675187499996, 0.26473345, 0.273914880, 0.273914881,
       0.273914882, 0.273914883, 0.273914884, 0.273914885, 0.273914886, 0.273914887, 0.273914888, 0.273914889, 0.273229788750000040, 0.273229788750000041},
      {0.47827698, 0.475951886875, 0.47346308, 0.470780008125, 0.46787212, 0.46471648187500003, 0.46125969, 0.457469608125, 0.4532595, 0.478276980, 0.478276981, 
       0.478276982, 0.478276983, 0.478276984, 0.478276985, 0.478276986, 0.478276987, 0.478276988, 0.478276989, 0.4759518868750, 0.4759518868751},
      {0.7051482, 0.6999409675, 0.6944809, 0.68872453, 0.68262839, 0.676159554375, 0.66924293, 0.6618309050000001, 0.65380811, 0.70514820, 0.70514821, 0.70514822,
       0.70514823, 0.70514824, 0.70514825, 0.70514826, 0.70514827, 0.70514828, 0.70514829, 0.69994096750, 0.69994096751},
      {0.94684489, 0.9379775662500001, 0.92872504, 0.91903238375, 0.90884467, 0.8981261375, 0.88676436, 0.874692630625, 0.86173803, 0.946844890, 0.946844891, 0.946844892,
       0.946844893, 0.946844894, 0.946844895, 0.946844896, 0.946844897, 0.946844898, 0.946844899, 0.93797756625000010, 0.93797756625000011},
      {1.1956081, 1.182173875, 1.168248, 1.153794825, 1.1387787, 1.1232168687500002, 1.106915, 1.0897819937500002, 1.0715254, 1.19560810, 1.19560811, 1.19560812,
       1.19560813, 1.19560814, 1.19560815, 1.19560816, 1.19560817, 1.19560818, 1.19560819, 1.1821738750, 1.1821738751},
      {1.4479106, 1.42742146875, 1.4071149, 1.3869360562500002, 1.3668301, 1.3469025125, 1.3266175, 1.30562450625, 1.2834733, 1.44791060, 1.44791061, 1.44791062,
       1.44791063, 1.44791064, 1.44791065, 1.44791066, 1.44791067, 1.44791068, 1.44791069, 1.427421468750, 1.427421468751},
      {1.7038413, 1.6682394875, 1.6389345, 1.6141381375, 1.5920622, 1.5704994375, 1.5489188, 1.5261732187500001, 1.5019035, 1.70384130, 1.70384131, 1.70384132,
       1.70384133, 1.70384134, 1.70384135, 1.70384136, 1.70384137, 1.70384138, 1.70384139, 1.66823948750, 1.66823948751}
    },
    {
      {0.08896231569, 0.08889680629, 0.08879830091000002, 0.08866357608, 0.08848852054, 0.08826792116, 0.08799516823, 0.08766184478, 0.08725714474000001, 0.088962315690,
       0.088962315691, 0.088962315692, 0.088962315693, 0.088962315694, 0.088962315695, 0.088962315696, 0.088962315697, 0.088962315698, 0.088962315699, 0.088896806290,
       0.088896806291},
      {0.2739148753, 0.2735425644, 0.2730584933, 0.2724524452, 0.2717113664, 0.2708187145, 0.2697535596, 0.2684893287, 0.2669920281, 0.27391487530, 0.27391487531,
       0.27391487532, 0.27391487533, 0.27391487534, 0.27391487535, 0.27391487536, 0.27391487537, 0.27391487538, 0.27391487539, 0.27354256440, 0.27354256441},
      {0.4782769831, 0.4770472602, 0.4755919695, 0.4738934351, 0.4719287462, 0.4696686456, 0.4670759855, 0.4641035656, 0.4606910631, 0.47827698310, 0.47827698311,
       0.47827698312, 0.47827698313, 0.47827698314, 0.47827698315, 0.47827698316, 0.47827698317, 0.47827698318, 0.47827698319, 0.47704726020, 0.47704726021},
      {0.7051482024, 0.7024234875, 0.6993167803, 0.6958034696, 0.6918507693, 0.6874160779, 0.6824446898, 0.6768665847, 0.6705918616, 0.70514820240, 0.70514820241,
       0.70514820242, 0.70514820243, 0.70514820244, 0.70514820245, 0.70514820246, 0.70514820247, 0.70514820248, 0.70514820249, 0.70242348750, 0.70242348751},
      {0.9468448909, 0.9422356977, 0.93705744, 0.9312829047, 0.9248731358, 0.91777495, 0.9099175682, 0.9012079878, 0.8915244957, 0.94684489090, 0.94684489091,
       0.94684489092, 0.94684489093, 0.94684489094, 0.94684489095, 0.94684489096, 0.94684489097, 0.94684489098, 0.94684489099, 0.94223569770, 0.94223569771},
      {1.195608054, 1.188677108, 1.180977518, 1.1725071, 1.1632447240000001, 1.153144601, 1.142130482, 1.130089248, 1.116862815, 1.1956080540, 1.1956080541, 1.1956080542,
       1.1956080543, 1.1956080544, 1.1956080545, 1.1956080546, 1.1956080547, 1.1956080548, 1.1956080549, 1.1886771080, 1.1886771081},
      {1.447910632, 1.4375155419999999, 1.426190421, 1.4141193760000002, 1.401396021, 1.3880076629999998, 1.373844971, 1.358717918, 1.3423653729999998, 1.4479106320,
       1.4479106321, 1.4479106322, 1.4479106323, 1.4479106324, 1.4479106325, 1.4479106326, 1.4479106327, 1.4479106328, 1.4479106329, 1.43751554199999990,
       1.43751554199999991},
      {1.703841327, 1.685683817, 1.66739584, 1.6502179380000002, 1.634024328, 1.6182230240000002, 1.602185642, 1.585341843, 1.567162999, 1.7038413270, 1.7038413271,
       1.7038413272, 1.7038413273, 1.7038413274, 1.7038413275, 1.7038413276, 1.7038413277, 1.7038413278, 1.7038413279, 1.6856838170, 1.6856838171}
    },
    {
      {0.09270304794, 0.09258685916, 0.09243049006000001, 0.09222808362000001, 0.09197261689000001, 0.09165558820000001, 0.09126659479, 0.09079275231, 0.09021787993000001,
       0.092703047940, 0.092703047941, 0.092703047942, 0.092703047943, 0.092703047944, 0.092703047945, 0.092703047946, 0.092703047947, 0.092703047948, 0.092703047949,
       0.092586859160, 0.092586859161},
      {0.2812981134, 0.2808065201, 0.2801911651, 0.2794343662, 0.2785149377, 0.2774072557, 0.2760799967, 0.2744944054, 0.2726018653, 0.28129811340, 0.28129811341,
       0.28129811342, 0.28129811343, 0.28129811344, 0.28129811345, 0.28129811346, 0.28129811347, 0.28129811348, 0.28129811349, 0.28080652010, 0.28080652011},
      {0.479092751, 0.4777864778, 0.4762674395, 0.4745058098, 0.472465889, 0.4701045517, 0.4673691578, 0.4641946867, 0.4604997228, 0.4790927510, 0.4790927511,
       0.4790927512, 0.4790927513, 0.4790927514, 0.4790927515, 0.4790927516, 0.4790927517, 0.4790927518, 0.4790927519, 0.47778647780, 0.47778647781},
      {0.690337096, 0.6875501955, 0.6844586019, 0.6810197989, 0.6771829398, 0.6728866764, 0.6680562408, 0.6625994523, 0.6564011379, 0.6903370960, 0.6903370961,
       0.6903370962, 0.6903370963, 0.6903370964, 0.6903370965, 0.6903370966, 0.6903370967, 0.6903370968, 0.6903370969, 0.68755019550, 0.68755019551},
      {0.9156493925, 0.9107553398, 0.9054557014, 0.8996949004, 0.893406404, 0.8865099108, 0.878907582, 0.8704788977, 0.8610734918, 0.91564939250, 0.91564939251,
       0.91564939252, 0.91564939253, 0.91564939254, 0.91564939255, 0.91564939256, 0.91564939257, 0.91564939258, 0.91564939259, 0.91075533980, 0.91075533981},
      {1.152151362, 1.144813134, 1.136946593, 1.128483085, 1.119340112, 1.109417842, 1.098594431, 1.086719669, 1.073606158, 1.1521513620, 1.1521513621, 1.1521513622,
       1.1521513623, 1.1521513624, 1.1521513625, 1.1521513626, 1.1521513627, 1.1521513628, 1.1521513629, 1.1448131340, 1.1448131341},
      {1.395912243, 1.3860695330000001, 1.375551676, 1.364276974, 1.352146664, 1.339040662, 1.324811907, 1.309278696, 1.292214086, 1.3959122430, 1.3959122431,
       1.3959122432, 1.3959122433, 1.3959122434, 1.3959122435, 1.3959122436, 1.3959122437, 1.3959122438, 1.3959122439, 1.38606953300000010, 1.38606953300000011},
      {1.643844528, 1.63156932, 1.61845552, 1.604409003, 1.589314895, 1.573032457, 1.5553883590000002, 1.536167599, 1.515100994, 1.6438445280, 1.6438445281,
       1.6438445282, 1.6438445283, 1.6438445284, 1.6438445285, 1.6438445286, 1.6438445287, 1.6438445288, 1.6438445289, 1.631569320, 1.631569321}
    },
    {
      {0.09270304794, 0.09262024788, 0.09249952992, 0.09233613828000001, 0.09212426248, 0.0918567622, 0.09152479482, 0.09111730247, 0.09062029139, 0.092703047940,
       0.092703047941, 0.092703047942, 0.092703047943, 0.092703047944, 0.092703047945, 0.092703047946, 0.092703047947, 0.092703047948, 0.092703047949, 0.092620247880,
       0.092620247881},
      {0.2812981134, 0.2809528348, 0.2804885375, 0.2798907537, 0.2791418021, 0.2782199578, 0.2770983276, 0.2757433015, 0.2741123772, 0.28129811340, 0.28129811341,
       0.28129811342, 0.28129811343, 0.28129811344, 0.28129811345, 0.28129811346, 0.28129811347, 0.28129811348, 0.28129811349, 0.28095283480, 0.28095283481},
      {0.479092751, 0.4781869944, 0.4770671536, 0.475708676, 0.4740814957, 0.4721486337, 0.4698643007, 0.4671712819, 0.4639972623, 0.4790927510, 0.4790927511,
       0.4790927512, 0.4790927513, 0.4790927514, 0.4790927515, 0.4790927516, 0.4790927517, 0.4790927518, 0.4790927519, 0.47818699440, 0.47818699441},
      {0.690337096, 0.6884184647, 0.6861720099, 0.6835628284, 0.6805479857, 0.6770745213, 0.6730767397, 0.668472474, 0.6631578309, 0.6903370960, 0.6903370961,
       0.6903370962, 0.6903370963, 0.6903370964, 0.6903370965, 0.6903370966, 0.6903370967, 0.6903370968, 0.6903370969, 0.68841846470, 0.68841846471},
      {0.9156493925, 0.9122926131, 0.9084731094, 0.9041452821, 0.8992527284, 0.893725605, 0.8874770451, 0.8803982119, 0.8723513363, 0.91564939250, 0.91564939251,
       0.91564939252, 0.91564939253, 0.91564939254, 0.91564939255, 0.91564939256, 0.91564939257, 0.91564939258, 0.91564939259, 0.91229261310, 0.91229261311},
      {1.152151362, 1.147128803, 1.141487763, 1.1351712840000001, 1.12810857, 1.120211636, 1.11137076, 1.1014481950000001, 1.09026932, 1.1521513620, 1.1521513621,
       1.1521513622, 1.1521513623, 1.1521513624, 1.1521513625, 1.1521513626, 1.1521513627, 1.1521513628, 1.1521513629, 1.1471288030, 1.1471288031},
      {1.395912243, 1.389184282, 1.381666731, 1.373290623, 1.3639698489999998, 1.353596998, 1.342037714, 1.329122907, 1.3146377879999998, 1.3959122430, 1.3959122431,
       1.3959122432, 1.3959122433, 1.3959122434, 1.3959122435, 1.3959122436, 1.3959122437, 1.3959122438, 1.3959122439, 1.3891842820, 1.3891842821},
      {1.643844528, 1.635460726, 1.626107937, 1.615705099, 1.604150435, 1.591316358, 1.5770425909999999, 1.561126678, 1.5433106399999998, 1.6438445280, 1.6438445281,
       1.6438445282, 1.6438445283, 1.6438445284, 1.6438445285, 1.6438445286, 1.6438445287, 1.6438445288, 1.6438445289, 1.6354607260, 1.6354607261}
    },
    {
      {0.09416396099, 0.09407682114, 0.09394777714000001, 0.09377049595, 0.0935373891, 0.09323928014, 0.09286495746000001, 0.09240056185000001, 0.09182873056000002,
       0.094163960990, 0.094163960991, 0.094163960992, 0.094163960993, 0.094163960994, 0.094163960995, 0.094163960996, 0.094163960997, 0.094163960998, 0.094163960999,
       0.094076821140, 0.094076821141},
      {0.2843343494, 0.2839903941, 0.2835188445, 0.2829006727, 0.2821130849, 0.2811285256, 0.2799133375, 0.2784259272, 0.2766142018, 0.28433434940, 0.28433434941,
       0.28433434942, 0.28433434943, 0.28433434944, 0.28433434945, 0.28433434946, 0.28433434947, 0.28433434948, 0.28433434949, 0.28399039410, 0.28399039411},
      {0.4799081751, 0.4790555364, 0.4779855396, 0.4766663776, 0.4750599634, 0.4731202724, 0.470791113, 0.4680030764, 0.4646692749, 0.47990817510, 0.47990817511,
       0.47990817512, 0.47990817513, 0.47990817514, 0.47990817515, 0.47990817516, 0.47990817517, 0.47990817518, 0.47990817519, 0.47905553640, 0.47905553641},
      {0.683924319, 0.6821511908, 0.6800677823, 0.67762932, 0.6747822157, 0.6714617464, 0.6675889375, 0.6630662989, 0.6577718751, 0.6839243190, 0.6839243191,
       0.6839243192, 0.6839243193, 0.6839243194, 0.6839243195, 0.6839243196, 0.6839243197, 0.6839243198, 0.6839243199, 0.68215119080, 0.68215119081},
      {0.8982389718, 0.8950432643, 0.8914421903, 0.8873776964, 0.8827803342, 0.8775662723, 0.8716332875, 0.8648552844, 0.8570746544, 0.89823897180, 0.89823897181,
       0.89823897182, 0.89823897183, 0.89823897184, 0.89823897185, 0.89823897186, 0.89823897187, 0.89823897188, 0.89823897189, 0.89504326430, 0.89504326431},
      {1.122976754, 1.117899684, 1.112315977, 1.106154037, 1.099328208, 1.091735117, 1.083248751, 1.073713752, 1.062936067, 1.1229767540, 1.1229767541, 1.1229767542,
       1.1229767543, 1.1229767544, 1.1229767545, 1.1229767546, 1.1229767547, 1.1229767548, 1.1229767549, 1.1178996840, 1.1178996841},
      {1.356686268, 1.3494277869999998, 1.3415477, 1.3329607030000001, 1.323564658, 1.3132362359999998, 1.301825067, 1.2891457819999999, 1.2749669240000001, 1.3566862680,
       1.3566862681, 1.3566862682, 1.3566862683, 1.3566862684, 1.3566862685, 1.3566862686, 1.3566862687, 1.3566862688, 1.3566862689, 1.34942778699999980,
       1.34942778699999981},
      {1.597170681, 1.587610776, 1.5772964980000002, 1.5661285559999998, 1.553987904, 1.540730661, 1.526181309, 1.510123428, 1.492286829, 1.5971706810, 1.5971706811,
       1.5971706812, 1.5971706813, 1.5971706814, 1.5971706815, 1.5971706816, 1.5971706817, 1.5971706818, 1.5971706819, 1.5876107760, 1.5876107761}
    }
  };
  
  for(unsigned int i=0; i<5; ++i) {
    for(unsigned int j=0; j<8; ++j) {
      omegasIm[i][j].assign(&omegasImArray[i][j][0], &omegasImArray[i][j][20]);
    }
  }
  
  
  Initialized = true;
  return;
}
