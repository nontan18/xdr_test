#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h> /* XDR は RPC のサブライブラリ */
#include <string.h>

#include "work.h"
#include "sshpopen.h"

int main()       /* writer.c */
{
  FILE *ssh_r;
  FILE *ssh_w;
  int pid = sshpopen2("/home/t16840nm/work2/xdr_test/reader", &ssh_r, &ssh_w);
  printf("procces id is %d\n", pid);

 	XDR xdrs;
  work work, *work_p;
  // work_p = &work;
  memset(&work,0,sizeof(struct work));

  // work_p = &work;
  // work_p = (struct work *)malloc(sizeof(struct work));
  work.height=10;
  // char* c = work.job_id;
  // c = "dafaf";
  // char *c = "d";
  // work.job_id = "dd";
  work.txs = (char*)malloc(sizeof(char)*32);
  work.workid = (char*)malloc(sizeof(char)*32);
  work.job_id = (char*)malloc(sizeof(char)*32);
  work.xnonce2 = (u_char*)malloc(sizeof(u_char)*32);

  strncpy(work.job_id, "don", 32);
  fprintf(stderr, "%s\n", work.job_id);
  // work.workid = 'd';
  // fprintf(stderr, "workidのaddress=>%s\n", work.workid);

xdrstdio_create(&xdrs, ssh_w, XDR_ENCODE);
  for (int h = 1; h;h = h+100){
    // fprintf(stderr,"write_job_id=>%s\n", work.job_id);
  	if (!xdr_work(&xdrs, &work)) {
  		fprintf(stderr, "failed!¥n");
  		exit(1);
  	}
    fflush(ssh_w);

    work.height = h;
  }

 	int i = 10;

xdrstdio_create(&xdrs, stdout, XDR_ENCODE);
	if (!xdr_int(&xdrs, &i)) {
		fprintf(stderr, "failed!¥n");
		exit(1);
	}
 	exit(0);
}
