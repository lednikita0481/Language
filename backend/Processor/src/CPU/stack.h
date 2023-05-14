const unsigned long long STACK_LEFT_CANNERY_VALUE = 0x8BADBEEF;
const unsigned long long STACK_RIGHT_CANNERY_VALUE = 0x8BADF00D;
const unsigned long long DATA_LEFT_CANNERY_VALUE =  0x32DED300;
const unsigned long long DATA_RIGHT_CANNERY_VALUE =  0x64DED600;
const double double_poison = 0xBADD00B1E;
const int integer_poison = 0xCCCCCCCC;

typedef unsigned long long CANNERY_SIZE;


struct creation_info
{
    const char* file;
    const char* function;
    size_t line;
    const char* first_name;
};

struct Stack
{
    CANNERY_SIZE Stack_Left_Cannery = STACK_LEFT_CANNERY_VALUE;
    CANNERY_SIZE* Data_Left_Cannery;
    Elem_t* data;
    CANNERY_SIZE* Data_Right_Cannery;
    size_t size;
    size_t capacity;
    creation_info info;
    
    #if (defined(NDEBUG))
        size_t data_hash;\
        size_t stack_hash;
    #endif
    CANNERY_SIZE Stack_Right_Cannery = STACK_RIGHT_CANNERY_VALUE;
};

enum Stack_Errors
{
    No_Errors          = 0,
    Null_Ptr_Err       = 1 << 0,
    Pop_Impossible_Err = 1 << 1,
    Null_Capacity_Err  = 1 << 2,
    Size_More_Cap_Err  = 1 << 3,
    Data_Null_Ptr_Err  = 1 << 4,
    St_Left_Can_Err    = 1 << 5,
    St_Right_Can_Err   = 1 << 6,
    Data_Left_Can_Err  = 1 << 7,
    #if (defined(NDEBUG))
        Data_Hash_Err  = 1 << 9,
        St_Hash_err    = 1 << 10,
    #endif
    Data_Right_Can_Err = 1 << 8
};

void StackConstructor(Stack* stk, size_t capacity);
void StackDestructor(Stack* stk);
void StackPush(Stack* stk, Elem_t value);
Elem_t StackPop(Stack* stk);
void StackRealloc(Stack* stk);
void* recalloc(void* mass, size_t num, size_t elem_size, size_t elements_now);
int CheckStack(Stack* stk, bool Called_From_Pop);
void PrintStackErrors(FILE* stream, int error);
void exit_errors(int error);
void StackDuump(Stack* stk, int error, const char* func, const char* file, const int line);
size_t Calculate_Hash(void* structure, size_t size_bites);
#if (defined(NDEBUG))
    void Stack_Calculate_Hash(Stack* stk);
#endif


#define LINES "----------------------------------------------------------------------------------------"

#define StackCtor( a, b ) (a)->info.file = __FILE__;\
    (a)->info.function = __FUNCTION__; (a)->info.line = __LINE__;\
    (a)->info.first_name = #a; StackConstructor( (a), (b) );


#if (!defined(NDEBUG))
    #define ASSERT_OK(stk)
#else
    #define ASSERT_OK(stk)\
        errors_blin_kill_that_shit = CheckStack((stk), (false));\
        StackDuump(stk, errors_blin_kill_that_shit, __FUNCTION__, __FILE__, __LINE__);\
        exit_errors(errors_blin_kill_that_shit);
#endif

#if (!defined(NDEBUG))
    #define ASSERT_POP(stk)
#else
    #define ASSERT_POP(stk)\
        if (stk->size <= 0)\
        {\
            errors_blin_kill_that_shit = CheckStack(stk, true);\
            StackDuump(stk, errors_blin_kill_that_shit, __FUNCTION__, __FILE__, __LINE__);\
            exit_errors(errors_blin_kill_that_shit);\
        }
#endif