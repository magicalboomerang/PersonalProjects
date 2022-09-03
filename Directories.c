// Likely just needs the full path passed to the recursive PrintSubdirectory function
// To build an iterative path to the file locations

#include <stdio.h>
#include <dirent.h>
#include <string.h>

void PrintSubdirectory(DIR *pOpenDirectory, int iDirectoryLevel);

int main(void)
{
    // opendir() returns a pointer of DIR type. 
    DIR *dr = opendir(".");
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }
  
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
	PrintSubdirectory(dr, 0);
	
    closedir(dr);    
    return 0;
}

void PrintSubdirectory(DIR *pOpenDirectory, int iDirectoryLevel){
	struct dirent *pSubdirectory;
	DIR *pOpenSubDirectory;
	char sLinePrefix[iDirectoryLevel + 1];
	
	memset((void *)sLinePrefix, ' ', iDirectoryLevel);
	sLinePrefix[iDirectoryLevel] = 0;
	
	if(!pOpenDirectory){
		printf("%s---- <! COULD NOT OPEN DIRECTORY !>\n", (char *)sLinePrefix);
		return;
	}
	
	pSubdirectory = readdir(pOpenDirectory);
	
	while(pSubdirectory){
		printf("%s----%s\n",(char *)sLinePrefix, pSubdirectory->d_name);
		if(pSubdirectory->d_type == DT_DIR && strcmp(pSubdirectory->d_name, ".") && strcmp(pSubdirectory->d_name, "..")){
			pOpenSubDirectory = opendir(pSubdirectory->d_name);
			PrintSubdirectory(pOpenSubDirectory, iDirectoryLevel + 1);
			closedir(pOpenSubDirectory);
		}
		pSubdirectory = readdir(pOpenDirectory);
	}
	
}