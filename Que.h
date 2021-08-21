/*
class Column
{
  public:
  Column(int size);
  ~Column();
  char *elements;
}; 

Column::Column(int size){
	elements = new char[size];
	for(int i = 0; i <size; i++)
		elements[i] = ' ';
	
}

Column::~Column() {
    delete[] elements;
}*/

typedef struct{
	char elements[16];
}Column;

// Define the default capacity of a queue
#define SIZE 256
 
// A class to store a queue
class Que
{
  public:
    Column *arr;     // array to store queue elements
    int capacity;   // maximum capacity of the queue
    int tail;       // tail points to the tail element in the queue (if any)
    int head;       // head points to the last element in the queue
    int count;      // current size of the queue
 

    Que(int size);     // constructor
    ~Que();                   // destructor
 
    Column *dequeue();
    void enqueue(Column *x);
    Column *peek();
    Column *look_at(int id);
    void dump();
    int size();
    bool isEmpty();
    bool isFull();
};
 
// Constructor to initialize a queue
Que::Que(int size)
{ 
    arr = new Column[size];
    capacity = size;
    tail = 0;
    head = -1;
    count = 0;
}
 
// Destructor to free memory allocated to the queue
Que::~Que() {
    delete[] arr;
}
 
// Utility function to dequeue the tail element
Column *Que::dequeue()
{
    // check for queue underflow
    if (isEmpty())
    {
        Serial.println( "Underflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }
 
   // Serial.println( "Removing " ); //<< arr[tail] << endl;
 
    tail = (tail + 1) % capacity;
    count--;
    return  &arr[tail];
}
 
// Utility function to add an item to the queue
void Que::enqueue(Column *item)
{
    // check for queue overflow
    if (isFull())
    {
       Serial.println( "Overflow\nProgram Terminated\n");
        //exit(EXIT_FAILURE);
        return 0;
    }
 
   //Serial.println( "Inserting: ");
    //Serial.println(item);
    head = (head + 1) % capacity;
    memcpy( &arr[head], item, sizeof(Column));
    count++;
}
 
// Utility function to return the head element of the queue
Column *Que::peek()
{
    if (isEmpty())
    {
       Serial.println( "Underflow\nProgram Terminated\n");
    }
    return &arr[head];
}

Column *Que::look_at(int id){
    return  &arr[(tail + id) % capacity];
}

 
// Utility function to return the size of the queue
int Que::size() {
    return count;
}
 
// Utility function to check if the queue is empty or not
bool Que::isEmpty() {
    return (size() == 0);
}
 
// Utility function to check if the queue is full or not
bool Que::isFull() {
    return (size() == capacity);
}
 
