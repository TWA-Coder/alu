#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ═══════════════════════════════════════════════════════════════
 *  Global state
 * ═══════════════════════════════════════════════════════════════ */
student_t    g_students[MAX_STUDENTS];
int          g_student_count = 0;
chain_node_t *g_chain        = NULL;
pool_node_t  *g_pool         = NULL;
utxo_t       *g_utxo_set     = NULL;
account_t    *g_accounts     = NULL;
tx_model_t    g_model        = MODEL_UTXO;   /* default: UTXO */
int           g_difficulty   = DEFAULT_DIFF;
uint32_t      g_chain_length = 0;

/* ═══════════════════════════════════════════════════════════════
 *  Utility helpers
 * ═══════════════════════════════════════════════════════════════ */

/**
 * clear_input - Flushes remaining characters from stdin after a read.
 */
static void clear_input(void)
{
	int c;

	while ((c = getchar()) != '\n' && c != EOF)
		;
}

/**
 * read_line - Reads a line from stdin into buf (max len-1 chars).
 *   Strips trailing newline.
 */
static void read_line(char *buf, size_t len)
{
	if (fgets(buf, (int)len, stdin))
	{
		buf[strcspn(buf, "\n")] = '\0';
	}
}

/**
 * find_student - Finds a registered student by ID.
 * Return: Pointer to student_t or NULL.
 */
static student_t *find_student(const char *id)
{
	int i;

	for (i = 0; i < g_student_count; i++)
		if (strcmp(g_students[i].id, id) == 0)
			return (&g_students[i]);
	return (NULL);
}

/**
 * model_name - Returns a human-readable model name.
 */
static const char *model_name(void)
{
	return (g_model == MODEL_UTXO) ? "UTXO" : "Account-Based";
}

/* ═══════════════════════════════════════════════════════════════
 *  Menu handlers
 * ═══════════════════════════════════════════════════════════════ */

/** 1. Register a student */
static void menu_register_student(void)
{
	student_t *s;

	if (g_student_count >= MAX_STUDENTS)
	{
		printf("  [Register] Registry full (%d students).\n", MAX_STUDENTS);
		return;
	}

	s = &g_students[g_student_count];
	memset(s, 0, sizeof(*s));

	printf("  Student ID   : ");
	read_line(s->id, STUDENT_ID_LEN);
	if (strlen(s->id) == 0)
	{
		printf("  Aborted.\n");
		return;
	}
	if (find_student(s->id) && find_student(s->id) != s)
	{
		printf("  [Register] Student ID '%s' already exists.\n", s->id);
		return;
	}

	printf("  Full Name    : ");
	read_line(s->name, NAME_LEN);

	/* Generate EC key pair for this student */
	s->key = ec_create();
	if (!s->key)
		printf("  Warning: EC key generation failed for %s.\n", s->id);

	g_student_count++;
	printf("  ✓ Student '%s' (%s) registered. EC key pair generated.\n",
		s->name, s->id);
}

/**
 * pool_count - Counts nodes in the pending pool.
 * Return: Number of blocks currently in the pool.
 */
static uint32_t pool_count(void)
{
	pool_node_t *p   = g_pool;
	uint32_t     cnt = 0;

	while (p) { cnt++; p = p->next; }
	return (cnt);
}

/** 2. Mark attendance */
static void menu_mark_attendance(void)
{
	char          sid[STUDENT_ID_LEN];
	char          course[COURSE_LEN];
	char          status_str[16];
	attendance_t  status;
	student_t    *student;
	block_t       block;
	uint8_t       prev_hash[SHA256_DIGEST_LENGTH];
	chain_node_t *last;

	printf("  Student ID : ");
	read_line(sid, STUDENT_ID_LEN);

	student = find_student(sid);
	if (!student)
	{
		printf("  [Attendance] Student '%s' not found. Register first.\n", sid);
		return;
	}

	printf("  Course     : ");
	read_line(course, COURSE_LEN);

	printf("  Status [present/late/absent]: ");
	read_line(status_str, sizeof(status_str));

	if (strcasecmp(status_str, "present") == 0)
		status = PRESENT;
	else if (strcasecmp(status_str, "late") == 0)
		status = LATE;
	else if (strcasecmp(status_str, "absent") == 0)
		status = ABSENT;
	else
	{
		printf("  Invalid status. Use: present / late / absent\n");
		return;
	}

	/* Get previous hash */
	if (g_pool)
	{
		/* Use the last pending block's hash as prev_hash */
		pool_node_t *p = g_pool;

		while (p->next)
			p = p->next;
		memcpy(prev_hash, p->block.hash, SHA256_DIGEST_LENGTH);
	}
	else
	{
		last = blockchain_last();
		if (last)
			memcpy(prev_hash, last->block.hash, SHA256_DIGEST_LENGTH);
		else
			memset(prev_hash, 0, SHA256_DIGEST_LENGTH);
	}

	block = block_create(student, course, status, prev_hash,
			g_chain_length + pool_count());

	pool_add(&block);
	printf("  ✓ Attendance recorded → PENDING pool | Reward: %u coins\n",
		block.token_reward);
	if (status == ABSENT)
		printf("  (ABSENT: no transaction will be created)\n");
}

/** 3. View pending pool */
static void menu_view_pool(void)
{
	pool_print();
}

/** 4. Mine pending blocks */
static void menu_mine(void)
{
	char   choice[8];
	int    num_miners;
	int    rounds;
	uint32_t rental_fee;

	printf("  Mining mode:\n");
	printf("    a) Solo\n");
	printf("    b) Pool\n");
	printf("    c) Cloud\n");
	printf("  Choice: ");
	read_line(choice, sizeof(choice));

	switch (choice[0])
	{
	case 'a': case 'A':
		mine_solo(g_difficulty);
		break;

	case 'b': case 'B':
		printf("  Number of miners (2–%d): ", MAX_POOL_MINERS);
		if (scanf("%d", &num_miners) != 1) num_miners = 3;
		clear_input();
		mine_pool(g_difficulty, num_miners);
		break;

	case 'c': case 'C':
		printf("  Rental duration in rounds (1–5): ");
		if (scanf("%d", &rounds) != 1) rounds = 3;
		clear_input();
		printf("  Rental fee per round (coins): ");
		if (scanf("%u", &rental_fee) != 1) rental_fee = 5;
		clear_input();
		mine_cloud(g_difficulty, rounds, rental_fee);
		break;

	default:
		printf("  Invalid choice.\n");
	}
}

/** 5. View confirmed blockchain */
static void menu_view_chain(void)
{
	blockchain_print();
}

/** 6. View balances / UTXO set */
static void menu_view_balances(void)
{
	if (g_model == MODEL_UTXO)
		utxo_print_all();
	else
		account_print_balances();
}

/** 7. Transfer tokens between students */
static void menu_transfer(void)
{
	char     sender[STUDENT_ID_LEN];
	char     recipient[STUDENT_ID_LEN];
	uint32_t amount;
	uint32_t nonce;

	printf("  Sender student ID   : ");
	read_line(sender, STUDENT_ID_LEN);
	printf("  Recipient student ID: ");
	read_line(recipient, STUDENT_ID_LEN);
	printf("  Amount (coins)      : ");
	if (scanf("%u", &amount) != 1) { clear_input(); return; }
	clear_input();

	if (g_model == MODEL_UTXO)
	{
		utxo_transfer(sender, recipient, amount);
	}
	else
	{
		printf("  Sender nonce        : ");
		if (scanf("%u", &nonce) != 1) { clear_input(); return; }
		clear_input();
		account_transfer(sender, recipient, amount, nonce);
	}
}

/** 8. Print tx history (account model only) */
static void menu_tx_history(void)
{
	char sid[STUDENT_ID_LEN];

	if (g_model != MODEL_ACCOUNT)
	{
		printf("  Transaction history is only available in Account-Based model.\n");
		return;
	}
	printf("  Student ID: ");
	read_line(sid, STUDENT_ID_LEN);
	account_print_history(sid);
}

/** 9. Switch transaction model */
static void menu_switch_model(void)
{
	printf("  Current model: %s\n", model_name());
	printf("  Switch to: [1] UTXO  [2] Account-Based: ");
	char c[4];

	read_line(c, sizeof(c));
	if (c[0] == '1')
	{
		g_model = MODEL_UTXO;
		printf("  ✓ Switched to UTXO model.\n");
	}
	else if (c[0] == '2')
	{
		g_model = MODEL_ACCOUNT;
		printf("  ✓ Switched to Account-Based model.\n");
	}
	else
		printf("  Invalid choice.\n");
}

/** 10. Set mining difficulty */
static void menu_set_difficulty(void)
{
	int d;

	printf("  Current difficulty: %d (leading zero hex chars, 1–4)\n",
		g_difficulty);
	printf("  New difficulty (%d–%d): ", MIN_DIFFICULTY, MAX_DIFFICULTY);
	if (scanf("%d", &d) != 1) { clear_input(); return; }
	clear_input();

	if (d < MIN_DIFFICULTY || d > MAX_DIFFICULTY)
	{
		printf("  Invalid difficulty. Must be %d–%d.\n",
			MIN_DIFFICULTY, MAX_DIFFICULTY);
		return;
	}
	g_difficulty = d;
	printf("  ✓ Mining difficulty set to %d.\n", d);
}

/* ═══════════════════════════════════════════════════════════════
 *  Main menu loop
 * ═══════════════════════════════════════════════════════════════ */

static void print_menu(void)
{
	printf("\n╔══════════════════════════════════════════════════════════════╗\n");
	printf("║       Blockchain Attendance System — Assignment 2            ║\n");
	printf("╠══════════════════════════════════════════════════════════════╣\n");
	printf("║  Model: %-10s    Difficulty: %d    Students: %d%-10s║\n",
		model_name(), g_difficulty, g_student_count, " ");
	printf("╠══════════════════════════════════════════════════════════════╣\n");
	printf("║  1.  Register student                                        ║\n");
	printf("║  2.  Mark attendance                                         ║\n");
	printf("║  3.  View pending pool                                       ║\n");
	printf("║  4.  Mine pending blocks (solo / pool / cloud)               ║\n");
	printf("║  5.  View confirmed blockchain                               ║\n");
	printf("║  6.  View balances / UTXO set                                ║\n");
	printf("║  7.  Transfer tokens                                         ║\n");
	printf("║  8.  View transaction history  [Account model]               ║\n");
	printf("║  9.  Switch transaction model                                ║\n");
	printf("║  10. Set mining difficulty (1–4)                             ║\n");
	printf("║  0.  Exit                                                    ║\n");
	printf("╚══════════════════════════════════════════════════════════════╝\n");
	printf("  Choice: ");
}

/**
 * main - Entry point.
 *   Supports optional command-line argument for initial difficulty:
 *     ./blockchain2 [difficulty]
 */
int main(int argc, char *argv[])
{
	char input[8];
	int  choice;

	/* Parse optional difficulty argument */
	if (argc >= 2)
	{
		int d = atoi(argv[1]);

		if (d >= MIN_DIFFICULTY && d <= MAX_DIFFICULTY)
			g_difficulty = d;
		else
			fprintf(stderr,
				"Warning: invalid difficulty '%s', using default %d\n",
				argv[1], DEFAULT_DIFF);
	}

	/* Seed RNG for pool mining */
	srand((unsigned int)time(NULL));

	/* Initialise blockchain with genesis block */
	blockchain_init();

	printf("\n  Blockchain Attendance System — Assignment 2\n");
	printf("  Transaction Model: %s | Difficulty: %d\n\n",
		model_name(), g_difficulty);

	while (1)
	{
		print_menu();
		read_line(input, sizeof(input));
		choice = atoi(input);

		switch (choice)
		{
		case 1:  menu_register_student(); break;
		case 2:  menu_mark_attendance();  break;
		case 3:  menu_view_pool();        break;
		case 4:  menu_mine();             break;
		case 5:  menu_view_chain();       break;
		case 6:  menu_view_balances();    break;
		case 7:  menu_transfer();         break;
		case 8:  menu_tx_history();       break;
		case 9:  menu_switch_model();     break;
		case 10: menu_set_difficulty();   break;
		case 0:
			printf("\n  Cleaning up and exiting...\n");
			blockchain_free();
			pool_free();
			utxo_free();
			accounts_free();
			/* Free EC keys */
			{
				int i;

				for (i = 0; i < g_student_count; i++)
					if (g_students[i].key)
						EC_KEY_free(g_students[i].key);
			}
			printf("  Goodbye.\n\n");
			return (0);
		default:
			printf("  Unknown option '%s'.\n", input);
		}
	}
}
