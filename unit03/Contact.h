
#include"CommonHeader.h"
//size of first and last names as they will be stored
#define NAME_SIZE 20

typedef struct CONTACT_P CONTACT;

typedef enum CONTACT_FIELD_P CONTACT_FIELD;
typedef enum PRINT_TYPE_P PRINT_TYPE;

CONTACT* newContact(char* key);
  int initContact(CONTACT* contact);
  CONTACT* loadContact(FILE* file, char* key);

int deleteContact(CONTACT* contact);
  int saveContact(FILE* file, CONTACT* contact);

int prompt(CONTACT_FIELD type, void* input);
  int promptFirstName(char* firstName);
  int promptLastName(char* lastName);
  int promptPhoneNumber(short* phoneNumber);
  int promptAreaCode(short* areaCode);

int setFirstNameP(CONTACT* contact);
  int setFirstName(CONTACT* contact, char* firstName);
char* getFirstName(CONTACT* contact);


int setLastNameP(CONTACT* contact);
  int setLastName(CONTACT* contact, char* lastName);
char* getLastName(CONTACT* contact);


int setPhoneNumberP(CONTACT* contact);
  int setPhoneNumber(CONTACT* contact, short* phoneNumber);
short* getPhoneNumberNum(CONTACT* contact);

int getPhoneNumberString(CONTACT* contact, char* number);

int editContact(CONTACT* contact);
  CONTACT_FIELD editContactMenu(CONTACT contact, CONTACT tempContact);

int contactCompareFind(CONTACT* contact, CONTACT_FIELD type, void* target);
  CONTACT_FIELD printByMenu(void);
  CONTACT_FIELD findMenu(void);
  int compareFirstName(CONTACT* contact, char* name);
  int compareLastName(CONTACT* contact, char* name);
  int compareNumber(CONTACT* contact, short* number);

int contactCompareSort(CONTACT* contact, CONTACT* input);
  int genKey(CONTACT* contact);
  char* getKey(CONTACT* contact);

int contactToString(CONTACT* contact, char* string, PRINT_TYPE type);
int contactTableHeader(char* string);

/*
  CONTACT_FIELD
  description
    enumeration representing applicable menu outputs.
  data:
    SAVE_CONTACT
                relevant only to the modify function, signaling 
                that changes to the contact should be saved 
                should be saved.
    LAST_NAME   menu output indicating that the user wishes to 
                take actions on the last name field.
    FIRST_NAME  menu output indicating that the user wishes to 
                take actions on the first name field.
    PHONE_NUMBER
                menu output indicating that the user wishes to 
                take actions on the phone number field.
    AREA_CODE   input indicating that contacts are to be printed
                by area code.

*/
enum CONTACT_FIELD_P 
{ 
  SAVE_CONTACT = 1, 
  LAST_NAME    = 2,
  FIRST_NAME   = 3, 
  PHONE_NUMBER = 4,
  AREA_CODE    = 5
};

/*
  PRINT_TYPE
  description
    enumeration representing whether the user wishes to print 
    contact info in full length or shortened (for a table).
  data:
    PRINT_SHORT print shortened string.
    PRINT_LONG  print full string.

*/
enum PRINT_TYPE_P
{

  PRINT_SHORT  = 0,
  PRINT_LONG   = 1
};

/*
  CONTACT
  description
    struct containing contact info.
    Designed to interact with the structure 
    defined in the LinkedList header file.
  data:
    firstName   contact first name.
    lastName    contact last name.
    phoneNumber an array of 3 shorts containing 
                area code, prefix, and line
                number respectively
    key         string containing the contacts
                'hash' value. It is used for
                sorting a list of contacts.
*/
struct CONTACT_P{
  char firstName[NAME_SIZE+1];
  char lastName[NAME_SIZE+1];
  short phoneNumber[3];
  char* key;
};

/*

  newContact
  description
    creates a new CONTACT struct using the 
    malloc function returning a pointer
    to the new struct. 
  params:
    void
  return: 
    contact*  a void pointer pointing to the 
              newly created contact.
*/

CONTACT* newContact(char* key)
{

  CONTACT* contact = (CONTACT*) malloc(sizeof(CONTACT));
  short phoneNumber[] = {0,0,0};
  char emptyName[1] = {'\0'};

  if(contact == NULL)
  {
    printf("sufficient memory could not be allocated to create contact");
    PAUSE
    exit(0);
  }
  contact->key = key;
  contact->key[0] = '\0';
  setPhoneNumber(contact, phoneNumber);
  setFirstName(contact, emptyName);
  setLastName(contact, emptyName);

  initContact(contact);

  return contact;
}

/*
  initContact
  description
    generates contact data based on user input.
  params:
    contact*  pointer to the contact being modified.
  return: 
    NULL      0 value indicating successful exicution.
*/
int initContact(CONTACT* contact)
{
  setFirstNameP(contact);
  setLastNameP(contact);
  setPhoneNumberP(contact);
  return 0;
}

/*
  readContactFromUser
  description
    generates contact data based on user input.
  params:
    file*     pointer to the file being loaded.
  return: 
    contact*  pointer to the contact being modified.
*/
CONTACT* loadContact(FILE* file, char* key)
{
  CONTACT* contact = (CONTACT*) malloc(sizeof(CONTACT));
	fread(contact, sizeof(CONTACT), 1, file);
  contact->key = key;
  genKey(contact);
  if(feof(file))
  {
    free(contact);
    contact = NULL;
  }
  return contact;
}

/*
  deleteContact
  description
    free's memory dynamically allocated to contact.
  params:
    contact*  pointer to the contact being deleted.
  return: 
    NULL      0 value indicating successful exicution.
*/
int deleteContact(CONTACT* contact)
{
  free(contact);
  return 0;
}

/*
  saveContact
  description
    saves contact info to provided file.
  params:
    file*     pointer to the file being saved to.
    contact*  pointer to the contact being saved.
  return: 
    NULL      0 value indicating successful exicution.
*/
int saveContact(FILE* file, CONTACT* contact)
{
	
  fwrite(contact, sizeof(CONTACT), 1, file);
  
  return 0;
}


/*
  prompt
  description
    encapsulation of prompt functions in order to
    make the contact more generic.
  params:
    type      the type of input which needs to be
              prompted for.
    input*    a void pointer to a BUFFER byte char array
              allowing different datatypes to be 
              returned.
  return: 
    NULL      0 value indicating successful exicution.
*/
int prompt(CONTACT_FIELD type, void* input)
{

  switch(type)
  {
    case           -1:   return NULL;
    case    LAST_NAME:   return promptLastName((char*) input);
    case   FIRST_NAME:   return promptFirstName((char*) input);
    case PHONE_NUMBER:   return promptPhoneNumber((short*) input);
    case    AREA_CODE:   return promptAreaCode((short*) input);
    default: 
            printf("invalid input");
            PAUSE
            return NULL;
  }
}

/*
  promptFirstName
  description
    prompts the user for and collect contact first name.
  params:
    firstName string pointer which will hold  
              first name input. 
    
  return: 
    NULL      0 value indicating successful exicution.
*/
int promptFirstName(char* firstName)
{
  printf("please enter (in %d charicters or less) the contact's first name.", NAME_SIZE);
  FLUSH
  scanf("%s", firstName);

  FLUSH
  return 0;
}

/*
  promptLastName
  description
    prompts the user for and collect contact last name.
  params:
    lastName  string pointer which will hold  
              last name input. 
    
  return: 
    NULL      0 value indicating successful exicution.
*/
int promptLastName(char* lastName)
{
  printf("please enter (in %d charicters or less) the contact's last name.", NAME_SIZE);
  FLUSH
  scanf("%s", lastName);
  FLUSH
  return 0;
}

/*
  promptPhoneNumber
  description
    prompts the user for and collect contact phone number.
  params:
    phoneNumber  
              short int array with 3 elements to hold the 
              area code, prefix and line number respectively. 
    
  return: 
    NULL      0 value indicating successful exicution.
*/
int promptPhoneNumber(short* phoneNumber)
{

  printf("please enter contact's phone number in the format: xxx xxx xxxx.");
  FLUSH
  //valideate phone number input
  scanf("%hu %hu %hu", &phoneNumber[0], &phoneNumber[1], &phoneNumber[2]);
  if(  phoneNumber[0] > 999 
    || phoneNumber[0] < 0
    || phoneNumber[1] > 999 
    || phoneNumber[1] < 0
    || phoneNumber[2] > 9999 
    || phoneNumber[2] < 0)
  {
	  printf("invalid phone number format");
	  promptPhoneNumber(phoneNumber);
  }
  FLUSH
  return 0;
}

/*
  promptAreaCode
  description
    prompts the user for and collect contact area code.
  params:
    areaCode  an array of short ints of size 3. only the first 
              element (representing the area code) will be filled.
    
  return: 
    NULL      0 value indicating successful exicution.
*/
int promptAreaCode(short* areaCode)
{

  printf("please enter contact's area code: xxx.");
  FLUSH
  //valideate area code input
  scanf("%hu", &areaCode[0]);
  if(areaCode[0] > 999 || areaCode[0] < 0)
  {
	  printf("invalid area code format");
	  promptAreaCode(areaCode);
  }
  FLUSH
  return 0;
}

/*
  setFirstNameP
  description
    prompts for and sets contact first name.
  params:
    contact*  the contact who's first name is being
              set.    
  return: 
    NULL      0 value indicating successful exicution.
*/
int setFirstNameP(CONTACT* contact)
{
  char* name = valueBufferA;

  promptFirstName(name);
  setFirstName(contact, name);

  //clear buffer
  name[0] = '\0';
  return 0;
}

/*
  setFirstName
  description
    prompts for and sets contact first name.
  params:
    contact*  the contact who's first name is being
              set.    
    firstName the first name which is being assigned 
              to the contact.
  return: 
    NULL      0 value indicating successful exicution.
*/
int setFirstName(CONTACT* contact, char* firstName)
{
  strncpy(getFirstName(contact), firstName, NAME_SIZE);
  if(strlen(firstName)>NAME_SIZE)
  {
    getFirstName(contact)[NAME_SIZE] = '\0';
    getFirstName(contact)[NAME_SIZE-1] = '+';
  }
  genKey(contact);
  return 0;
}

/*
  getFirstName
  description
    returns a pointer to contacts first name.
  params:
    contact*  the contact who's first name is being
              retrieved.    
  return: 
    firstName pointer to the contacts first name.
*/
char* getFirstName(CONTACT* contact)
{
  return contact->firstName;
}


/*
  setLastNameP
  description
    prompts for and sets contact last name.
  params: 
    contact*  the contact who's last name is being
              set.    
  return: 
    NULL      0 value indicating successful exicution.
*/
int setLastNameP(CONTACT* contact)
{
  char* name = valueBufferA;

  promptLastName(name);
  setLastName(contact, name);

  //clear buffer
  name[0] = '\0';
  return 0;
}

/*
  setLastName
  description
    prompts for and sets contact last name.
  params:
    contact*  the contact who's last name is being
              set.    
    firstName the last name which is being assigned 
              to the contact.
  return: 
    NULL      0 value indicating successful exicution.
*/
int setLastName(CONTACT* contact, char* lastName)
{
  strncpy(getLastName(contact), lastName, NAME_SIZE);
  if(strlen(lastName)>NAME_SIZE)
  {
    getLastName(contact)[NAME_SIZE] = '\0';
    getLastName(contact)[NAME_SIZE-1] = '+';
  }

  genKey(contact);
  return 0;
}

/*
  getLastName
  description
    returns a pointer to contacts last name.
  params:
    contact*  the contact who's last name is being
              retrieved.    
  return: 
    firstName pointer to the contacts first name.
*/
char* getLastName(CONTACT* contact)
{
  return contact->lastName;
}


/*
  setPhoneNumberP
  description
    prompts for and sets contact last phone number.
  params: 
    contact*  the contact who's phone number is being
              set.    
  return: 
    NULL      0 value indicating successful exicution.
*/
int setPhoneNumberP(CONTACT* contact)
{
  short phoneNumber[3];

  promptPhoneNumber(phoneNumber);
  setPhoneNumber(contact, phoneNumber);
  return 0;
}

/*
  setPhoneNumber
  description
    prompts for and sets contact phone number.
  params:
    contact*  the contact who's phone number is being
              set.    
    firstName the phone number which is being assigned 
              to the contact.
  return: 
    NULL      0 value indicating successful exicution.
*/
int setPhoneNumber(CONTACT* contact, short* phoneNumber)
{
  contact->phoneNumber[0] = phoneNumber[0];
  contact->phoneNumber[1] = phoneNumber[1];
  contact->phoneNumber[2] = phoneNumber[2];

  genKey(contact);
  return 0;
}

/*
  getPhoneNumberNum
  description
    returns a pointer to contacts phone number.
  params:
    contact*  the contact who's phone number is being
              retrieved.    
  return: 
    phoneNumber
              pointer to the contacts phone number field.
*/
short* getPhoneNumberNum(CONTACT* contact)
{
  return contact->phoneNumber;
}

/*
  getPhoneNumberString
  description
    formats contacts phone number as a string and places it
    into a provided character string.
  params:
    contact*  the contact who's phone number is being
              retrieved.    
    number*   the character string which is to hold the 
              contacts phone number.
  return: 
    NULL      0 value indicating successful exicution.
*/
int getPhoneNumberString(CONTACT* contact, char* number)
{
  short* phoneNumber = getPhoneNumberNum(contact);

  sprintf(number, "(%.3hu) %.3hu-%.4hu", contact->phoneNumber[0], contact->phoneNumber[1], contact->phoneNumber[2]);
  return 0;
}


/*
  editContact
  description
    calls the editContactMenu function and processes 
    the user's instructions.
  params:
    contact*  the contact who's data is to be edited.
  return: 
    NULL      0 value indicating successful exicution.
*/
int editContact(CONTACT* contact)
{
  CONTACT tempContact = *contact;
  CONTACT_FIELD choice = editContactMenu(*contact, tempContact);

  while(choice)
  {
    CLEAR
    switch(choice)
    {
      case SAVE_CONTACT: *contact = tempContact;
		                     break;
	    case    LAST_NAME: setLastNameP(&tempContact);
                         break;
      case   FIRST_NAME: setFirstNameP(&tempContact);
                         break;
      case PHONE_NUMBER: setPhoneNumberP(&tempContact);
                         break;
    }

    choice = editContactMenu(*((CONTACT*) contact), tempContact);
  }
  return 0;
}

/*
  editContactMenu
  description
    determine which fields of the contact structure
	  the user wishes to modify.
  params:
    contact   the contact which is being modified
    tempContact
              the dummy contact being modified 
              holding unsaved updates
  return: 
    choice    the user's instructions.
*/
CONTACT_FIELD editContactMenu(CONTACT contact, CONTACT tempContact)
{
  CONTACT_FIELD choice;
  char string_contact[NAME_SIZE*2 + 25], string_temp[NAME_SIZE*2 + 25];
  contactToString(&contact, string_contact, PRINT_LONG);
  contactToString(&tempContact, string_temp, PRINT_LONG);
  CLEAR
  printf("saved contact:         %s\n", string_contact); 
  printf("contact being edited:  %s\n", string_temp); 
  printf("1. Save contact.\n");
  printf("2. Edit contact last name.\n");
  printf("3. Edit contact first name.\n");
  printf("4. Edit contact phone number.\n");

  printf("0. Exit (to Menu).\n");
  FLUSH
  scanf("%d", &choice);
  FLUSH
  while(choice > 4 || choice < 0)
  {
    CLEAR
    printf("   input '%d' invalid; input must be among those listed.\n", choice);
    printf("saved contact:         %s", string_contact); 
    printf("contact being edited:  %s", string_temp); 
    printf("1. Save contact.\n");
    printf("2. Edit contact last name.\n");
    printf("3. Edit contact first name.\n");
    printf("4. Edit contact phone number.\n");
    printf("0. Exit (to Menu).\n");
    FLUSH
    scanf("%d", &choice);
    FLUSH
  }
  return choice;

}


/*
  contactCompareFind
  description
    carry out a comparison between the contact provided and the
    target info in the interest of determining equality.
  params:
    contact   the contact to which a comparison is being made.
    type      the type of comparison being carried out.
    target    the value being searched for.
  return: 
    comparison
              the product of the ordinal comparison functions.
*/
int contactCompareFind(CONTACT* contact, CONTACT_FIELD type, void* target)
{

  if(contact == NULL)
  {
    return NULL;
  }
  switch(type) 
  {
    //should never execute

    case    LAST_NAME: return compareLastName(contact, (char*) target);
    case   FIRST_NAME: return compareFirstName(contact, (char*) target);
    case PHONE_NUMBER: return compareNumber(contact, (short*) target);
		case    AREA_CODE: return compareNumber(contact, (short*) target) <= 1;

    //exit
    case 0: return 0;
            
    default: 
            printf("invalid input");
    
            PAUSE
            return NULL;
  }

}

/*
  findMenu
  description
    determine which field of the contact structure
	  the user wishes to use to find the desired contact.
  params:
    void
  return: 
    choice    the users menu selection
*/
CONTACT_FIELD findMenu(void)
{
  CONTACT_FIELD choice;
  printf("1. Locate contact by index.\n");
  printf("2. Locate contact by last name.\n");
  printf("3. Locate contact by first name.\n");
  printf("4. Locate contact by phone number.\n");
  printf("5. Locate contact by area code.\n");
  printf("0. Exit (to Menu).\n");
  scanf("%d", &choice);
  while(choice > 5 || choice < 0)
  {
    CLEAR
    printf("   input '%d' invalid; input must be among those listed.\n", choice);
    printf("1. Locate contact by index.\n");
    printf("2. Locate contact by last name.\n");
    printf("3. Locate contact by first name.\n");
    printf("4. Locate contact by phone number.\n");
    printf("5. Locate contact by area code.\n");
    printf("0. Exit (to Menu).\n");
    FLUSH
    scanf("%d", &choice);
    FLUSH
  }
  return choice;

}

/*
  printByMenu
  description
    determine which standard to use to display data.
  params:
    void
  return: 
    choice    the users menu selection
*/
CONTACT_FIELD printByMenu(void)
{
  CONTACT_FIELD choice;
  printf("1. Print iff leaf.\n");
  printf("2. Locate contacts by last name.\n");
  printf("3. Locate contacts by first name.\n");
  printf("4. Locate contacts by phone number.\n");
  printf("5. Locate contact by area code.\n");
  printf("0. Exit (to Menu).\n");
  scanf("%d", &choice);
  while(choice > 5 || choice < 0)
  {
    CLEAR
    printf("   input '%d' invalid; input must be among those listed.\n", choice);
    printf("1. Print iff leaf.\n");
    printf("2. Locate contacts by last name.\n");
    printf("3. Locate contacts by first name.\n");
    printf("4. Locate contacts by phone number.\n");
    printf("5. Locate contact by area code.\n");
    printf("0. Exit (to Menu).\n");
    FLUSH
    scanf("%d", &choice);
    FLUSH
  }
  //if user wishes to display contacts which are only leaves
  //in the tree structure, then their input of 1, should be
  //repackaged as -1 indicating the unusual request.
  return (CONTACT_FIELD) ((choice != 1) ? choice : -1);

}

/*
  compareLastName
  description
    compare equality based on last name.
  params:
    contact   the contact to which a comparison is being made.
    name      the name being compared against.
  return: 
    comparison
              the product of the comparison function.
*/
int compareLastName(CONTACT* contact, char* name)
{
  int compare = strcmp( contact->lastName, (char*) name);

  return compare;
}

/*
  compareFirstName
  description
    compare equality based on first name.
  params:
    contact   the contact to which a comparison is being made.
    name      the name being compared against.
  return: 
    comparison
              the product of the comparison function.
*/
int compareFirstName(CONTACT* contact, char* name)
{
  int compare = strcmp(contact->firstName, (char*) name);
  return compare;
}

/*
  compareNumber
  description
    compare equality based on phone number or area code.
  params:
    contact   the contact to which a comparison is being made.
    number    the phone number or area code being compared.
              against.
  return: 
    comparison
              the first element in which the contact and input 
              numbers differ.
                1) indicates area codes do not match.
                2) indicates area codes do but prifixes do not.
                3) difference only exist between line numbers.
                0) they are identical.
*/
int compareNumber(CONTACT* contact, short*  number)
{
  if(contact->phoneNumber[0] != number[0])
  {
    return 1;
  }
  else if(contact->phoneNumber[1] != number[1])
  {
    return 2;
  }
  else if(contact->phoneNumber[2] != number[2])
  {
    return 3;
  }
  else
  {
    return 0;
  }
}

/*
  contactCompareSort
  description
    compare ordinally based on hash key.
  params:
    contact   the contact to which a comparison is being made.
    input     key value being compared against.
  return: 
    comparison
              the ordinal comparison of contact key values.
*/
int contactCompareSort(CONTACT* contact, CONTACT* input)
{
  char* contactKey = contact->key;
  char* inputKey = input->key;
  int comp = strcmp(contactKey, inputKey);
  return comp;
}

/*
  genKey
  description
    create contact hash key.
  params:
    contact   contact for which key is being generated.
  return: 
    NULL      0 value indicating successful exicution.
*/
int genKey(CONTACT* contact)
{
  //key reset

  contact->key[0] = '\0';
  //phone number area code followed by last name
  sprintf(contact->key, "%s%s%d%d%d", contact->lastName, contact->firstName, contact->phoneNumber[0], contact->phoneNumber[1], contact->phoneNumber[2]);
  return 0;
}

/*
  getKey
  description
    returns hash key.
  params:
    contact   contact who's key is required.
  return: 
    key       contact's key.
*/
char* getKey(CONTACT* contact)
{
  return contact->key;
}

/*
  contactToString
  description
    populates contact string.
  params:
    contact   contact who's info is required.
    string    string to be filled with contact info.
    type      determines how long the names should be,
              0 cuts the names short (for a table)
              and 1 leaves them at their original size.
  return: 
    NULL      0 value indicating successful exicution.
*/
int contactToString(CONTACT* contact, char* string, PRINT_TYPE type)
{
  //truncated printing for table
	if(type == PRINT_SHORT)
	{
	  char name[NAME_SIZE*2 + 3];
	  char number[25];

	  getPhoneNumberString(contact, number);
	  sprintf(name, "%10.10s     %10.10s", getLastName(contact), getFirstName(contact));

	  if(strlen(getLastName(contact)) > 10)
	  {
		  name[9] = '+';
	  }
	  if(strlen(getFirstName(contact)) > 10)
	  {
		  name[24] = '+';
	  }

	  sprintf(string, "%s     %s", name, number);
	}
  //print table with untruncated data
	else if(type == PRINT_LONG)
	{
    char name[NAME_SIZE*2 + 10];
	  char number[25];

	  getPhoneNumberString(contact, number);

	  sprintf(name, "%s     %s", getLastName(contact), getFirstName(contact));
	  sprintf(string, "%s     %s", name, number);
	}
  return 0;
}

/*
  contactTableHeader
  description
    populates contact table header string.
  params:
    string    string to be filled with table header.
  return: 
    NULL      0 value indicating successful exicution.
*/
int contactTableHeader(char* string)
{
  sprintf(string, "%10.10s     %10.10s     %s", "Last Name", "First Name", "Phone Number");

  return 0;
}