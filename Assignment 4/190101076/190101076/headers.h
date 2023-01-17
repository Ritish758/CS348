// Changing YYSTYPE to ptr
#define YYSTYPE uintptr_t

// Declaring Node struct for Hashmap
struct Node {
    int tokenType;
    int datatype;
    char symName[100];
    struct Node* next;
};
