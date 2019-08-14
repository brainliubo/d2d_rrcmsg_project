#include <stdio.h>
#include <sys/types.h>

#include <MasterInformationBlock.h>
#include <PDCCH-ConfigCommon.h>
#include <SIB-PDCCH-Config.h>
#include <SystemInformationBlockType1.h>
#include <P-Max.h>
#include <TDD-Config.h>
#include <RRCConnectionSetup.h>
#include <RRCConnectionRequest.h>
#include <RRCConnectionRelease.h>
/* Write the encoded output into some FILE stream. */
int write_out(const void *buffer, size_t size, void *app_key) {
	FILE *out_fp = app_key;
	size_t wrote = fwrite(buffer, 1, size, out_fp);
	return (wrote == size) ? 0 : -1;
}


int EncodeD2dMib(int argc, char *argv[])
{
	MasterInformationBlock_t  *bch_msg; 
	FILE *fp;
	asn_enc_rval_t ec; 
	const char *filename; 


	uint16_t *spare = calloc(1, sizeof(uint16_t));
	unsigned char buf[1024]; 
	int  size ; 
	int i; 
	char sfn = (0x7f << 1); 
	//char msg2_pdcch = (0x3f << 2); //!6bit，6bit从高位开始
	//char rb_GroupIndex = (0x2 << 5); //!3bit ，取高3bit 
	//char rb_Index = (0x25 << 2);  //!6bit,取高6bit 
	char pdcch_config = (0x7f << 1); //!7bit ,取高7bit 

	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "wb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}
 	
	/* Allocate the Rectangle_t */
	bch_msg = calloc(1, sizeof(MasterInformationBlock_t)); /* not malloc! */
	if(!bch_msg) {
		perror("calloc() failed");
		exit(1);
	}


	bch_msg->dl_Bandwidth = 1; 

	bch_msg->systemFrameNumber.buf = &sfn; 
	bch_msg->systemFrameNumber.size = 1; 
	bch_msg->systemFrameNumber.bits_unused = 1; 
  
	
    
	bch_msg->pdcch_Config.buf =  &pdcch_config;
	bch_msg->pdcch_Config.size = 1; 
	bch_msg->pdcch_Config.bits_unused  = 1; 
	 
	

    //!<encode BCH 
    ec = asn_encode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_MasterInformationBlock, bch_msg, write_out, fp); 
   // ec = asn_encode_to_buffer(0,ATS_UNALIGNED_BASIC_PER,&BCCH_BCH_Message_t, bch_msg,encode_buffer,buffersize); 
	fclose(fp);
	if(ec.encoded == -1) {
		fprintf(stderr, "Could not encode  (at %s)\n",
		 ec.failed_type->name);  //ec.failed_type ? 
		system("pause");
		exit(1);
	} else {
		printf("ec.encoded = %d bytes\n",ec.encoded); 
		fprintf(stderr, "Created %s with PER encoded success\n", filename);

		
		fp = fopen(filename,"rb"); 
		size = fread(buf, 1, sizeof(buf), fp); 
		if (size !=0)
		{
			printf("encode output byte: ");
		    for (i = 0; i < size;i++)
			{
			   fprintf(stdout,"%x", buf[i]); 
			}
			printf("  \n");
		}
	}
	fclose(fp);
	xer_fprint(stdout,&asn_DEF_MasterInformationBlock,(void*)bch_msg); //！提取出IE 内容
	//asn_fprint(stdout, &asn_DEF_MasterInformationBlock, bch_msg); //!不提取bit,只打印原始Bite
    system("pause");
	return 0; /* Encoding finished successfully */



}




int DecodeD2dMib(int argc,char * argv[])
{

    MasterInformationBlock_t  *bch_msg = NULL; 
	FILE *fp;
	asn_dec_rval_t ec; 
	const char *filename; 
    char buf[1024] = {0}; 
    int size ; 


	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "rb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}


	size = fread(buf, 1, sizeof(buf), fp); //!<鐏忓棙鏆熼幑鐢t娴犲孩鏋冩禒鏈佃厬鐠囪鍙嗛崚鐧噓ffer娑擃叏绱濇潻鏂挎礀size 
	fclose(fp);
	if(!size) {
		fprintf(stderr, "%s: Empty or broken\n", filename);
		exit(1);
	}

	
	

    //!<encode BCH 
    ec = asn_decode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_MasterInformationBlock, (void **)&bch_msg, buf, size); 
   
	fclose(fp);
	if(ec.code != RC_OK) {
		fprintf(stderr, "%s: Broken  decoding at byte %ld\n", filename,
		(long)ec.consumed);
		exit(1);
	} else {
		printf("ec.consumed = %d bytes\n",ec.consumed); 
		fprintf(stderr, " %s with PER decode success\n", filename);
	} 
	
	/* Also print the constructed Rectangle XER encoded (XML) */
	//xer_fprint(stdout, &asn_DEF_BCCH_BCH_Message, bch_msg); //!<
    	asn_fprint(stdout, &asn_DEF_MasterInformationBlock, bch_msg);  
    system("pause");
	return 0; /* Encoding finished successfully */
}



int EncodeD2dSib1(int argc, char *argv[])
{
	SystemInformationBlockType1_t  *sib1_msg; 
	FILE *fp;
	asn_enc_rval_t ec; 
	const char *filename; 


	uint16_t *spare = calloc(1, sizeof(uint16_t));
	unsigned char buf[1024]; 
	int  size ; 
	int i;
	long p_max = 30; 
	

	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "wb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}
 	
	/* Allocate the Rectangle_t */
	sib1_msg = calloc(1, sizeof(SystemInformationBlockType1_t)); /* not malloc! */
	if(!sib1_msg) {
		perror("calloc() failed");
		exit(1);
	}


	sib1_msg->p_Max = p_max; 
	
	sib1_msg->tdd_Config.subframeAssignment = (long)TDD_Config__subframeAssignment_dduu;

      sib1_msg->carrierFreq_info.band_info = 0; 
	  sib1_msg->carrierFreq_info.dl_carrier_freq = 2300; //![0-65535]
	  sib1_msg->carrierFreq_info.ul_carrier_freq = 2300; 
	
	sib1_msg->radioResourceConfigCommon.psush_Hop_Config.hop_enable = 0; 

	sib1_msg->radioResourceConfigCommon.ul_ref_signal_pusch.group_hopping_enable = 0; 

	sib1_msg->radioResourceConfigCommon.ul_ref_signal_pusch.seq_hopping_enable = 0; 
	
    //!<encode BCH 
    ec = asn_encode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_SystemInformationBlockType1, sib1_msg, write_out, fp); 
   // ec = asn_encode_to_buffer(0,ATS_UNALIGNED_BASIC_PER,&BCCH_BCH_Message_t, bch_msg,encode_buffer,buffersize); 
	fclose(fp);
	if(ec.encoded == -1) {
		fprintf(stderr, "Could not encode  (at %s)\n",
		 ec.failed_type->name);  //ec.failed_type ? 
		system("pause");
		exit(1);
	} else {
		printf("ec.encoded = %d bytes\n",ec.encoded); 
		fprintf(stderr, "Created %s with PER encoded success\n", filename);

		
		fp = fopen(filename,"rb"); 
		size = fread(buf, 1, sizeof(buf), fp); 
		if (size !=0)
		{
			printf("encode output byte: ");
		    for (i = 0; i < size;i++)
			{
			   fprintf(stdout,"%x", buf[i]); 
			}
			printf("  \n");
		}
	}
	fclose(fp);
	xer_fprint(stdout,&asn_DEF_SystemInformationBlockType1,(void*)sib1_msg);
	//asn_fprint(stdout, &asn_DEF_MasterInformationBlock, bch_msg); 
    system("pause");
	return 0; /* Encoding finished successfully */



}

 

int DecodeD2dSib1(int argc,char * argv[])
{

    SystemInformationBlockType1_t  *decode_msg = NULL; 
	FILE *fp;
	asn_dec_rval_t ec; 
	const char *filename; 
    char buf[1024] = {0}; 
    int size ; 


	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "rb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}


	size = fread(buf, 1, sizeof(buf), fp); //!<鐏忓棙鏆熼幑鐢t娴犲孩鏋冩禒鏈佃厬鐠囪鍙嗛崚鐧噓ffer娑擃叏绱濇潻鏂挎礀size 
	fclose(fp);
	if(!size) {
		fprintf(stderr, "%s: Empty or broken\n", filename);
		exit(1);
	}

	
	

    //!<encode BCH 
    ec = asn_decode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_SystemInformationBlockType1, (void **)&decode_msg, buf, size); 
   
	fclose(fp);
	if(ec.code != RC_OK) {
		fprintf(stderr, "%s: Broken  decoding at byte %ld\n", filename,
		(long)ec.consumed);
		exit(1);
	} else {
		printf("ec.consumed = %d bytes\n",ec.consumed); 
		fprintf(stderr, " %s with PER decode success\n", filename);
	} 
	
	/* Also print the constructed Rectangle XER encoded (XML) */
	//xer_fprint(stdout, &asn_DEF_BCCH_BCH_Message, bch_msg); //!<
    	asn_fprint(stdout, &asn_DEF_SystemInformationBlockType1, decode_msg);  
    system("pause");
	return 0; /* Encoding finished successfully */
}



int EncodeD2dRrcConnectionSetup(int argc, char *argv[])
{
	RRCConnectionSetup_t  *encode_msg; 
	FILE *fp;
	asn_enc_rval_t ec; 
	const char *filename; 

	SRB_ToAddMod_t  *srb_addmod; 


	DRB_ToAddMod_t   *drb_addmod;
	DRB_Identity_t   drb_id; 

	unsigned char buf[2048]; 
	int  size ; 
	int i;
	

	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "wb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}
 	
	/* Allocate the Rectangle_t */
	encode_msg = calloc(1, sizeof(RRCConnectionSetup_t)); /* not malloc! */
	if(!encode_msg) {
		perror("calloc() failed");
		exit(1);
	}

	encode_msg->radioResourceConfigCommon.pusch_dedi_config.beta_off_ack_ind = 0;
	encode_msg->mac_config.maxharq_tx = 4;
	encode_msg->mac_config.max_out_sync	 = 4 ;



	srb_addmod = (calloc(1,sizeof(SRB_ToAddMod_t)));

	  //! step1:srb_add
	(srb_addmod)->srb_Identity = 1; //SRB1
	(srb_addmod)->rlc_config.present = RLC_Config_PR_um_bi_direction; 
	//!尤其注意ASN1C中的定义的枚举，要使用枚举值，而不能自己写int值
	(srb_addmod)->rlc_config.choice.um_bi_direction.ul_um_rlc.sn_FieldLength = SN_FieldLength_size10;
	(srb_addmod)->rlc_config.choice.um_bi_direction.dl_um_rlc.sn_FieldLength = SN_FieldLength_size10; 
	(srb_addmod)->rlc_config.choice.um_bi_direction.dl_um_rlc.t_Reordering = T_Reordering_ms200; 

	(srb_addmod)->logicChannelConfig.logicch_id = 0; //![0-4]
	(srb_addmod)->logicChannelConfig.priority = 1; //![1-16]
	(srb_addmod)->logicChannelConfig.channel_type = LogicChannelConfig__channel_type_ccch; 
	encode_msg->srb_ToAddModList = calloc(1,sizeof(SRB_ToAddModList_t)); 
	
	ASN_SEQUENCE_ADD(&(encode_msg->srb_ToAddModList->list),srb_addmod);

	//!step2: drb add 
	drb_addmod = calloc(1,sizeof(DRB_ToAddMod_t)); 
	encode_msg->drb_ToAddModList = calloc(1,sizeof(DRB_ToAddModList_t)); 
	drb_addmod->drb_Identity = 1; //![1-4]
	drb_addmod->rlc_config.present = RLC_Config_PR_um_bi_direction; 
	drb_addmod->rlc_config.choice.um_bi_direction.dl_um_rlc.sn_FieldLength = SN_FieldLength_size10; 
	drb_addmod->rlc_config.choice.um_bi_direction.dl_um_rlc.t_Reordering = T_Reordering_ms200; 
	drb_addmod->rlc_config.choice.um_bi_direction.ul_um_rlc.sn_FieldLength = SN_FieldLength_size10; 
	drb_addmod->logicChannelConfig.logicch_id = 1; //![0-4]
	drb_addmod->logicChannelConfig.priority = 2; //![1..16]
	drb_addmod->logicChannelConfig.channel_type = LogicChannelConfig__channel_type_dtch; 
	ASN_SEQUENCE_ADD(&(encode_msg->drb_ToAddModList->list),drb_addmod);

	//!step3: drb release, 在connect setup时一般不使用，为了今后扩展使用此消息进行release

	/*
	encode_msg->drb_ToReleaselist = calloc(1,sizeof(DRB_ToReleaseList_t)); 
	drb_id = 1; 
	ASN_SEQUENCE_ADD(&(encode_msg->drb_ToReleaselist->list),&drb_id);
	*/

    //!<encode BCH 
    ec = asn_encode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_RRCConnectionSetup, encode_msg, write_out, fp); 
   // ec = asn_encode_to_buffer(0,ATS_UNALIGNED_BASIC_PER,&BCCH_BCH_Message_t, bch_msg,encode_buffer,buffersize); 
	fclose(fp);
	if(ec.encoded == -1) {
		fprintf(stderr, "Could not encode  (at %s)\n",
		 ec.failed_type->name);  //ec.failed_type ? 
		system("pause");
		exit(1);
	} else {
		printf("ec.encoded = %d bytes\n",ec.encoded); 
		fprintf(stderr, "Created %s with PER encoded success\n", filename);

		
		fp = fopen(filename,"rb"); 
		size = fread(buf, 1, sizeof(buf), fp); 
		if (size !=0)
		{
			printf("encode output byte: ");
		    for (i = 0; i < size;i++)
			{
			   fprintf(stdout,"%x", buf[i]); 
			}
			printf("  \n");
		}
	}
	fclose(fp);
	xer_fprint(stdout,&asn_DEF_RRCConnectionSetup,(void*)encode_msg);
	//asn_fprint(stdout, &asn_DEF_MasterInformationBlock, bch_msg); 
    system("pause");
	return 0; /* Encoding finished successfully */

}

 

int DecodeD2dRrcConnectionSetup(int argc,char * argv[])
{

    RRCConnectionSetup_t  *decode_msg = NULL; 
	FILE *fp;
	asn_dec_rval_t ec; 
	const char *filename; 
    char buf[1024] = {0}; 
    int size ; 


	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "rb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}


	size = fread(buf, 1, sizeof(buf), fp); //!<鐏忓棙鏆熼幑鐢t娴犲孩鏋冩禒鏈佃厬鐠囪鍙嗛崚鐧噓ffer娑擃叏绱濇潻鏂挎礀size 
	fclose(fp);
	if(!size) {
		fprintf(stderr, "%s: Empty or broken\n", filename);
		exit(1);
	}

	
	

    //!<encode BCH 
    ec = asn_decode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_RRCConnectionSetup, (void **)&decode_msg, buf, size); 
   
	fclose(fp);
	if(ec.code != RC_OK) {
		fprintf(stderr, "%s: Broken  decoding at byte %ld\n", filename,
		(long)ec.consumed);
		exit(1);
	} else {
		printf("ec.consumed = %d bytes\n",ec.consumed); 
		fprintf(stderr, " %s with PER decode success\n", filename);
	} 
	
	/* Also print the constructed Rectangle XER encoded (XML) */
	//xer_fprint(stdout, &asn_DEF_BCCH_BCH_Message, bch_msg); //!<
    	asn_fprint(stdout, &asn_DEF_RRCConnectionSetup, decode_msg);  
    system("pause");
	return 0; /* Encoding finished successfully */
}



int EncodeD2dRrcConnectRequest(int argc, char *argv[])
{
	RRCConnectionRequest_t  *encode_msg; 
	FILE *fp;
	asn_enc_rval_t ec; 
	const char *filename; 

	short ueip = 0xf01e; //!15bit,注意这里是按照大端模式编码

	unsigned char buf[1024]; 
	int  size ; 
	int i;
	long p_max = 30; 
	

	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "wb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}
 	
	/* Allocate the Rectangle_t */
	encode_msg = calloc(1, sizeof(RRCConnectionRequest_t)); /* not malloc! */
	if(!encode_msg) {
		perror("calloc() failed");
		exit(1);
	}
	
	encode_msg->ue_Identity = 8; //![0-8]
	encode_msg->establishmentCause = RRCConnectionRequest__establishmentCause_emergency;
	//RRCConnectionRequest__establishmentCause_normal;
	encode_msg->ue_Ip.buf = &ueip; 

	encode_msg->ue_Ip.size = 2; 
	encode_msg->ue_Ip.bits_unused = 1; 


	
	
    //!<encode BCH 
    ec = asn_encode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_RRCConnectionRequest, encode_msg, write_out, fp); 
   // ec = asn_encode_to_buffer(0,ATS_UNALIGNED_BASIC_PER,&BCCH_BCH_Message_t, bch_msg,encode_buffer,buffersize); 
	fclose(fp);
	if(ec.encoded == -1) {
		fprintf(stderr, "Could not encode  (at %s)\n",
		 ec.failed_type->name);  //ec.failed_type ? 
		system("pause");
		exit(1);
	} else {
		printf("ec.encoded = %d bytes\n",ec.encoded); 
		fprintf(stderr, "Created %s with PER encoded success\n", filename);

		
		fp = fopen(filename,"rb"); 
		size = fread(buf, 1, sizeof(buf), fp); 
		if (size !=0)
		{
			printf("encode output byte: ");
		    for (i = 0; i < size;i++)
			{
			   fprintf(stdout,"%x", buf[i]); 
			}
			printf("  \n");
		}
	}
	fclose(fp);
	xer_fprint(stdout,&asn_DEF_RRCConnectionRequest,(void*)encode_msg);
	//asn_fprint(stdout, &asn_DEF_MasterInformationBlock, bch_msg); 
    system("pause");
	return 0; /* Encoding finished successfully */
}




int EncodeD2dRrcConnectRelease(int argc, char *argv[])
{
	RRCConnectionRelease_t  *encode_msg; 
	FILE *fp;
	asn_enc_rval_t ec; 
	const char *filename; 

	short ueip = 0xf01e; //!15bit,注意这里是按照大端模式编码

	unsigned char buf[1024]; 
	int  size ; 
	int i;
	long p_max = 30; 
	

	if(argc < 2) 
	{
		fprintf(stderr, "Specify filename for BER output\n");
	} else 
	{
		filename = argv[1];
		fp = fopen(filename, "wb");
		/* for BER output */
		if(!fp) {
			perror(filename);
			exit(1);
	    }
	}
 	
	/* Allocate the Rectangle_t */
	encode_msg = calloc(1, sizeof(RRCConnectionRequest_t)); /* not malloc! */
	if(!encode_msg) {
		perror("calloc() failed");
		exit(1);
	}
	
	encode_msg->ue_Identity = 8; //![0-8]
	encode_msg->releaseCause = RRCConnectionRelease__releaseCause_normal;
	//RRCConnectionRequest__establishmentCause_normal;



	
	
    //!<encode BCH 
    ec = asn_encode(0,ATS_UNALIGNED_BASIC_PER,&asn_DEF_RRCConnectionRelease, encode_msg, write_out, fp); 
   // ec = asn_encode_to_buffer(0,ATS_UNALIGNED_BASIC_PER,&BCCH_BCH_Message_t, bch_msg,encode_buffer,buffersize); 
	fclose(fp);
	if(ec.encoded == -1) {
		fprintf(stderr, "Could not encode  (at %s)\n",
		 ec.failed_type->name);  //ec.failed_type ? 
		system("pause");
		exit(1);
	} else {
		printf("ec.encoded = %d bytes\n",ec.encoded); 
		fprintf(stderr, "Created %s with PER encoded success\n", filename);

		
		fp = fopen(filename,"rb"); 
		size = fread(buf, 1, sizeof(buf), fp); 
		if (size !=0)
		{
			printf("encode output byte: ");
		    for (i = 0; i < size;i++)
			{
			   fprintf(stdout,"%x", buf[i]); 
			}
			printf("  \n");
		}
	}
	fclose(fp);
	xer_fprint(stdout,&asn_DEF_RRCConnectionRelease,(void*)encode_msg);
	//asn_fprint(stdout, &asn_DEF_MasterInformationBlock, bch_msg); 
    system("pause");
	return 0; /* Encoding finished successfully */
}
