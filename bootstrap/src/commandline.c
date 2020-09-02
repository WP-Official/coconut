#include "commandline.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *yy_filename;

struct command_line global_command_line;

static
void InitGlobalOptions() {
    global_command_line.verbose = 0;
    global_command_line.break_inspect_points = false;
    global_command_line.consistcheck = false;
    global_command_line.profiling = false;
    global_command_line.serialise = false;
    global_command_line.gen_user_files = false;
    global_command_line.user_dir = NULL;
    global_command_line.dot_dir = NULL;
    global_command_line.doc_dir = NULL;
    global_command_line.backend = NULL;
}

static
void Usage(char *program) {
    char *program_bin = strrchr(program, '/');
    if (program_bin)
        program = program_bin + 1;

    printf("Usage: %s [OPTION...] COCONUT_FILE\n", program);
    printf("Options:\n");
    printf("  --help                       This help message.\n");
    printf(
        "  --user-dir <directory>     Directory to write generated user files"
        "user files to.\n");
    printf("                               Defaults to ./user/src/\n");
    printf("  --list-gen-files             Outputs a list of files which "
           "would be (re)generated,\n");
    printf("                               but does not actually modify any "
           "files.(NOT IMPLEMENTED)\n");
    printf("  --verbose/-v                 Enable verbose mode.\n");
    printf("  --dot <directory>            Will produce ast.dot in "
           "<directory>.\n");
    printf("                               Prints the AST after parsing the "
           "input file\n");
    printf("  --consistency-checks         Do consistency checks on the AST "
           "during runtime.\n");
    printf("  --profiling                  Generate the requirements for a "
           "time and memory profile in your compiler.\n");
    printf("  --breakpoints                Enable setting breakpoints in your "
           "compiler, generates an API for this.\n");
    printf("  --gen_user_files             Generates user traversal files. "
           "WARNING: This will overwrite your current traversal files.\n");
    printf("  --backend <backend name>     Selects generation backend, either "
           "typed or dynamic.\n");
}

static
void Version(void) { printf("cocogen 0.1\nCoCoNut Metacompiler\n"); }

void CLprocessArgs(int argc, char *argv[]) {
    InitGlobalOptions();

    // TODO: remove the hardcoded identifiers.
    static struct option long_options[] = {
        {"verbose", no_argument, &global_command_line.verbose, 1},
        {"help", no_argument, 0, 'h'},
        {"user-dir", required_argument, 0, 21},
        {"dot", required_argument, 0, 23},
        {"version", no_argument, 0, 20},
        {"profiling", no_argument, 0, 24},
        {"breakpoints", no_argument, 0, 25},
        {"consistency-checks", no_argument, 0, 26},
        {"serialise", no_argument, 0, 27},
        {"inspectpoints", no_argument, 0, 28},
        {"gen-user-files", no_argument, 0, 29},
        {"backend", required_argument, 0, 30},
        {0, 0, 0, 0}};

    int option_index;
    int c;

    while (1) {
        c = getopt_long(argc, argv, "v", long_options, &option_index);

        // End of options
        if (c == -1)
            break;

        switch (c) {
        case 20:
            Version();
            exit(EXIT_SUCCESS);
        case 'v':
            global_command_line.verbose = 1;
            break;
        case 21: // User file output directory.
            global_command_line.user_dir = optarg;
            break;
        case 23: // ast.dot output directory.
            global_command_line.dot_dir = optarg;
            break;
        case 24:
            global_command_line.profiling = true;
            break;
        case 25:
            global_command_line.break_inspect_points = true;
            break;
        case 26:
            global_command_line.consistcheck = true;
            break;
        case 27:
            global_command_line.serialise = true;
            break;
        case 28:
            global_command_line.break_inspect_points = true;
            break;
        case 29:
            global_command_line.gen_user_files = true;
            break;
        case 30:
            global_command_line.backend = optarg;
            break;
        case 'h':
            Usage(argv[0]);
            exit(EXIT_SUCCESS);
        case '?':
            Usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind == argc - 1) {
        global_command_line.input_file = argv[optind];
    } else {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }
}