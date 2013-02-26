//windows作为客户端，向linux服务端发送文件
//需要设置的参数：绑定的IP;inpath;MAXLINE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>//声明inet_pton
#include <dirent.h>

#define MAXLINE 500
#define SERV_PORT 8000

int main()
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
    	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	
	
	strcpy(buf,"begin send");
	write(sockfd,buf,sizeof(buf)); 


	char *inpath="/home/cloud/文档/发送测试/输出文件/";	  
	DIR *pdir;
	struct dirent *pdirent;	
	char inname[100];//文件全路径名
	pdir=opendir(inpath);
	if(pdir==NULL)
		printf("null");
	else
		printf("222222");
	printf("%s\n",inpath);
	while(NULL!=(pdirent=readdir(pdir)))
	{	
		//发送文件的名字
		strcpy(buf,pdirent->d_name);
		if(strcmp(buf,"..")&&strcmp(buf,"."))
		{
			printf("sending the name of file: %s\n",buf);
			if(buf)
			{
				printf("yes\n");			
				write(sockfd, buf, sizeof(buf));
			}
			else
			{		
				printf("no\n");
			}
			bzero(buf,sizeof(buf)); 

		
			strcpy(inname,"");
			strcpy(inname,inpath);
			strcat(inname,pdirent->d_name);
			FILE * fp = fopen(inname,"r");
			printf("the name of file%s\n",inname);
			if(NULL == fp )
			{
			    printf("error\n");
			}
			else
			{
				int file_block_length = 0;
				while( (file_block_length = fread(buf,sizeof(char),MAXLINE,fp))>0)
				{	
					write(sockfd,buf,file_block_length);
					bzero(buf, MAXLINE);
				} 
				strcpy(buf,"end");
				write(sockfd,buf,sizeof(buf));//告诉服务端一个文件传送完毕			
				bzero(buf, MAXLINE);
			}
			fclose(fp);                 
		}

	}
	
	strcpy(buf,"allend");
	write(sockfd,buf,sizeof(buf));//告诉服务端一个文件传送完毕	


	

	close(sockfd);
	return 0;
}
