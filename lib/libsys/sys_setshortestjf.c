#include "syslib.h"

int sys_setshortestjf(endpoint_t proc_ep, int exp_time) { /* sjf_2018 */
    message m;

    m.m1_i1 = proc_ep;
    m.m1_i2 = exp_time;
    /*inserting process into message*/
    return(_kernel_call(SYS_SETSHORTESTJF, &m));
}