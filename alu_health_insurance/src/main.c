#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#include "mining.h"
#include "account.h"
#include "mempool.h"
#include "insurance_logic.h"

int main(int argc, char *argv[]) {
    printf("Initializing ALU Health Insurance Blockchain...\n");
    
    // Initialize components
    init_accounts();
    init_mempool();
    init_insurance_state();
    
    ChainState state = {0};
    init_mining_engine(&state);
    
    // Start Interactive Command CLI
    start_cli();
    
    return 0;
}
