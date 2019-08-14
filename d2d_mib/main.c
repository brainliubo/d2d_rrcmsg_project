extern int EncodeD2dMib(int argc, char *argv[]);
extern int DecodeD2dMib(int argc,char * argv[]);
extern int EncodeD2dSib1(int argc, char *argv[]); 
extern int DecodeD2dSib1(int argc, char *argv[]); 
extern int EncodeD2dRrcConnectionSetup(int argc, char *argv[]); 
extern int DecodeD2dRrcConnectionSetup(int argc,char * argv[]);
extern int EncodeD2dRrcConnectRequest(int argc, char *argv[]);

extern int EncodeD2dRrcConnectRelease(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	//EncodeD2dMib( argc, argv);
	//DecodeD2dMib( argc, argv);
	//EncodeD2dSib1(argc,argv);
	//DecodeD2dSib1(argc,argv);

	//EncodeD2dRrcConnectionSetup(argc,argv);
	//DecodeD2dRrcConnectionSetup(argc,argv);
	//EncodeD2dRrcConnectRequest(argc,argv);
	EncodeD2dRrcConnectRelease(argc,argv);
	return 0;
}