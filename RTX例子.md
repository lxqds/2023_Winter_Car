# RTX例程

>   Thread

```c
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void Thread (void const *argument);                             // thread function
osThreadId tid_Thread;                                          // thread id
osThreadDef (Thread, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread (void) {

  tid_Thread = osThreadCreate (osThread(Thread), NULL);
  if (!tid_Thread) return(-1);
  
  return(0);
}

void Thread (void const *argument) {

  while (1) {
    ; // Insert thread code here...
    osThreadYield ();                                           // suspend thread
  }
}

```



>   Timer

```c
/*----------------------------------------------------------------------------

 *      Timer: Sample timer functions
         *---------------------------------------------------------------------------*/


/*----- One-Shoot Timer Example -----*/
static void Timer1_Callback (void const *arg);                  // prototype for timer callback function

static osTimerId id1;                                           // timer id
static uint32_t  exec1;                                         // argument for the timer call back function
static osTimerDef (Timer1, Timer1_Callback);                    // define timers

// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) {
  // add user code here
}


/*----- Periodic Timer Example -----*/
static void Timer2_Callback (void const *arg);                  // prototype for timer callback function

static osTimerId id2;                                           // timer id
static uint32_t  exec2;                                         // argument for the timer call back function
static osTimerDef (Timer2, Timer2_Callback);

// Periodic Timer Example
static void Timer2_Callback (void const *arg) {
  // add user code here
}


// Example: Create and Start timers
void Init_Timers (void) {
  osStatus status;                                              // function return status

  // Create one-shoot timer
  exec1 = 1;
  id1 = osTimerCreate (osTimer(Timer1), osTimerOnce, &exec1);
  if (id1 != NULL) {    // One-shot timer created
    // start timer with delay 100ms
    status = osTimerStart (id1, 100);            
    if (status != osOK) {
      // Timer could not be started
    }
  }

  // Create periodic timer
  exec2 = 2;
  id2 = osTimerCreate (osTimer(Timer2), osTimerPeriodic, &exec2);
  if (id2 != NULL) {    // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart (id2, 1000);            
    if (status != osOK) {
      // Timer could not be started
    }
  }
}
```

>   Semaphore

```c
/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
void Thread_Semaphore (void const *argument);                   // thread function
osThreadId tid_Thread_Semaphore;                                // thread id
osThreadDef (Thread_Semaphore, osPriorityNormal, 1, 0);         // thread object

osSemaphoreId sid_Thread_Semaphore;                             // semaphore id
osSemaphoreDef (SampleSemaphore);                               // semaphore object


int Init_Semaphore (void) {

  sid_Thread_Semaphore = osSemaphoreCreate (osSemaphore(SampleSemaphore), 1);
  if (!sid_Thread_Semaphore) {
    ; // Semaphore object not created, handle failure
  }
  
  tid_Thread_Semaphore = osThreadCreate (osThread(Thread_Semaphore), NULL);
  if (!tid_Thread_Semaphore) return(-1);
  
  return(0);
}

void Thread_Semaphore (void const *argument) {
  int32_t val;

  while (1) {
    ; // Insert thread code here...

    val = osSemaphoreWait (sid_Thread_Semaphore, 10);           // wait 10 mSec
    switch (val) {
      case osOK:
        ; // Use protected code here...
        osSemaphoreRelease (sid_Thread_Semaphore);              // Return a token back to a semaphore
        break;
      case osErrorResource:
        break;
      case osErrorParameter:
        break;
      default:
        break;
    }

    osThreadYield ();                                           // suspend thread
  }
}
```

>   Mutex

```c
/*----------------------------------------------------------------------------
 *      Mutex creation & usage
 *---------------------------------------------------------------------------*/
 
void Thread_Mutex (void const *argument);                       // thread function
osThreadId tid_Thread_Mutex;                                    // thread id
osThreadDef (Thread_Mutex, osPriorityNormal, 1, 0);             // thread object

osMutexId mid_Thread_Mutex;                                     // mutex id
osMutexDef (SampleMutex);                                       // mutex name definition


int Init_Mutex (void) {

  mid_Thread_Mutex = osMutexCreate (osMutex (SampleMutex));
  if (!tid_Thread_Mutex) {
    ; // Mutex object not created, handle failure
  }
  
  tid_Thread_Mutex = osThreadCreate (osThread(Thread_Mutex), NULL);
  if (!tid_Thread_Mutex) return(-1);
  
  return(0);
}

void Thread_Mutex (void const *argument) {
  osStatus status;

  while (1) {
    ; // Insert thread code here...

    status = osMutexWait (mid_Thread_Mutex, NULL);
    switch (status) {
      case osOK:
        ; // Use protected code here...
        osMutexRelease (mid_Thread_Mutex);
        break;
      case osErrorTimeoutResource:
        break;
      case osErrorResource:
        break;
      case osErrorParameter:
        break;
      case osErrorISR:
        break;
      default:
        break;
    }

    osThreadYield ();                                           // suspend thread
  }
}
```

>   MessageQueue

```c
/*----------------------------------------------------------------------------
 *      Message Queue creation & usage
 *---------------------------------------------------------------------------*/
 
void Thread_MsgQueue1 (void const *argument);                   // thread function 1
void Thread_MsgQueue2 (void const *argument);                   // thread function 2
osThreadId tid_Thread_MsgQueue1;                                // thread id 1
osThreadId tid_Thread_MsgQueue2;                                // thread id 2
osThreadDef (Thread_MsgQueue1, osPriorityNormal, 1, 0);         // thread object 1
osThreadDef (Thread_MsgQueue2, osPriorityNormal, 1, 0);         // thread object 2

#define MSGQUEUE_OBJECTS      16                                // number of Message Queue Objects

typedef struct {                                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MEM_BLOCK_t;

typedef struct {                                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_OBJ_t;

osPoolId mpid_MemPool2;                                         // memory pool id
osPoolDef (MemPool2, MSGQUEUE_OBJECTS, MEM_BLOCK_t);            // memory pool object
  
osMessageQId mid_MsgQueue;                                      // message queue id
osMessageQDef (MsgQueue, MSGQUEUE_OBJECTS, MSGQUEUE_OBJ_t);     // message queue object


int Init_MsgQueue (void) {

  mpid_MemPool2 = osPoolCreate (osPool (MemPool2));             // create Mem Pool
  if (!mpid_MemPool2) {
    ; // MemPool object not created, handle failure
  }
  
  mid_MsgQueue = osMessageCreate (osMessageQ(MsgQueue), NULL);  // create msg queue
  if (!mid_MsgQueue) {
    ; // Message Queue object not created, handle failure
  }
  
  tid_Thread_MsgQueue1 = osThreadCreate (osThread(Thread_MsgQueue1), NULL);
  if (!tid_Thread_MsgQueue1) return(-1);
  tid_Thread_MsgQueue2 = osThreadCreate (osThread(Thread_MsgQueue2), NULL);
  if (!tid_Thread_MsgQueue2) return(-1);
  
  return(0);
}

void Thread_MsgQueue1 (void const *argument) {
  MEM_BLOCK_t *pMsg = 0;

  while (1) {
    ; // Insert thread code here...
    pMsg = (MEM_BLOCK_t *)osPoolCAlloc (mpid_MemPool2);         // get Mem Block
    if (pMsg) {                                                 // Mem Block was available
      pMsg->Buf[0] = 0x55;                                      // do some work...
      pMsg->Idx    = 0;
      osMessagePut (mid_MsgQueue, (uint32_t)pMsg, osWaitForever); // Send Message
    }

    osThreadYield ();                                           // suspend thread
  }
}

void Thread_MsgQueue2 (void const *argument) {
  osEvent      evt;
  MEM_BLOCK_t *pMsg = 0;

  while (1) {
    ; // Insert thread code here...
    evt = osMessageGet (mid_MsgQueue, osWaitForever);           // wait for message
    if (evt.status == osEventMessage) {
      pMsg = evt.value.p;
      if (pMsg) {
        ; // process data
        osPoolFree (mpid_MemPool2, pMsg);                       // free memory allocated for message
      }
    }
  }
}
```

>   Mempool

```
/*----------------------------------------------------------------------------
 *      Memory Pool creation & usage
 *---------------------------------------------------------------------------*/
 
#define MEMPOOL_OBJECTS      16                                 // number of Memory Pool Objects

typedef struct {                                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MEM_BLOCK_t;

void Thread_MemPool (void const *argument);                     // thread function
osThreadId tid_Thread_MemPool;                                  // thread id
osThreadDef (Thread_MemPool, osPriorityNormal, 1, 0);           // thread object

osPoolId mpid_MemPool;                                          // memory pool id
osPoolDef (MemPool, MEMPOOL_OBJECTS, MEM_BLOCK_t);              // memory pool object


int Init_MemPool (void) {

  mpid_MemPool = osPoolCreate (osPool (MemPool));               // create Mem Pool
  if (!mpid_MemPool) {
    ; // MemPool object not created, handle failure
  }
  
  tid_Thread_MemPool = osThreadCreate (osThread(Thread_MemPool), NULL);
  if (!tid_Thread_MemPool) return(-1);
  
  return(0);
}

void Thread_MemPool (void const *argument) {
  osStatus     status;
  MEM_BLOCK_t *pMem = 0;

  while (1) {
    ; // Insert thread code here...

    pMem = (MEM_BLOCK_t *)osPoolCAlloc (mpid_MemPool);          // get Mem Block
    if (pMem) {                                                 // Mem Block was available
      pMem->Buf[0] = 0x55;                                      // do some work...
      pMem->Idx    = 0;      
      
      status = osPoolFree (mpid_MemPool, pMem);                 // free mem block
      switch (status)  {
        case osOK:
          break;
        case osErrorParameter:
          break;
        case osErrorValue:
          break;
        default:
          break;
      }
    }

    osThreadYield ();                                           // suspend thread
  }
}
```

>   MailQueue

```
/*----------------------------------------------------------------------------
 *      Mail Queue creation & usage
 *---------------------------------------------------------------------------*/
 
void Thread_MailQueue1 (void const *argument);                  // thread function 1
void Thread_MailQueue2 (void const *argument);                  // thread function 2
osThreadId tid_Thread_MailQueue1;                               // thread id 1
osThreadId tid_Thread_MailQueue2;                               // thread id 2
osThreadDef (Thread_MailQueue1, osPriorityNormal, 1, 0);        // thread object 1
osThreadDef (Thread_MailQueue2, osPriorityNormal, 1, 0);        // thread object 2

#define MAILQUEUE_OBJECTS      16                               // number of Message Queue Objects
typedef struct {                                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MAILQUEUE_OBJ_t;

osMailQId qid_MailQueue;                                        // mail queue id
osMailQDef (MailQueue, MAILQUEUE_OBJECTS, MAILQUEUE_OBJ_t);     // mail queue object


int Init_MailQueue (void) {

  qid_MailQueue = osMailCreate (osMailQ(MailQueue), NULL);      // create mail queue
  if (!qid_MailQueue) {
    ; // Mail Queue object not created, handle failure
  }
  
  tid_Thread_MailQueue1 = osThreadCreate (osThread(Thread_MailQueue1),  NULL);
  if (!tid_Thread_MailQueue1) return(-1);
  tid_Thread_MailQueue2 = osThreadCreate (osThread(Thread_MailQueue2),  NULL);
  if (!tid_Thread_MailQueue2) return(-1);
  
  return(0);
}

void Thread_MailQueue1 (void const *argument) {
  MAILQUEUE_OBJ_t *pMail = 0;

  while (1) {
    ; // Insert thread code here...
    pMail = osMailAlloc (qid_MailQueue, osWaitForever);         // Allocate memory
    if (pMail) {
      pMail->Buf[0] = 0xff;                                     // Set the mail content
      pMail->Idx = 0;
      osMailPut (qid_MailQueue, pMail);                         // Send Mail
    }

    osThreadYield ();                                           // suspend thread
  }
}

void Thread_MailQueue2 (void const *argument) {
  MAILQUEUE_OBJ_t  *pMail = 0;
  osEvent           evt;

  while (1) {
    ; // Insert thread code here...
    evt = osMailGet (qid_MailQueue, osWaitForever);             // wait for mail
    if (evt.status == osEventMail) {
      pMail = evt.value.p;
      if (pMail) {
        ; // process data
        osMailFree (qid_MailQueue, pMail);                      // free memory allocated for mail
      }
    }
  }
}
```

