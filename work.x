struct work {
	int data[32];
	int target[8];

	int height;
	char *txs;
	char *workid;

	char *job_id;
	int xnonce2_len;
	unsigned char *xnonce2;
};
