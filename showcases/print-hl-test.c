#include <stddef.h>
#define LEX_PROFILER
#define LEX_IMPLEMENTATION
#include "../lex.h"

enum {
  WS,
  AA,AB,AC,AD,AE,AF,AG,AH,AI,AJ,AK,AL,AM,AN,AO,AP,AQ,AR,AS,AT,AU,AV,AW,AX,AY,AZ,
  BA,BB,BC,BD,BE,BF,BG,BH,BI,BJ,BK,BL,BM,BN,BO,BP,BQ,BR,BS,BT,BU,BV,BW,BX,BY,BZ,
  CA,CB,CC,CD,CE,CF,CG,CH,CI,CJ,CK,CL,CM,CN,CO,CP,CQ,CR,CS,CT,CU,CV,CW,CX,CY,CZ,
  DA,DB,DC,DD,DE,DF,DG,DH,DI,DJ,DK,DL,DM,DN,DO,DP,DQ,DR,DS,DT,DU,DV,DW,DX,DY,DZ,
  COUNT
};

size_t rule_A(LexCursor cursor) { return lex_match_chars(cursor, "A"); }
size_t rule_AA(LexCursor c) { return lex_match_exact(c, "AA"); }
size_t rule_AB(LexCursor c) { return lex_match_exact(c, "AB"); }
size_t rule_AC(LexCursor c) { return lex_match_exact(c, "AC"); }
size_t rule_AD(LexCursor c) { return lex_match_exact(c, "AD"); }
size_t rule_AE(LexCursor c) { return lex_match_exact(c, "AE"); }
size_t rule_AF(LexCursor c) { return lex_match_exact(c, "AF"); }
size_t rule_AG(LexCursor c) { return lex_match_exact(c, "AG"); }
size_t rule_AH(LexCursor c) { return lex_match_exact(c, "AH"); }
size_t rule_AI(LexCursor c) { return lex_match_exact(c, "AI"); }
size_t rule_AJ(LexCursor c) { return lex_match_exact(c, "AJ"); }
size_t rule_AK(LexCursor c) { return lex_match_exact(c, "AK"); }
size_t rule_AL(LexCursor c) { return lex_match_exact(c, "AL"); }
size_t rule_AM(LexCursor c) { return lex_match_exact(c, "AM"); }
size_t rule_AN(LexCursor c) { return lex_match_exact(c, "AN"); }
size_t rule_AO(LexCursor c) { return lex_match_exact(c, "AO"); }
size_t rule_AP(LexCursor c) { return lex_match_exact(c, "AP"); }
size_t rule_AQ(LexCursor c) { return lex_match_exact(c, "AQ"); }
size_t rule_AR(LexCursor c) { return lex_match_exact(c, "AR"); }
size_t rule_AS(LexCursor c) { return lex_match_exact(c, "AS"); }
size_t rule_AT(LexCursor c) { return lex_match_exact(c, "AT"); }
size_t rule_AU(LexCursor c) { return lex_match_exact(c, "AU"); }
size_t rule_AV(LexCursor c) { return lex_match_exact(c, "AV"); }
size_t rule_AW(LexCursor c) { return lex_match_exact(c, "AW"); }
size_t rule_AX(LexCursor c) { return lex_match_exact(c, "AX"); }
size_t rule_AY(LexCursor c) { return lex_match_exact(c, "AY"); }
size_t rule_AZ(LexCursor c) { return lex_match_exact(c, "AZ"); }
size_t rule_BA(LexCursor c) { return lex_match_exact(c, "BA"); }
size_t rule_BB(LexCursor c) { return lex_match_exact(c, "BB"); }
size_t rule_BC(LexCursor c) { return lex_match_exact(c, "BC"); }
size_t rule_BD(LexCursor c) { return lex_match_exact(c, "BD"); }
size_t rule_BE(LexCursor c) { return lex_match_exact(c, "BE"); }
size_t rule_BF(LexCursor c) { return lex_match_exact(c, "BF"); }
size_t rule_BG(LexCursor c) { return lex_match_exact(c, "BG"); }
size_t rule_BH(LexCursor c) { return lex_match_exact(c, "BH"); }
size_t rule_BI(LexCursor c) { return lex_match_exact(c, "BI"); }
size_t rule_BJ(LexCursor c) { return lex_match_exact(c, "BJ"); }
size_t rule_BK(LexCursor c) { return lex_match_exact(c, "BK"); }
size_t rule_BL(LexCursor c) { return lex_match_exact(c, "BL"); }
size_t rule_BM(LexCursor c) { return lex_match_exact(c, "BM"); }
size_t rule_BN(LexCursor c) { return lex_match_exact(c, "BN"); }
size_t rule_BO(LexCursor c) { return lex_match_exact(c, "BO"); }
size_t rule_BP(LexCursor c) { return lex_match_exact(c, "BP"); }
size_t rule_BQ(LexCursor c) { return lex_match_exact(c, "BQ"); }
size_t rule_BR(LexCursor c) { return lex_match_exact(c, "BR"); }
size_t rule_BS(LexCursor c) { return lex_match_exact(c, "BS"); }
size_t rule_BT(LexCursor c) { return lex_match_exact(c, "BT"); }
size_t rule_BU(LexCursor c) { return lex_match_exact(c, "BU"); }
size_t rule_BV(LexCursor c) { return lex_match_exact(c, "BV"); }
size_t rule_BW(LexCursor c) { return lex_match_exact(c, "BW"); }
size_t rule_BX(LexCursor c) { return lex_match_exact(c, "BX"); }
size_t rule_BY(LexCursor c) { return lex_match_exact(c, "BY"); }
size_t rule_BZ(LexCursor c) { return lex_match_exact(c, "BZ"); }
size_t rule_CA(LexCursor c) { return lex_match_exact(c, "CA"); }
size_t rule_CB(LexCursor c) { return lex_match_exact(c, "CB"); }
size_t rule_CC(LexCursor c) { return lex_match_exact(c, "CC"); }
size_t rule_CD(LexCursor c) { return lex_match_exact(c, "CD"); }
size_t rule_CE(LexCursor c) { return lex_match_exact(c, "CE"); }
size_t rule_CF(LexCursor c) { return lex_match_exact(c, "CF"); }
size_t rule_CG(LexCursor c) { return lex_match_exact(c, "CG"); }
size_t rule_CH(LexCursor c) { return lex_match_exact(c, "CH"); }
size_t rule_CI(LexCursor c) { return lex_match_exact(c, "CI"); }
size_t rule_CJ(LexCursor c) { return lex_match_exact(c, "CJ"); }
size_t rule_CK(LexCursor c) { return lex_match_exact(c, "CK"); }
size_t rule_CL(LexCursor c) { return lex_match_exact(c, "CL"); }
size_t rule_CM(LexCursor c) { return lex_match_exact(c, "CM"); }
size_t rule_CN(LexCursor c) { return lex_match_exact(c, "CN"); }
size_t rule_CO(LexCursor c) { return lex_match_exact(c, "CO"); }
size_t rule_CP(LexCursor c) { return lex_match_exact(c, "CP"); }
size_t rule_CQ(LexCursor c) { return lex_match_exact(c, "CQ"); }
size_t rule_CR(LexCursor c) { return lex_match_exact(c, "CR"); }
size_t rule_CS(LexCursor c) { return lex_match_exact(c, "CS"); }
size_t rule_CT(LexCursor c) { return lex_match_exact(c, "CT"); }
size_t rule_CU(LexCursor c) { return lex_match_exact(c, "CU"); }
size_t rule_CV(LexCursor c) { return lex_match_exact(c, "CV"); }
size_t rule_CW(LexCursor c) { return lex_match_exact(c, "CW"); }
size_t rule_CX(LexCursor c) { return lex_match_exact(c, "CX"); }
size_t rule_CY(LexCursor c) { return lex_match_exact(c, "CY"); }
size_t rule_CZ(LexCursor c) { return lex_match_exact(c, "CZ"); }
size_t rule_DA(LexCursor c) { return lex_match_exact(c, "DA"); }
size_t rule_DB(LexCursor c) { return lex_match_exact(c, "DB"); }
size_t rule_DC(LexCursor c) { return lex_match_exact(c, "DC"); }
size_t rule_DD(LexCursor c) { return lex_match_exact(c, "DD"); }
size_t rule_DE(LexCursor c) { return lex_match_exact(c, "DE"); }
size_t rule_DF(LexCursor c) { return lex_match_exact(c, "DF"); }
size_t rule_DG(LexCursor c) { return lex_match_exact(c, "DG"); }
size_t rule_DH(LexCursor c) { return lex_match_exact(c, "DH"); }
size_t rule_DI(LexCursor c) { return lex_match_exact(c, "DI"); }
size_t rule_DJ(LexCursor c) { return lex_match_exact(c, "DJ"); }
size_t rule_DK(LexCursor c) { return lex_match_exact(c, "DK"); }
size_t rule_DL(LexCursor c) { return lex_match_exact(c, "DL"); }
size_t rule_DM(LexCursor c) { return lex_match_exact(c, "DM"); }
size_t rule_DN(LexCursor c) { return lex_match_exact(c, "DN"); }
size_t rule_DO(LexCursor c) { return lex_match_exact(c, "DO"); }
size_t rule_DP(LexCursor c) { return lex_match_exact(c, "DP"); }
size_t rule_DQ(LexCursor c) { return lex_match_exact(c, "DQ"); }
size_t rule_DR(LexCursor c) { return lex_match_exact(c, "DR"); }
size_t rule_DS(LexCursor c) { return lex_match_exact(c, "DS"); }
size_t rule_DT(LexCursor c) { return lex_match_exact(c, "DT"); }
size_t rule_DU(LexCursor c) { return lex_match_exact(c, "DU"); }
size_t rule_DV(LexCursor c) { return lex_match_exact(c, "DV"); }
size_t rule_DW(LexCursor c) { return lex_match_exact(c, "DW"); }
size_t rule_DX(LexCursor c) { return lex_match_exact(c, "DX"); }
size_t rule_DY(LexCursor c) { return lex_match_exact(c, "DY"); }
size_t rule_DZ(LexCursor c) { return lex_match_exact(c, "DZ"); }


LexType abc[COUNT] = {
  LEX_TYPE(WS, lex_builtin_rule_ws, .skip = true),
  LEX_TYPE(AA, rule_AA), LEX_TYPE(AB, rule_AB), LEX_TYPE(AC, rule_AC),
  LEX_TYPE(AD, rule_AD), LEX_TYPE(AE, rule_AE), LEX_TYPE(AF, rule_AF),
  LEX_TYPE(AG, rule_AG), LEX_TYPE(AH, rule_AH), LEX_TYPE(AI, rule_AI),
  LEX_TYPE(AJ, rule_AJ), LEX_TYPE(AK, rule_AK), LEX_TYPE(AL, rule_AL),
  LEX_TYPE(AM, rule_AM), LEX_TYPE(AN, rule_AN), LEX_TYPE(AO, rule_AO),
  LEX_TYPE(AP, rule_AP), LEX_TYPE(AQ, rule_AQ), LEX_TYPE(AR, rule_AR),
  LEX_TYPE(AS, rule_AS), LEX_TYPE(AT, rule_AT), LEX_TYPE(AU, rule_AU),
  LEX_TYPE(AV, rule_AV), LEX_TYPE(AW, rule_AW), LEX_TYPE(AX, rule_AX),
  LEX_TYPE(AY, rule_AY), LEX_TYPE(AZ, rule_AZ), LEX_TYPE(BA, rule_BA),
  LEX_TYPE(BB, rule_BB), LEX_TYPE(BC, rule_BC), LEX_TYPE(BD, rule_BD),
  LEX_TYPE(BE, rule_BE), LEX_TYPE(BF, rule_BF), LEX_TYPE(BG, rule_BG),
  LEX_TYPE(BH, rule_BH), LEX_TYPE(BI, rule_BI), LEX_TYPE(BJ, rule_BJ),
  LEX_TYPE(BK, rule_BK), LEX_TYPE(BL, rule_BL), LEX_TYPE(BM, rule_BM),
  LEX_TYPE(BN, rule_BN), LEX_TYPE(BO, rule_BO), LEX_TYPE(BP, rule_BP),
  LEX_TYPE(BQ, rule_BQ), LEX_TYPE(BR, rule_BR), LEX_TYPE(BS, rule_BS),
  LEX_TYPE(BT, rule_BT), LEX_TYPE(BU, rule_BU), LEX_TYPE(BV, rule_BV),
  LEX_TYPE(BW, rule_BW), LEX_TYPE(BX, rule_BX), LEX_TYPE(BY, rule_BY),
  LEX_TYPE(BZ, rule_BZ), LEX_TYPE(CA, rule_CA), LEX_TYPE(CB, rule_CB),
  LEX_TYPE(CC, rule_CC), LEX_TYPE(CD, rule_CD), LEX_TYPE(CE, rule_CE),
  LEX_TYPE(CF, rule_CF), LEX_TYPE(CG, rule_CG), LEX_TYPE(CH, rule_CH),
  LEX_TYPE(CI, rule_CI), LEX_TYPE(CJ, rule_CJ), LEX_TYPE(CK, rule_CK),
  LEX_TYPE(CL, rule_CL), LEX_TYPE(CM, rule_CM), LEX_TYPE(CN, rule_CN),
  LEX_TYPE(CO, rule_CO), LEX_TYPE(CP, rule_CP), LEX_TYPE(CQ, rule_CQ),
  LEX_TYPE(CR, rule_CR), LEX_TYPE(CS, rule_CS), LEX_TYPE(CT, rule_CT),
  LEX_TYPE(CU, rule_CU), LEX_TYPE(CV, rule_CV), LEX_TYPE(CW, rule_CW),
  LEX_TYPE(CX, rule_CX), LEX_TYPE(CY, rule_CY), LEX_TYPE(CZ, rule_CZ),
  LEX_TYPE(DA, rule_DA), LEX_TYPE(DB, rule_DB), LEX_TYPE(DC, rule_DC),
  LEX_TYPE(DD, rule_DD), LEX_TYPE(DE, rule_DE), LEX_TYPE(DF, rule_DF),
  LEX_TYPE(DG, rule_DG), LEX_TYPE(DH, rule_DH), LEX_TYPE(DI, rule_DI),
  LEX_TYPE(DJ, rule_DJ), LEX_TYPE(DK, rule_DK), LEX_TYPE(DL, rule_DL),
  LEX_TYPE(DM, rule_DM), LEX_TYPE(DN, rule_DN), LEX_TYPE(DO, rule_DO),
  LEX_TYPE(DP, rule_DP), LEX_TYPE(DQ, rule_DQ), LEX_TYPE(DR, rule_DR),
  LEX_TYPE(DS, rule_DS), LEX_TYPE(DT, rule_DT), LEX_TYPE(DU, rule_DU),
  LEX_TYPE(DV, rule_DV), LEX_TYPE(DW, rule_DW), LEX_TYPE(DX, rule_DX),
  LEX_TYPE(DY, rule_DY), LEX_TYPE(DZ, rule_DZ),
};

int main() {
  Lex abboba = lex_init(LEX_TYPEARRAY(abc),
    "AA AB AC AD AE AF AG AH AI AJ AK AL AM AN AO AP AQ AR AS AT AU AV AW AX AY AZ\n"
    "BA BB BC BD BE BF BG BH BI BJ BK BL BM BN BO BP BQ BR BS BT BU BV BW BX BY BZ\n"
    "CA CB CC CD CE CF CG CH CI CJ CK CL CM CN CO CP CQ CR CS CT CU CV CW CX CY CZ\n"
    "DA DB DC DD DE DF DG DH DI DJ DK DL DM DN DO DP DQ DR DS DT DU DV DW DX DY DZ\n"
  );

  lex_print_hl(abboba, false);
}
