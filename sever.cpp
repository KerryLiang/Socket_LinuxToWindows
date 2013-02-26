//linux服务端接受客户端发来的数据
//需要设置的参数：outpath;MAXLINE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 500
#define SERV_PORT 8000

int main(void)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;
	char *outpath="/home/cloud/文档/发送测试/接收文件/";
	char file_name[100];//file 的全路径
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 20);
	printf("Accepting connections ...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
	  
		


		read(connfd,buf,sizeof(buf));//
		printf("-----%s------\n",buf);
		printf("reading file:");	
		while(1)//还要读文件
		{
					
			//开始读文件的名字
			bzero(buf, sizeof(buf));
			int n=read(connfd,buf,MAXLINE);
			if(strcmp(buf,"allend")==0)
				break;	

			if(n!=0)
			{
				printf("接受到的文件名字：%s\n",buf);
				strcpy(file_name,"");
				strcpy(file_name, outpath);
				strcat(file_name,buf);
				//开始读文件
				FILE * fp = fopen(file_name,"w");
				if(NULL == fp )
				{
				    	printf("File:\t%s Can Not Open To Write\n", file_name);
					exit(1);
				}
				while(1)
				{
					bzero(buf,MAXLINE);
					int m=read(connfd,buf,MAXLINE);
					if(strcmp(buf,"end")==0)
						break;
					if(m!=0)
					{
					printf("%s",buf);						
					fwrite(buf,sizeof(char),m,fp);		

					}

				 }
				fclose(fp);


			}


		}
		 

		
		close(connfd);
	}
}
