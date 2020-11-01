#include "gen_helpers/out_macros.h"
#include "ccn/dynamic_core.h"
#include "gen_helpers/format.h"
#include "globals.h"

static int indent = 0;
node_st *DGTDCHitraversal(node_st *node)
{
    FILE *fp = globals.fp;
    node_st *id = ITRAVERSAL_NAME(node);
    if (ITRAVERSAL_DATA(node)) {
        OUT("void TRAVdataInit%s(struct ccn_trav *trav);\n", ID_ORIG(id));
        OUT("void TRAVdataFree%s(struct ccn_trav *trav);\n", ID_ORIG(id));
    }
    TRAVopt(ITRAVERSAL_NEXT(node));
    return node;
}
