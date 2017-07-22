#include <stdio.h>
#include <rpc/rpc.h> /* xdr は rpc のサブライブラリ */
#include <string.h>
#include <stdlib.h>
#include "work.h"

static inline void work_copy(struct work *dest, const struct work *src)
{
	memcpy(dest, src, sizeof(struct work));
	if (src->txs)
		dest->txs = strdup(src->txs);
	if (src->workid)
		dest->workid = strdup(src->workid);
	if (src->job_id)
		dest->job_id = strdup(src->job_id);
	if (src->xnonce2) {
		dest->xnonce2 = malloc(src->xnonce2_len);
		memcpy(dest->xnonce2, src->xnonce2, src->xnonce2_len);
	}
}

int main()                /* reader.c */
{
 	XDR xdrs;
  struct work work;
  memset(&work,0,sizeof(struct work));
  struct work g_work;
  memset(&g_work,0,sizeof(struct work));
  work.job_id = (char*)malloc(sizeof(char)*32);

xdrstdio_create(&xdrs, stdin, XDR_DECODE);
  while (1) {
    /* code */
    if (!xdr_work(&xdrs, &work)) {
      fprintf(stderr, "failed!\n");
      continue;
    }
    // if(memcmp(g_work.job_id, work.job_id, 76)){
    //   printf("cmp\n");
    //   // work_copy(&g_work, &work);
    //   // int size = sizeof(work.job_id);
    //   printf("flag2\n");
    //   printf("job_id=>%20s", work.job_id);
    //   printf("\n");
    //   printf("data[31]=>%#X", work.data[31]);
    //   printf("\n");
    //   printf("height=>%d", work.height);
    //   printf("\n");
    // }

    // printf("cmp\n");
    // work_copy(&g_work, &work);
    // int size = sizeof(work.job_id);
    // printf("flag2\n");
    // printf("job_id=>%s", (&work)->job_id);
    fprintf(stderr, "job_id=>%s", work.job_id);
    fprintf(stderr, "\n");
    fprintf(stderr, "data[31]=>%#X", work.data[31]);
    fprintf(stderr, "\n");
    fprintf(stderr, "height=>%d", work.height);
    fprintf(stderr, "\n");

  }

 	exit(0);
}
