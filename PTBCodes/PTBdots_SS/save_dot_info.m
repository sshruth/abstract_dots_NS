function save_dotInfo(sDir,sFile)

global save_struct screen_struct 

save([sDir sFile '.mat'],'save_struct','screen_struct');


