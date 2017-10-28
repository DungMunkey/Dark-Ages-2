#include "DA2ItemController.h"

bool cItemController::LoadItems(){
	FILE *f;
	int i;
	short count;

	f=fopen("Data/pitems.itm","rb");
	fread(&count,2,1,f);
	for(i=0;i<=count;i++){
		fread(&Items[i].Item,2,1,f);
		fread(&Items[i].WeaponStr,2,1,f);
		fread(&Items[i].WeaponBal,2,1,f);
		fread(&Items[i].ArmorStr,2,1,f);
		fread(&Items[i].ArmorEncumb,2,1,f);
		fread(&Items[i].StrMod,2,1,f);
		fread(&Items[i].AglMod,2,1,f);
		fread(&Items[i].IntMod,2,1,f);
		fread(&Items[i].SpeedMod,2,1,f);
		fread(&Items[i].HPMod,2,1,f);
		fread(&Items[i].MPMod,2,1,f);
		fread(&Items[i].user,2,1,f);
		fread(&Items[i].ItemPic,2,1,f);
		fread(&Items[i].ItemName,25,1,f);
		fread(&Items[i].value,4,1,f);
		fread(&Items[i].Ranged,2,1,f);
		//fread(&Items[i],sizeof(sItem),1,f);
	}
	fclose(f);

	return true;

}