/* 046267 Computer Architecture - Winter 20/21 - HW #3               */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <stdbool.h>

// Each command will be described with this struct, and we will create an array of those structs.
struct CommandEntry {
    // Internal command cycles.
    unsigned int cycles;
    // The index of the first dependency.
    int dep1;
    // The index of the second dependency.
    int dep2;
    // If the command is of "exit" type, so no other command is depened on it, this bool is true, else false.
    bool is_exit;
    // Will hold the depth of the longest path of dep1 or dep2.
    int longest_depth;
};

int array_size;

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {

    struct CommandEntry* command_array = (struct CommandEntry*)malloc(numOfInsts*sizeof(struct CommandEntry));

    // We will hold an array pf 32 ints, to represent the registers, and the latest command that changed register i, will
    // be written to regs[i]. if regs[i] == -1, no one changed the register.
    int regs[32];
    for (int i = 0; i < 32; ++i) {
        regs[i] = -1;
    }
    array_size = numOfInsts;

    for (int i = 0; i < numOfInsts; ++i) {
        // Each command is assumed to be exit, else will be changed later.
        command_array[i].is_exit = true;
        // Init the command cycles.
        command_array[i].cycles = opsLatency[progTrace[i].opcode];
        // update dep1 based on who changed the reister it depends on latest.
        command_array[i].dep1 = regs[progTrace[i].src1Idx];
        // Initially, we assume each command has a depth of 0.
        command_array[i].longest_depth = 0;
        // If the some comand changed the register so the current command depends on it, we will update is_exit status,
        // and longest depth if needed.
        if(command_array[i].dep1 != -1){
            command_array[command_array[i].dep1].is_exit = false;
            int potential_longest_depth = command_array[command_array[i].dep1].longest_depth + command_array[command_array[i].dep1].cycles;
            if(potential_longest_depth > command_array[i].longest_depth){
                command_array[i].longest_depth = potential_longest_depth;
            }
        }
        // update dep2 based on who changed the reister it depends on latest.
        command_array[i].dep2 = regs[progTrace[i].src2Idx];
        // If the some comand changed the register so the current command depends on it, we will update is_exit status,
        // and longest depth if needed.
        if(command_array[i].dep2 != -1){
            command_array[command_array[i].dep2].is_exit = false;
            int potential_longest_depth = command_array[command_array[i].dep2].longest_depth + command_array[command_array[i].dep2].cycles;
            if(potential_longest_depth > command_array[i].longest_depth){
                command_array[i].longest_depth = potential_longest_depth;
            }
        }
        // We update regs based on the dstIdx of the current command.
        regs[progTrace[i].dstIdx] = i;
    }

    return (ProgCtx)command_array;
}

void freeProgCtx(ProgCtx ctx) {

    struct CommandEntry* command_array =  (struct CommandEntry*)ctx;
    free(command_array);

}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {

    struct CommandEntry* command_array =  (struct CommandEntry*)ctx;
    // We return the specific longest depth of the specified idx.
    return command_array[theInst].longest_depth;

}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {

    struct CommandEntry* command_array =  (struct CommandEntry*)ctx;

    if(theInst < 0 || theInst >= array_size){
        return -1;
    }

    // We return the specified idx deps.
    *src1DepInst = command_array[theInst].dep1;
    *src2DepInst = command_array[theInst].dep2;

    return 0;
}

int getProgDepth(ProgCtx ctx) {

    struct CommandEntry* command_array =  (struct CommandEntry*)ctx;
//    int array_size =  sizeof(command_array)/sizeof(struct CommandEntry);
    int max_depth = 0;

    // We go through each command that is an exit type, and return the largest depth + command cycles.
    for (int i = 0; i < array_size; ++i) {
        if(command_array[i].is_exit){
            int current_depth = command_array[i].longest_depth + command_array[i].cycles;
            if(current_depth > max_depth){
                max_depth = current_depth;
            }
        }
    }

    return max_depth;
}


