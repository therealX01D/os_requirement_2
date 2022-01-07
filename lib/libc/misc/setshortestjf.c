#include <unistd.h>
#include <stdio.h>
#include <lib.h>
#include <minix/config.h>
#include <errno.h>

#define S_ERROR -1
#define S_OK 0
/*T_8: helper function to int setshortestjf(int exp_time)*/
int failwith(int stat) { //T_8: if everything is all RETURN S_OK =0 else return -1 as an error
	if (stat == 0) return S_OK;
	else if (stat < 0) stat = -stat; // T_8: absolute value of error
	errno = (_SIGN stat);//T_8: adding sign to value - and saving it to general error variable
	return S_ERROR;
}
/*T_8: sanity check where expected time doesnt exceed permissioned range */

int setshortestjf(int exp_time) { 
	message m;
	m.m1_i1 = exp_time;//T_8: setting message parameter to exp_time
	if (exp_time < SHORTESTJF_MIN_EXP_TIME || SHORTESTJF_MAX_EXP_TIME < exp_time) {//T_8: check for exp_time
		return failwith(EINVAL); //T_8: argument out of range
	}
	return failwith(_syscall(PM_PROC_NR, PM_SETSHORTESTJF, &m));  // T_8: in case something else fails return S_ERROR
}
