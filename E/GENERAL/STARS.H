#include "w:\general\var.h"
#include "w:\vectors\scn_run.h"

struct Star
{
	float	X, Y, Z;
};


class STR_StarField
{
public:
		STR_StarField();
	       ~STR_StarField();
	int 	Init(int NumStars, SCN_Header *SCN);
	void 	SetRange(float XMin, float YMin, float ZMin,
			 float XMax, float YMax, float ZMax);
	void 	SetSpeed(float XSpeed, float YSpeed, float ZSpeed);
static  void	STR_Run(void);

	int 	STR_Type;
	int	STR_Plugins[16];

	int	STR_NumStars;

	float	STR_XMin, STR_YMin, STR_ZMin;
	float	STR_XMax, STR_YMax, STR_ZMax;

	float	STR_XSpeed, STR_YSpeed, STR_ZSpeed;//Meters per second

	SCN_Header *STR_SCN;

	Star   *STR_StarTable;

private:
};


































