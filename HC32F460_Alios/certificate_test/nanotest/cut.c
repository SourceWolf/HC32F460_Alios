/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include "cut.h"
#include <k_api.h>

struct cut_runtime cut;

static void filter(int argc, char** argv)
{
    int i = 0;
    struct cut_case *c = NULL;
    if(argc==2 && 0==strcmp(argv[1], "all"))
        return;

    for (i = 0; i < cut.ccnt_total; i++) {
        c = cut.clist[i];
        if ((argc==2 && (NULL==strstr(c->sname, argv[1]))) ||
            (argc==3 && (NULL==strstr(c->sname, argv[1]) || NULL==strstr(c->cname, argv[2])))) {
            cut.clist[i]->skip = 1;
            cut.ccnt_skip++;
        }
    }
}

static void usage(const char* me)
{
    cut_printf("Usage: %s [OPTION] [SUITE] [CASE]\r\n\r\n" \
    "OPTION:\r\n" \
    "  --help  : print this help\r\n" \
    "  --list  : list cases\r\n" \
    "  --count : print case count\r\n" \
    "    SUITE : suite name filter, e.g. '%s all' means run all suites\r\n" \
    "    CASE  : case name filter\r\n", me, me);
}

static int parse_arg(int argc, char** argv)
{
    if (argc >= 2) {
        if (0 == strcmp(argv[1], "--list")) {
            int i = 0;
            int cnt = 0;
            cut_printf("\33[1;34mCASE_LIST_BEGIN\33[0m\r\n");
            for(i=0; i<cut.ccnt_total; i++) {
                struct cut_case *c = cut.clist[i];
                if(argc==2 ||
                  (argc==3 && 0==strcmp(argv[2], "all")) ||
                  (argc==3 && NULL!=strstr(c->sname, argv[2])) ||
                  (argc==4 && NULL!=strstr(c->sname, argv[2]) && NULL!=strstr(c->cname, argv[3])))
                    cut_printf("\33[1;34m[%02d] %s.%s\33[0m\r\n", ++cnt, c->sname, c->cname);
            }
            cut_printf("\33[1;34mCASE_LIST_END\33[0m\r\n");
            return 0;
        }
        if (0 == strcmp(argv[1], "--count")) {
            cut_printf("total %d case(s).\r\n", cut.ccnt_total);
            return 0;
        }
        if (0 == strcmp(argv[1], "--help")) {
            usage(argv[0]);
            return 0;
        }
    }
    return 1;
}

static void cut_result_report(struct cut_runtime *cut)
{
    
#if 0
	  int i = 0;
    /* print test result locally */
    cut_printf("===========================================================================\r\n");
    if (cut->ccnt_fail > 0) {
        cut_printf("FAIL LIST:\r\n");
        for (i = 0; i < cut->ccnt_fail; i++) {
            cut_printf("  [%02d] %s\r\n", i + 1, cut->cerrmsg[i]);
            cut_free(cut->cerrmsg[i]);
        }
        cut_printf("---------------------------------------------------------------------------\r\n");
    }
#endif
    cut_printf("SUMMARY:\r\n" \
               "     TOTAL:    %d\r\n" \
               "   SKIPPED:    %d\r\n" \
               "   MATCHED:    %d\r\n" \
               "      PASS:    %d\r\n" \
               "    FAILED:    %d\r\n", cut->ccnt_total, cut->ccnt_skip,
               cut->ccnt_total-cut->ccnt_skip, cut->ccnt_pass, cut->ccnt_fail);
    cut_printf("===========================================================================\r\n");
}

void cut_get_stack_min_free(ktask_t *task)
{
    size_t stack_free_size = 0;
    ktask_t *test_task = NULL;

    test_task = task;

    if(NULL == test_task)
        test_task = krhino_cur_task_get();

    krhino_task_stack_min_free(test_task, &stack_free_size);
    //stack_free_size *= sizeof(cpu_stack_t);
    cut_printf("===========================================================================\r\n");
    cut_printf("###STACK_min_free### %s : %ld * cpu_stack_t\r\n", test_task->task_name, stack_free_size);
    cut_printf("===========================================================================\r\n");
    krhino_task_sleep(100);
    return;
}

static char *cut_int2str(int num, char *str)
{
    char index[] = "0123456789ABCDEF";
    unsigned int usnum = (unsigned int)num;

    str[7] = index[usnum % 16];
    usnum /= 16;
    str[6] = index[usnum % 16];
    usnum /= 16;
    str[5] = index[usnum % 16];
    usnum /= 16;
    str[4] = index[usnum % 16];
    usnum /= 16;
    str[3] = index[usnum % 16];
    usnum /= 16;
    str[2] = index[usnum % 16];
    usnum /= 16;
    str[1] = index[usnum % 16];
    usnum /= 16;
    str[0] = index[usnum % 16];
    usnum /= 16;

    return str;
}

#if (K_MM_STATISTIC > 0)
void cut_mm_overview()
{

    char s_heap_overview[]  =
        "      | 0x         | 0x         | 0x         | 0x         |\r\n";

    cut_printf("===========================================================================\r\n");
    cut_printf("[HEAP]| TotalSz    | FreeSz     | UsedSz     | MinFreeSz  |\r\n");

    cut_int2str((int)(g_kmm_head->free_size + g_kmm_head->used_size), &s_heap_overview[10]);
    cut_int2str((int)g_kmm_head->free_size, &s_heap_overview[23]);
    cut_int2str((int)g_kmm_head->used_size, &s_heap_overview[36]);
    cut_int2str((int)(g_kmm_head->free_size + g_kmm_head->used_size - g_kmm_head->maxused_size), &s_heap_overview[49]);
    cut_printf("%s\r\n",s_heap_overview);

    cut_printf("===========================================================================\r\n");
    return;
}
#endif


int cut_main(int argc, char** argv)
{
    int i = 0, cnt = 0;
    char *_argv[2] = {"None", "--list"};

    cut.ccnt_pass = 0;
    cut.ccnt_fail = 0;
    cut.ccnt_skip = 0;
    for(i=0; i<cut.ccnt_total; i++) {
        cut.clist[i]->skip = 0;
        cut.clist[i]->flag = 1;
    }

    if (0 == parse_arg(argc, argv))
        return 0;

    parse_arg(2, _argv);

    filter(argc, argv);

    for(i=0; i < cut.ccnt_total; i++) {
        cut.ccur = cut.clist[i];
        if (cut.ccur->skip)
            continue;

        cut_printf("\33[1;33mTEST [%d/%d] %s.%s...\33[0m\r\n",
                    ++cnt, cut.ccnt_total-cut.ccnt_skip, cut.ccur->sname, cut.ccur->cname);
        if (cut.ccur->setup)
            cut.ccur->setup(cut.ccur->data);
        cut.ccur->run((struct cut_case *)(cut.ccur->data));
        if (cut.ccur->teardown)
            cut.ccur->teardown(cut.ccur->data);
        if (cut.ccur->flag) {
            cut_printf("\33[1;32m[OK]\33[0m\r\n\r\n");
            cut.ccnt_pass++;
        }
        else {
            //cut_printf("\33[1;31m[FAIL]: %s\33[0m\r\n\r\n", cut.cerrmsg[cut.ccnt_fail]);
            cut_printf("\33[1;31m[FAIL]\r\n");
            cut.ccnt_fail++;
        }
    }
    cut_get_stack_min_free(NULL);

    #if (K_MM_STATISTIC > 0)
    cut_mm_overview();
    #endif

    cut_result_report(&cut);
    
    return cut.ccnt_fail;
}

