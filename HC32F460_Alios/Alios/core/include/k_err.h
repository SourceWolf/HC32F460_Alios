/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef K_ERR_H
#define K_ERR_H

typedef enum {
    RHINO_SUCCESS                      = 0u,
    RHINO_SYS_FATAL_ERR,
    RHINO_SYS_SP_ERR,
    RHINO_RUNNING,
    RHINO_STOPPED,
    RHINO_INV_PARAM,
    RHINO_NULL_PTR,
    RHINO_INV_ALIGN,
    RHINO_KOBJ_TYPE_ERR,
    RHINO_KOBJ_DEL_ERR,
    RHINO_KOBJ_DOCKER_EXIST,
    RHINO_KOBJ_BLK,
    RHINO_KOBJ_SET_FULL,
    RHINO_NOTIFY_FUNC_EXIST,

    RHINO_MM_POOL_SIZE_ERR             = 100u,
    RHINO_MM_ALLOC_SIZE_ERR,
    RHINO_MM_FREE_ADDR_ERR,
    RHINO_MM_CORRUPT_ERR,
    RHINO_DYN_MEM_PROC_ERR,
    RHINO_NO_MEM,
    RHINO_RINGBUF_FULL,
    RHINO_RINGBUF_EMPTY,

    RHINO_SCHED_DISABLE                = 200u,
    RHINO_SCHED_ALREADY_ENABLED,
    RHINO_SCHED_LOCK_COUNT_OVF,
    RHINO_INV_SCHED_WAY,

    RHINO_TASK_INV_STACK_SIZE          = 300u,
    RHINO_TASK_NOT_SUSPENDED,
    RHINO_TASK_DEL_NOT_ALLOWED,
    RHINO_TASK_SUSPEND_NOT_ALLOWED,
    RHINO_SUSPENDED_COUNT_OVF,
    RHINO_BEYOND_MAX_PRI,
    RHINO_PRI_CHG_NOT_ALLOWED,
    RHINO_INV_TASK_STATE,
    RHINO_IDLE_TASK_EXIST,

    RHINO_NO_PEND_WAIT                 = 400u,
    RHINO_BLK_ABORT,
    RHINO_BLK_TIMEOUT,
    RHINO_BLK_DEL,
    RHINO_BLK_INV_STATE,
    RHINO_BLK_POOL_SIZE_ERR,

    RHINO_TIMER_STATE_INV              = 500u,

    RHINO_NO_THIS_EVENT_OPT            = 600u,

    RHINO_BUF_QUEUE_INV_SIZE           = 700u,
    RHINO_BUF_QUEUE_SIZE_ZERO,
    RHINO_BUF_QUEUE_FULL,
    RHINO_BUF_QUEUE_MSG_SIZE_OVERFLOW,
    RHINO_QUEUE_FULL,
    RHINO_QUEUE_NOT_FULL,

    RHINO_SEM_OVF                      = 800u,
    RHINO_SEM_TASK_WAITING,

    RHINO_MUTEX_NOT_RELEASED_BY_OWNER  = 900u,
    RHINO_MUTEX_OWNER_NESTED,
    RHINO_MUTEX_NESTED_OVF,

    RHINO_NOT_CALLED_BY_INTRPT         = 1000u,
    RHINO_TRY_AGAIN,

    RHINO_WORKQUEUE_EXIST              = 1100u,
    RHINO_WORKQUEUE_NOT_EXIST,
    RHINO_WORKQUEUE_WORK_EXIST,
    RHINO_WORKQUEUE_BUSY,
    RHINO_WORKQUEUE_WORK_RUNNING,

    RHINO_TASK_STACK_OVF               = 1200u,
    RHINO_INTRPT_STACK_OVF
} kstat_t;

typedef void (*krhino_err_proc_t)(kstat_t err);
extern krhino_err_proc_t g_err_proc;

/**
 * convert int to ascii(HEX)
 * while using format % in libc, malloc/free is involved.
 * this function avoid using malloc/free. so it works when heap corrupt.
 * @param[in]   num      number
 * @param[in]   str      fix 8 character str
 * @return  str
 */
char *k_int2str(int num, char *str);

/**
 * call g_err_proc
 * @param[in]   err      error_id
 * @return      void
 */
void k_err_proc(kstat_t err);


#endif /* K_ERR_H */

