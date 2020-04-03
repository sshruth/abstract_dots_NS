/*
 *	Usage:	mkfnt <outfile> [-o]
 *
 *	B. Zoltick, 04Dec89
 */

#include <stdio.h>
#include /usr/local/include/tig.h

#define	LBSZ	150	/* Line buffer size */
#define	RESP(x)	resp(0,1,x,lbuf,LBSZ)
#define	OSZ	(3*512)

extern int errno;
extern char *alloc();
extern char *mktemp();
extern long aton();
extern long tell();

struct fdict chars[NCHARS];

struct {
	char height;
	char voff;
	char wd;
	char hoff;
	char buf[OSZ];
} ofnt;


char lbuf[LBSZ];
char newfile[100];
char *template	= "/tmp/XXXXXX";

main(argc,argv, envp)
int argc;
char **argv;
char **envp;
{
	register int *ip, i;
	int index, j, *jp;
	int nfd, ofd, vfd;	/* file descriptors */
	int nvecs;

	if(argc < 2 || argc > 3) {
		fprintf(stderr,"Usage:\nmkfnt font\n");
		exit(0);
	}

	if((ofd=open(argv[1], O_RDONLY)) < 0) {
		perror(argv[1]);
		exit(errno);
	}
	
	mktemp(template);
	if( (vfd=open(template,O_RDWR)) < 0 ) {
		perror(template);
		exit(errno);
	}
	
	strcpy(newfile, argv[1]);
	strcat(newfile, ".mod");	
	if( (nfd=open(newfile, O_RDWR)) < 0) {
		perror(newfile);
		exit(errno);
	}
	
	if( read(ofd,chars,sizeof(chars) ) == sizeof(chars) ) {
		for(i=0; i < NCHARS; i++) {

			if( !(vad=chars[i].vaddr) ) {
				chars[i].vaddr = -1L;
				continue;
			}
			if( (j=chars[i].finfo.nvecs*sizeof(int)) <= 0)
				continue;
			if( (chars[i].vaddr=calloc(&vhd,j)) < 0) {
				fprintf(stderr,"Initialization calloc error: %D\n",chars[i].vaddr);
				exit(1);
			}
			if( xfer(nfd,vhd.fd,vad,chars[i].vaddr,j) )
				fprintf(stderr,"Transfer error on index 0%o\n",i);
			else fprintf(stdout,"0%o index, %d vecs\n",i,chars[i].finfo.nvecs);
		}
	}
	else	for( i=0; i < NCHARS; chars[i++].vaddr = -1L );

	while( RESP("Read block: ") > 0) {
		if(lbuf[0] == 'd') {	/* Delete a indx */
			if(RESP("Delete index: ") > 0) {
				struct vdescr vinf;

delete:
				if((index=aton(lbuf,8,'9',0)) < 0 || index >= NCHARS) {
					writs(2,"Index range must be from 0-->177\n");
					continue;
				}
				if( chars[index].vaddr < 0 ) {
					prints(2,"0%o is already deallocated\n",index);
					continue;
				}
				vinf.v_addr = chars[index].vaddr;
				vinf.v_size = chars[index].finfo.nvecs*2;
				vfree(&vhd,&vinf);
			}
			continue;
		}

		if(seek(ofd,atoi(lbuf),3)) {
			perror("Selected block");
			continue;
		}
		if(read(ofd,&ofnt,OSZ) <= 0) {
			perror(oldfile);
			continue;
		}

		prints(2,"Height, width, Voffst, Hoffst: %d, %d, %d, %d\n",ofnt.height&0377,ofnt.wd&0377,ofnt.voff,ofnt.hoff);

		j.lobyte = (ofnt.wd&0377) >> 1;
		j.hibyte = ofnt.voff;
		v_xwidth = ((ofnt.wd&0377)+7)&~07;
		v_xbiwd = BYTCNV( v_xwidth );
		v_xbywd = v_xbiwd >> 3;
		v_ywidth = ofnt.height + ofnt.voff;

		if(argc == 3)
			jp = ip = outline(ofnt.buf);
		else	ip = ofnt.buf;

		/* VERS fonts are upside down since Versatec puts top out first */
		if((ip=mkvectr(vflip(ip),j,&nvecs)) >= -3) {
			prints(2,"Mvectr() error: %d\n",ip);
			continue;
		}
		if(argc == 3)
			free(jp);
		prints(2,"Used %d vectors\n",nvecs);

		if((nvecs+1)*2 > *(ip-1)) {
			prints(2,"Required %d bytes\n",(nvecs+1)*2);
			prints(2,"Overran memory of %d bytes\n",*(ip-1));
			exit(77);
		}

		if(RESP("Index: ") == '\033')
			index = aton( &lbuf[1], 8, '9', 0 );
		else	index = lbuf[0];
		if(index < 0 || index > 127)
			goto delete;
		if( chars[index].vaddr >= 0	&&
		    RESP("Index already allocated.  Deallocate? ") != 'y'
		 )
			goto jump;

		i = chars[index].finfo.nvecs = nvecs;
		i *= sizeof(int);
		if( (chars[index].vaddr=valloc(&vhd,i)) < 0) {
			prints(2,"VALLOC() error: %D\n",chars[index].vaddr);
			exit(78);
		}
		chars[index].finfo.width = ofnt.wd&0377;
		lseek(vhd.fd,chars[index].vaddr,0);
		if( write(vhd.fd,ip,i) != i) {
			perror(vtmp);
			exit(errno);
		}
jump:
		if((i=free(ip)) < 0) {
			prints(2,"Free error: %d\n",i);
			exit(76);
		}
	}

	/* Now dump the results */

	seek(nfd,sizeof chars,0);	/* Leave room for header later on */
	for(i=0; i < NCHARS; i++) {
		vaddr vad;

		if(chars[i].vaddr < 0) {
			chars[i].vaddr = 0L;
			chars[i].finfo.nvecs = 0;
			continue;
		}
		if( (j=chars[i].finfo.nvecs*sizeof(int)) <= 0 ) {
			chars[i].vaddr = 0L;
			chars[i].finfo.nvecs = 0;
			continue;
		}
		if( xfer(vhd.fd,nfd,chars[i].vaddr,vad=tell(nfd),j) ) {
			fprint(stderr,"Fatal transfer error on index 0%o\n",i);
			exit(1);
		}
		chars[i].vaddr = vad;
		fprintf(stdout,"Index 0%o: Transfered %d vecs starting at %D\n",i,chars[i].finfo.nvecs,chars[i].vaddr);
	}
	seek(nfd,0,0);		/* Load header */
	write(nfd,chars,sizeof chars);
	exit(0);
}

/*
 *	XFER( <fd1>, <fd2>, <vaddr1>, <vaddr2>, <n BYTEs> )
 *
 *	Calls:	read(II), write(II), lseek(III)
 *
 *	Returns:	0	Success
 *			-1	Error
 */

xfer( fd1, fd2, vad1, vad2, nbytes )	/* Transfer from fd1 to fd2 nbytes */
int fd1, fd2;
vaddr vad1, vad2;
register int nbytes;
{
	register int i;
	char buf[512];

	lseek( fd1, vad1, 0 );
	lseek( fd2, vad2, 0 );

	for( ; nbytes > 0; nbytes -= i ) {
		if( nbytes > 512 )
			i = 512;
		else	i = nbytes;
		if( read(fd1,buf,i) != i || write(fd2,buf,i) != i )
			return(-1);
	}
	return(0);
}

_cleanup()
{

	unlink(vtmp);
}

atoi(ptr)
register char *ptr;
{

	return(aton(ptr,10,'9',0));
}
