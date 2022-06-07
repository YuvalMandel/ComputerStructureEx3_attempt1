/* 046267 Computer Architecture - Winter 20/21 - HW #3               */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <stdbool.h>

struct CommandEntry {
    unsigned int cycles;
    unsigned int dep1;
    unsigned int dep2;
    bool is_exit;
};

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {

    struct CommandEntry* command_array = (struct CommandEntry*)malloc(numOfInsts*sizeof(struct CommandEntry));
    int regs[32] = {-1};

    for (int i = 0; i < numOfInsts; ++i) {
        command_array[i].is_exit = true;
        command_array[i].cycles = opsLatency[progTrace[i].opcode];
        command_array[i].dep1 = regs[progTrace[i].src1Idx];
        if(command_array[i].dep1 != -1){
            command_array[command_array[i].dep1].is_exit = false;
        }
        command_array[i].dep2 = regs[progTrace[i].src2Idx];
        if(command_array[i].dep2 != -1){
            command_array[command_array[i].dep2].is_exit = false;
        }
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
    int depth1 =  command_array[theInst].cycles, depth2 =  command_array[theInst].cycles;

    if(command_array[theInst].dep1 != -1){
        depth1 =  command_array[theInst].cycles + getInstDepth(ctx, command_array[theInst].dep1);
    }

    if(command_array[theInst].dep2 != -1){
        depth2 =  command_array[theInst].cycles + getInstDepth(ctx, command_array[theInst].dep2);
    }

    if(depth1 > depth2){
        return depth1;
    }else{
        return depth2;
    }
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {

    struct CommandEntry* command_array =  (struct CommandEntry*)ctx;

    if(theInst < 0 || theInst > (sizeof(command_array)/sizeof(struct CommandEntry))){
        return -1;
    }

    *src1DepInst = command_array[theInst].dep1;
    *src2DepInst = command_array[theInst].dep2;

    return 0;
}

int getProgDepth(ProgCtx ctx) {

    struct CommandEntry* command_array =  (struct CommandEntry*)ctx;
    int array_size =  sizeof(command_array)/sizeof(struct CommandEntry);
    int max_depth = 0;

    for (int i = 0; i < array_size; ++i) {
        if(command_array[i].is_exit){
            int current_depth = getInstDepth;
            if(current_depth > max_depth){
                max_depth = current_depth;
            }
        }
    }

    return max_depth;
}


