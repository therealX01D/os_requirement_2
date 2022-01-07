#include "kernel/system.h"
#include <stdio.h>
#include <minix/endpoint.h>
#include <minix/config.h>
#include "kernel/clock.h"
#include <errno.h>

/*===========================================================================*
 *				do_setshortestjobfirst	T_8: control the queues process execution with the new added queue			     *
 *===========================================================================*/
int do_setshortestjf(struct proc* caller, message* m_ptr) { 
	struct proc* p;
	int proc_nr, new_priority = SHORTESTJF_Q,         
		new_quantum = SHORTESTJF_QUANTUM, cpu = -1;//T_8: setting priority to be in schedule queue of sjf
	int result, expected_time = m_ptr->m1_i2; //T_8: added in message from sys_setshortestjf in kernel/system/sys_shortestjf.c

	if (expected_time < SHORTESTJF_MIN_EXP_TIME || SHORTESTJF_MAX_EXP_TIME < expected_time)//T_8: double check
		return EINVAL;

	if (!isokendpt(m_ptr->m1_i1, &proc_nr)) {//T_8: check endpoint of process
		printf("endpoint not ok\n");
		return EINVAL;
	}

	p = proc_addr(proc_nr);//T_8: add it to proc addr


	if (expected_time == SHORTESTJF_RESET_POLICY) { //T_8: restore scheduling policy to non-SJF default
		if (p->p_priority != SHORTESTJF_Q) {
			return EPERM;  // process is already scheduled with default method
		}
		if (p->p_scheduler) {  // SCHED manages this process
			new_priority = USER_Q;
		}
		else {  // no scheduler - managed by kernel
			new_priority = TASK_Q;
		}
		new_quantum = USER_QUANTUM;
	}

	else { // switch to scheduling with SJF
		if (p->p_priority == SHORTESTJF_Q) {
			return EPERM;  // process already scheduled with SJF method
		}
	}

	p->expected_time = expected_time;

	result = sched_proc(p, new_priority, new_quantum, cpu);
	if (result != OK) return result;
	return -(_SIGN new_priority);  // return with sign other that the one for returning errors
}