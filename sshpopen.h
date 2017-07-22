#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <err.h>
#include <unistd.h>

#define R (0)
#define W (1)
#define BFSIZE 1024

// popenではrかwのどちらかしかパイプを貼れないので、
// 同時にrとwにパイプを貼る関数
// 引数のfd_r,fd_wのポインタにパイプを割り当てる
// 参考
// http://www.ncad.co.jp/~komata/c-kouza3.htm
// http://www.retro11.de/ouxr/211bsd/usr/src/lib/libc/gen/popen.c.html
int popen2(command, fd_r, fd_w)
char *command;
register FILE **fd_r;
register FILE **fd_w;
{
int pipe_c2p[2],pipe_p2c[2];
int pid;

	// ミスったらpipe閉じるやつ
	if(pipe(pipe_c2p)<0){
		perror("popen2");
		return(-1);
	}
	if(pipe(pipe_p2c)<0){
		perror("popen2");
		close(pipe_c2p[R]);
		close(pipe_c2p[W]);
		return(-1);
	}

	if((pid=fork())<0){
		perror("popen2");
		close(pipe_c2p[R]);
		close(pipe_c2p[W]);
		close(pipe_p2c[R]);
		close(pipe_p2c[W]);
		return(-1);
	}

	// サブプロセスとのpipeを作り、入出力を繋げて、プロセスそのものをexecで入れ替える。
	if(pid==0){
		close(pipe_p2c[W]);
		close(pipe_c2p[R]);
		dup2(pipe_p2c[R],0);
		dup2(pipe_c2p[W],1);
		close(pipe_p2c[R]);
		close(pipe_c2p[W]);

		if(execl("/bin/sh", "sh", "-c", command, NULL)<0){
			perror("popen2");
			close(pipe_p2c[R]);
			close(pipe_c2p[W]);
			exit(1);
		}
	}

close(pipe_p2c[R]);
close(pipe_c2p[W]);

// いっかい代入しないとfdopen()関数のポインタ参照しちゃう？
// やっぱりポインタよくわかんない。
FILE *test1 = fdopen(pipe_p2c[W], "w");
FILE *test2 = fdopen(pipe_c2p[R], "r");
*fd_w = test1;
*fd_r = test2;

return(pid);
}

int sshpopen2(command, ssh_r, ssh_w)
char *command;
register FILE **ssh_r;
register FILE **ssh_w;
{
	// CNSのid
	char *cns_id = "t16840nm";
	char command_base[128];
	char buf[BFSIZE];
	// ローカルテスト用
  // sprintf(command_base, "/Users/irregular0/work/課題/c/xdr_test/reader");

	// popen2関数を使ってssh先でecho serverを立て、そのstdinとstdoutと繋がったpipeを取得
	sprintf(command_base, "ssh %s@ccx11.sfc.keio.ac.jp %s", cns_id, command);

	int process_id = popen2(command_base, ssh_r, ssh_w);
	return process_id;
}

extern  int sshpopen2(char*, FILE**, FILE**);
