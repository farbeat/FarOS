/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* ---------------------------------------------------------------------------
Module Name:
    fsm.h
Abstract:
    This file includes the definition of finite state machine functions.
--------------------------------------------------------------------------- */
#ifndef FSM_H
#define FSM_H

#include "bth_conf.h"
#include "datatype.h"

#define MAX_INST_NAME_LEN        7
#define MAX_TIMER_NAME_LEN        7
#define FSM_RAND_BITS_LEN 48

/* Finite state machine instance */
struct fsm_inst {                /* fsm instance */
    void *user_data;            /* User specific data for the fsm */
    struct fsm *fsm;            /* The fsm entity for this fsm instance. */
    uint8_t state;                /* The Current state for the fsm. */
};

typedef void (fsm_func) (const struct fsm_inst *, uint8_t, void *);

/* Entry to fill the jump matrix for FSM */
struct fsm_node {        /* fsm node */
    uint8_t state;
    uint8_t event;        /* state and event */
    fsm_func *routine;
};

/* Finite state machine */
struct fsm {                    /* fsm entity */
    uint8_t *st;                    /* St offest table */
    uint8_t *it;                    /* Ev->Index Table */
    fsm_func **func_array;        /* Function array */
    fsm_func *default_func;        /* The entity of the default function */
    uint8_t ref_count;            /* Number of fsm instances that use this fsm. */
};

struct event {                    /* event for the FSM */
    struct fsm_inst *fi;         /* Correspond fsm Instance */
    void *arg;                    /* event Argument. */
    fsm_func *func;                /* "no status need to be checked" eg: if this is a control fsm func */
    fsm_func *freefunc;            /* Use to free arg */
    uint8_t event;                /* event ID. */
};

enum { /* mask value of s_block_ev */
    SCH_READY = 0x01,            /* quit block following event/timer */
};

/* FSM functions */
#define fsm_change_state(fi_, newstate_)                     ((fi_)->state = (newstate_))

/* Internal */
struct fsm *fsm_new_func(const struct fsm_node *fnlist, fsm_func *def_func, uint8_t fncount, uint8_t state_count);
#define fsm_new(list, fn, st, ev, func)    fsm_new_func((list), (func), (fn), (st))
struct fsm *fsm_dup(struct fsm *fsm);
struct fsm_inst *fsm_inst_new_(void *user_data); /* Internal */

#ifdef CONFIG_MEMORY_LEAK_DETECT
#define fsm_inst_new(ud, name) dbg_memory_leak_new(fsm_inst_new_(ud), 0, sizeof(struct fsm_inst), MEMORY_LEAK_POSINFO)
#else
#define fsm_inst_new(ud, name)    fsm_inst_new_(ud)
#endif

void fsm_event_func_internal(struct fsm_inst *fi, fsm_func *func, fsm_func *freefunc, void *arg, uint8_t event);
void fsm_event_func_external(struct fsm_inst *fi, fsm_func *func, fsm_func *freefunc, void *arg, uint8_t event);

/* FSM functions UI interfaces */
/* Internal of SCH thread */
void fsm_free(const struct fsm_inst *fi, uint8_t event, void *arg);
void fsm_inst_free(struct fsm_inst *fi);
void fsm_direct_event(struct fsm_inst *fi, uint8_t event, void *arg);

#if 1 /* For New */
#define fsm_event_kx(fi_, ev_, func_, arg_)                 fsm_event_func_internal((fi_), (fsm_func *)(func_), \
    (fsm_func *)(func_), (arg_), (ev_))
/* event not care */
#define fsm_event_fx(fi_, func_, arg_)                     fsm_event_func_internal((fi_), (fsm_func *)(func_), \
    (fsm_func *)(func_), (arg_), 1)
/* event not care */
#define fsm_event_tx(fi_, func_, arg_) fsm_event_func_internal((fi_), (fsm_func *)(func_), fsm_free, (arg_), 1)
#define fsm_event_cx(fi_, ev_, func_, arg_) fsm_event_func_internal((fi_), (fsm_func *)(func_), fsm_free, (arg_), (ev_))
#define fsm_event(fi_, ev_, arg_)                         fsm_event_func_internal((fi_), NULL, fsm_free, (arg_), (ev_))
#else /* For old applications, called outside of SCH */
#define fsm_event_kx(fi_, ev_, func_, arg_)                 fsm_event_func_external((fi_), (func_), (func_), (arg_), \
    (ev_))
/* event not care */
#define fsm_event_fx(fi_, func_, arg_)                     fsm_event_func_external((fi_), (func_), (func_), (arg_), 1)
/* event not care */
#define fsm_event_tx(fi_, func_, arg_)                     fsm_event_func_external((fi_), (func_), fsm_free, (arg_), 1)
#define fsm_event_cx(fi_, ev_, func_, arg_)                 fsm_event_func_external((fi_), (func_), fsm_free, (arg_), \
    (ev_))
#define fsm_event(fi_, ev_, arg_)                         fsm_event_func_external((fi_), NULL, fsm_free, (arg_), (ev_))
#endif

/* External of SCH thread */
/* Schloop is going done */
#define doneloop()                                         fsm_event_func_external(NULL, NULL, NULL, NULL, 0)
#define fsm_event_external_kx(fi_, ev_, func_, arg_)         fsm_event_func_external((fi_), (fsm_func *)(func_), \
    (fsm_func *)(func_), (arg_), (ev_))
/* event not care */
#define fsm_event_external_fx(fi_, func_, arg_)             fsm_event_func_external((fi_), (fsm_func *)(func_), \
    (fsm_func *)(func_), (arg_), 1)
#define fsm_event_external_tx(fi_, func_, arg_) fsm_event_func_external((fi_), (fsm_func *)(func_), fsm_free, (arg_), 1)
#define fsm_event_external_cx(fi_, ev_, func_, arg_) \
    fsm_event_func_external((fi_), (fsm_func *)(func_), fsm_free, (arg_), (ev_))
#define fsm_event_external(fi_, ev_, arg_)                 fsm_event_func_external((fi_), NULL, fsm_free, (arg_), (ev_))

/* Timer structure for UI */
struct fsm_timer { /* a number, TIMER_ID, user visit as void *, this is forbidden to visit */
    void *forbidden2visit; /* This is a void *, for historial reason */
};

/* Timer structure internal used, forbidden for timer user */
struct fsm_timer_stru {        /* Timer in the fsm instance */
    uint32_t magic_num;
#if (BTH_WITH_SMART_WEAR == NO)
    void *magic_arg;
#endif
    uint32_t u_id;             /* User defined id to index the timer */
    struct fsm_inst *fi;     /* Correspond fsm Instance */
    fsm_func *func;            /* Default Timer expire function */
    void *arg;                /* Argument for the event. */
    TIMER_ID timer_id;        /* System timer id */
    uint8_t event;            /* event when time out. */
#ifdef CONFIG_DBG_TIMER
    uint32_t millisec;         /* Timeout value */
    char name[MAX_TIMER_NAME_LEN]; /* Name for the timer */
#endif
};

/* Timer functions */
typedef void (UserTimerFunc)(uint32_t param);
/* DBG */
struct fsm_timer *timer_restart(struct fsm_timer *ft, struct fsm_inst *fi, uint32_t millisec,
    uint8_t event, fsm_func *func,  void *arg, void *name, uint32_t u_id);
struct fsm_timer *fsm_timer_add(struct fsm_inst *fi, uint32_t millisec, uint8_t event, fsm_func *func,
    void *arg, const char *name, uint32_t u_id);
/* Remaining timeout */
void timer_expire(TIMER_ID timer_id, uint32_t millisec);
uint8_t *timer_del_by_uid(const struct fsm_inst *fi, uint32_t u_id);
uint8_t *timer_del_byp(struct fsm_timer *ft);
void timer_del_by_p_free(struct fsm_timer **ft);

#ifdef CONFIG_OS_LITEOS
/* DBG */
struct fsm_timer *timer_restart_lite(struct fsm_timer *ft, struct fsm_inst *fi, uint32_t millisec, uint8_t event,
    fsm_func *func, void *arg, void *name, uint32_t u_id);
struct fsm_timer *timer_add_lite(struct fsm_inst *fi, uint32_t millisec, uint8_t event, fsm_func *func, void *arg,
    const char *name, uint32_t u_id);
uint8_t *timer_del_lite(struct fsm_timer *in_ft);
void timer_del_free_lite(struct fsm_timer **ft);

/* Timer UI interface functions */
#define fsm_add_timer(fi_, millisec_, ev_, arg_, name_, u_id_) timer_add_lite((fi_), (millisec_), (ev_), NULL, \
    (arg_), (name_), (u_id_))
#define fsm_add_timer_cx(fi_, millisec_, ev_, func_, arg_, name_, u_id_)     timer_add_lite((fi_), (millisec_), (ev_), \
    (fsm_func *)(func_), (arg_), (name_), (u_id_))
#define FSM_DEL_TIMER_2                                                     timer_del_lite
#define FSM_DEL_TIMER_1                                                     timer_del_free_lite
#define fsm_restart_timer_1(ft_, fi_, ms_, ev_, func_, arg_, name_, u_id_) \
    timer_restart_lite((ft_), (fi_), (ms_), (ev_), \
    (fsm_func *)(func_), (arg_), (name_), (u_id_))
#elif defined(CONFIG_OS_WIN32)
/* Timer UI interface functions */
#define fsm_add_timer(fi_, millisec_, ev_, arg_, name_, u_id_) fsm_timer_add((fi_), (millisec_), (ev_), NULL, (arg_), \
    (name_), (u_id_))
#define fsm_add_timer_cx(fi_, millisec_, ev_, func_, arg_, name_, u_id_)     fsm_timer_add((fi_), (millisec_), (ev_), \
    (fsm_func *)(func_), (arg_), (name_), (u_id_))
#define FSM_EXPIRE_TIMER                                                    timer_expire
#define FSM_DEL_TIMER_0                                                     timer_del_by_uid
#define FSM_DEL_TIMER_2                                                     timer_del_byp
#define FSM_DEL_TIMER_1                                                     timer_del_by_p_free
#define fsm_restart_timer_1(ft_, fi_, ms_, ev_, func_, arg_, name_, u_id_) timer_restart((ft_), (fi_), (ms_), (ev_), \
    (fsm_func *)(func_), (arg_), (name_), (u_id_))
#else
/* define nothing */
#endif
void init_schedule(void);
void schedule_loop(void);

/* -----------------------------------------------------------------------------
Description:
    Federal Information Processing Standards.
    Core spec 4.0 page 1061/1114,
    A device shall use a pseudo random number generator compliant with [FIPS PUB 140-2]
    An example of a possible random generator is provided in [FIPS PUB 186-2] Appendix 3.1
    which can be used by replacing SHA-1 function with the SHA-256 function

Description:
    Pseudorandom Number Generators - Blum Blum Shub.
Prime Numbers:
    2��3��5��7��11��13��17��19��23��29��31��37��41��43��47��53��59��61��67��71��
    73��79��83��89��97�� 101��103��107��109��113��127��131��137��139��149��151��
    157��163��167��173��179��181��191��193��197��199��211��223��227��229��233��
    239��241��251��257��263��269��271
----------------------------------------------------------------------------- */
#define fsm_random(x_)                (fsm_get_random(0) % (x_))
#define FSM_BBSRAND_N                  (7687 * 1061) /* Random selected Prime */

void fsm_rand(uint8_t *out, uint8_t bits);
void fsm_set_random(uint32_t s);
uint32_t fsm_get_random(uint32_t s);
void schedule_loop_cleanup(void);

typedef void (FsmSetRandomCb)(uint32_t s);
typedef uint32_t (*FsmRandomCb)(uint32_t s);
typedef uint32_t (FsmRandomCallback)(uint32_t s);

#endif /* FSM_H */

