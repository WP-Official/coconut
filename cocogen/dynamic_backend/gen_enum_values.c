#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "globals.h"

static FILE *fp;
static int indent = 0;


node_st *DGEVast(node_st *node)
{
    fp = globals.fp;
    TRAVopt(AST_ENUMS(node));
    return node;
}

static char *enum_prefx;
node_st *DGEVienum(node_st *node)
{
    OUT_ENUM("%s", ID_LWR(IENUM_NAME(node)));
    enum_prefx = ID_UPR(IENUM_IPREFIX(node));
    TRAVopt(IENUM_VALS(node));
    OUT_ENUM_END();
    TRAVopt(IENUM_NEXT(node));
    return node;
}

node_st *DGEVid(node_st *node)
{
    fp = globals.fp;
    OUT_ENUM_FIELD("%s_%s", enum_prefx, ID_LWR(node));
    TRAVchildren(node);
    return node;
}
