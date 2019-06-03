

class MBL_Data					//Motion Blur
{
public:
		MBL_Data();
	       ~MBL_Data();
	int	Init(void);

	int 	MBL_Type;			//Objecttype
	int 	MBL_Plugins[16];		//Plugins...

	byte   *DBuf;

private:
};

extern		void 	MBL_Process(void);



class BLN2_Data
{
public:
		BLN2_Data();
	       ~BLN2_Data();
	int	Init(void);

	int	BLN2_Type;
	int	BLN2_Plugins[16];

	byte   *Picture;
private:
};


class BLN3_Data
{
public:
		BLN3_Data();

	int	BLN3_Type;
	int	BLN3_Plugins[16];

	int	BLN3_StartTime;
	int	BLN3_FadingTime;

	float	BLN3_Opacity0;
	float	BLN3_Opacity1;

	int	BLN3_BlendR;
	int	BLN3_BlendG;
	int	BLN3_BlendB;
private:
};

extern		void 	BLN2_Process(void);
extern		void 	BLN3_Process(void);
extern   "C"    void 	ASM_CALL BLN2(void);
