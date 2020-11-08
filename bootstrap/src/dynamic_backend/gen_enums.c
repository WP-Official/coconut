#include <stddef.h>
#include <stdio.h>
#include "assert.h"

#include "gen_helpers/out_macros.h"
#include "palm/ctinfo.h"
#include "palm/str.h"
#include "ccn/dynamic_core.h"
#include "globals.h"

void *DGEallocTravData()
{
    return 0;
}

void DGEfreeTravData(void *data)
{

}

static FILE *fp;
static int indent = 0;
static char *basic_node_type = "node_st";
static char *curr_node_name = NULL;
static char *curr_node_name_upr = NULL;
static int child_num = 0;
static char *node_type_enum_prefix = "NT_";

static void do_enum(char *name, node_st *node, char *prefix)
{
    OUT_ENUM("%s", name);
    OUT_ENUM_FIELD("%sNULL", prefix);
    TRAVopt(node);
    if (node && NODE_TYPE(node) == NT_ITRAVERSAL) {
        OUT_ENUM_FIELD("TRAV_free");
        OUT_ENUM_FIELD("TRAV_check");
        OUT_ENUM_FIELD("TRAV_cpy");
    }
    OUT_ENUM_FIELD("_%sSIZE", prefix);
    OUT_ENUM_END();
}

node_st *DGEast(node_st *node)
{
    fp = globals.fp;
    do_enum("nodesettype", AST_INODESETS(node), "NS_");
    do_enum("ccn_nodetype", AST_INODES((node)), "NT_");
    do_enum("ccn_traversal_type", AST_ITRAVERSALS(node), "TRAV_");
    do_enum("ccn_pass_type", AST_IPASSES(node), "PASS_");

    return node;
}


node_st *DGEitraversal(node_st *node)
{
    OUT_ENUM_FIELD("TRAV_%s", ID_UPR(ITRAVERSAL_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEipass(node_st *node)
{
    OUT_ENUM_FIELD("PASS_%s", ID_UPR(IPASS_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEinode(node_st *node)
{
    OUT_ENUM_FIELD("NT_%s", ID_UPR(INODE_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEinodeset(node_st *node)
{
    OUT_ENUM_FIELD("NS_%s", ID_UPR(INODESET_NAME(node)));
    TRAVchildren(node);
    return node;
}

node_st *DGEchild(node_st *node)
{
    TRAVchildren(node);
    return node;
}
