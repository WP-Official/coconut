#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "filegen/driver.h"
#include "filegen/genmacros.h"
#include "gen-functions.h"

static int indent = 0;

void gen_default_values(Config *config, FILE *fp, Attr *attr) {
    switch (attr->type) {
    case AT_int:
    case AT_int8:
    case AT_int16:
    case AT_int32:
    case AT_int64:
        out("%i", 0);
        break;
    case AT_uint:
    case AT_uint8:
    case AT_uint16:
    case AT_uint32:
    case AT_uint64:
        out("%u", 0);
        break;
    case AT_float:
    case AT_double:
        out("%f", 0.0);
        break;
    case AT_bool:
        out("%s", "false");
        break;
    case AT_string:
    case AT_link:
        out("NULL");
        break;
    case AT_enum:
        out("%d", 0);
        break;
    }
}

void gen_init_arguments(Config *config, FILE *fp, Node *node) {
    for (int i = 0; i < array_size(node->children); ++i) {
        Child *child = (Child *)array_get(node->children, i);
        out("NULL");
        if (i != array_size(node->children) - 1 ||
            array_size(node->attrs) > 0) {
            out(", ");
        }
    }
    for (int i = 0; i < array_size(node->attrs); ++i) {
        Attr *attr = (Attr *)array_get(node->attrs, i);
        gen_default_values(config, fp, attr);
        if (i != array_size(node->attrs) - 1) {
            out(", ");
        }
    }
}

void gen_copy_vtable(Config *config, FILE *fp) {
    out("TravFunc vtable_copy[_NT_SIZE] = { &trav_error, ");
    for (int j = 0; j < array_size(config->nodes); j++) {
        Node *node = array_get(config->nodes, j);
        char *nodelwr = strlwr(node->id);
        out("&copy_%s, ", nodelwr);
        free(nodelwr);
    }
    out(" };\n\n");
}

void gen_copy_header(Config *config, FILE *fp) {
    out("#ifndef _CCN_COPY_H_\n");
    out("#define _CCN_COPY_H_\n\n");
    out("#include \"core/ast_core.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        char *nodelwr = strlwr(node->id);
        out("extern Node *copy_%s(Node *arg_node);\n", nodelwr);
        free(nodelwr);
    }
    out("\n");
    out("extern TravFunc vtable_copy[_NT_SIZE];");
    out("\n");
    out("#endif /* _CCN_COPY_H_ */\n");
}

void gen_copy_func(Config *config, FILE *fp, Node *node) {
    char *nodelwr = strlwr(node->id);
    char *nodeupr = strupr(node->id);
    out_comment("%s", node->id);
    out_start_func("Node *copy_%s(Node *arg_node)", nodelwr);
    out("    Node *result = node_init_%s(", nodelwr);
    gen_init_arguments(config, fp, node);
    out(");\n");
    for (int i = 0; i < array_size(node->attrs); i++) {
        Attr *attr = array_get(node->attrs, i);
        char *attrupr = strupr(attr->id);
        char *copyfunc;
        if (attr->type == AT_string) {
            copyfunc = "ccn_str_cpy";
        } else if (attr->type == AT_link) {
            copyfunc = "copy_node";
        } else {
            continue;
        }
        out_field("%s_%s(arg_node) = %s(%s_%s(arg_node))", nodeupr, attrupr,
                  copyfunc, nodeupr, attrupr);
        free(attrupr);
    }
    for (int i = 0; i < array_size(node->children); i++) {
        Child *child = array_get(node->children, i);
        char *childupr = strupr(child->id);
        out_field("%s_%s(arg_node) = traverse(%s_%s(arg_node))", nodeupr,
                  childupr, nodeupr, childupr);
        free(childupr);
    }
    out_field("return result");
    out_end_func();
    free(nodelwr);
    free(nodeupr);
}

void gen_copy_src(Config *config, FILE *fp) {
    out("#include \"core/copy_core.h\"\n");
    out("#include \"core/trav_core.h\"\n");
    out("#include \"lib/str.h\"\n");
    out("\n");
    for (int i = 0; i < array_size(config->nodes); ++i) {
        Node *node = array_get(config->nodes, i);
        gen_copy_func(config, fp, node);
    }
    gen_copy_vtable(config, fp);
}