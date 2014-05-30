/*
  Author:  Chris Ottersen
  Program: lab3.c
  Date:    23 FEB 2013
  VERSION: 1.8
*/
#include"CommonHeader.h"

#include "BinaryTree.h"
#include "Contact.h"
typedef enum MAIN_MENU_CHOICE_P MAIN_MENU_CHOICE;
MAIN_MENU_CHOICE mainMenu(void);



/*
  MAIN_MENU_CHOICE
  description
    used by the main menu function. Holds a value 
    determining what actions the user wishes to 
    take on the tree.
  data:
    EXIT_PROGRAM      save tree, free memory, and exit.
    ADD_CONTACT       add a new contact to tree.
    MODIFY_CONTACT    modify an existing contact.
    DELETE_CONTACT    remove an existing contact.
    PRINT_RECORD      print a specific contact
    PRINT_BY_CRITERIA print all contacts fitting specified
                      criteria.
    PRINT_ALL         print all contacts.

*/
enum MAIN_MENU_CHOICE_P 
{ 
  EXIT_PROGRAM      = 0 ,
  ADD_CONTACT       = 1 ,
  MODIFY_CONTACT    = 2 , 
  DELETE_CONTACT    = 3 ,
  PRINT_RECORD      = 4 ,
  PRINT_BY_CRITERIA = 5 ,
  PRINT_ALL         = 6
};

int main(int argc, char* argv)
{
  TREE* tree;
  int choice;

  FunctionPointers nodeFunctionPointers;
  TreeDataPointers treeDataPointers;
  char* fileAddress = "contacts.data";

  nodeFunctionPointers.newValue		 = (void *(*)(char* key))                         &newContact;
  nodeFunctionPointers.deleteValue = (int (*)(void *value))                         &deleteContact;
  nodeFunctionPointers.saveValue	 = (int (*)(FILE *file, void *value))             &saveContact;
  nodeFunctionPointers.loadValue	 = (void *(*)(FILE *file, char* key))             &loadContact;
  nodeFunctionPointers.edit			   = (int (*)(void *value))                         &editContact;
  nodeFunctionPointers.compareFind = (int (*)(void *value, int type, void *target)) &contactCompareFind;
  nodeFunctionPointers.compareSort = (int (*)(void *value, void *key))              &contactCompareSort;
  nodeFunctionPointers.toString		 = (int (*)(void* value, char* string, int type)) &contactToString;
  treeDataPointers.findMenu			   = (int (*)())                                    &findMenu;
  treeDataPointers.prompt			     = (int (*)(int type, void* input))               &prompt;
  treeDataPointers.tableHeader	   =                                                &contactTableHeader;
  treeDataPointers.fileAddress	   =                                                 fileAddress;
  
  tree = newBinaryTree(&treeDataPointers, &nodeFunctionPointers);
  choice = 1;
  while(choice)
  {
    char* inputBuf;
    CONTACT_FIELD type;

    
    choice = mainMenu();
    switch(choice)
    {
      case ADD_CONTACT: 
              newElement(tree, NULL);
              break;
      case MODIFY_CONTACT: 
              modifyElement(tree, MODIFY);
				      break;
      case DELETE_CONTACT: 
              modifyElement(tree, REMOVE);
              break;
      case PRINT_RECORD: 
              printEntry(tree);
              break;
      case PRINT_BY_CRITERIA: 
              inputBuf = valueBufferC;
              inputBuf[0] = '\0';

              type = printByMenu();
              if(type == 1) break;

              prompt(type, inputBuf);
              printTree(tree, (FIND_BY) type, inputBuf);

              inputBuf[0] = '\0';
              break;
      case PRINT_ALL: 
              printTree(tree, BY_KEY, NULL);
              break;
      case EXIT_PROGRAM: 
              saveTree(tree);
              deleteTree(tree);
    }
    PAUSE
  }

}

/*
  mainMenu
  description
    displays main menu and collects user's choice.
  params:
    void
  return: 
    int   menu choice
*/
MAIN_MENU_CHOICE mainMenu(void)
{
  MAIN_MENU_CHOICE choice;
  CLEAR
  printf("WELCOME TO CONTACT TREE SET MENU!!\n\n");
  printf("1. Inserting a new structure into the tree set.\n");
  printf("2. Modifying an existing structure in the tree set.\n");
  printf("3. Delete an existing structure from the tree.\n");
  printf("4. Print a specific name and phone number (first occurance).\n");
  printf("5. Print a specific name and phone number (all fitting criteria).\n");
  printf("6. Print all names and phone numbers.\n");
  printf("0. Exit from program.\n");
  scanf("%d", &choice);
  FLUSH
  while(choice > 6 || choice < 0)
  {
    CLEAR
    printf("WELCOME TO CONTACT TREE SET MENU!!\n\n");
    printf("   input '%d' invalid; input must be among those listed.\n", choice);
    printf("1. Inserting a new structure into the tree set.\n");
    printf("2. Modifying an existing structure in the tree set.\n");
    printf("3. Delete an existing structure from the tree.\n");
    printf("4. Print a specific name and phone number (first occurance).\n");
    printf("5. Print a specific name and phone number (all fitting criteria).\n");
    printf("6. Print all names and phone numbers.\n");
    printf("0. Exit from program.\n");
    scanf("%d", &choice);
    FLUSH
  }

  return choice;

}

