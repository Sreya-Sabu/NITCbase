#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include<cstring>
#include<bits/stdc++.h>
using namespace std;

// int main(int argc, char *argv[]) {
//   Disk disk_run;

//   // create objects for the relation catalog and attribute catalog
//   RecBuffer relCatBuffer(RELCAT_BLOCK);
//   RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

//   HeadInfo relCatHeader;
//   HeadInfo attrCatHeader;

//   // load the headers of both the blocks into relCatHeader and attrCatHeader.
//   // (we will implement these functions later)
//   relCatBuffer.getHeader(&relCatHeader);
//   attrCatBuffer.getHeader(&attrCatHeader);

//   for (int i=0;i< relCatHeader.numEntries;i++) {

//     Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

//     relCatBuffer.getRecord(relCatRecord, i);

//     printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

//     for (int j=0;j<attrCatHeader.numEntries;j++) {

//       // declare attrCatRecord and load the attribute catalog entry into it
//       Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
//       attrCatBuffer.getRecord(attrCatRecord,j);

//        if(strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal , relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0){
//         const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER? "NUM" : "STR";
//         printf(" %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
//       }
//     }
//     printf("\n");
//   }

//   return 0;
// }


// //EX 1
// int main(int argc ,char* argv[]){
//   Disk disk_run;
  
//   //create an object for the recordcatalog
//   RecBuffer relCatBuffer(RELCAT_BLOCK);
  

//   // got the header info from relcat
//   HeadInfo relCatHeader;
//   relCatBuffer.getHeader(&relCatHeader);

//   for(int i=0;i < relCatHeader.numEntries;i++){

//     // record is an array of attributes so we have to declare an array of attributes for printing the relation name from each record of relcat
//     Attribute relCatRecord[RELCAT_NO_ATTRS];

//     // now we have to call getrecord fn to get the record from relCat
//     relCatBuffer.getRecord(relCatRecord,i);
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
//EX2
int main(int argc , char*argv[]){
  Disk disk_run;

  //an object only for relcatalog
  RecBuffer RelCatBlock(RELCAT_BLOCK);
  HeadInfo RelCathead;
  RelCatBlock.getHeader(&RelCathead);

  for(int i=0;i<RelCathead.numEntries;i++){
    Attribute RelCatRecord[RELCAT_NO_ATTRS];
    RelCatBlock.getRecord(RelCatRecord,i);
    printf("Relation : %s\n",RelCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    int currentAttrBlock = ATTRCAT_BLOCK;
    while(currentAttrBlock != -1){
      RecBuffer AttrCatBlock(currentAttrBlock);
      HeadInfo AttrCathead;
      AttrCatBlock.getHeader(&AttrCathead);
      for(int j=0;j<AttrCathead.numEntries;j++){
        Attribute AttrCatRecord[ATTRCAT_NO_ATTRS];
        AttrCatBlock.getRecord(AttrCatRecord,j);
        if(strcmp(AttrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,RelCatRecord[RELCAT_REL_NAME_INDEX].sVal)==0){
          if(strcmp(AttrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,"Class")==0 && strcmp(AttrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,"Students")==0){
              strcpy(AttrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, "Batch");
              AttrCatBlock.setRecord(AttrCatRecord,j);
          }
          const char* attrType = AttrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER? "NUM":"STR";
          printf(" %s: %s\n",AttrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,attrType);
        }
      }
      currentAttrBlock = AttrCathead.rblock;
    }
    printf("\n");
  }
  return 0;
}
