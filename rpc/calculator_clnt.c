/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "calculator.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
addition_1(argp, clnt)
	intpair *argp;
	CLIENT *clnt;
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, ADDITION, xdr_intpair, argp, xdr_int, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}

int *
substraction_1(argp, clnt)
	intpair *argp;
	CLIENT *clnt;
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, SUBSTRACTION, xdr_intpair, argp, xdr_int, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}

int *
multiplication_1(argp, clnt)
	intpair *argp;
	CLIENT *clnt;
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, MULTIPLICATION, xdr_intpair, argp, xdr_int, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}

double *
division_1(argp, clnt)
	intpair *argp;
	CLIENT *clnt;
{
	static double clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, DIVISION, xdr_intpair, argp, xdr_double, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}
