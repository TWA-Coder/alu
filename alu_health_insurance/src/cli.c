#include "cli.h"
#include "blockchain.h"
#include "account.h"
#include "mempool.h"
#include "mining.h"
#include "insurance_logic.h"
#include "persistence.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void execute_command(const char *cmd, char *args) {
    if (strcmp(cmd, "help") == 0) {
        printf("Available commands:\n");
        printf("  register_member <id> <balance>\n");
        printf("  enroll_policy <member_id> <policy_id> <plan>\n");
        printf("  pay_premium <member_id> <amount>\n");
        printf("  submit_claim <provider_id> <policy_id> <amount>\n");
        printf("  mine_solo <miner_id>\n");
        printf("  blockchain_verify\n");
        printf("  chain_save <file>\n");
        printf("  chain_load <file>\n");
        printf("  exit\n");
    } else if (strcmp(cmd, "register_member") == 0) {
        char id[135];
        uint64_t bal;
        if (sscanf(args, "%134s %llu", id, &bal) == 2) {
            if (create_account(id, bal)) printf("Member %s registered.\n", id);
        } else printf("Usage: register_member <id> <balance>\n");
    } else if (strcmp(cmd, "enroll_policy") == 0) {
        char mid[135], pid[64], plan[32];
        if (sscanf(args, "%134s %63s %31s", mid, pid, plan) == 3) {
            enroll_policy(mid, pid, plan);
            printf("Policy %s enrolled for member %s.\n", pid, mid);
        } else printf("Usage: enroll_policy <member_id> <policy_id> <plan>\n");
    } else if (strcmp(cmd, "mine_solo") == 0) {
        char miner[135];
        if (sscanf(args, "%134s", miner) == 1) {
            Block *b = mine_solo(miner);
            if (b) {
                printf("Mined Block! ID: %s, Tx Count: %d\n", b->block_id, b->transaction_count);
                free(b);
            } else {
                printf("Nothing to mine.\n");
            }
        } else printf("Usage: mine_solo <miner_id>\n");
    } else if (strcmp(cmd, "blockchain_verify") == 0) {
        verify_chain();
    } else if (strcmp(cmd, "chain_save") == 0) {
        char file[64];
        if (sscanf(args, "%63s", file) == 1) save_chain(file);
    } else if (strcmp(cmd, "chain_load") == 0) {
        char file[64];
        if (sscanf(args, "%63s", file) == 1) load_chain(file);
    } else if (strcmp(cmd, "pay_premium") == 0) {
        char mid[135];
        uint64_t amount;
        if (sscanf(args, "%134s %llu", mid, &amount) == 2) {
            printf("Premium of %llu paid effectively for member %s (Demo).\n", amount, mid);
        } else {
            printf("Usage: pay_premium <member_id> <amount>\n");
        }
    } else if (strcmp(cmd, "submit_claim") == 0) {
        char provider[135], policy[64];
        uint64_t amount;
        if (sscanf(args, "%134s %63s %llu", provider, policy, &amount) == 3) {
            printf("Claim of %llu submitted effectively by provider %s for policy %s (Demo).\n", amount, provider, policy);
        } else {
            printf("Usage: submit_claim <provider_id> <policy_id> <amount>\n");
        }
    } else {
        printf("Unknown command: %s (Type 'help' to see valid commands)\n", cmd);
    }
}

void start_cli() {
    char line[512];
    printf("==================================================\n");
    printf("ALU Health Insurance Blockchain CLI\n");
    printf("Type 'help' to view available commands.\n");
    printf("==================================================\n");
    while (1) {
        printf("ALU-Block> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\r\n")] = 0;
        
        char *cmd = strtok(line, " ");
        if (!cmd) continue;
        if (strcmp(cmd, "exit") == 0) break;
        
        char *args = strtok(NULL, ""); 
        if (!args) args = "";
        
        execute_command(cmd, args);
    }
}
