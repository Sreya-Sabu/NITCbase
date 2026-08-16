#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include<cstring>
#include<bits/stdc++.h>
using namespace std;

// int main(int argc ,char* argv[]){
//   Disk disk_run;
//   StaticBuffer buffer;
//   OpenRelTable cache;
//   //create an object for the recordcatalog
//   RecBuffer relCatBuffer(RELCAT_BLOCK);
  

//   // got the header info from relcat
//   HeadInfo relCatHeader;
//   relCatBuffer.getHeader(&relCatHeader);

//   for(int i=0;i < relCatHeader.numEntries;i++){

//     Attribute relCatRecord[RELCAT_NO_ATTRS]; // record is an array of attributes so we have to declare an array of attributes for printing the relation name from each record of relcat
    
//     relCatBuffer.getRecord(relCatRecord,i); // now we have to call getrecord fn to get the record from relCat
//     // from the record we have to identify the position of the rel name(rember that attribute could be STR or NUM but name gotta be STR so we do .sVal)
//     printf("Relation: %s\n",relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

//     //for printing all the attributes of thet relation
//     //identify the first attribute block
//     int currentBlockattr = ATTRCAT_BLOCK;

//     while(currentBlockattr != -1){
//       RecBuffer attrCatBlock(currentBlockattr);

//       // we have a attrblock here now we have to traverse through the records so we need the numentries

//       HeadInfo headattr;
//       attrCatBlock.getHeader(&headattr);

//       for(int j=0; j<headattr.numEntries;j++){

//         Attribute attrRecord[ATTRCAT_NO_ATTRS];
//         attrCatBlock.getRecord(attrRecord,j);

//         // we got the record of attrcatalog block now we have to check if the relation name in the record is same as our entry from relcat blk
//         if(strcmp(attrRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal)==0){
//           const char* attrType = attrRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
//           printf(" %s %s\n",attrRecord[ATTRCAT_ATTR_NAME_INDEX].sVal , attrType);
//         }
//       }
//       currentBlockattr = headattr.rblock;
//     }
//     printf("\n");
//   }
//   return 0;
// }

int main(int argc,char*argv[]){
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  RelCatEntry relEntry;
  AttrCatEntry attrEntry;

  //RELCAT_RELid = 0 ATTRCAT_RELID = 1;
  for(int i=0;i<3;i++){
    RelCacheTable::getRelCatEntry(i,&relEntry);
    printf("Relation: %s\n",relEntry.relName);

    for(int j=0;j<relEntry.numAttrs;j++){    // instead of 5 i gave relEntry.numAttr cuzue student has only 4 so if 5 given btach printed twice
      AttrCacheTable::getAttrCatEntry(i,j,&attrEntry);
      printf(" %s %s\n",attrEntry.attrName,(attrEntry.attrType==NUMBER)?"NUM":"STR");
    }
    printf("\n");
  }
  // for ex we add student table as well so we give i<3
  return 0;
}