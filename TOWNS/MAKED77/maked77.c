#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FMCFRB.H"


extern void XModemSend(unsigned int dataLength,const unsigned char data[],int baud); // baud  2:38400bps  4:19200bps


#define BUFFERSIZE 3200*1024
#define NUM_SECTOR_BUF 64


struct D77Header
{
	char diskName[17];             // +0
	char reserveBytes[9];          // +0x11
	unsigned char writeProtected;  // +0x1A
	unsigned char mediaType;       // +0x1B
	unsigned int diskSize;         // +0x1C
	// Must be 0x20 bytes.
};

struct D77SectorHeader
{
	unsigned char C,H,R,N;
	unsigned short numSectorPerTrack;
	unsigned char densityFlag;     // 0:Double Density   0x40 Single Density
	unsigned char DDM;             // 0:Not DDM          0x10:DDM
	unsigned char CRCError;        // 0:No Error         0xB0:CRC Error
	unsigned char reserved[5];
	unsigned short actualSectorLength;
};

void InitializeD77Header(struct D77Header *hdr)
{
	unsigned char *ptr=(unsigned char *)hdr;
	for(int i=0; i<sizeof(struct D77Header); ++i)
	{
		ptr[i]=0;
	}
}

void InitializeD77SectorHeader(struct D77SectorHeader *hdr)
{
	hdr->C=0;
	hdr->H=0;
	hdr->R=0;
	hdr->N=0;
	hdr->numSectorPerTrack=0;
	hdr->densityFlag=0;
	hdr->DDM=0;
	hdr->CRCError=0;
	for(int i=0; i<5; ++i)
	{
		hdr->reserved[i]=0;
	}
	hdr->actualSectorLength=0;
}

enum
{
	MODE_2D,       // 320K
	MODE_2DD,      // 640/720K
	MODE_2HD_1232K,// 1232K
};

#define MODE1_MFM_MODE          0x00
#define MODE1_FM_MODE           0x80
#define MODE1_2HD               0x00
#define MODE1_2DD               0x10
#define MODE1_2D                0x20
#define MODE1_128_BYTE_PER_SEC  0x00
#define MODE1_256_BYTE_PER_SEC  0x01
#define MODE1_512_BYTE_PER_SEC  0x02
#define MODE1_1024_BYTE_PER_SEC 0x03

void SetDriveMode(unsigned int drive,unsigned int mode)
{
	drive&=0x0F;
	drive|=0x20;

	switch(mode)
	{
	case MODE_2D:
		DKB_setmode(drive,MODE1_MFM_MODE|MODE1_2D|MODE1_256_BYTE_PER_SEC,0x2F);
		break;
	case MODE_2DD:
		DKB_setmode(drive,MODE1_MFM_MODE|MODE1_2DD|MODE1_512_BYTE_PER_SEC,0x28);
		break;
	case MODE_2HD_1232K:
		DKB_setmode(drive,MODE1_MFM_MODE|MODE1_2HD|MODE1_1024_BYTE_PER_SEC,0x28);
		break;
	}
}

int NumberOfCylinders(unsigned int mode)
{
	switch(mode)
	{
	case MODE_2D:
		return 40;
	case MODE_2DD:
		return 80;
	case MODE_2HD_1232K:
		return 77;
	}
	return 0;
}


struct CommandParameterInfo
{
	unsigned char listOnly;
	unsigned int drive;  // 0:A  1:B
	unsigned int mode;
	unsigned int baudRate;
	unsigned int startTrk,endTrk;
};

unsigned int ReadTrack(
   int devNo,int track,int side,struct CommandParameterInfo *cpi,unsigned char d77Image[],struct D77Header *hdr,unsigned int trackTable[],unsigned char *nextTrackData)
{
	printf("Track:%d  Side:%d\n",track,side);

	int i;
	int nTrackSector=0;
	DKB_SEC sector[NUM_SECTOR_BUF];
	for(i=0; i<NUM_SECTOR_BUF; ++i)
	{
		if(0==DKB_rdsecid(devNo,track,side,sector+nTrackSector))
		{
			++nTrackSector;
		}
	}

	// Remove Duplicates >>
	for(i=nTrackSector-1; 0<i; --i)
	{
		int j;
		for(j=i-1; 0<=j; --j)
		{
			if(sector[i].trakno==sector[j].trakno &&
			   sector[i].hedno ==sector[j].hedno &&
			   sector[i].secno ==sector[j].secno &&
			   sector[i].seccnt==sector[j].seccnt)
			{
				sector[i]=sector[nTrackSector-1];
				--nTrackSector;
				break;
			}
		}
	}
	// Remove Duplicates <<


	trackTable[track*2+side]=(nextTrackData-d77Image);


	unsigned char *trackDataPtr=nextTrackData;
	int nActual=0;
	for(i=0; i<nTrackSector; ++i)
	{
		int retry;
		for(retry=0; retry<3; ++retry)
		{
			printf("%02x%02x%02x%02x",sector[i].trakno,sector[i].hedno,sector[i].secno,sector[i].seccnt);

			struct D77SectorHeader *sectorHdr=(struct D77SectorHeader *)trackDataPtr;
			InitializeD77SectorHeader(sectorHdr);

			sectorHdr->C=sector[i].trakno;
			sectorHdr->H=sector[i].hedno;
			sectorHdr->R=sector[i].secno;
			sectorHdr->N=sector[i].seccnt;

			sectorHdr->numSectorPerTrack=nTrackSector;

			sectorHdr->densityFlag=0;     // 0:Double Density   0x40 Single Density
			sectorHdr->actualSectorLength=(128<<sectorHdr->N);


			char *dataBuf=(char *)(sectorHdr+1);
			int secnum,biosErr;
			biosErr=DKB_read(devNo,sector[i].trakno,sector[i].hedno,sector[i].secno,1,dataBuf,&secnum);

			if(0!=(0x8000&biosErr))
			{
				if(0!=(biosErr&4))
				{
					sectorHdr->DDM=0x10;
				}
				if(0!=(biosErr&0x10))
				{
					sectorHdr->CRCError=0xB0;
				}
			}

			char sta[4]={0,0,0,0};

			if(0x10==sectorHdr->DDM)
			{
				strcat(sta,"D");
			}
			if(0xB0==sectorHdr->CRCError)
			{
				strcat(sta,"C");
			}
			while(strlen(sta)<3)
			{
				strcat(sta," ");
			}
			printf("%s",sta);

			trackDataPtr+=sizeof(struct D77SectorHeader)+sectorHdr->actualSectorLength;

			if(5==(nActual%6))
			{
				printf("\n");
			}
			++nActual;

			if(0==sectorHdr->CRCError) // No retry if no CRC error.
			{
				break;
			}
		}
	}
	if(0!=nActual%6)
	{
		printf("\n");
	}

	unsigned char *updatePtr=nextTrackData;
	for(i=0; i<nActual; ++i)
	{
		struct D77SectorHeader *sectorHdr=(struct D77SectorHeader *)updatePtr;
		sectorHdr->numSectorPerTrack=nActual;
		updatePtr+=sizeof(struct D77SectorHeader)+sectorHdr->actualSectorLength;
	}

	if(updatePtr!=trackDataPtr)
	{
		fprintf(stderr,"Something Went Wrong in ReadTrack\n");
		fprintf(stderr,"  trackDataPtr:%08x\n",trackDataPtr);
		fprintf(stderr,"  updatePtr   :%08x\n",updatePtr);
	}

	return trackDataPtr-nextTrackData;
}

int CheckDiskMediaType(unsigned int driveStatus,unsigned int mediaType)
{
	printf("BIOS Detected:\n");
	switch(driveStatus&0x30)
	{
	case 0x10:
		printf("2D or 2DD disk\n");
		break;
	case 0x00:
		printf("2HD disk\n");
		break;
	default:
		printf("Unsupported disk type.\n");
		return -1;
	}

	if((driveStatus&0x30)==0 && mediaType!=MODE_2HD_1232K)
	{
		return -1;
	}
	if((driveStatus&0x30)==0x10 && (mediaType!=MODE_2D && mediaType!=MODE_2DD))
	{
		return -1;
	}
	return 0;
}

int ReadDisk(struct CommandParameterInfo *cpi,unsigned char d77Image[])
{
	int i;
	unsigned int devNo;

	struct D77Header *d77HeaderPtr=(struct D77Header *)d77Image;
	unsigned int *trackTable=(unsigned int *)(d77Image+0x20);

	InitializeD77Header(d77HeaderPtr);

	for(i=0; i<164; ++i)
	{
		trackTable[i]=0;
	}

	unsigned char *trackData=(unsigned char *)(trackTable+164);



	cpi->drive&=0x0F;
	devNo=0x20|cpi->drive;

	DKB_restore(devNo);
	unsigned int driveStatus;
	int biosError;
	if(0!=(biosError=DKB_rdstatus(devNo,&driveStatus)))
	{
		fprintf(stderr,"Cannot get the drive status.\n");
		fprintf(stderr,"  Bios Error Code 0x%02x\n",biosError);
		return -1;
	}
	if(0!=CheckDiskMediaType(driveStatus,cpi->mode))
	{
		fprintf(stderr,"Wrong Disk Media.\n");
		return -1;
	}

	SetDriveMode(cpi->drive,MODE_2HD_1232K);



	int track;
	for(track=cpi->startTrk; track<=cpi->endTrk; ++track)
	{
		trackData+=ReadTrack(devNo,track,0,cpi,d77Image,d77HeaderPtr,trackTable,trackData);
		trackData+=ReadTrack(devNo,track,1,cpi,d77Image,d77HeaderPtr,trackTable,trackData);
	}
	d77HeaderPtr->diskSize=(trackData-d77Image);


	printf("D77 Image Size=%d bytes\n",d77HeaderPtr->diskSize);


	return d77HeaderPtr->diskSize;
}

void InitializeCommandParameterInfo(struct CommandParameterInfo *cpi)
{
	cpi->listOnly=0;
	cpi->drive=0;
	cpi->mode=MODE_2HD_1232K;
	cpi->baudRate=38400;
}

int RecognizeCommandParameter(struct CommandParameterInfo *cpi,struct D77Header *hdr,int ac,char *av[])
{
	if(ac<3)
	{
		return -1;
	}

	// ac is supposed to be minimum 3.
	cpi->drive=av[1][0];
	if('A'<=cpi->drive && cpi->drive<='Z')
	{
		cpi->drive-='A';
	}
	else if('a'<=cpi->drive && cpi->drive<='a')
	{
		cpi->drive-='a';
	}
	if(4<=cpi->drive || ':'!=av[1][1])
	{
		fprintf(stderr,"Error!  Invalid drive.\n");
		return -1;
	}

	if(0==strcmp(av[2],"2D") || 0==strcmp(av[2],"2d") ||
	   0==strcmp(av[2],"320KB") || 0==strcmp(av[2],"320kb"))
	{
		cpi->mode=MODE_2D;
		cpi->startTrk=0;
		cpi->endTrk=39;
		hdr->mediaType=0;
	}
	else if(0==strcmp(av[2],"2DD") || 0==strcmp(av[2],"2dd") ||
	        0==strcmp(av[2],"640KB") || 0==strcmp(av[2],"640kb") ||
	        0==strcmp(av[2],"720KB") || 0==strcmp(av[2],"720kb"))
	{
		cpi->mode=MODE_2DD;
		cpi->startTrk=0;
		cpi->endTrk=79;
		hdr->mediaType=0x10;
	}
	else if(0==strcmp(av[2],"2HD") || 0==strcmp(av[2],"2hd") ||
	        0==strcmp(av[2],"1232KB") || 0==strcmp(av[2],"1232kb"))
	{
		cpi->mode=MODE_2HD_1232K;
		cpi->startTrk=0;
		cpi->endTrk=76;
		hdr->mediaType=0x20;
	}

	for(int i=3; i<ac; ++i)
	{
		if(0==strcmp(av[i],"-STARTTRK") || 0==strcmp(av[i],"-starttrk"))
		{
			if(i+1<ac)
			{
				cpi->startTrk=atoi(av[i+1]);
			}
			else
			{
				fprintf(stderr,"Too few arguments for %s\n",av[i]);
				return -1;
			}
		}
		else if(0==strcmp(av[i],"-ENDTRK") || 0==strcmp(av[i],"-endtrk"))
		{
			if(i+1<ac)
			{
				cpi->endTrk=atoi(av[i+1]);
			}
			else
			{
				fprintf(stderr,"Too few arguments for %s\n",av[i]);
				return -1;
			}
		}
		else if(0==strcmp(av[i],"-LISTONLY") || 0==strcmp(av[i],"-listonly"))
		{
			cpi->listOnly=1;
		}
		else if(0==strcmp(av[i],"-19200BPS") || 0==strcmp(av[i],"-19200bps"))
		{
			cpi->baudRate=19200;
		}
		else if(0==strcmp(av[i],"-38400BPS") || 0==strcmp(av[i],"-38400bps"))
		{
			cpi->baudRate=38400;
		}
		else if(0==strcmp(av[i],"-NAME") || 0==strcmp(av[i],"-name"))
		{
			if(i+1<ac)
			{
				strncpy(hdr->diskName,av[i+1],17);
			}
			else
			{
				fprintf(stderr,"Too few arguments for %s\n",av[i]);
				return -1;
			}
		}
		else if(0==strcmp(av[i],"-WRITEPROTECT") || 0==strcmp(av[i],"-writeprotect"))
		{
			if(i+1<ac)
			{
				hdr->writeProtected=1;
			}
			else
			{
				fprintf(stderr,"Too few arguments for %s\n",av[i]);
				return -1;
			}
		}
	}

	return 0;
}

int main(int ac,char *av[])
{
	unsigned char *d77Image=(unsigned char *)malloc(BUFFERSIZE);
	if(NULL==d77Image)
	{
		printf("Out of memory.\n");
		printf("Cannot allocate memory for making D77 image.\n");
		printf("The program requires minimum 4MB RAM.\n");
		return 0;
	}
	struct D77Header *hdr=(struct D77Header *)d77Image;


	struct CommandParameterInfo cpi;
	if(ac<3 || 0!=RecognizeCommandParameter(&cpi,hdr,ac,av))
	{
		printf("Make D77 file and send to RS232C via XMODEM\n");
		printf("  by CaptainYS\n");
		printf("Usage:\n");
		printf("  RUN386 MAKED77 A: 1232KB [options]\n");
		printf("    A:      Drive\n");
		printf("    1232KB  Disk Type\n");
		printf("  Disk Type can be one of:\n");
		printf("    2D,320KB         2D Disk (from FM-7)\n");
		printf("    2DD,640KB,720KB  2DD Disk\n");
		printf("    2HD,1232KB       2HD 1232K Disk\n");
		printf("    (1440KB not supported at this time.\n");
		printf("  Options:\n");
		printf("    -starttrk trackNum  Start track (Between 0 and 76 if 2HD)\n");
		printf("    -endtrk   trackNum  End track (Between 0 and 76 if 2HD)\n");
		printf("    -listonly           List track info only.  No RS232C transmission\n");
		printf("    -19200bps           Slow down to 19200bps (default 38400bps)\n");
		printf("    -38400bps           Transmit at 38400bps (default)\n");
		printf("    -name diskName      Specify disk name up to 16 chars.\n");
		printf("    -writeprotect       Write protect the disk image.\n");
		return 1;
	}

	int d77Size=ReadDisk(&cpi,d77Image);


	// Force it to be 128xN.  XMODEM's limitation.
	d77Size=((d77Size+127)&~127);
	struct D77Header *d77HeaderPtr=(struct D77Header *)d77Image;
	d77HeaderPtr->diskSize=d77Size;


	if(d77Size<0)
	{
		fprintf(stderr,"Disk Read Error.\n");
		return 1;
	}
	if(0==cpi.listOnly)
	{
		int baud=(cpi.baudRate==38400 ? 2 : 4);
		XModemSend(d77Size,d77Image,baud);
	}

	return 0;
}
