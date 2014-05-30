#include"CommonHeader.h"

typedef struct FunctionPointersP FunctionPointers;
typedef struct NODE_P NODE;


NODE* newNode(FunctionPointers* functionPointers, int *staticIndex, FILE* file);
int initNode(NODE* node);
int deleteNode(NODE* node, int clear);

int saveNode(NODE* node, FILE* file);
int loadNode(NODE* node, FILE* file);

void* getValue(NODE* node);
int initValue(NODE* node);
int deleteValue(NODE* node);
int setValue(NODE* node, void* value);

NODE* getGreater(NODE* node);
int setGreater(NODE* node, NODE* greater);

NODE* getLess(NODE* node);
int setLess(NODE* node, NODE* less);

int getIndex(NODE* node);
int setIndex(NODE* node, int index);

FunctionPointers* getFunctions(NODE* node);

int nodeCompareFind(NODE* node, int type, void* target);
int nodeCompareSort(NODE* node, NODE* nodeIn);
int editNode(NODE* node);
int nodeToString(NODE* node, char* string);


/*
  FunctionPointers
  description
    struct containing the function pointers
    neccessary to perform operations on node values.
  data:
    newValue    calls the constructor for node value.
                  param   -void
                  return  -pointer to new value
    deleteValue frees node value.
                  param   -value to be free'd
                  return  -NULL
    saveValue   writes value in binary to provided file
                  param   -file to be written to
                          -value to be saved
                  return -NULL
    loadValue   reads value from binary file.
                  param   -file to be read from
                  return  -pointer to new value
    edit        edits value.
                  param   -value to be edited
                  return  -NULL
    compareFind searches for specific value.
                  param   -value to be tested
                          -test to be run
                          -target value
                  return  -NULL
    compareSort searches for the optimal insertion 
                point for a given value.
                  param   -value to be tested
                          -target key
                  return  -NULL
    toString    modifies given string to hold a 
                representation of value
                  param   -value to be toString'd
                          -mode in which toString is to operate
                  return  -NULL
*/
struct FunctionPointersP
{
  void* (*newValue)(char* key);
  int (*deleteValue)(void* value);
  int (*saveValue)(FILE* file, void* value);
  void* (*loadValue)(FILE* file, char* key);
  int (*edit)(void* value);
  int (*compareFind)(void* value, int type, void* target);
  int (*compareSort)(void* value, void* key);
  int (*toString)(void* value, char* string, int type);

};

/*
  NODE
  description
    node structure.
  data:
    functionPointers
                pointers to external value 
                  param   -void
                  return  -function pointers.

    value       value held in the node.
    index       index of node in tree.
    height      the height of the node above its lowest leaf
                (this could be used for illustration of a the
                tree in its "tree" form).
    weight      the number of nodes with this node in their lineage +1.
    greater     branch with a higher ordinal value.
    less        branch with a lower ordinal value.
*/
struct NODE_P
{
  FunctionPointers* functionPointers;
  void* value;
  int index;
  int height;
  NODE* greater;
  NODE* less;
  char key[100];
};

/*
  newNode
  description
    creates a new node to be inserted
    into a tree.
  params:
    functionPointers
              function pointers being 
              associated with the new node.
    staticIndex
              a value representing how many nodes
              have been added to the tree, this
              determines the nodes index value.
    file      file from which the node should 
              be read.
  return: 
    node*     node created 
*/
NODE* newNode(FunctionPointers* functionPointers, int *staticIndex, FILE* file)
{
  NODE* node = (NODE*) malloc(sizeof(NODE));
  if(node == NULL)
  {
    printf("sufficient memory could not be allocated to create node");
    PAUSE
    exit(0);
  }
  //init node data
  node->functionPointers = functionPointers;
  node->value = NULL;
  node->key[0] = '\0';
  //populate value
  if(file != NULL)
  {
    loadNode(node, file);
    if(feof(file))
    {
      free(node);
      return NULL;
    }
  }
  else
  {
    initNode(node);
  } 
  setGreater(node, NULL);
  setLess(node, NULL);
  setIndex(node, (*staticIndex)++);
  return node;
}

/*
  editNode
  description
    calls edit function pointer.
  params:
    node      node to be edited.
  return: 
    NULL      0 value indicating successful exicution.
*/
int editNode(NODE* node)
{
  getFunctions(node)->edit(node->value);
  return 0;
}

/*
  initNode
  description
    creates a value node based on user input.
  params:
    node      node for which the value is being created.
  return: 
    NULL      0 value indicating successful exicution.
*/
int initNode(NODE* node)
{
  setValue(  node, ( getFunctions(node)->newValue(node->key) )  );
  return 0;
}

/*
  deleteNode
  description
    frees node value and node.
  params:
    node      node which is being destroyed.
    clear     determines whether this should begin a
              recursive loop freeing all child nodes
  return: 
    NULL      0 value indicating successful exicution.
*/
int deleteNode(NODE* node, int clear)
{

  if(node != NULL)
  {
    if(clear && getGreater(node) != NULL) deleteNode(getGreater(node), clear);

    if(clear && getLess(node) != NULL) deleteNode(getLess(node), clear);
    
    getFunctions(node)->deleteValue(getValue(node));
    free(node);
  }
  node = NULL;
  return 0;
}

/*
  saveNode
  description
    saves existing node to provided file.
  params:
    node      node who's value is being saved.
    file      file being saved to.
  return: 
    NULL      0 value indicating successful exicution.
*/
int saveNode(NODE* node, FILE* file)
{
  if(node != NULL)
  {
    
    getFunctions(node)->saveValue(file, getValue(node));
    saveNode(getGreater(node), file);
    saveNode(getLess(node), file);

  }
  
  return 0;
}

/*
  loadNode
  description
    uses imports a node value using the 
    loadValue function pointer.
  params:
    node      node being loaded.
    file      file being loaded from.
  return: 
    NULL      0 value indicating successful exicution.
*/
int loadNode(NODE* node, FILE* file)
{
  setValue(  node, getFunctions(node)->loadValue(file, node->key)  );
  
  
  return feof(file);
}

/*
  getValue
  description
    gets node value.
  params:
    node      node who's value is 
              being retrieved.
  return: 
    value     node value.
*/
void* getValue(NODE* node)
{
  return node->value;
}

/*
  initValue
  description
    inits node value to something provided by the user.
  params:
    node      node who's value is being initialized.
  return: 
    NULL      0 value indicating successful exicution.
*/
int initValue(NODE* node)
{
  setValue(node, getFunctions(node)->newValue(node->key));
  return 0;
}

/*
  deleteValue
  description
    use deleteValue function pointer to free the value structure.
  params:
    node      node who's value is being deleted.
  return: 
    NULL      0 value indicating successful exicution.
*/
int deleteValue(NODE* node)
{
  getFunctions(node)->deleteValue(getValue(node));
  return 0;
}

/*
  setValue
  description
    free's current value and sets a new one.
  params:
    node      node being modified.
    value     new value
  return: 
    NULL      0 value indicating successful exicution.
*/
int setValue(NODE* node, void* value)
{
  if(node->value != NULL)
  {
    getFunctions(node)->deleteValue(node);
  }
  node->value = value;
  return 0;
}

/*
  getGreater
  description
    returns a pointer to the child node which is 
    ordinally greater than the parent node.
  params:
    node      node being retrieved from.
  return: 
    greater   greater node.
*/
NODE* getGreater(NODE* node)
{
  return node->greater;
}


/*
  setGreater
  description
    replaces the pointer to the child node which is 
    greater than the parent node.
  params:
    node      node being retrieved from.
    greater   pointer to be inserted.
  return: 
    NULL      0 value indicating successful exicution.
*/
int setGreater(NODE* node, NODE* greater)
{
  node->greater = greater;
  return 0;
}

/*
  getLess
  description
    returns a pointer to the child node which is 
    less than the parent node.
  params:
    node      node being retrieved from.
  return: 
    less      lesser node.
*/
NODE* getLess(NODE* node)
{
  return node->less;
}


/*
  setLess
  description
    replaces the pointer to the child node which is 
    less than the parent node.
  params:
    node      node being retrieved from.
    less      pointer to be inserted.
  return: 
    NULL      0 value indicating successful exicution.
*/
int setLess(NODE* node, NODE* less)
{
  node->less = less;
  return 0;
}

/*
  getIndex
  description
    gets index value.
  params:
    node      node being retrieved from.
  return: 
    index     node index.
*/
int getIndex(NODE* node)
{
  return node->index;
}

/*
  setIndex
  description
    sets the index field of the parameter node to the provided value.
  params:
    node      node being modified.
    index     new index value.
  return: 
    NULL      0 value indicating successful exicution.
*/
int setIndex(NODE* node, int index)
{
  node->index = index;
  return 0;
}

/*
  getFunctions
  description
    returns function pointers.
  params:
    node      node being retrieved from.
  return: 
    functionPointers
              node function pointers
*/
FunctionPointers* getFunctions(NODE* node)
{
  return node->functionPointers;
}

/*
  nodeCompareFind
  description
    compare value in 'type' form.
  params:
    node      the node to which a comparison is being made.
    type      the type of comparison being carried out.
    target    the value being searched for.
  return: 
    comparison
              the product of the comparison functions.
*/
int nodeCompareFind(NODE* node, int type, void* target)
{
  
  if(type == 1)
  {
    int index = *((int*) target);
    return node->index != index;
  }

  return getFunctions(node)->compareFind(getValue(node), type, target);
}

/*
  nodeCompareSort
  description
    compare nodes ordinally.
  params:
    node      node compared against.
    nodeIn    node being inserted.
  return: 
    comparison
              the ordinal comparison of contact key values.
*/
int nodeCompareSort(NODE* node, NODE* nodeIn)
{
  int comp;
  if(node == NULL || getValue(node) == NULL) return 0;
  comp = getFunctions(node)->compareSort(node->value, nodeIn->value);

  return comp ? comp : ((node->index) - (nodeIn->index));
}

/*
  nodeToString
  description
    populates node string.
  params:
    node      node who's data is being retrieved.
    string    string to be filled with node info
  return: 
    NULL      0 value indicating successful exicution.
*/
int nodeToString(NODE* node, char* string)
{
  char* valueString = valueBufferB;
  getFunctions(node)->toString(getValue(node), valueString, 0);
  sprintf(string, "%5d     %s", getIndex(node), valueString);
  valueBufferB[0] = '\0';
  return 0;
}

