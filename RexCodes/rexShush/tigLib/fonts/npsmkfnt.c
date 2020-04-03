
/*
 *	Usage:	npsmkfnt <outfile> <infile> [-o]
 *
 *	To compile:	cc -n -O npsmkfnt.c -lu -lm -o npsmkfnt
 */

#include	<tig.h>
#include	<valloc.h>
#include	<vector.h>

#define	LBSZ	150	/* Line buffer size */
#define	RESP(x)	resp(0,1,x,lbuf,LBSZ)
#define	OSZ	(3*512)

extern int errno;
extern char *alloc();
extern long aton();
extern long tell();

struct fdict chars[NCHARS];
struct {
	int rwidth, lftkrn, rft, drc;		/* See page 38 of thesis */
	char buf[OSZ];
} ofnt;

struct {
	char cwidth;
	char block;
	int coffset;
} header[128];		/* Header of NPS fonts */

struct {
	int ftht;	/* Font height */
	int mxcwid;	/* Max char width */
	int loght;	/* logical height; see page 39 */
} fntdim;		/* Font's dimensions */

	/* Virtual Memory stuff */

struct vsystm vhd;

char lbuf[LBSZ];
char *vtmp	= "/tmp/npsmkfnt_tmp";

main(argc,argv)
int argc;
char **argv;
{
	register int nfd, *ip, i;
	int index, npsindx, ofd, j, *jp;
	int nvecs;
	double scale;

	if( argc != 3 && argc != 4 )
		err("Usage: npsmkfnt <TIG-font> <NPS-font> [-o]",75);

	if((ofd=open(argv[2],0)) < 0) {
		perror(argv[2]);
		exit(errno);
	}
	if( read(ofd,header,sizeof header) != sizeof header	||
	    read(ofd,&fntdim,sizeof fntdim) != sizeof fntdim
	  ) {
		perror(argv[2]);
		exit(errno);
	}
	if( (vhd.fd=creat(vtmp,0644)) < 0 || close(vhd.fd)	||
	    (vhd.fd=open(vtmp,2)) < 0
	  ) {
		perror(vtmp);
		exit(errno);
	}
	if( (nfd=open(argv[1],2)) < 0 && (nfd=creat(argv[1],0644)) < 0) {
		perror(argv[1]);
		exit(errno);
	}
	if(read(nfd,chars,sizeof chars) == sizeof chars) {
		for(i=0; i < NCHARS; i++) {
			vaddr vad;

			if( !(vad=chars[i].vaddr) ) {
				chars[i].vaddr = -1L;
				continue;
			}
			if( (j=chars[i].finfo.nvecs*sizeof(int)) <= 0)
				continue;
			if( (chars[i].vaddr=valloc(&vhd,j)) < 0) {
				prints(2,"Initialization VALLOC error: %D\n",chars[i].vaddr);
				exit(78);
			}
			if( xfer(nfd,vhd.fd,vad,chars[i].vaddr,j) )
				prints(2,"Transfer error on index 0%o\n",i);
			else prints(1,"0%o index, %d vecs\n",i,chars[i].finfo.nvecs);
		}
	}
	else	for( i=0; i < NCHARS; chars[i++].vaddr = -1L );

	scale = 75.0 / fntdim.loght;		/* Set scale factor */

	while( RESP("NPS index: ") > 0 ) {
		if( (npsindx=aton(lbuf,8,'9',0)) < 0 || npsindx > 0177 ) {
			writs(2,"NPS index out of range\n");
			continue;
		}

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

		if( header[npsindx].cwidth == 0 ) {
			prints(2,"0%o: no character present\n",npsindx);
			continue;
		}
		if( seek(ofd,header[npsindx].block,3) || seek(ofd,header[npsindx].coffset,1) ) {
			perror("Selected index");
			continue;
		}
		if(read(ofd,&ofnt,OSZ) <= 0) {
			perror(argv[2]);
			continue;
		}

		prints(2,"DRC, height, width: %d,%d,%d\n",ofnt.drc,
			(int)((fntdim.loght-ofnt.rft)*scale),
			(int)((header[npsindx].cwidth&0377)*scale) );

		j.lobyte = (header[npsindx].cwidth&0377) >> 1;
		j.hibyte = ofnt.rft + ofnt.drc - fntdim.loght;
		v_xwidth = ((header[npsindx].cwidth&0377)+7)&~07;
		v_xbiwd = BYTCNV( v_xwidth );
		v_xbywd = v_xbiwd >> 3;
		v_ywidth = ofnt.drc;

		if(argc == 4)
			jp = ip = outline(ofnt.buf);
		else	ip = ofnt.buf;

		/* VERS fonts are upside down since Versatec puts top out first */
		if((ip=mkvectr(vflip(ip),j,&nvecs)) >= -3) {
			prints(2,"Mvectr() error: %d\n",ip);
			continue;
		}
		if(argc == 4)
			free(jp);

		if((nvecs+1)*2 > *(ip-1)) {
			prints(2,"Required %d bytes\n",(nvecs+1)*2);
			prints(2,"Overran memory of %d bytes\n",*(ip-1));
			exit(77);
		}
prints(2,"nvecs: %d\n",nvecs);
		nvecs = vscale( ip, nvecs, scale );
		prints(2,"Used %d vectors\n",nvecs);

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
		chars[index].finfo.width = (header[npsindx].cwidth&0377) * scale;
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
			prints(2,"Fatal transfer error on index 0%o\n",i);
			exit(77);
		}
		chars[i].vaddr = vad;
		prints(1,"Index 0%o: Transfered %d vecs starting at %D\n",i,chars[i].finfo.nvecs,chars[i].vaddr);
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
