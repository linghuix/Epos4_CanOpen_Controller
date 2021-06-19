
#include "gait.h"

/*int test_angle[24] = {	1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
							500,0,
							(0.0),0.0,0.0,0.0,0.0,
							(0.0),0.0,0.0,0.0,0.0,
							0,500};
*/
int test_angle[114] = {500, 
1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
500,0, -500,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-500, 0};




//float hip_flexion[50] = {28.237,27.53,26.662,25.546,24.135,22.423,20.469,18.409,16.358,14.332,12.297,10.229,8.136,6.069,4.064,2.153,0.341,-1.363,-2.986,-4.531,-6.007,-7.399,-8.683,-9.837,-10.814,-11.531,-11.848,-11.586,-10.581,-8.744,-6.111,-2.832,0.895,4.868,8.909,12.829,16.48,19.735,22.537,24.883,26.794,28.29,29.369,30.06,30.427,30.561,30.551,30.452,30.282,29.988};

//float knee_flexion[50] = {7.102,8.449,10.158,11.772,12.992,13.599,13.527,12.995,12.22,11.283,10.256,9.193,8.152,7.174,6.271,5.442,4.702,4.079,3.597,3.299,3.222,3.418,3.947,4.855,6.2,8.084,10.621,13.945,18.181,23.361,29.341,35.748,42.002,47.489,51.734,54.476,55.651,55.3,53.502,50.384,46.118,40.922,35.037,28.801,22.629,16.957,12.215,8.741,6.708,6.017};

//int hip[50]={128478,125262,121312,116234,109814,102025,93134,83761,74429,65211,55951,46542,37019,27614,18491,9796,1552,-6202,-13586,-20616,-27332,-33665,-39508,-44758,-49204,-52466,-53908,-52716,-48144,-39785,-27805,-12886,4072,22149,40536,58372,74984,89794,102543,113218,121913,128720,133629,136773,138443,139053,139007,138557,137783,136445};
////int knee[50]={32314,38443,46219,53563,59114,61875,61548,59127,55601,51338,46665,41828,37092,32642,28533,24761,21394,18559,16366,15010,14660,15552,17959,22090,28210,36782,48326,63450,82724,106293,133502,162653,191109,216075,235390,247866,253212,251615,243434,229247,209837,186195,159418,131045,102962,77154,55578,39772,30521,27377};
//int hip[200] = {128478,127731,126959,126156,125315,124430,123494,122502,121446,120320,119122,117846,116492,115055,113535,111930,110241,108466,106608,104670,102656,100570,98419,96215,93968,91689,89388,87073,84754,82439,80133,77835,75547,73267,70995,68727,66460,64192,61920,59641,57355,55060,52755,50439,48114,45778,43434,41087,38740,36399,34067,31747,29440,27147,24872,22617,20384,18177,15999,13848,11724,9626,7554,5510,3495,1512,-439,-2360,-4255,-6126,-7977,-9808,-11617,-13405,-15170,-16913,-18635,-20339,-22026,-23694,-25343,-26970,-28574,-30154,-31708,-33234,-34731,-36196,-37630,-39030,-40396,-41726,-43015,-44262,-45463,-46613,-47708,-48741,-49709,-50602,-51411,-52127,-52740,-53239,-53610,-53842,-53921,-53834,-53571,-53119,-52466,-51603,-50522,-49217,-47681,-45910,-43907,-41674,-39213,-36530,-33636,-30541,-27256,-23795,-20169,-16393,-12481,-8446,-4303,-66,4250,8631,13067,17546,22057,26588,31127,35659,40169,44644,49074,53450,57762,62001,66159,70225,74188,78041,81778,85394,88888,92256,95496,98609,101595,104455,107189,109799,112286,114653,116900,119030,121045,122947,124736,126410,127969,129412,130738,131950,133050,134040,134924,135706,136389,136979,137481,137901,138245,138520,138734,138891,139000,139066,139094,139090,139055,138994,138910,138804,138680,138539,138381,138203,138001,137768,137501,137194,136844,136445};
//int knee[200] = {32314,33548,34985,36591,38333,40176,42088,44035,45982,47899,49763,51556,53261,54859,56334,57669,58850,59861,60691,61333,61779,62023,62075,61954,61682,61278,60760,60144,59447,58684,57866,56994,56071,55100,54085,53032,51947,50836,49704,48553,47386,46206,45017,43824,42630,41440,40259,39088,37931,36791,35670,34569,33488,32427,31388,30370,29372,28394,27436,26498,25582,24689,23820,22977,22162,21378,20628,19911,19230,18585,17977,17410,16887,16412,15988,15618,15303,15046,14849,14715,14647,14649,14726,14881,15118,15443,15859,16370,16979,17689,18502,19421,20449,21591,22849,24229,25739,27387,29180,31126,33232,35505,37954,40585,43410,46436,49674,53132,56821,60750,64929,69364,74060,79019,84244,89735,95483,101479,107713,114172,120832,127667,134652,141757,148943,156167,163388,170564,177651,184605,191382,197940,204245,210263,215962,221311,226290,230881,235067,238831,242168,245075,247548,249586,251190,252364,253111,253437,253344,252839,251925,250609,248897,246799,244323,241478,238275,234726,230841,226633,222114,217301,212209,206855,201252,195417,189363,183106,176664,170062,163322,156468,149527,142526,135495,128464,121460,114514,107655,100912,94316,87898,81687,75715,70010,64595,59494,54729,50322,46288,42642,39397,36562,34131,32090,30430,29140,28208,27624,27377};	

int hip[400] = {128884,128429,127960,127477,126980,126468,125941,125401,124845,124274,123689,123088,122472,121841,121195,120533,119856,119164,118457,117734,116996,116243,115475,114692,113895,113082,112255,111413,110557,109687,108804,107906,106995,106070,105132,104181,103326,102461,101585,100698,99801,98894,97976,97048,96110,95162,94204,93237,92260,91274,90278,89274,88261,87239,86209,85171,84124,83071,82009,80941,79866,78784,77696,76602,75502,74398,73288,72173,71055,69932,68806,67677,66545,65410,64273,63135,61995,60853,59711,58569,57426,56283,55141,53999,52858,51718,50579,49442,48307,47173,46040,44910,43782,42656,41532,40411,39292,38175,37061,35950,34841,33735,32632,31532,30434,29340,28248,27160,26074,24992,23914,22838,21766,20698,19634,18573,17516,16463,15415,14370,13330,12294,11263,10236,9214,8197,7185,6177,5175,4178,3187,2200,1220,245,-725,-1689,-2647,-3599,-4546,-5486,-6420,-7349,-8271,-9187,-10096,-11000,-11897,-12788,-13672,-14549,-15420,-16285,-17143,-17994,-18838,-19675,-20505,-21328,-22144,-22953,-23754,-24547,-25332,-26110,-26879,-27640,-28393,-29137,-29872,-30597,-31313,-32019,-32715,-33400,-34075,-34737,-35388,-36027,-36653,-37265,-37864,-38448,-39017,-39571,-40108,-40628,-41131,-41616,-42082,-42528,-42954,-43359,-43742,-44103,-44440,-44754,-45042,-45305,-45542,-45752,-45935,-46088,-46213,-46308,-46373,-46406,-46408,-46378,-46314,-46217,-46087,-45922,-45722,-45486,-45215,-44908,-44564,-44183,-43765,-43310,-42817,-42286,-41716,-41109,-40463,-39778,-39055,-38293,-37492,-36653,-35776,-34860,-33905,-32913,-31882,-30813,-29707,-28564,-27384,-26167,-24914,-23625,-22302,-20944,-19551,-18126,-16668,-15178,-13657,-12106,-10525,-8915,-7278,-5614,-3924,-2209,-470,1291,3074,4877,6699,8539,10396,12269,14156,16056,17968,19891,21823,23763,25710,27662,29619,31578,33540,35502,37464,39424,41382,43335,45284,47226,49162,51089,53007,54916,56813,58698,60571,62430,64275,66104,67917,69714,71494,73255,74997,76720,78423,80105,81766,83405,85022,86616,88186,89733,91255,92752,94225,95671,97092,98487,99855,101196,102511,103798,105059,106292,107497,108676,109826,110949,112044,113112,114152,115165,116151,117110,118041,118946,119825,120677,121503,122304,123079,123829,124554,125255,125932,126585,127215,127822,128406,128968,129508,130026,130524,131001,131457,131893,132310,132708,133086,133446,133788,134112,134419,134708,134980,135235,135474,135696,135903,136093,136269,136319,136353,136369,136369,136354,136322,136274,136212,136134,136042,135935,135813,135678,135529,135365,135189,134998,134795,134578,134347,134104,133848,133578,133296,133000,132692,132371,132036,131689,131328,130954,130567,130167,129753,129325};
int knee[400] = {32880,33314,33828,34419,35082,35813,36608,37462,38372,39332,40340,41389,42261,43144,44034,44929,45824,46716,47603,48481,49347,50198,51032,51846,52638,53404,54143,54854,55532,56178,56790,57364,57901,58399,58857,59275,59650,59984,60276,60526,60733,60900,61025,61110,61155,61163,61132,61066,60966,60832,60666,60471,60247,59996,59720,59421,59100,58757,58396,58017,57620,57209,56782,56342,55889,55424,54948,54462,53966,53460,52946,52424,51894,51356,50812,50262,49706,49144,48579,48009,47435,46859,46280,45699,45116,44532,43948,43363,42778,42194,41611,41030,40450,39872,39297,38724,38155,37588,37026,36467,35913,35363,34817,34276,33739,33207,32680,32158,31641,31128,30621,30119,29623,29131,28645,28164,27689,27219,26755,26297,25845,25398,24958,24524,24097,23676,23262,22855,22456,22063,21679,21302,20933,20573,20221,19878,19544,19219,18904,18600,18305,18021,17747,17485,17234,16995,16768,16553,16350,16161,15985,15822,15674,15539,15420,15316,15227,15155,15099,15059,15037,15032,15046,15078,15129,15199,15290,15400,15531,15684,15858,16054,16273,16515,16780,17069,17383,17721,18084,18474,18889,19330,19799,20296,20821,21374,21957,22570,23214,23889,24595,25335,26108,26915,27758,28636,29551,30504,31495,32526,33597,34709,35865,37064,38307,39596,40932,42315,43748,45230,46763,48349,49987,51680,53428,55233,57094,59013,60992,63030,65129,67289,69510,71793,74139,76547,79017,81551,84146,86803,89521,92300,95138,98033,100986,103994,107055,110167,113329,116537,119788,123080,126410,129773,133168,136588,140032,143494,146971,150457,153949,157441,160929,164408,167874,171320,174743,178137,181498,184821,188101,191333,194513,197636,200699,203697,206627,209484,212266,214968,217587,220122,222568,224924,227187,229355,231427,233400,235274,237046,238715,240282,241744,243102,244355,245502,246544,247479,248309,249032,249651,250163,250571,250874,251073,251168,251160,251049,250837,250523,250110,249596,248984,248274,247468,246565,245568,244476,243292,242016,240650,239195,237652,236023,234309,232511,230631,228671,226632,224515,222323,220058,217720,215311,212834,210290,207681,205009,202276,199484,196635,193732,190777,187771,184718,181620,178479,175298,172080,168827,165542,162229,158890,155528,152146,148749,145338,141917,138490,135060,131631,128206,124789,121383,117992,114620,111270,107946,104652,101391,98167,94984,91844,88752,85710,82723,79793,76925,74120,71382,68715,66120,63601,61160,58801,56524,54333,52229,50214,48288,46454,44712,43062,41506,40044,38676,37403,36223,35137,34359,33696,33146,32707,32378,32154,32034,32015,32093,32266,32529};
/**
 *@brief:获取位置控制的第一个数据
	
 */
#include "canopen_interface.h"
#include "func_CanOpen.h"
#define QC_TO_Degree_EC90 1//4551.1//1820.44
#define ARRAY_K   knee
#define ARRAY_H   hip
void firstPos(uint32_t * pos){
	
	int Position_int[4];
	int x=0, temp_x;
	
	
	//Position_float[0] = (ARRAY_H[x]*QC_TO_Degree_EC90);
	Position_int[0] = (ARRAY_H[x]*QC_TO_Degree_EC90);		// postive means go forward
	pos[0] = (Uint32)Position_int[0];					//for node2
	
	//Position_float[1] = (ARRAY_K[x]*QC_TO_Degree_EC90);
	Position_int[1] = (ARRAY_K[x]*-QC_TO_Degree_EC90);
	pos[1] = (Uint32)Position_int[1];					//for node3 counterclkwise
	
	int endP = sizeof(ARRAY_H)/sizeof(*ARRAY_H);
	temp_x = x + endP/2;									//start form half period. another side
	if(temp_x >= endP){
		temp_x = temp_x - endP;
	}
	
	//Position_float[2] = (ARRAY_H[temp_x]*QC_TO_Degree_EC90);
	Position_int[2] = (ARRAY_H[temp_x]*-QC_TO_Degree_EC90);
	pos[2] = (Uint32)Position_int[2];					//for node4
	
	//Position_float[3] = (ARRAY_K[temp_x]*QC_TO_Degree_RE40);
	Position_int[3] = (ARRAY_K[temp_x]*QC_TO_Degree_EC90);
	pos[3] = (Uint32)Position_int[3];					//for node5  clkwise
}


